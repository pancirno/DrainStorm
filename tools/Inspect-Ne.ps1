param(
    [Parameter(Mandatory = $true)]
    [string] $Path
)

$ErrorActionPreference = 'Stop'
$resolved = (Resolve-Path -LiteralPath $Path).Path
$bytes = [IO.File]::ReadAllBytes($resolved)

function U16([int] $offset) { [BitConverter]::ToUInt16($bytes, $offset) }
function U32([int] $offset) { [BitConverter]::ToUInt32($bytes, $offset) }
function PascalString([int] $offset) {
    $length = $bytes[$offset]
    if ($length -eq 0) { return '' }
    [Text.Encoding]::ASCII.GetString($bytes, $offset + 1, $length)
}

if ([Text.Encoding]::ASCII.GetString($bytes, 0, 2) -ne 'MZ') {
    throw 'Not an MZ executable.'
}

$ne = [int](U32 0x3c)
if ([Text.Encoding]::ASCII.GetString($bytes, $ne, 2) -ne 'NE') {
    throw 'The executable does not contain an NE header.'
}

$segmentTable = $ne + (U16 ($ne + 0x22))
$resourceTable = $ne + (U16 ($ne + 0x24))
$residentNames = $ne + (U16 ($ne + 0x26))
$moduleReferences = $ne + (U16 ($ne + 0x28))
$importedNames = $ne + (U16 ($ne + 0x2a))
$segmentCount = U16 ($ne + 0x1c)
$moduleCount = U16 ($ne + 0x1e)
$alignmentShift = U16 ($ne + 0x32)

$modules = for ($i = 0; $i -lt $moduleCount; $i++) {
    $nameOffset = U16 ($moduleReferences + 2 * $i)
    PascalString ($importedNames + $nameOffset)
}

$segments = for ($i = 0; $i -lt $segmentCount; $i++) {
    $entry = $segmentTable + 8 * $i
    $sector = U16 $entry
    $length = U16 ($entry + 2)
    if ($length -eq 0) { $length = 0x10000 }
    [pscustomobject]@{
        Number = $i + 1
        FileOffset = [int]$sector -shl $alignmentShift
        Length = $length
        Flags = ('0x{0:X4}' -f (U16 ($entry + 4)))
        MinimumAllocation = U16 ($entry + 6)
    }
}

$resourceShift = U16 $resourceTable
$cursor = $resourceTable + 2
$resources = @()
while ($true) {
    $type = U16 $cursor
    if ($type -eq 0) { break }
    $count = U16 ($cursor + 2)
    $cursor += 8
    for ($i = 0; $i -lt $count; $i++) {
        $id = U16 ($cursor + 6)
        $resources += [pscustomobject]@{
            Type = if ($type -band 0x8000) { $type -band 0x7fff } else { PascalString ($resourceTable + $type) }
            Id = if ($id -band 0x8000) { $id -band 0x7fff } else { PascalString ($resourceTable + $id) }
            FileOffset = [int](U16 $cursor) -shl $resourceShift
            Length = [int](U16 ($cursor + 2)) -shl $resourceShift
            Flags = ('0x{0:X4}' -f (U16 ($cursor + 4)))
        }
        $cursor += 12
    }
}

[pscustomobject]@{
    Path = $resolved
    LinkerVersion = '{0}.{1}' -f $bytes[$ne + 2], $bytes[$ne + 3]
    TargetOS = $bytes[$ne + 0x36]
    SegmentCount = $segmentCount
    ModuleCount = $moduleCount
    AlignmentShift = $alignmentShift
    ResidentModuleName = PascalString $residentNames
    ImportedModules = @($modules)
    Segments = @($segments)
    Resources = @($resources)
} | ConvertTo-Json -Depth 5
