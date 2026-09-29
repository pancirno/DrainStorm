from pathlib import Path
import re
import struct

from capstone import Cs, CS_ARCH_X86, CS_MODE_16

root = Path(__file__).resolve().parents[1]
exe = (root / "Drain Storm" / "DRAIN.EXE").read_bytes()
text = (root / "analysis" / "export" / "decompiled.c").read_text(encoding="utf-8")

# Selector 1010 is NE segment 3. Its file base was recovered from the segment table.
segment_base = 0x11D80
vtable_bases = sorted({int(value, 16) for value in re.findall(
    r"\*param_1 = 0x([0-9a-f]+);\s+(?:\(\(undefined2 \*\)param_1\)|param_1)\[1\] = 0x1010", text)})
md = Cs(CS_ARCH_X86, CS_MODE_16)

# Resolve NE relocation chains. Vtable slots contain the next source offset on disk;
# the loader replaces every member of the chain with the record's target.
relocation_base = segment_base + 0xEF31
relocation_count = struct.unpack_from("<H", exe, relocation_base)[0]
relocations = []
for index in range(relocation_count):
    record = relocation_base + 2 + index * 8
    source_type, flags, source, target_segment, zero, target = struct.unpack_from(
        "<BBHBBH", exe, record)
    chain = []
    cursor = source
    seen = set()
    while cursor != 0xFFFF and cursor not in seen and cursor < 0xEF31:
        seen.add(cursor)
        chain.append(cursor)
        cursor = struct.unpack_from("<H", exe, segment_base + cursor)[0]
    relocations.append((source_type, flags, target_segment, target, chain))

for base in vtable_bases:
    slot = base + 0x44
    callback = struct.unpack_from("<H", exe, segment_base + slot)[0]
    matching = [entry for entry in relocations if slot + 2 in entry[4]]
    if not matching or matching[0][1] != 0:
        print(f"\n=== vtable {base:04x}, slot {slot:04x}, unresolved {matching} ===")
        continue
    target_segment = matching[0][2]
    target_base = {2: 0x2B00, 3: 0x11D80}[target_segment]
    code = exe[target_base + callback:target_base + callback + 96]
    selector = {2: "1008", 3: "1010"}[target_segment]
    print(f"\n=== vtable {base:04x}, slot {slot:04x}, callback {selector}:{callback:04x} ===")
    for instruction in md.disasm(code, callback):
        print(f"{instruction.address:04x}: {instruction.mnemonic:8} {instruction.op_str}")
        if instruction.mnemonic.startswith("ret"):
            break

print("\n=== complete derived vtable targets ===")
for base in vtable_bases:
    targets = []
    for method_offset in range(0, 0x50, 4):
        slot = base + method_offset
        target = struct.unpack_from("<H", exe, segment_base + slot)[0]
        matching = [entry for entry in relocations if slot + 2 in entry[4]]
        if matching and matching[0][1] == 0:
            selector = {2: "1008", 3: "1010"}.get(matching[0][2], f"seg{matching[0][2]}")
            targets.append(f"+{method_offset:02x}={selector}:{target:04x}")
        else:
            targets.append(f"+{method_offset:02x}=?{target:04x}")
    print(f"{base:04x}: " + " ".join(targets))

print("\n=== class-specific gameplay callbacks requiring audit ===")
for callback in (0x0000, 0x00EC, 0x0B2C, 0x0B4E, 0x0C26, 0x0CD4,
                 0x0D26, 0x0D2C, 0x1126, 0x117C, 0x1180, 0x19A0, 0x1CBC, 0x1DA4,
                 0x20C2, 0x22C6,
                 0x235C, 0x23AA, 0x26DC, 0x26FE, 0x282C, 0x2874,
                 0x379E, 0x37A2, 0x38D4, 0x39A4, 0x3D4C, 0x3D50, 0x3FA2,
                 0x4382, 0x4692, 0x498C, 0x5062, 0x518C, 0x54CA, 0x54EC,
                 0x582E, 0x596A, 0x5A46, 0x5EEA, 0x61E2, 0x6308, 0x68A8,
                 0x6956, 0x6E3C, 0x6F7A, 0x71F2, 0x7258, 0x8D82, 0x8F56,
                 0x902E, 0xC8CC, 0xC9F8, 0xCE62, 0xD06A, 0xD298, 0xD5D4,
                 0xD71E, 0xD826, 0xD848, 0xD964, 0xD968):
    print(f"\n--- 1010:{callback:04x} ---")
    callback_length = 2300 if callback == 0x1180 else 1536
    code = exe[segment_base + callback:segment_base + callback + callback_length]
    for instruction in md.disasm(code, callback):
        print(f"{instruction.address:04x}: {instruction.mnemonic:8} {instruction.op_str}")
        if instruction.mnemonic == "retf":
            break

print("\n--- 1008:a16a ---")
for instruction in md.disasm(exe[0x2B00 + 0xA16A:0x2B00 + 0xA16A + 1536], 0xA16A):
    print(f"{instruction.address:04x}: {instruction.mnemonic:8} {instruction.op_str}")
    if instruction.mnemonic == "retf":
        break

print("\n--- 1008:23aa ---")
for instruction in md.disasm(exe[0x2B00 + 0x23AA:0x2B00 + 0x23AA + 256], 0x23AA):
    print(f"{instruction.address:04x}: {instruction.mnemonic:8} {instruction.op_str}")
    if instruction.mnemonic == "retf":
        break

print("\n--- 1008:7db4 ---")
for instruction in md.disasm(exe[0x2B00 + 0x7DB4:0x2B00 + 0x7DB4 + 1024], 0x7DB4):
    print(f"{instruction.address:04x}: {instruction.mnemonic:8} {instruction.op_str}")
    if instruction.mnemonic == "retf":
        break

print("\n--- 1008:bfc4 ---")
for instruction in md.disasm(exe[0x2B00 + 0xBFC4:0x2B00 + 0xBFC4 + 1536], 0xBFC4):
    print(f"{instruction.address:04x}: {instruction.mnemonic:8} {instruction.op_str}")
    if instruction.mnemonic == "retf":
        break

print("\n--- 1008:bed2 ---")
for instruction in md.disasm(exe[0x2B00 + 0xBED2:0x2B00 + 0xBED2 + 512], 0xBED2):
    print(f"{instruction.address:04x}: {instruction.mnemonic:8} {instruction.op_str}")
    if instruction.mnemonic == "retf":
        break

print("\n--- 1008:bdb0-unmarked ---")
for instruction in md.disasm(exe[0x2B00 + 0xBDB0:0x2B00 + 0xBDB0 + 288], 0xBDB0):
    print(f"{instruction.address:04x}: {instruction.mnemonic:8} {instruction.op_str}")
    if instruction.mnemonic == "retf":
        break

for callback, length in ((0xBC2C, 256), (0xBC56, 128), (0xBC60, 128),
                         (0xBC64, 512), (0xB882, 1024), (0xBA30, 1024),
                         (0xEC6E, 1024)):
    print(f"\n--- 1008:{callback:04x} ---")
    for instruction in md.disasm(exe[0x2B00 + callback:0x2B00 + callback + length], callback):
        print(f"{instruction.address:04x}: {instruction.mnemonic:8} {instruction.op_str}")
        if instruction.mnemonic == "retf":
            break
