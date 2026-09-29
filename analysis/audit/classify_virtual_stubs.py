"""Classify only provably trivial gameplay callbacks; leave all others open."""
import csv
import re
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
from build_inventory import OUT, exe, segments, virtual_methods

decoder = Cs(CS_ARCH_X86, CS_MODE_16)
rows = []
for target in sorted({row[3] for row in virtual_methods}):
    selector, offset = (int(word, 16) for word in target.split(":"))
    base, length, _ = segments[(selector - 0x1000) // 8 + 1]
    instructions = []
    for instruction in decoder.disasm(exe[base + offset:base + min(length, offset + 48)], offset):
        instructions.append((instruction.mnemonic, instruction.op_str))
        if instruction.mnemonic.startswith("ret"):
            break
    category = "nontrivial-not-classified"
    if len(instructions) == 1 and instructions[0][0] == "retf":
        category = "empty-callback"
    elif (len(instructions) == 2 and instructions[0][0] == "mov"
          and re.fullmatch(r"ax, (?:0x[0-9a-f]+|[0-9]+)", instructions[0][1])
          and instructions[1][0] == "retf"):
        category = "constant-return"
    elif (len(instructions) == 2 and instructions[0] == ("xor", "ax, ax")
          and instructions[1][0] == "retf"):
        category = "zero-return"
    if category != "nontrivial-not-classified":
        rows.append((target, category, "; ".join(f"{op} {args}".strip() for op, args in instructions)))
with (OUT / "virtual-stubs.csv").open("w", encoding="utf-8", newline="") as stream:
    writer = csv.writer(stream)
    writer.writerow(("target", "classification", "complete_instructions"))
    writer.writerows(rows)
print(f"Proven trivial virtual targets: {len(rows)} / {len({r[3] for r in virtual_methods})}")
for row in rows:
    print(" | ".join(row))
