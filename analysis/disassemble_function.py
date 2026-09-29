from pathlib import Path
import sys
import struct

from capstone import Cs, CS_ARCH_X86, CS_MODE_16

root = Path(__file__).resolve().parents[1]
exe = (root / "Drain Storm" / "DRAIN.EXE").read_bytes()
selector = sys.argv[1].lower()
offset = int(sys.argv[2], 16)
length = int(sys.argv[3], 0) if len(sys.argv) > 3 else 2048
selector_value = int(selector, 16)
if selector_value < 0x1000 or (selector_value - 0x1000) % 8:
    raise ValueError("Expected an original selector: 1000, 1008, ...")
segment_index = (selector_value - 0x1000) // 8
ne = struct.unpack_from("<I", exe, 0x3c)[0]
segment_count = struct.unpack_from("<H", exe, ne + 0x1c)[0]
if segment_index >= segment_count:
    raise ValueError("Selector is outside the NE segment table")
segment_table = ne + struct.unpack_from("<H", exe, ne + 0x22)[0]
alignment = struct.unpack_from("<H", exe, ne + 0x32)[0]
sector, segment_length = struct.unpack_from("<HH", exe, segment_table + segment_index * 8)
if not sector:
    raise ValueError("Segment has no file-backed bytes")
segment_base = sector << alignment
segment_length = segment_length or 65536
if not 0 <= offset < segment_length:
    raise ValueError("Offset is outside the file-backed segment")
length = min(length, segment_length - offset)
md = Cs(CS_ARCH_X86, CS_MODE_16)
for instruction in md.disasm(exe[segment_base + offset:segment_base + offset + length], offset):
    print(f"{instruction.address:04x}: {instruction.mnemonic:8} {instruction.op_str}")
    if instruction.mnemonic == "retf":
        break
