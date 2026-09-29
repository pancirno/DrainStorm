"""Build an evidence index, not an automatic parity verdict.

NE relocation targets without exported functions remain candidates: pointers can
refer to data or instruction interiors. Explicit source comments are links only.
"""
import csv
import hashlib
import json
import re
import struct
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = Path(__file__).resolve().parent
exe = (ROOT / "Drain Storm/DRAIN.EXE").read_bytes()
ne = struct.unpack_from("<I", exe, 0x3c)[0]
assert exe[ne:ne + 2] == b"NE"
count = struct.unpack_from("<H", exe, ne + 0x1c)[0]
table = ne + struct.unpack_from("<H", exe, ne + 0x22)[0]
shift = struct.unpack_from("<H", exe, ne + 0x32)[0]
segments = {}
for index in range(count):
    sector, length, flags, minimum = struct.unpack_from("<4H", exe, table + index * 8)
    segments[index + 1] = (sector << shift, length or 65536, flags)

with (ROOT / "analysis/export/functions.csv").open(encoding="utf-8", newline="") as stream:
    exported = {r["address"].lower(): r for r in csv.DictReader(stream)}
evidence = defaultdict(set)
for address in exported:
    evidence[address].add("ghidra-export")

# Internal NE fixups in all file-backed segments. Type 2 fixes a segment word and
# preserves the preceding offset; type 3 fixes a complete far pointer.
fixups = []
for segment in segments:
    base, length, flags = segments[segment]
    if base == 0 or not flags & 0x100:
        continue
    records = base + length
    n = struct.unpack_from("<H", exe, records)[0]
    for index in range(n):
        kind, flags, source, target_segment, target_offset = struct.unpack_from(
            "<BBHHH", exe, records + 2 + index * 8)
        if flags & 3 or target_segment not in segments:
            continue
        visited = set()
        while source != 0xffff and source not in visited and source + 2 <= length:
            visited.add(source)
            next_source = struct.unpack_from("<H", exe, base + source)[0]
            offset = target_offset
            call = False
            if kind == 2 and source >= 2:
                offset = struct.unpack_from("<H", exe, base + source - 2)[0]
                call = source >= 3 and exe[base + source - 3] == 0x9a
            elif kind == 3:
                call = source >= 1 and exe[base + source - 1] == 0x9a
            else:
                break
            address = f"{0x1000 + (target_segment - 1) * 8:04x}:{offset:04x}"
            origin = f"{0x1000 + (segment - 1) * 8:04x}:{source:04x}"
            category = "direct-far-call-fixup" if call else "pointer-candidate"
            evidence[address].add(category)
            fixups.append((origin, address, category))
            if flags & 4:  # additive fixup has no source chain
                break
            source = next_source

# NE exported entry bundles: fixed entries contain flags/offset; movable
# entries contain flags, INT 3F thunk, segment and offset. Keep ordinals and
# attributes, since an entry may be a callback rather than a direct-call target.
entries = []
entry_start = ne + struct.unpack_from("<H", exe, ne + 4)[0]
entry_end = entry_start + struct.unpack_from("<H", exe, ne + 6)[0]
cursor, ordinal = entry_start, 1
while cursor + 2 <= entry_end:
    bundle_count, bundle_segment = exe[cursor:cursor + 2]
    cursor += 2
    if bundle_count == 0:
        break
    if bundle_segment == 0:
        ordinal += bundle_count
        continue
    for _ in range(bundle_count):
        size = 6 if bundle_segment == 255 else 3
        assert cursor + size <= entry_end
        attributes = exe[cursor]
        target_segment = exe[cursor + 3] if size == 6 else bundle_segment
        offset = struct.unpack_from("<H", exe, cursor + size - 2)[0]
        address = f"{0x1000 + (target_segment - 1) * 8:04x}:{offset:04x}"
        entries.append((ordinal, address, attributes))
        evidence[address].add("ne-entry-table")
        cursor += size
        ordinal += 1

