"""Read-only comparison of original long help narratives; no layout claims."""
import ast
from pathlib import Path
import re
import struct

root = Path(__file__).resolve().parents[2]
exe = (root / 'Drain Storm/DRAIN.EXE').read_bytes()
ne = struct.unpack_from('<I', exe, 0x3c)[0]
table = ne + struct.unpack_from('<H', exe, ne + 0x22)[0]
shift = struct.unpack_from('<H', exe, ne + 0x32)[0]

def original_string(selector, offset):
    sector = struct.unpack_from('<H', exe, table + ((selector-0x1000)//8)*8)[0]
    start = (sector << shift) + offset
    return exe[start:].split(b'\0',1)[0].decode('cp1252')

source = (root / 'reconstructed/src/application_state.cpp').read_text(encoding='utf-8')
body = source.split('std::wstring ApplicationState::HelpPageText(int page) const {',1)[1].split(
    'bool ApplicationState::DrawHelpPage',1)[0]
def normalize(value):
    return ' '.join(value.split())

for page, selector, offset in [(1,0x1080,0x1bf6),(2,0x1080,0x1d4a),
                              (3,0x1080,0x1f02),(8,0x1010,0xbe22),(9,0x1010,0xc052)]:
    native_body = body.split(f'case {page}:',1)[1].split('case ',1)[0]
    native = ''.join(ast.literal_eval(value) for value in re.findall(
        r'L("(?:[^"\\]|\\.)*")', native_body))
    expected = original_string(selector,offset)
    # Heading is separately drawn in original, prepended in native narrative.
    actual = native.split('\r\n\r\n',1)[1]
    exact = normalize(actual) == normalize(expected)
    print(f'help_page={page} normalized_narrative_exact={exact}')
    if not exact:
        print('ORIGINAL:',repr(expected))
        print('NATIVE:',repr(actual))

# 6cdc calls 40c0 on the hidden attribution payload. Decode the complete
# nibble-pair/XOR algorithm rather than interpreting Ghidra's string labels.
encoded = original_string(0x1080, 0x102a).encode('cp1252')
decoded = bytearray(((encoded[i] << 4) ^ (encoded[i+1] & 15)) & 255
                    for i in range(0, len(encoded), 2))
for offset in (0x4540, 0x454c, 0x455a):
    key = original_string(0x1008, offset).encode('cp1252')
    for i in range(len(decoded)):
        decoded[i] ^= key[i % len(key)]
print('hidden_attribution:', repr(decoded.decode('cp1252')))

for offset in (0xc6c6,0xc6e6,0xc6fe,0xc728,0xc74c,0xc774,0xc794,0xc7d0):
    expected = original_string(0x1010,offset)
    print(f'weapon_description={offset:04x} literal_present={expected in source}', repr(expected))

# 4222 expands LF to CRLF, then appends one additional LF. Read supplied
# artifacts only; do not reproduce the original startup's filesystem writes.
for filename, payload in [
    ('README.TXT', original_string(0x1080,0x020e)+original_string(0x1080,0x064c)),
    ('FILE_ID.DIZ', original_string(0x1080,0x0b82)),
]:
    expected = payload.replace('\n','\r\n').encode('cp1252')+b'\n'
    actual = (root/'Drain Storm'/filename).read_bytes()
    print(f'bundled_document={filename} bytes={len(actual)} exact={actual==expected}')

# a3d0's reward-specific names are not the abbreviated arsenal labels.
descriptors = (root/'analysis/export/data_1080.bin').read_bytes()
for item, offset in [(1,0xd234),(2,0x69c0),(3,0xd248),(4,0x6932),
                     (5,0x6938),(6,0xd28a),(7,0xd24e),(8,0xd268),
                     (10,0xd27e),(11,0x6972),(12,0xd296),(13,0xd2a0),
                     (15,0xd224),(19,0xd25c)]:
    expected = original_string(0x1008,offset)
    actual = descriptors[0xd3a+item*34:0xd3a+item*34+30].split(b'\0',1)[0].decode('ascii')
    if expected != actual:
        print(f'reward_name item={item} original={expected!r} native={actual!r}')
