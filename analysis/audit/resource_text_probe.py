"""Compare all fortune literals against original fixed-width NE records."""
import ast
from pathlib import Path
import re
import struct

root = Path(__file__).resolve().parents[2]
exe = (root / 'Drain Storm/DRAIN.EXE').read_bytes()
ne = struct.unpack_from('<I', exe, 0x3c)[0]
table = ne + struct.unpack_from('<H', exe, ne + 0x22)[0]
shift = struct.unpack_from('<H', exe, ne + 0x32)[0]
sector, length = struct.unpack_from('<HH', exe, table + 4*8)
original = exe[sector << shift:(sector << shift)+length]
source = (root / 'reconstructed/src/game_data.cpp').read_text(encoding='utf-8')
body = source.split('kLaws = {',1)[1].split('\n};',1)[0]
native = [ast.literal_eval(value) for value in re.findall(r'L("(?:[^"\\]|\\.)*")',body)]
assert len(native) == 47
differences = []
for index,value in enumerate(native):
    expected = original[index*80:(index+1)*80].split(b'\0',1)[0].decode('cp1252')
    if value != expected:
        differences.append((index,expected,value))
print(f'fortune_records=47 literal_differences={len(differences)}')
for difference in differences:
    print(repr(difference))

# 1008:2eba's complete 31-entry sound dispatch table, independently read above.
sound_offsets = [0x3860,0x386a,0x3876,0x3884,0x3894,0x38a6,0x38ba,0x38c8,
                 0x38d6,0x38e4,0x38ec,0x38fa,0x3908,0x399a,0x39a6,0x39b4,
                 0x39c2,0x39d0,0x39e2,0x39ee,0x393a,0x3948,0x3a00,0x392c,
                 0x3914,0x3920,0x3960,0x3956,0x3968,0x3976,0x398a]
game_sector = struct.unpack_from('<H',exe,table+8)[0] << shift
sound_names = [exe[game_sector+offset:].split(b'\0',1)[0].decode('ascii')
               for offset in sound_offsets]
sound_names = [name.upper() for name in sound_names]  # Win16 resource names ignore case.
app = (root / 'reconstructed/src/application_state.cpp').read_text(encoding='utf-8')
native_sounds = re.findall(r'L"wave_([^"]+)\.wav"',app)
assert len(native_sounds) == 31
print(f'sound_dispatch_entries=31 exact_order={native_sounds == sound_names}')
for index,(expected,actual) in enumerate(zip(sound_names,native_sounds)):
    if expected != actual:
        print(f'sound_dispatch index={index} expected={expected} actual={actual}')

resource_table = ne + struct.unpack_from('<H',exe,ne+0x24)[0]
resource_shift = struct.unpack_from('<H',exe,resource_table)[0]
def resource_name(identifier):
    if identifier & 0x8000:
        return identifier & 0x7fff
    offset = resource_table + identifier
    return exe[offset+1:offset+1+exe[offset]].decode('ascii')

resources = {}
cursor = resource_table + 2
while True:
    kind,count = struct.unpack_from('<HH',exe,cursor)
    if kind == 0:
        break
    cursor += 8
    for index in range(count):
        offset,size,flags,name,handle,usage = struct.unpack_from('<6H',exe,cursor)
        resources[(resource_name(kind),resource_name(name))] = exe[
            offset << resource_shift:(offset+size) << resource_shift]
        cursor += 12

missing = []
payload_differences = []
for name in sound_names:
    key = ('WAVE', name)
    asset = root / f'reconstructed/assets/original/wave_{name}.wav'
    if key not in resources or not asset.is_file():
        missing.append(name)
    elif asset.read_bytes() != resources[key]:
        payload_differences.append(name)
print(f'sound_resources_missing={missing} sound_payload_differences={payload_differences}')

# Table offsets independently follow the original 916e/b09e/4976 consumers.
data_sector, data_length = struct.unpack_from('<HH',exe,table+16*8)
data = exe[data_sector << shift:(data_sector << shift)+data_length]
sprite_sector, sprite_length = struct.unpack_from('<HH',exe,table+5*8)
sprite_data = exe[sprite_sector << shift:(sprite_sector << shift)+sprite_length]
asset_root = root / 'reconstructed/assets/original/game-data'
for name, expected in [
    ('sprite_map.bin', sprite_data),
    ('tile_descriptors.bin', data[0x10a4:0x10a4+192*6]),
    ('overlay_sprites.bin', data[0x1524:0x1524+46*2]),
    ('item_descriptors.bin', data[0x0d36:0x0d36+22*34]),
]:
    actual = (asset_root / name).read_bytes()
    print(f'table={name} bytes={len(actual)} exact={actual == expected}')
    assert actual == expected
names = [data[0xd3a+i*34:0xd3a+i*34+30].split(b'\0',1)[0] for i in range(22)]
assert all(all(value < 128 for value in name) for name in names)
print('item_names=22 all_ascii=True native_byte_to_wchar_preserves_labels=True')
direction_body = source.split('constexpr int directionSprites[8][3] = {',1)[1].split('};',1)[0]
directions = [tuple(map(int, values)) for values in re.findall(
    r'\{\s*(-?\d+),\s*(-?\d+),\s*(-?\d+)\}',direction_body)]
original_directions = [struct.unpack_from('<hhh',data,0x1072+index*6) for index in range(8)]
print(f'missile_direction_records=8 exact={directions == original_directions}')
assert directions == original_directions
