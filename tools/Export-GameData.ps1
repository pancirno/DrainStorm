param(
    [Parameter(Mandatory = $true)]
    [string] $Path,

    [Parameter(Mandatory = $true)]
    [string] $OutputDirectory
)

$ErrorActionPreference = 'Stop'
$sourcePath = (Resolve-Path -LiteralPath $Path).Path
$outputPath = [IO.Path]::GetFullPath((Join-Path (Get-Location) $OutputDirectory))
$inspectScript = Join-Path $PSScriptRoot 'Inspect-Ne.ps1'
$image = [IO.File]::ReadAllBytes($sourcePath)
$ne = (& $inspectScript -Path $sourcePath) | ConvertFrom-Json
[IO.Directory]::CreateDirectory($outputPath) | Out-Null
[IO.Directory]::CreateDirectory((Join-Path $outputPath 'levels')) | Out-Null

function Slice([int64] $offset, [int64] $length) {
    $result = [byte[]]::new($length)
    [Array]::Copy($image, $offset, $result, 0, $length)
    return $result
}

function WriteArtifact([string] $relativePath, [byte[]] $data, [string] $description) {
    $destination = Join-Path $outputPath $relativePath
    [IO.File]::WriteAllBytes($destination, $data)
    [pscustomobject]@{
        file = $relativePath.Replace('\', '/')
        description = $description
        length = $data.Length
        sha256 = [Convert]::ToHexString([Security.Cryptography.SHA256]::HashData($data))
    }
}

$artifacts = @()
$spriteSegment = $ne.Segments | Where-Object Number -eq 6
$spriteMap = Slice $spriteSegment.FileOffset $spriteSegment.Length
$artifacts += WriteArtifact 'sprite_map.bin' $spriteMap '475 sprite atlas records: id, x, y, width, height'

$dataSegment = $ne.Segments | Where-Object Number -eq 17
$tileDescriptorOffset = [int64]$dataSegment.FileOffset + 0x10a4
$tileDescriptors = Slice $tileDescriptorOffset (192 * 6)
$artifacts += WriteArtifact 'tile_descriptors.bin' $tileDescriptors '192 tile records: sprite index, flags, value'

# FUN_1008_b09e indexes this table by overlay-object type before calling the common
# sprite blitter.  Types 0..45 cover every caller of FUN_1008_af5a.
$overlaySpriteOffset = [int64]$dataSegment.FileOffset + 0x1524
$overlaySprites = Slice $overlaySpriteOffset (46 * 2)
$artifacts += WriteArtifact 'overlay_sprites.bin' $overlaySprites '46 signed sprite indices for board overlay objects'

# Arsenal item records at 1080:0d36 are 34 bytes each. The first word is the item
# category and the second word is the sprite used by FUN_1008_4976.
$itemDescriptorOffset = [int64]$dataSegment.FileOffset + 0x0d36
$itemDescriptors = Slice $itemDescriptorOffset (22 * 34)
$artifacts += WriteArtifact 'item_descriptors.bin' $itemDescriptors '22 arsenal item records of 34 bytes'

for ($level = 1; $level -le 9; $level++) {
    $segment = $ne.Segments | Where-Object Number -eq ($level + 7)
    $data = Slice $segment.FileOffset $segment.Length
    if ($data.Length -ne 1476) {
        throw "Unexpected level segment size: $($data.Length)"
    }
    $relative = 'levels/level_{0:D2}.map' -f $level
    $artifacts += WriteArtifact $relative $data "Level ${level}: 36 null-terminated rows of 40 characters"
}

$manifestPath = Join-Path $outputPath 'manifest.json'
[IO.File]::WriteAllText(
    $manifestPath,
    ($artifacts | ConvertTo-Json -Depth 4),
    [Text.UTF8Encoding]::new($false)
)

Write-Output "Exported $($artifacts.Count) game-data artifacts to $outputPath"
