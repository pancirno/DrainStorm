"""Compare native attract scripts with original selector-1080 command pairs.

Read-only model check, not an execution of the original binary or GUI.
"""
from pathlib import Path
import re
import struct

root = Path(__file__).resolve().parents[2]
data = (root / "analysis/export/data_1080.bin").read_bytes()
source = (root / "reconstructed/src/application_state.cpp").read_text(encoding="utf-8")
scripts = dict(re.findall(r'case (\d+): return "([A-Za-z5]+)";', source))
keys = {0x0c: "5", 0x21: "Q", 0x22: "E", 0x23: "N", 0x24: "H",
        0x25: "L", 0x26: "U", 0x27: "R", 0x28: "D", 0x41: "A", 0x5a: "Z"}
for level, offset in ((1, 0x1588), (2, 0x1638), (10, 0x186a), (9, 0x207a)):
    tokens = []
    for index in range(1000):
        key, shift = struct.unpack_from("<HH", data, offset + index * 4)
        if shift == 2:
            break
        if key not in keys or shift not in (0, 1):
            raise ValueError((level, index, key, shift))
        token = keys[key]
        tokens.append(token.lower() if shift else token)
    else:
        raise ValueError("Missing demo sentinel")
    original = "".join(tokens)
    native = scripts[str(level)]
    print(f"class={level} original_commands={len(original)} native_commands={len(native)} "
          f"exact_match={original == native}")
    if original != native:
        print(f"original={original}\nnative={native}")