links = defaultdict(set)
for path in sorted((ROOT / "reconstructed/src").glob("*")):
    if path.suffix not in (".cpp", ".h"):
        continue
    for line, text in enumerate(path.read_text(encoding="utf-8-sig").splitlines(), 1):
        for segment, offset in re.findall(r"\b(?:FUN_)?(1008|1010|1018)[:_]([0-9a-f]{4})\b", text, re.I):
            address = f"{segment}:{offset.lower()}"
            links[address].add(f"{path.relative_to(ROOT).as_posix()}:{line}")
            evidence[address].add("explicit-source-reference")

# Concrete gameplay vtables established by constructor assignments. Slot targets
# are loader-resolved pointers, unlike arbitrary relocation pointer candidates.
# The three maze classes share a table; procedural depths share a table too.
gameplay_tables = {
    "1008:d490": "base", "1008:e02a": "17-25",
    "1010:0e50": "1", "1010:3364": "2", "1010:4f26": "3",
    "1010:555c": "4", "1010:5f74": "5", "1010:6a06": "6",
    "1010:73e0": "7", "1010:cb4c": "8", "1010:dc1a": "9",
    "1010:3a46": "10", "1010:d2ea": "11", "1010:d8a4": "12",
    "1010:906c": "13-15", "1010:3ff4": "16",
}
pointer_targets = {source: target for source, target, _ in fixups}
virtual_methods = []
for table_address, classes in gameplay_tables.items():
    selector, offset = (int(word, 16) for word in table_address.split(":"))
    for slot in range(0, 0x50, 4):
        source = f"{selector:04x}:{offset + slot + 2:04x}"
        target = pointer_targets.get(source)
        assert target, f"Unresolved known gameplay vtable slot {source}"
        evidence[target].add("known-gameplay-vtable")
        virtual_methods.append((classes, table_address, f"{slot:02x}", target,
            exported.get(target, {}).get("name", ""), ";".join(sorted(links[target]))))

with (OUT / "function-inventory.csv").open("w", encoding="utf-8", newline="") as stream:
    writer = csv.writer(stream)
    writer.writerow(("address", "exported_name", "exported_size", "evidence", "implementation_links", "review_status"))
    for address in sorted(evidence):
        record = exported.get(address, {})
        writer.writerow((address, record.get("name", ""), record.get("size", ""),
            ";".join(sorted(evidence[address])), ";".join(sorted(links[address])),
            "unreviewed" if record or evidence[address] & {
                "direct-far-call-fixup", "known-gameplay-vtable", "ne-entry-table"}
            else "candidate-not-proven-function"))
with (OUT / "internal-fixups.csv").open("w", encoding="utf-8", newline="") as stream:
    writer = csv.writer(stream)
    writer.writerow(("source", "target", "kind"))
    writer.writerows(sorted(set(fixups)))
with (OUT / "entry-points.csv").open("w", encoding="utf-8", newline="") as stream:
    writer = csv.writer(stream)
    writer.writerow(("ordinal", "address", "attributes"))
    writer.writerows(entries)
with (OUT / "gameplay-vtables.csv").open("w", encoding="utf-8", newline="") as stream:
    writer = csv.writer(stream)
    writer.writerow(("native_classes", "original_table", "slot", "target", "exported_name", "implementation_links"))
    writer.writerows(virtual_methods)
summary = {
    "original_sha256": hashlib.sha256(exe).hexdigest(),
    "exported_functions": len(exported),
    "ne_segments": len(segments),
    "ne_entries": len(entries),
    "gameplay_vtables": len(gameplay_tables),
    "gameplay_virtual_slots": len(virtual_methods),
    "unique_gameplay_virtual_targets": len({row[3] for row in virtual_methods}),
    "indexed_addresses": len(evidence),
    "unexported_direct_call_targets": sum(address not in exported and
        "direct-far-call-fixup" in kinds for address, kinds in evidence.items()),
    "addresses_with_explicit_source_links": sum(bool(value) for value in links.values()),
    "warning": "Links and relocation candidates do not establish implementation parity; inventory is not yet exhaustive.",
}
(OUT / "inventory-summary.json").write_text(json.dumps(summary, indent=2) + "\n", encoding="utf-8")
print(json.dumps(summary, indent=2))
