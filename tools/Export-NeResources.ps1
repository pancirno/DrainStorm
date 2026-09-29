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

function SafeName([string] $value) {
    $safe = $value -replace '[^A-Za-z0-9_.-]', '_'
    if ([string]::IsNullOrWhiteSpace($safe)) { return 'unnamed' }
    return $safe
}

function TypeName($type) {
    switch ([string]$type) {
        '2' { 'bitmap' }
        '3' { 'icon' }
        '5' { 'dialog' }
        '14' { 'group_icon' }
        default { SafeName ([string]$type).ToLowerInvariant() }
    }
}

function Extension($type) {
    switch ([string]$type) {
        'WAVE' { '.wav' }
        '2' { '.dib' }
        default { '.bin' }
    }
}

function WriteBitmapFile([byte[]] $dib, [string] $destination) {
    $headerSize = [BitConverter]::ToInt32($dib, 0)
    $bitsPerPixel = [BitConverter]::ToUInt16($dib, 14)
    $colorsUsed = [BitConverter]::ToUInt32($dib, 32)
    if ($colorsUsed -eq 0 -and $bitsPerPixel -le 8) {
        $colorsUsed = 1 -shl $bitsPerPixel
    }
    $pixelOffset = 14 + $headerSize + 4 * $colorsUsed
    $bitmap = [byte[]]::new(14 + $dib.Length)
    $bitmap[0] = [byte][char]'B'
    $bitmap[1] = [byte][char]'M'
    [BitConverter]::GetBytes([uint32]$bitmap.Length).CopyTo($bitmap, 2)
    [BitConverter]::GetBytes([uint32]$pixelOffset).CopyTo($bitmap, 10)
    [Array]::Copy($dib, 0, $bitmap, 14, $dib.Length)
    [IO.File]::WriteAllBytes($destination, $bitmap)
}

$manifest = foreach ($resource in $ne.Resources) {
    $offset = [int64]$resource.FileOffset
    $length = [int64]$resource.Length
    if ($offset -lt 0 -or $length -lt 0 -or $offset + $length -gt $image.LongLength) {
        throw "Resource $($resource.Type)/$($resource.Id) lies outside the executable"
    }

    $typeName = TypeName $resource.Type
    $idName = SafeName ([string]$resource.Id)
    $fileName = '{0}_{1}{2}' -f $typeName, $idName, (Extension $resource.Type)
    $destination = Join-Path $outputPath $fileName
    $data = [byte[]]::new($length)
    [Array]::Copy($image, $offset, $data, 0, $length)
    [IO.File]::WriteAllBytes($destination, $data)
    if ([string]$resource.Type -eq '2') {
        WriteBitmapFile $data ([IO.Path]::ChangeExtension($destination, '.bmp'))
    }
    $hash = [Convert]::ToHexString([Security.Cryptography.SHA256]::HashData($data))

    [pscustomobject]@{
        type = [string]$resource.Type
        id = [string]$resource.Id
        file = $fileName
        offset = $offset
        length = $length
        flags = $resource.Flags
        sha256 = $hash
    }
}

$manifestPath = Join-Path $outputPath 'manifest.json'
[IO.File]::WriteAllText(
    $manifestPath,
    ($manifest | ConvertTo-Json -Depth 4),
    [Text.UTF8Encoding]::new($false)
)

Write-Output "Exported $($manifest.Count) resources to $outputPath"
Write-Output $manifestPath
