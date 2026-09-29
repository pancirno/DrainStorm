"""Read-only terrain-model comparison of b164 and the native two-pass parser.

Excludes constructor postprocessing, actors and overlays (P-013/P-015).
Models original space-as-no-write, inline occupancy checks and column traversal.
"""
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SCALAR = dict(zip(
    "&()*+-/0;<>@BCEIKOSTUWZbcnosw{|}~",
    [0x43,0x38,0x39,3,0x40,0x36,0x41,1,0x6d,0x23,0x27,7,
     0x5d,0x5c,0x3c,0xb1,0x7e,0xb0,0x6c,0x3f,0xaf,0x6e,0x20,
     0x60,0x5f,0x6f,2,0x68,0x3e,0x3a,0x37,0x3b,0x3d]))
ZERO = set(".$?dgmz")

def place(board, x, y, ch, force):
    pattern = ([[8,9,10],[11,{'D':0x1a,'G':0x18,'R':0x1c}[ch],12],[13,14,15]]
               if ch in 'DGR' else [[0x7f,0x80],[0x81,0x82]])
    size = len(pattern)
    if x + size > 40 or y + size > 36:
        return
    if not force and any(board[a][b] for a in range(x,x+size) for b in range(y,y+size)):
        return
    for a in range(size):
        for b in range(size):
            board[x+a][y+b] = pattern[b][a]

parsed = {}
for path in sorted((ROOT / 'reconstructed/assets/original/game-data/levels').glob('*.map')):
    data = path.read_bytes()
    assert len(data) == 36*41
    original = [[0]*36 for _ in range(40)]
    native = [[0]*36 for _ in range(40)]
    for x in range(40):
        for y in range(36):
            ch = chr(data[y*41+x])
            assert ch in SCALAR or ch in ZERO or ch in ' DGRt', ch
            if ch in 'DGRt':
                place(original,x,y,ch,False)
            elif ch != ' ':
                original[x][y] = SCALAR.get(ch,0)
            native[x][y] = SCALAR.get(ch,0)
    for markers in ('DGR','t'):
        for y in range(36):
            for x in range(40):
                ch = chr(data[y*41+x])
                if ch in markers:
                    place(native,x,y,ch,True)
    differences = [(x,y,original[x][y],native[x][y])
                   for x in range(40) for y in range(36) if original[x][y] != native[x][y]]
    print(f'{path.name} terrain_differences={len(differences)} examples={differences[:8]}')
    parsed[path.stem] = original

# Other constructor writes inspected in 5fc4/6a56 do not touch these footprints.
for level, locations in ((6, ((17,14),(17,19))),
                         (7, ((8,7),(30,7),(8,29),(30,29),(30,1)))):
    board = parsed[f'level_{level:02}']
    for x,y in locations:
        occupied = sum(board[a][b] != 0 for a in range(x,x+3) for b in range(y,y+3))
        print(f'constructor_force0 class={level} top_left={x},{y} occupied={occupied}')
