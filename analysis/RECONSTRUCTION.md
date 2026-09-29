# Drain Storm reconstruction map

## Provenance

- Original: `Drain Storm/DRAIN.EXE`
- SHA-256: `36FA1A907E94D5E4B2A4562F366C497A89C076C9F5438603C5A3C9273A2D1988`
- Format: Windows New Executable (NE), 16-bit protected-mode x86
- Ghidra language: `x86:LE:16:Protected Mode:default`
- Linker version recorded in the NE header: 5.60

The bundled permission from David Horner is in `Drain Storm/PERMISSION.TXT`.

## Function corpus

The current analysis pass has discovered 467 functions. Additional routines became visible after
the indirectly stored main and child window procedures were declared explicitly:

| Selector | Role | Functions | Discovered bytes |
|---|---|---:|---:|
| `1000` | Microsoft C startup/runtime and glue | 87 | 7,071 |
| `1008` | Application/framework/game code | 177 | 47,953 |
| `1010` | Application/framework/game code | 105 | 35,123 |
| `1018` | Auxiliary code | 7 | 881 |
| `11d0` | Imported Win16 API stubs | 91 | 364 |

The `1000` classification is supported by startup imports such as `INITTASK`, `INITAPP`,
`LOCKSEGMENT`, and `DOS3CALL`, plus runtime strings `_C_FILE_INFO=` and
`R6000 - stack overflow`. Modern CRT equivalents should replace this code; it should not be
translated instruction by instruction.

## Confirmed application routines

Names below are semantic working names. Ghidra names remain in the database and CSV exports until
the signatures and roles are verified.

| Original address | Working interpretation | Evidence |
|---|---|---|
| `1008:6b88` | Main window procedure | Far pointer stored in `WNDCLASS`; dispatches core window messages |
| `1008:4584` | Arsenal child-window procedure | Handles create, paint, command, and left-button messages |
| `1008:5be4` | Board child-window procedure | Handles create/destroy, paint, timer, erase, and mouse messages |
| `1008:1aec` | Main message loop | Calls `GETMESSAGE`, `TRANSLATEMESSAGE`, `DISPATCHMESSAGE` |
| `1008:6d30` | Main-window creation | Calls `CREATEWINDOW`, `SHOWWINDOW`, `UPDATEWINDOW`; uses 636×468 dimensions |
| `1008:6dcc` | Main-window class registration | Builds a `WNDCLASS` and calls `REGISTERCLASS` |
| `1008:6e4c` | Main-window `WM_CREATE` handler | Adds Help and About system-menu commands; lays out child windows |
| `1008:6f3c` | Main-window `WM_DESTROY` handler | Calls `POSTQUITMESSAGE` |
| `1008:6f46` | Main-window `WM_KEYDOWN` handler | Maps configured keys into movement, weapons, and actions |
| `1008:73ce` | Main-window `WM_SYSKEYDOWN` handler | Handles Alt+Up/Alt+Down arsenal selection |
| `1008:a5fa` | Mark entire board dirty | Sets bit 0 in each cell of a 40×36 flag grid |
| `1008:29e8` | Select previous arsenal item | Decrements selection and scrolls the five-row arsenal view |
| `1008:2a98` | Select next arsenal item | Increments selection and scrolls the five-row arsenal view |
| `1008:481e` | Paint arsenal window | Paints four vertical arsenal regions and selection state |
| `1008:5e70` | Paint board window | Renders dirty board cells and overlays through a compatible bitmap DC |
| `1008:6048` | Board left-click handler | Converts 12-pixel coordinates into an adjacent movement/action |
| `1008:60f4` | Board right-click handler | Converts 12-pixel coordinates and identifies the selected object |

## Recovered child layout

`FUN_1008_6e4c` lays out two children around the same vertically centered 434-pixel height:

- Arsenal: x = 12, width = 120, height = 2 × (`SM_CYBORDER` + 216).
- Board: right margin = 6, width = 2 × (`SM_CXBORDER` + 240), same height.
- The board interior is 40 × 36 cells at 12 × 12 pixels, with a one-pixel border.

## Recovered graphics and level data

- NE segment 6 is exactly 4,750 bytes: 475 ten-byte sprite records.
- Each sprite record is five 16-bit values: id, atlas column, atlas row, width, and height.
- Sprite coordinates and dimensions use 12-pixel cells.
- The tile descriptor table is at data address `1080:10a4` and contains 192 six-byte records before
  the next table at `1080:1524`.
- Each tile descriptor contains a sprite index, behavior flags, and a value/id field.
- NE segments 8 through 16 are the nine built-in levels.
- Each level segment is 1,476 bytes: 36 null-terminated rows of 40 ASCII map characters.
- `FUN_1008_b164` clears the 40×36 board and translates those characters into tile ids or dynamic
  object constructors.
- `FUN_1008_3bd0` resolves a sprite record and performs the atlas-to-board `BitBlt`.
- `FUN_1008_9070` repaints dirty static board cells; `FUN_1008_8ea4`, `FUN_1008_8f32`, and
  `FUN_1008_b09e` add monsters, projectiles/items, and other overlays.

The native renderer now decodes level 1 through the recovered character mapping and draws
individual 12×12 cells from the original atlas. Its board paint path is double-buffered.
`$`/`?` overlay nodes use the original type-to-sprite table; `d`/`g`/`m`/`z` actors use the
animation selectors from `FUN_1008_8ea4`; `D`/`G`/`R` and `t` expand to their original 3×3 and
2×2 tile structures. The selector-1038 start position is restored, the original eight movement
keys are wired, and movement tests tile-descriptor flag bit 0 as in `FUN_1008_27b8`.
Adjacent left-click movement now follows `FUN_1008_6048`. After each successful player move,
actors advance using the 3×3 candidate search and Chebyshev-distance rule recovered from
`FUN_1008_8310`/`FUN_1008_93a4`. Native actor records also preserve the original 12-byte record's
signed movement vector and behavior fields: stationary mode is honored and vector-driven actors
follow their stored direction until obstructed. Contact removes the actor and applies the type-dependent one- or
two-point health loss from `FUN_1008_286c`/`FUN_1008_2920`; zero health disables further movement.
Hit and death events now play the extracted `SNDPLAYER_HIT` and `SNDPLAYER_DIES` resources through
WinMM. The twelve-slot inventory initialized by `FUN_1008_169c` is represented natively; arsenal
icons come from the 34-byte item records at `1080:0d36`, five visible entries follow
`FUN_1008_4976`, and Alt+Up/Alt+Down selection follows `FUN_1008_73ce`.
The default BB.BIN bindings recovered by `FUN_1010_ed68` are also active for arsenal navigation:
`A` selects the preceding item and `Z` the following item. Binding 16 (`T`) toggles asynchronous
WinMM playback and keeps the native Options menu check state synchronized; disabling it immediately
stops any playing wave. The keyboard path also displays the exact recovered `Sound On`/`Sound Off`
notice through the original modal-feedback channel.
Shift plus any of the eight movement directions now takes the weapon-use branch of
`FUN_1008_1b22`. The implemented `FUN_1008_1fe2` cases cover the inventory's thuk, triple-thuk,
bomb, missile, eight-way bomb, stationary charge, and alternate-thuk launch forms, including the
150-projectile cap, consumption exceptions for types 0/1, empty-slot removal, original atlas
sprites from `FUN_1008_8f32`, and sound indices 0 through 5. `FUN_1008_77d0` advances projectiles
once per accepted player action; `WM_TIMER` only advances visual animation. Impacts now
distinguish projectile types 0–4, missile hits spawn the original eight secondary projectiles,
stationary type-3 charges persist, and inventory type 9 detonates all placed charges using the
blast footprint from `FUN_1008_a7a8`.
The ordinary Bubble (item type 0) launches projectile type 0; projectile type 4 is exclusive to
the Fireball branch. The arsenal paint loop bounds its five visible rows by the actual item count,
which is essential for the three-entry playable New Game inventory.

Bindings 17 and 18 from `FUN_1010_ed68` are restored as the optional `U` and `D` drain commands.
More importantly, the accepted movement path in `FUN_1008_1b22` reaches each board's `+0x1c`
callback, whose common `FUN_1008_ba30` tail automatically dispatches `+0x18` after walking onto
tiles `0x23` through `0x26` and `+0x14` after walking onto `0x27` through `0x2a`. Thus ordinary
movement activates drains; the dedicated keys are an additional route to the same callbacks.

The remaining multi-shot branches of `FUN_1008_1fe2` are translated as well: inventory type 7
launches eight type-2 missiles in the complete neighboring direction set and inventory type 10
launches ten coincident type-1 bombs along the selected direction. Both retain the original
sound-event selection and ordinary consumable handling. The common randomized-container handler
below makes both normally non-starting types obtainable through the native UI.

Mine and radio-bomb placement now retain `FUN_1008_1fe2`'s destination checks and exact rejection
messages. Both require a passable adjacent square, mines additionally reject actor occupancy, and
failed placement neither creates a projectile nor consumes stock. The dispatcher's recovered
150-projectile guard likewise rejects the item effect when the pool is full; the enclosing action
still advances the world turn.
Save-patched solid-square fixtures independently pin the mine and radio-bomb failure paths.
`FUN_1008_a732` also treats an empty detonator as a successful consumable action and selects the
recovered radio-beep sound rather than silently rejecting it. Case 9 leaves `local_6` set, so the
common `FUN_1008_1fe2` tail consumes one detonator whether or not a charge existed; zero stock
removes its inventory slot. Smart Card, Moon Key, Emerald, and
Bronze Key failures now expose the common exact "Nothing happens." feedback without consuming the
item.

Projectile-to-board damage now follows the common portion of `FUN_1008_77d0`: type-1 and type-2
impacts pass strengths one and two to board mutation only for impassable tiles without protection
bit four. Charge detonation applies strength three over the recovered `FUN_1008_a7a8` footprint.
Ordinary destructible squares expose tile zero; 3x3 device tiles 8 through 0x1f route through the
shared guarded-object callback described below so they are never partially corrupted.

The common collectible handler was recovered from the previously unmarked vtable target at
`1008:a16a`, including its 17-entry dispatch table. Player movement now resolves overlays 1–17,
removes collected nodes, applies the health pickup, and maps every fixed collectible to its
original arsenal type. Random containers replay the three data-segment item tables at 1080:14ce,
1080:14f0, and 1080:1518 with the recovered LCG. The item-stock helper at `1008:a3d0` and merge
routine at `1008:26ec` are translated, including per-item random quantities, the type-8 radio-bomb/
type-9 detonator pair, and the type-15 nested random-container effect. This makes inventory types 7 and 10
obtainable through normal board pickups rather than only existing as dormant reconstructed cases.
The health branch increments current health without clamping it to the initial maximum of three,
matching the direct `inc` at `1008:a32d`. DAH64 validation now accepts the resulting positive
16-bit range, and an above-three pickup/save/reload fixture prevents the portable reader from
rejecting legitimate native state.
Correction (September 2026): `1008:a108-a113` requires exact player/pickup coordinate equality.
The former adjacent-solid-tile collection workaround was not original behavior and has been removed.
Treasure above Crud stays in place until the obstruction is cleared and the vessel enters its cell;
the `1008:1b22` caller does not collect on stationary Wait actions.
`FUN_1008_a16a` also restores its pickup notices: fixed keys, Smart Card, Emerald, Casino Chip,
and health use their exact strings, while `FUN_1008_a3d0` constructs the original quantity-aware
"You found a/some ..." inventory message.

The common Bronze Key/chest path at `1008:bc64` now normalizes any of tiles 0x7f–0x82 to the
complete 2x2 chest, emits its four distinct key-turn effects, removes all four tiles, and invokes
the board's vtable +0x3c treasure callback at each former chest square. Its one-in-five intact-key
result and both recovered messages are preserved. Selector 1060 separately accepts the Moon Key
for its Moon Room door.

Selector 1038's inherited item callback `1010:0000` is translated. Smart Card type 0x0e is accepted
only at the Power Plant reader at (20,28), advances both reader tiles, clears the three-square
security barrier at column 23, produces the original destruction effects and message, and is
consumed only on a successful interaction.

Selector 1038's `FUN_1008_ec6e` damage override now recognizes active moving-target tiles
0x87–0x8f. Strength-one shots leave them untouched; bomb and missile hits retain the target tile,
add the recovered impact effect, and increment the appropriate upper/lower target counter. Hits
one through nine award 2500 points. Both counters are retained in DAH64 version 11, while versions
2–10 remain readable.

Selector 1038's missing `FUN_1010_00ec` turn callback is now active. Both 3x3 targets recover their
distance-sensitive corner defenders, missile-proximity response, one-in-three pursuit step,
collision avoidance, board bounds, strength-three terrain clearing, and conversion from initial
0x90–0x98 art to the active 0x87–0x8f target tiles. Projectiles caught by a moving target are
removed; missiles retain the first-nine-hit score bookkeeping and nearby radio charges retain
their difficulty-scaled interception roll. A serialized fixture holds unrelated actors stationary
and proves that an initial target relocates into a complete active tile set.

Selector 1038's coordinate-aware description callback `FUN_1010_0b4e` is also represented. Every
square in the upper moving-target rectangle reports the complete recovered Power Plant access
label, while the lower rectangle identifies Gormaug with his original title and description.

Selector 1038's vtable +0x44 override `FUN_1010_0bd2` now drains active Repellent by ten units per
accepted turn. Other boards retain the common `FUN_1008_c55c` two-unit drain. Deterministic item-use
fixtures pin both rates from the same initial energy value.

The Power Plant's vtable +0x48 Smart Bomb override `FUN_1008_eec2` is restored as well. Actors in
the central force-field rectangle survive unconditionally, every actor outside it independently
has a one-in-two resistance chance, and only the remaining actors receive the common removal,
captain split, and debris behavior. The two recovered mitigation/shield messages retain their
native priority. A save-patched fixture proves both protection mechanisms in one deterministic use.

Inventory type 0x13 is the extracted Bronze Key, not a drill. Its non-chest branch in
`FUN_1008_1fe2` accepts an adjacent tile-0x5c door, clears it, leaves overlay 0x1d, and preserves
the key on the recovered one-in-four result while playing sound-table entry 0x12. Both exact door
messages are restored. Attempts against other ordinary tiles remain rejected. Child-board travel belongs to the U/D transporter command
and the vtable +0x14 callbacks (`FUN_1010_5062`, `FUN_1010_c8cc`, and `FUN_1008_dbdc`); it is no
longer conflated with using a key on a 2x2 chest.

The shared destruction callback is now correctly identified as `1008:7db4` through the NE
selector-relocation chains. Its `FUN_1008_bfc4` branch is translated: hits on perimeter tiles
8–0x17 locate the 3x3 origin, inspect center types 0x18–0x1f, apply the original guarded/even
strength rules and one-in-twenty resistance rolls, then clear the complete object with nine debris
overlays. Ordinary impacts create overlay 0x14 and use sound entry 12; tile 3 retains its
one-in-fifty item drop, tile 0x5c resists strength one, and strength-two impacts spawn the original
eight secondary type-1 projectiles. This replaces the earlier conservative policy that left every
multi-square object intact.

Accepted weapon actions now complete the same turn pipeline as movement in `FUN_1008_1b22`:
projectiles advance, actors take their turn, player collisions resolve, and the active board's
runtime callback executes. Selector 1050's `FUN_1010_518c` runtime has its two transporter watches:
destroying top-left tile (33,5) or (5,29) performs seven recovered LCG placements of tile 0x22 and
overlay 0x16, excluding the player's square. Its post-800-turn flood performs twenty-four such
placements and removes the board actors. The constructor words at offsets 0x54/0x56 are now
confirmed as a 32-bit countdown initialized to 24. On each of those turns the default difficulty
makes four independent one-in-two rolls for fixed type-4 vector hazards; expiration sets every
actor to hunting behavior and replaces all sealed tile-0x20 squares with tile 0x21. The countdown
is persisted in DAH64 version 4, while versions 2 and 3 remain readable with their constructor
default. Separate fixtures verify the early release and isolate the later turn-801 flood.

Selector 1068's `FUN_1010_6f7a` per-turn mechanism is now represented. The four guarded structure
centers at (9,8), (31,8), (9,30), and (31,30) are watched independently; destruction advances the
exact spawn divisor 25→10→2→1→0. While any structure survives, the recovered one-in-N roll spawns
a type-0/type-1 actor at each surviving center with the original secondary roll. Once all four are
gone, the runtime makes up to twelve RNG-positioned type-0x18 3x3 structures, excludes the player's
square, enforces empty placement, caps the shared search at 200 attempts, and creates all nine
overlay-0x16 construction effects per object.

The native player state now preserves the original 32-bit score. Enemy removals award the
`FUN_1008_ad16` values (10, 15, 100, 250, and 5 for actor types 0–4). Destructible objects and
special tiles use the complete `FUN_1008_ad60` table: centers 0x18–0x1d award 100, 500, 350,
1000, 2500, and 7500; tile 0x20 awards 50; and tiles 0x87–0x8f award 2500. Centers 0x1e/0x1f
remain intentionally scoreless. Score resets on a fresh load and is exposed for UI and save-game
integration.

The recovered score is also rendered in the arsenal status strip and refreshed with the rest of
the arsenal after every accepted turn, connecting the `1008:2840` mutations to the visible status
surface represented by `1008:4f4e`.

The arsenal renderer now decodes and displays the null-terminated item names stored at byte four
of every 34-byte descriptor, alongside the recovered sprite index and quantity. All twenty-two
original labels—from Bubble and Radio bomb through the four keys, Emerald, and the final Lucky
Chip record at `1080:1000`—therefore remain available when pickups append inventory types that
were absent from the starting twelve slots.

Inventory type 12 (Repellent) now restores the player fields set by `FUN_1008_1fe2`: activation
loads 100 energy units, and the shared vtable callback at `1008:c55c` subtracts two after each
accepted action and clears the state at zero. `FUN_1008_2d50` adds sprite offset 0x18 while the
state is active, so the native player uses the original repellent animation frames for the same
fifty-turn lifetime. A raw-reference audit found no additional collision or actor-AI reads of the
flag; its original observable behavior is this timed player-state/animation change.

The rest of `FUN_1008_2d50`'s player renderer is now complete. Normal right/left facing uses
the recovered eight-frame 0x15a/0x152 cycles, holding Shift selects the corresponding four-frame
0x162/0x166 attack sequence and clamps at its final pose until release, and terminal root death
uses the three-frame wreck cycle at 0x3e. Repellent affects only the normal walking branch, exactly
as in the original. `FUN_1008_1b22` updates horizontal facing before action validation, so a
left/right arsenal command—or even a blocked horizontal movement command—turns the vessel before
paint; vertical commands preserve its previous orientation.

Inventory type 13 (Smart Bomb) now invokes the recovered common vtable target `1008:ca6a`.
Ordinary actors are removed globally without score and leave overlay-0x14 effects. Type-3 actors
first execute `FUN_1008_bc2c`, producing one to seven type-2 actors at the same square with
behavior 2, so the moving hazard rings split rather than vanish. The shared destruction sound is
raised only when at least one actor existed, matching the callback's guarded sound call.

The selector-1038 board no longer stops at its segment-8 map template. Its `FUN_1008_e07a`
post-load constructor now applies the four `FUN_1008_bf2a` blocks, alternating perimeter tiles,
seven `FUN_1008_c1e6` 3×3 objects, the sixteen-unit inner hazard ring, two type-2 actors, special
overlay 0x2d, player-start overwrite at (14,32), lower corridor edits, and the two 0x90–0x98
moving-block seeds. Regression checks pin representative coordinates and the constructed actor
count and verify the ring's exact clockwise vector state so later board work cannot silently
regress the native layout. Asymmetric object and actor coordinates explicitly account for the
original helpers' row-before-column calling convention.
The selector-specific damage override at `1008:ec6e` now recognizes all four 0x51–0x54 Energy
Sphere quadrants. A bomb-class hit removes the complete 2x2 sphere, drops its linked barrier cell,
adds the recovered destruction effects, and reports the original “You destroyed the Energy
Sphere!” message instead of treating one quadrant as an unrelated ordinary wall.

Selector 1040's `FUN_1010_0ea0` initial constructor is also represented: its six type-3 overlays,
two large objects, two `FUN_1008_bf2a` structures, seven top-edge transporters, three large-object
stations, five special overlays, and diagonal tile mutations are applied after the level-2 map is
decoded. All nine randomized constructor squares now consume the exact recovered ranges through
the MSVC LCG, and `FUN_1010_1c1e`'s one-or-two initial wild-card rolls are replayed to keep
subsequent random state aligned. `FUN_1010_28ac` chooses the upper or lower retaliation machine
through that LCG, opens its exact 5x5 boundary on the recovered five-turn cycle, clears effects
inside the region, and restores the eight edge/corner tile forms. The machines remain dormant
until a qualifying casino win requests them. Their sequence state is stored in DAH64 version 3;
version-2 saves remain readable with the original unstarted default.
The class's pre-movement override `FUN_1010_22c6` now enforces the recovered force line at row
three for columns 10–31 outside difficulty three. Northward movement is cancelled, a valid
horizontal component of diagonal input is retained, and the exact Rubble Roadster deflection message is
reported.

Selector 1040's casino callback `FUN_1010_19a0` now intercepts arsenal use on the four betting
squares. Its exact admissible item classes (2–11, 15, and 19), closed/dealt/unsupported/mixed-item
rejections, twenty-item ceiling, first-bet lever animation, accumulated-bet message, item
consumption, and sound are represented. The active wager's status, count, and item type are stored
in DAH64 version 12; versions 2–11 remain readable.

The first live casino phase in `FUN_1010_1180`, `FUN_1010_1cbc`, and `FUN_1010_1126` is restored.
Stepping onto the activated lever constructs the original ten-card unique deck through the MSVC
LCG, deals its first five one-based rank/suit records, creates the five card and hold-control
overlays, and advances the table to its dealt state. Moving onto any hold control toggles that exact
card and exposes the recovered Held/Unheld identification. Deck, hand, hold, and draw-index state
are retained in DAH64 version 13; versions 2–12 remain readable.

The second lever pass now redraws only unheld cards from the remaining five deck entries, clears
all hold controls, and evaluates the resulting hand through the recovered `FUN_1010_1da4` category
order. Royal flush, five-of-a-kind, straight flush, four-of-a-kind, full house, flush, straight,
three-of-a-kind, two-pair, and pair use their exact ordinary/deuce-wild/seven-wild multipliers;
wild hands are evaluated by the same missing-rank/suit constraints expressed by the native helper
chain. Winnings are returned in the wagered inventory type, the original win/loss messages and
sounds are surfaced, and the table resets for another wager. The selected wild rank is retained in
DAH64 version 14; versions 2–13 remain readable. Deterministic regression coverage pins the
ordinary royal-flush 200-to-1 result. The high-win pit-boss, bonus-room, force-field, and casino
closure branches are also restored. Returns above one-to-one now start `FUN_1010_28ac` at the
exact mode derived from wager size, multiplier, and the 500/1500 score thresholds, with the four
recovered pit-boss/Mob warning variants. Twelve-times returns advance the persistent left prize,
right prize, and treasure-room sequence; the third such award closes the casino and removes both
recovered force-field regions. The bonus stage is retained in DAH64 version 15, while versions
2–14 remain readable.

The casino's class-specific `FUN_1010_23ce` paint callback is now represented instead of drawing
the five card objects as ordinary overlays. Each hand uses the recovered 3x4 border, rank, suit,
red-suit rank offset, held-card border offset, face-down backing, and deuce/seven-wild label sprite
indices from the original atlas. Starting a new wager after a resolved hand also removes the old
card objects before the next deal, preventing stale hands from remaining visible.
The wager, draw lever, winning-hand, and losing-hand paths now dispatch their four dedicated
`SNDCASINO*` wave resources instead of temporary generic explosion/pop/death effects.
`FUN_1010_26fe`'s post-result lifetime is restored as well: a completed hand remains visible for
its recovered 102 animation callbacks, then `FUN_1010_1c1e` clears the old card objects and rerolls
the next table's ordinary/deuces-wild/sevens-wild mode. Betting again during that interval performs
the same reset immediately. The partially elapsed display timer survives DAH64 version 16 saves;
versions 2–15 remain readable.

Selector 1048's fixed `FUN_1010_4044` state is now translated as well: both `FUN_1008_bf2a`
structures, its edge and vertical tile edits, special object and overlays, one central actor, and
all forty vector-driven perimeter actors are constructed with preserved movement records and
their independent type-0/type-1 LCG rolls. The optional top/left overlays and otherwise constant
central actor also consume their original constructor rolls, and the three procedural
`FUN_1010_4a0e` placements use their exact inner-board ranges, twenty-attempt cap, recovered LCG,
and exclusion rules for transporters, existing 3x3 objects, and the player square.

The replenishment portion of selector 1048's `FUN_1010_4692` turn callback is connected as well.
At the default difficulty it checks every fifty turns, counts center tile 0x18 only in the original
inner region, and recreates a missing structure through `FUN_1010_4a0e` with nine overlay-0x16
construction effects. A serialized destroyed-center fixture verifies both the replacement and its
effect footprint without exposing test-only mutation APIs.
The four `FUN_1008_cdbe` corner updates are also connected to the native actor records. The forty
vector-driven perimeter guards now turn clockwise at (28,24), (39,24), (39,35), and (28,35)
without falling into hunter behavior at the first corner.
Selector 1048's common structure-spawn inputs now use the constructor's `difficulty+24` chance and
`difficulty` group size through the turn-six callback, then drop to the recovered `difficulty+4`
chance. This replaces the generic ten-percent/single-actor fallback previously used on this board.
The same callback's lower-machine branch is restored: reaching the lever at (38,19) while its
sentinel wall form is closed clears the inner chamber, installs the eight recovered edge forms,
activates the lower 0x18 structure, emits construction effects, and releases the original
type-0/type-1/type-2 groups at (21,32) with their difficulty-scaled LCG counts. The changed
sentinel tile makes this event naturally one-shot and save-compatible without additional state.

The common square callback in `FUN_1008_ba30` now raises navigation only for accepted movement
actions, matching the original exclusion of weapon turns. The recovered selector-1048 graph edges
0x27→selector 1060, 0x28→selector 1068, 0x29→selector 1040, and
0x2a→`FUN_1010_cb9c` are represented. Selector 1060's `FUN_1010_61e2` returns to its parent through
its 0x23 entry, enters the shared selector-1058 board through 0x27, and lazily creates selector
1050 through 0x28. Selector 1068 likewise returns through 0x23 and reaches selector 1058 through
0x27; selector 1058's 0x23 and 0x24 links lead back to selectors 1060 and 1068 respectively.
Native transitions retain the complete
parent `GameData`, copy global player/inventory/score/RNG/law state into the child, enter through
the first 0x23 connection square found by the reconstructed `FUN_1008_b946` rule, and restore the
parent on return at its original 0x27–0x2a square. Regression checks pin each translated edge.

The random-number chain is recovered exactly: `FUN_1000_1f6e` advances the classic Microsoft
32-bit LCG by `state * 0x343FD + 0x269EC3`, and `FUN_1008_3c88(n)` returns its upper 15 bits modulo
`n`, plus one. Native behavior-2 actors now use that generator for the original eight-attempt
random-neighbor selection instead of incorrectly following the player.

Attract-mode initialization now replays `FUN_1008_169c`: its five randomized inventory quantities
consume the original rolls, the sixth selects selector 1040, selector 1038, `FUN_1010_33b4`, or
selector 1078, and the resulting RNG state flows into that showcase board. The four selector-1080
command tables are decoded exactly, including their shift flags and the original ten-tick initial
pause; reaching a table sentinel starts another randomized demonstration. Normal input is blocked
during this mode, while New Game exits it.
Raw `1010:dafe` confirms the distinction: its sentinel branch destroys the current showcase and
calls `FUN_1008_169c` again, then resets the command index. It never invokes the procedural
`FUN_1008_0630` path automatically; a playable expedition begins only through New Game.
The earlier gate in `FUN_1008_6f46` is restored as well: attract boards accept only literal `N`,
`L`, and `P`. Other command bindings return before dispatch, and `N` enters the difficulty dialog
without the abandon-current-game confirmation used by a live expedition.

The playable New Game path is separately restored from `FUN_1008_0630`. It constructs generic
depth one (native level 17) with the original compact Bubble/Radio-bomb/Mine-or-Bomb arsenal and
passes its three inventory rolls into procedural generation. Regression tests distinguish both
initializers and pin the deterministic seed-one attract branch.

Selector 1050's `FUN_1010_4f76` constructor now clears the original bounded tile-0x21 region,
places both `FUN_1008_c452` transporters, and creates the default-difficulty set of six scattered
type-1/type-2 pickups with the recovered RNG. Overlay state is exposed read-only for constructor
regression checks. Its `FUN_1010_518c` runtime now performs both transporter-triggered spawning
sequences and the turn-801 flood, including actor removal and all twenty-four hazard overlays.

Selector 1058's `FUN_1010_55ac` constructor now creates its three guarded type-0x19 structures,
twelve surrounding type-1 actors, fixed exit and overlay state, and six lower-edge pickups. The
intermediate adjacent-tile-3 random probe is replayed against the decoded board so subsequent RNG
consumption remains aligned. `FUN_1010_5a46` is now restored as the class's movement/runtime
callback: the switches at (37,2) and (37,3) alternately close and restore their exact 3×3 machinery
chambers, while weapon turns and stationary waits do not operate them. Destruction of either saved
probe square is observed, the second square becomes tile 0x5d, and reaching the breached pair
activates the original 17-cell mechanism with construction effects. Probe coordinates and all
one-shot flags are retained in DAH64 version 7; versions 2–6 remain readable.

Selector 1060's `FUN_1010_5fc4` constructor now applies its eight stacked overlays, central tile
edits, two type-0x1f structures, exit/transition squares, and the default-difficulty sixty type-4
random actors. Tests pin the first repeated 12-byte actor record. Its 0x28 transition now enters
the selector-1050 board. Its `FUN_1010_6308` runtime now watches both breakable transporters and
the central machinery, releases the recovered type-2 actor groups, opens the two hidden center
squares, operates both paired lower-right seals, and retains all four one-shot flags in DAH64
version 8. The original selector-specific structure spawn chance and group limit are also active;
after turn 1000 its one-in-100 event constructs three new guarded objects and raises that chance
to 50. Versions 2–7 remain readable.
Selector 1060's other class override, `FUN_1010_68a8`, now accepts the Moon Key only when aimed
at the center door square (10,17), clears all three vertical door tiles with the recovered effect,
consumes the key, and emits the exact "The key turns... The door opens." message.

Selector 1068's `FUN_1010_6a56` constructor uses the exported data-segment constants
`DAT_1080_192a=19` and `DAT_1080_192c=18`. The native board now builds the 0x70–0x78 central
device, six RNG-sized actor groups, four distant type-0x1f structures, lower alternating gate,
two overlays, and final type-0x1e exit structure. `FUN_1010_6f7a` now observes destruction of each
guarded structure, advances the original 25→10→2→1→0 spawn divisor, emits surviving-structure
actors on the recovered RNG condition, and enters the twelve-object end phase when all four are
gone. End-phase object placement uses the original column-36/row-32 call order and debris effects.
Its class-specific item callback `FUN_1010_7258` is also restored. Using the Emerald against any
square of the central 3×3 idol changes the four animated tile families to 0x79–0x7c, opens all
five lower gate squares with destruction effects, consumes the Emerald, emits both original
messages, and awards 3000 points. Other boards and targets continue to reject the key item.

The selector-1078 map used by `FUN_1010_d8f4` now receives its two constructor-time 3×3
structures at the recovered asymmetric and guarded coordinates. The remaining eighth extracted
map and the procedural board constructors are tracked separately rather than being assigned to a
top-level selector without call-site evidence.
Its `FUN_1010_d968` runtime now emits the three recovered timed enemy waves: a type-0 group at
turn five, four alternating type-0/type-2 groups at turn 28, and the final type-0/2/1/3 groups at
turn 181. Each wave retains its exact coordinates, random group-size bounds, and RNG call order.

The eighth gameplay class is now correctly treated as procedural rather than mapped.
`FUN_1010_c80e` starts from a blank board, invokes the recovered `FUN_1008_9504`/
`FUN_1008_9594` LCG-driven terrain-growth algorithm twenty times, clears its two exact central
rectangles, places the start at (20,7), and builds the original 4×3 transporter grid. The unused
eighth extracted map template is no longer mistaken for this board's initial state.
Its `FUN_1010_c9f8` callback now detects the first breached member of that transporter grid,
emits the exact alarm message, and uses the shared 200-attempt budget to construct twelve guarded
objects. The event raises structure spawning from six percent/single actors to ten percent/groups
of four, and its one-shot state is retained in DAH64 version 9; versions 2–8 remain readable.

The previously missing `FUN_1010_33b4` class is represented as native level 10 because it has no
embedded map resource. Its constructor starts from a blank 40×36 board, replays five exact
terrain-growth seeds and six empty-square placements, then restores the recovered rooms, central
0x83–0x86 device, four guarded structures, gates, overlays, and randomized connection square.
The main chamber and its upper passage preserve `FUN_1008_ce88`'s row-before-column parameter
order; this prevents their far corners from being transposed into the previously rendered wide,
shallow clearing.
Selector 1050's 0x27 connection now lazily enters this board through `FUN_1010_5062`; stepping
back onto the generated 0x23 square restores the retained parent board. Constructor and graph-edge
regressions cover this path.

That class's previously disconnected post-turn callback `FUN_1010_37a2` is now restored too.
Entering the central 2×2 device clears tiles 0x83–0x86, grants exactly one class-20 Giant Emerald,
and emits the recovered pickup text. Destruction of any of its four guarded corner sentinels clears
every tile-0x21 fortress seal and changes tile 0x20 to 0x22 only inside columns 9–12 / rows 19–22.
Both phases are one-shot and survive DAH64 version 21 saves; versions 2–20 remain readable and arm
the newly reconstructed phases so an old native save can catch up. Save-patched fixtures prove the
coordinates, inventory award, exact message, tile filters, and persistence independently.

The final selector-1048 branch, procedural `FUN_1010_cb9c`, is represented as native level 11.
At its recovered fixed depth three and default difficulty one, the constructor replays randomized
type-2/type-7 pickups, raw 0xaf terrain, three conditional guarded-object attempts, two-to-four
terrain-growth passes, and collision-checked randomized 0x23/0x27 connection squares. Its entry
mode also restores the sign two cells diagonally from the return square. The 0x2a parent edge and
0x23 return edge are regression-tested, completing selector-1048's four-way dispatch.
Its `FUN_1010_d06a` runtime now restores all four depth-three escalation phases: structure spawn
pressure rises after turns 150 and 300, one guarded object with nine construction effects appears
at turn 490, and eight more appear at turn 800 while the spawn chance rises to fifty percent.
The phase bitset is retained in DAH64 version 10, with versions 2–9 remaining readable.

The common `FUN_1008_81dc` structure-spawn pass is active on every board. Each surviving center
from 0x18 through 0x22 consumes its original one-through-100 roll, with 0x21 suppressed and 0x22
using four times the board rate. The 0x18/0x19, 0x1a/0x1b, and 0x1c/0x1d families reproduce their
progressively stronger enemy mixes and preserve the original extra RNG rolls; 0x20/0x22 emit
type-4 actors. The 300-actor cap and randomized group size are retained.

Procedural depths now retain `FUN_1008_d4e0`'s difficulty-derived spawn chance, group size, and
terrain-break divisor instead of falling back to the base class's 10/1/20 values. Their
`FUN_1008_dd76` runtime restores the turn-50 and turn-150 rate changes, the depth-scaled first and
second guarded-structure waves, and the final 25/10 then 40/5 spawn/break settings. These values
and the four one-shot phase flags are persisted in DAH64 version 17; the serialized threshold
fixture verifies the common spawn and hunter passes will consume the recovered state next turn.

The saved-game class factory's final-arena class `FUN_1010_d33a` is represented as native level
12. Its constructor restores the patterned perimeter and lower chambers, three framed connection
points, start/sign/exit squares, the complete 12×10 RNG-selected guarded-object grid, thirteen
overlay-grid entries, and three lower transporters. Its `FUN_1010_d71e` runtime counts the remaining
0x18/0x19 grid centers and raises the structure spawn rate through the recovered 120, 100, 60, 30,
20, and 10-object thresholds, including the difficulty-scaled final group size. This class is
procedural and consumes no map segment; fixed geometry and dynamic record counts are covered by
constructor regressions.

The three `FUN_1010_86a0` maze stages are represented as native levels 13–15. The reconstruction
includes `FUN_1008_c5b0` and its `FUN_1010_90bc`/`FUN_1010_91a2` randomized recursive carving
order, the `FUN_1008_c662` eleven-way wall autotiling pass, the central chamber and guarded
structures, stage-dependent passage widths, randomized actors/pickups/transporter attempt, and
collision-checked connection placement. Tile 0x27 advances through stages one and two, while the
stage-one 0x28 branch enters `FUN_1010_cb9c`; all stages return through 0x23. Regression checks
cover constructor geometry, dynamic counts, both stage-one branches, and a traversable route from
the entry region to the generated exit.

Selector-cb9c's previously disconnected tile-0x27 callback `FUN_1010_ce62` now follows its exact
child factory: a one-in-eight branch creates selector-c80e, the next branch delegates to
`FUN_1008_2b54`'s one-shot final-arena/maze choice, and the fallback recursively creates another
cb9c board. Each cb9c exit caches its selected class, while the two factory-use flags travel with
the player graph and persist through native saves. Final arena callback `FUN_1010_d5d4` now exposes
its tile-0x27 cb9c edge as well. Seed-patched tests distinguish all four ce62 outcomes and prove
that repeated activation reuses the chosen target.
Their shared `FUN_1010_8f56` turn callback now scans the player's complete 3×3 neighborhood and
advances maze wall forms 0x2b–0x35 into their active 0x36–0x40 variants before invoking the
inherited square callback. A serialized fixture pins the transformation independently of random
maze construction.

Maze stage three's 0x27 edge now enters `FUN_1010_3a96`, represented as native level 16. Its
constructor runs the recovered width-one recursive maze and wall-autotiling sequence, restores the
four cleared chambers and three guarded structures, places the fixed start/sign pair and randomized
type-3 actor, converts ordinary maze terrain to the 0x5d/0x5e set, then applies `FUN_1010_3ece` to
reactivate and retile only the 5×5 entry chamber. Tests pin the chamber geometry, alien passage
population, actor bounds, and the stage-three traversal edge.

Its `FUN_1010_3d50` post-turn callback is now connected. Destroying the guarded centers at (33,7),
(33,29), and (7,29) independently converts and autotiles their exact dormant alien quadrants.
After all three are open, the central columns 17–23 / rows 15–21 region activates, tile (20,16)
becomes 0x5c, an overlay-4 Emerald appears at (18,20), and three plus a one-through-three roll of
type-2 defenders are released at (20,18). Contact also clears a dormant 0x5e passage exactly as the
callback's leading branch does. DAH64 version 22 preserves the three chamber flags and final core
phase; versions 2–21 migrate with the absent phases armed. Regression fixtures pin all coordinates,
the actor bounds, overlay, conversion, and one-shot reload behavior.

The depth-one-through-nine/default-difficulty forms of the generic recursive board
`FUN_1008_d4e0` are represented as native levels 17–25. Their constructors replay the conditional
type-1/type-2/type-7
pickups, transporter and guarded-object attempts, actor creation, `FUN_1008_9504` terrain growth,
optional bordered room population, collision-checked 0x23 entry with its depth-one sign, and the
randomized 0x27 forward connection. Depth-dependent attempt counts, guarded-object cap, actor
branches, first room-count range, and the depth-five second-room/transporter branch are preserved.
The deterministic pre-depth-five transition policy advances 17→18→19→20→21. From depth five,
`FUN_1008_dbdc` consumes its original one-through-eight roll to choose another generic depth only
when the roll is at least the current depth; otherwise it enters selector 1048. Consequently depth
nine necessarily terminates the recursion at selector 1048. Tests pin the seed-one depth-five
choice and the mandatory depth-nine boundary.
| `1008:0c70` | Saved-game open path | References `Saved Game *.VKW` |
| `1008:0d46` | Saved-game save path | References `Saved Game *.VKW` |
| `1008:40c0` | Registration/validation transform | References three deliberately irregular key strings |
| `1008:b164` | Board-square validation/error path | References `Invalid Square Value:` |
| `1010:a64a` | High-score load/error path | References `Unable to load high scores.` |
| `1010:a966` | First laws/help page | References the first long rules block |
| `1010:a9ee` | Second laws/help page | References the second long rules block |

## Native target

- Language: C++20
- Architecture: x64 only
- UI: direct Win32 window/message APIs
- Rendering: GDI-compatible path initially, preserving the original palette and raster behavior
- Audio: WinMM-compatible playback of the original RIFF/WAVE resources
- Original client dimensions recovered from `1008:6d30`: 636×468

Every reconstructed subsystem should retain an address-to-symbol mapping in this document or a
generated manifest. A routine is considered reconstructed only after its inputs, state mutations,
calls, and observable behavior have been checked against the original.

The native File menu now reconstructs the `1008:0c70`/`1008:0d46` common-dialog behavior and the
`.VKW` filter. Individual boards use a versioned `DAH64` payload because the original recursive format
embeds Win16 class records, far pointers, and compiler-specific object bytes that cannot be loaded
verbatim into a 64-bit process. The replacement serializes the complete mutable gameplay state:
board cells, overlays, actors, projectiles, inventory and selection, player state, score, RNG and
turn counters, and selector-specific runtime flags. Loading is transactional and validates the
signature, version, record bounds, coordinates, tile indices, health, and selection before changing
the running game. Round-trip and corrupt-header regression checks cover this compatibility boundary.

Application saves now wrap those payloads in a `DAG64` graph container. Version 2 records every
retained parent in traversal order, each exact connection square, the active child, and the unique
cached instances of selectors 1048, 1058, 1060, and 1068. Cross-links therefore restore the same
mutated board instead of constructing a duplicate with the wrong parent. Loading is transactional,
caps ancestry and cache sizes, rejects duplicate or mismatched cache records, and still accepts
version-1 DAG64 and standalone DAH64 files. The graph regression traverses
1048→1060→1058→1060→1048 across repeated saves and proves that shared instances and their root
relationship survive serialization.

The native shell now also exposes a complete new/open/save lifecycle. New Game rebuilds the original
random starting inventory and starting-board selection and discards retained board ancestry only
after confirmation. Ctrl+N, Ctrl+O, and Ctrl+S dispatch the corresponding commands. The two recovered
system-menu entries now open native Help & Information and About dialogs instead of being inert;
the help text documents the reconstructed eight-direction movement, mouse movement, arsenal
selection, aimed-item controls, and game-file shortcuts.

The New Game Skill prompt now reproduces the recovered `DIFFICULTYLEVEL` resource: its original
caption, MS Sans Serif metrics, four radio labels, group-box geometry, and single `Begin` button.
`FUN_1008_3fd6` initializes Skill to Beginner before opening the dialog and ignores the dialog return
code, so closing the window also begins a Skill 1 game; the native flow preserves that unusual rule.

The `Q`/Quit Current path now follows `FUN_1008_1efe` instead of using a generic restart prompt. It
asks the recovered “Do you want to save the current game?” question with Yes, No, and Cancel. Cancel
leaves the run untouched; Yes offers the save dialog and then starts a fresh run without submitting
the saved score; No immediately resets and submits a qualifying abandoned-run score. Both reset
paths retain the selected Skill and rebuild the original randomized playable start.

The zero-health path from `FUN_1008_2920` and the post-action checks in `FUN_1008_1b22` is now a
complete lifecycle transition rather than a frozen root board. The native shell snapshots the final
score, chooses the same sub-1,000 / sub-20,000 / upper score tier used by the original, displays the
exact recovered tier text, and rebuilds the random new-game inventory and root board. Both keyboard
and mouse movement surface pending gameplay and lifecycle messages after the accepted action. The
original death gate is preserved as well: a board whose offset `0x0c` script field is non-null is an
attract board and suppresses terminal death. That field is not the retained gameplay parent pointer.
Every playable board in the navigation graph therefore queues the lethal sample and runs the same
game-over lifecycle, including descendants and restored graph saves.

Those death-message thresholds now retain the original signed-long boundary as well. The high-word
checks in `FUN_1008_1b22` make a score whose wrapping 32-bit value has bit 31 set fall into the
sub-1,000 message tier, rather than treating it as a huge unsigned score. A lethal save-patched
fixture with score 4,000,000,000 proves both the low-tier message and high-score rejection through
the complete application lifecycle.

The sibling victory branch in `FUN_1008_1b22` displays the recovered “YOU ARE A HERO!” retirement
message when the application word at offset `0x100` is one. Later instruction-level recovery of the
previously undiscovered selector-1038 post-turn routine at `1008:e636` found the missing writer: it
sets that word after both Power Plant targets have been destroyed. The native endgame now connects
that writer to the shell lifecycle as described in the Power Plant section below.

The ten-entry table owned by `FUN_1010_edea` and updated by `FUN_1018_0252` is now represented by a
portable, versioned `DSH64` sidecar (`DRAIN64.SCO`). Scores qualify against the lowest occupied slot,
open a modal name-entry dialog corresponding to the extracted `HIGHSCORENAME` resource, are inserted
in descending order with the selected Skill, survive process restarts, and are
available through the native High Scores command. Missing tables initialize cleanly; malformed
signatures, versions, counts, names, and out-of-order records are rejected transactionally. The
regression suite covers qualification, ordering, round-trip persistence, display, and corruption.
The original `BB.BIN` layout is deliberately not reused: its 296 bytes interleave ten compiler-era
24-byte score records, 19 Win16 virtual-key values, and three persistent 32-bit counters.
The score record's word at offset 0x12 is now sourced from application offset 0xe4, which
`FUN_1008_0630` fills from the difficulty dialog. The native table therefore reproduces the
original `Skill` column (`EZ`, `2`, `3`, or `4`) instead of incorrectly labeling the active board
selector as expedition depth. DSH64 version 2 validates that range and migrates valid version-1
values, mapping impossible legacy selectors back to `EZ`.

High-score qualification now also preserves the signed Win16 `long` comparison in
`FUN_1010_ec94`. `FUN_1010_ec10` initializes every empty record to score -1, so a zero-point run
does qualify for an empty table; scores whose wrapped 32-bit representation has bit 31 set compare
as negative and do not. Application pending-score state uses an explicit flag instead of treating
zero as “none,” allowing the original zero-score name prompt and insertion path to complete.

The second recovered score-record flag is now retained too. Every submission passes application
word `0x100` into record offset `0x16`; on the hero ending that word is one, and
`FUN_1010_ed28` adds 10,000,000 with wrapping 32-bit arithmetic before signed ranking. DSH64
version 3 stores this hero flag, migrates versions 1 and 2 with it clear, and formats the same
effective ranked value. The other `ed28` flag would add 5,000,000, but application word `0xfe` is
initialized to zero and has no writer in this executable, so the native reconstruction does not
invent a trigger for it.

The score record's leading fourteen bytes and the name dialog are now matched at their actual
boundary. `FUN_1008_4014` calls `GetWindowText` with `nMaxCount = 0x0d`, retaining at most twelve
characters plus the null terminator; native normalization and the edit control use that same limit.
The original accepts an empty edit value, so native records now carry occupancy independently of
their name text. A blank-name score therefore remains an occupied, ranked, persistent entry instead
of being replaced with the invented name `PLAYER` or mistaken for an unused slot. The native dialog
also starts blank and uses the extracted `HIGH SCORE` caption, “Add your name to the High Score
Table” prompt, MS Sans Serif font, and original three-control geometry.

Inventory type 15 now executes `FUN_1008_2c08`. Its one-through-47 RNG selection, zero-based
duplicate suppression, and discovered-law set are preserved, and the selected selector-1020
80-byte record follows the preceding "It's a fortune cookie... It says:" notice in the arsenal message
history. `FUN_1008_554e` initially selects that introduction, so the M binding advances to the
law rather than the blank live-message slot. All 47 records were extracted directly from NE
segment 5, including the two repeated hints. The discovered set is also part of the versioned
native `.VKW` state so collecting and reloading cannot lose law-book progress.

Default binding 11 (`F`) now executes the recovered `FUN_1008_2c84` law recall. Discovery order and
the reverse-cycling cursor are retained in DAH64 version 6, duplicate laws remain suppressed, and
older saves reconstruct a deterministic order from their discovery bitmap.
Recall is again routed through `FUN_1008_67e6`: it appends the law through `FUN_1008_55e0` and
selects it through `FUN_1008_554e`, matching newly discovered laws in the review stream.

The board's `WM_RBUTTONDOWN` path (`FUN_1008_60f4`) now identifies the player, actors,
projectiles, overlays, drains, objects, and terrain using the original strings and precedence,
then appends and selects the result through the recovered `FUN_1008_67e6` history path.
Its opening board-owner check is retained as well: right-click identification is inert while the
attract showcase is playing and on retained child boards, matching the same non-null owner/parent
sentinel used by the original keyboard and mouse procedures.
The keyboard path also restores numpad 5 as a stationary turn, `C`/F1 help, and the unmodified
`N`, `L`, and `S` handling shown by the original command page. On a live expedition, literal `N`
and `L` display their exact “must quit this game” notices; on an attract board they start New or
Load directly, as gated by `FUN_1008_6f46`.
The same dispatcher’s hidden two-key latch is retained: Shift+PageUp at board coordinate (39,0),
immediately followed by the downward-drain binding, invokes `FUN_1008_6cdc`. Its `FUN_1008_40c0`
payload at `1080:102a` decodes to `(c) Copyright 1997 by\nDave Horner`.
Bindings 12 and 13 (`P` and `M`) now retain and navigate the message stream produced by gameplay
notices, matching `FUN_1008_55be`/`FUN_1008_5588`.
The entire 19-entry `FUN_1010_ed68` virtual-key table is now editable through a native Keyboard
Commands dialog. The defaults, movement/action dispatch, transactional validation, reset action,
and versioned `DRAIN64.KEY` persistence replace the keyboard portion of `BB.BIN` without retaining
Win16 object bytes.

Movement onto tile 0x7e now restores `FUN_1008_ba30`'s signpost dispatch through vtable +0x34.
All recovered fixed-board, procedural-board, casino-coordinate, Sewer City coordinate, and Moon
Room coordinate texts are preserved. The generic notice enters the review stream through
`FUN_1008_2822`, then the concrete sign callback adds the second history entry through
`FUN_1008_67e6`; M advances from the preface to the sign text. A patched
selector-33b4 fixture proves that actual movement invokes its "The Xoid Fortress." callback;
stationary turns do not reread the sign.
The procedural `FUN_1008_d4e0` class now also dispatches its previously omitted `1008:df52`
second entry, "Welcome to your shower drain. Hey, it's tiled!", on depths one through nine.

Application field `+0xee` in `FUN_1008_1b22` is now modeled as horizontal facing. Left/right
movement and arsenal actions select the `0x15a`/`0x152` sprite families; vertical commands retain
the prior orientation. DAH64 version 20 records this corrected state. Versions 2-19 remain readable
and default to right-facing because their byte stored the earlier, incorrect vertical interpretation.

Selector 1040's `FUN_1010_28ac` machine callback now releases its recovered type-0/type-1 groups,
adds the type-2 and optional type-3 groups on higher modes, opens each five-by-five enclosure, and
restores it after four, five, or six states according to machine mode. Skill four uses the original
highest (third) callback branch.

The common `FUN_1008_8310` hunter pass now uses `FUN_1008_adee`/`FUN_1008_93a4`'s complete target
metric rather than a player-only approximation. Actors minimize Chebyshev distance to the player
or any tile 7/0x74/0x7c lure; active repellent changes the player term to distance from a five-cell
ring, and equal candidates retain the original column-major scan and thirty-percent replacement
roll. Hunter-mode bulldozer/captain classes also recover the native one-in-20 destruction of tiles
3–6 and one-in-40 destruction of tile 7, including the debris effect, sound, and no-movement turn.
Serialized deterministic fixtures prove both lure diversion and terrain destruction. The divisor
is now selected from each concrete board's recovered state: selector 1048 uses 25 normally and one
during turns 180–199 of every 200-turn cycle, selector 1060 retains 80, selector 1068 changes 15
to five after its four structures fall, selector 1070 changes 20 to four after the transporter
alarm, and procedural depths use their escalating persisted divisor. Tile 7 continues to use
exactly twice the active board value. A save-patched turn-180 fixture makes selector 1048's
otherwise RNG-sensitive destruction transition deterministic.
The same actor pass now restores its stationary type-1 projectile collision: a creature stepping
onto a placed mine consumes the projectile and creates the native transient type-0x24 object.
The common `FUN_1008_aacc` follow-up then removes that marker, expands its 3x3 strength-one blast,
damages eligible terrain, scores and removes caught actors, preserves lieutenant drops and captain
splitting, and emits the explosion sound. Moving type-1 bombs continue through the distinct lethal
impact path. Save-patched stationary-creature fixtures keep the complete lifecycle independently
regression-tested. Actor contact with the player also restores `FUN_1008_286c`/`FUN_1008_2920`'s
complete contact sweep: every actor sharing the square is damaged and removed without skipping the
next shifted record, and each creates its own type-0x18 impact effect. Types 0, 1, 2, and 4 cost one
health while type 3 costs two; health clamps at zero, repeated nonlethal hits promote to the native
double-hit sample, and a lethal contact retains the higher-priority death sound. Save-patched
double-contact fixtures cover both the repeated-hit and mixed heavy/lethal paths.
Radio-bomb detonation now follows `FUN_1008_a7a8`/`FUN_1008_a980` square by square: diagonal and
axial two-cell extensions occur only when their nearer square accepts the blast, protected flag
combinations stop propagation, every accepted cell receives the explosion overlay, destructible
terrain uses strength three, and affected actors receive their native score and lieutenant-drop
roll before removal. A captain-in-blast fixture pins the recovered 250-point award.

Type-4 fireballs now reach the common terrain callback instead of disappearing harmlessly at the
first solid square. Their strength-four impact bypasses the guarded-object resistance checks,
clears and scores the complete 3x3 object, and invokes vtable slot +0x3c once per cleared square
for the original optional treasure roll. The common 1/2/7 distribution, selector 1050's denser
`FUN_1010_54ec` distribution, and the class-19 branches of the common and final-arena callbacks are
translated. `FUN_1008_1fe2` still rejects using the Bronze Key directly on ordinary object terrain;
its accepted targets remain tile 0x5c and 2x2 chests. Seed-patched common and
selector-1050 fireball fixtures independently prove terrain damage and distinguish the tables.

The actor-impact branches of `FUN_1008_77d0` now preserve their board mutations instead of merely
deleting a hit actor: type-0 projectiles fossilize squids and bulldozers into tiles 5/6 and 4,
lieutenants retain their one-in-ten treasure drop, captains split through the recovered one-to-seven
subordinate roll, and Xoid/bubble/bomb impacts select the extracted death, pop, and explosion waves.
The corresponding post-movement collision matrix in `FUN_1008_8310` is also restored for every
projectile class, rather than only placed mines. Actor-landed bubbles fossilize eligible creatures
and disappear harmlessly against demon ranks, moving bombs and missiles kill with their native
drops and captain split, missiles emit their eight-way bomb ring, and piercing fireballs survive
actor impacts. Type-0 fossilization now emits its missing type-0x16 transition effect in both the
projectile-first and actor-first paths. A stationary-fireball fixture proves the actor-first path,
score award, effect, player safety, and projectile survival independently.

The native keyboard and shift-click shells now drain gameplay feedback after every attempted item
activation, including rejected placements. This preserves `FUN_1008_1fe2`'s immediate "Nothing
happens." and placement-obstruction dialogs alongside the world mutations made by the enclosing
accepted turn.

`FUN_1008_6048`'s complete left-click gate is also restored. Only a root board accepts mouse
actions, clicked coordinates must lie within the 3x3 neighborhood centered on the vessel, and a
plain click on the vessel dispatches the native zero-vector wait turn. Shift-click uses the same
adjacency and root checks before entering the arsenal branch, so a distant click no longer fires in
its general direction. `FUN_1008_60f4` applies the same root gate to right-click identification, so
retained child boards remain entirely keyboard-controlled as in Win16. Root/child
status is reconstructed with the board graph after transitions and loads rather than written into
the portable board payload.
The same identification routine now distinguishes a moving type-1 Bomb from stationary Mines and
counts every stationary type-1 projectile stacked on the selected square, reproducing its native
`Mines (N).` description. Tile-descriptor bit one also regains its original precedence rule:
intrinsically identifiable terrain is described before an overlay on the same coordinate, while
ordinary passable squares expose their pickup or control node.

Lazy non-shared child pointers are now modeled as path-addressed board instances rather than fresh
levels on every entry. Returning through 0x23 caches the complete changed child; using the same
parent connection later restores that instance, including its turn state, actors, terrain, and
runtime flags. DAG64 version 3 persists these cached instances while retaining readers for versions
1 and 2. Recursive cb9c edges remain distinct because repeated connection tiles extend the path.

The complete 31-entry sound resource table loaded by `FUN_1008_2eba` and its `FUN_1008_31d4`
dispatcher are now native. Events retain the original numeric priority instead of overwriting one
another in call order; repeated pop, Xoid-death, and player-hit requests promote to the supplied
double/triple mixes. All extracted samples are addressable, including treasure, construction,
special-door, crackle, wake-up, and Yeow sounds. `FUN_1008_a16a` pickup sounds, `FUN_1008_cf50`
construction sounds, and the 1010:0000/1010:68a8 special-door calls are connected. Bronze-Key
doors and chests now correctly select table entry 0x12 rather than the previously mis-mapped casino
sample.

Both actor-collision phases now preserve the dedicated Fireball sound branch as well.
`FUN_1008_77d0` and `FUN_1008_8310` select entry 13 (`SNDCRACKLE`) when persistent projectile type
4 kills an actor; only ordinary damaging projectiles select entry 12's small explosion. A
pair of save-patched moving and stationary Fireball fixtures pins both collision phases
independently of terrain impact sounds.

The transient board-node painter in `FUN_1008_8b2c`/`FUN_1008_8be6` is now restored rather than
leaving explosions, construction flashes, fossilization, player impacts, and door effects on the
board indefinitely. Types 0x14–0x1d and 0x1f use their recovered five-frame sprite sequences and
are removed after frame five; persistent casino-card node 0x1e remains exempt. The native shell
advances these frames from its existing animation timer instead of reproducing the Win16 routine's
blocking `PeekMessage` loop. DAH64 version 18 appends the in-flight frame so a save made during the
modern asynchronous sequence resumes cleanly, while older saves containing transient nodes arm a
fresh five-frame cleanup pass.

The accepted-action pipeline now preserves the two distinct virtual callback phases in
`FUN_1008_1b22`. Selector 1038's `1010:00ec` vtable-`+0x4c` target runs after the player's immediate
collision check but before `FUN_1008_8310`; the board-specific `+0x1c` callbacks remain after the
actor pass. Defenders released by a moving Power Plant target therefore move and collide during the
same turn, as in Win16. Weapon and casino-bet turns also invoke `FUN_1008_81dc` structure spawning
inside their common actor pass instead of restricting portal spawns to movement. A seed-pinned
fixture proves a newly released target defender has already left its release corner when the action
returns.

Rejected arsenal effects now retain `FUN_1008_1b22`'s outer control flow. Once a directed Shift
action reaches `FUN_1008_1fe2`, blocked mine/radio-bomb placements, invalid board-specific keys,
the inert Star/Fire Key common cases, and the 150-projectile guard preserve inventory and apply no
item effect, but projectiles, `1010:00ec`, portal spawning, actors, the post-actor callback, and
repellent still advance. Turn-count fixtures distinguish this from an invalid zero-direction key
command, which never enters the item dispatcher.

The two nonstandard system-menu command words appended by `FUN_1008_6e4c` are dispatched exactly
as `MainWindowProcedure` does: `0xe998` opens Help & Info and `0xe999` opens About. They must not be
normalized with the conventional `SC_*` low-nibble mask because neither original identifier is
16-byte aligned; doing so made both reconstructed entries silently fall through to DefWindowProc.

The previously undiscovered selector-1038 post-turn routine beginning at `1008:e636` now completes
the Power Plant endgame. A mobile 3x3 target reaching ten registered hits becomes inactive, awards
the recovered 50,000-point kill bonus, advances each of its nine tiles by `0x12`, paints nine
explosion nodes, and selects sound-table entry `0x19`. Once both tracked targets are inactive the
routine raises application word `0x100`. `FUN_1008_1b22` consumes that flag before its root-only
death gate, displays the exact “YOU ARE A HERO!” ending, submits the final score and Skill, and
rebuilds a randomized playable expedition. Fixtures independently prove the one-target kill and
the second-target victory transition.

The preceding Power Plant gate in `1008:e636` is restored as well. Its two mobile targets now
remain dormant as tiles `0x90`–`0x98` until `FUN_1008_ec6e` destroys the Energy Sphere. Sphere
destruction accepts only impact classes one through three (not Fireballs), awards the missing
20,000 points, opens the linked barrier, and selects the large-explosion sample. It also invokes
the four recovered `FUN_1008_a7a8` blasts centered around the cleared 2x2 sphere, so the complete
guard ring is removed through the ordinary actor scoring path. On five-turn
boundaries each eligible target has the recovered one-in-two wake-up roll; the lower target waits
until twelve turns after the upper one woke. Wake-up changes all nine tiles to `0x87`–`0x8f`,
paints the nine transition effects, and enables `FUN_1010_00ec` movement and defender release.
DAH64 version 19 appends the first activation turn so the twelve-turn gate survives reloads;
it also retains the arena-entry phase independently of later ring damage. Versions 2–18 remain
readable and reconstruct both values from their board state.

The earlier `1008:e636` phases no longer remain inert. Its first accepted turn freezes the actors
in the recovered right-half rectangle, converts all four corner center tiles from dormant `0x1f`
to spawning `0x1d` structures with their nine-square construction effects, and changes the spawn
pair from disabled to `(Skill + 9, Skill + 5)`. Reaching the entry at (19,1) builds the complete
tile-`0x22` containment ring around the central arena, converts its resident vector actors to
hunter behavior, and changes the spawn pair to `(Skill * 2 + 3, 1)`. These phases run after the
actor pass, preserving the original first-turn sound priority and state transition ordering. Until
the Energy Sphere falls, missing containment segments are rebuilt with tile `0x22` and construction
effects every twentieth turn while the vessel remains outside the arena.

The four remaining `FUN_1008_cdbe` calls in the pre-entry Power Plant phase are now represented
explicitly. After the normal actor pass they redirect vector actors standing at (10,10), (15,10),
(15,15), and (10,15), preserving the original clockwise patrol around the Energy Sphere instead
of letting a straight-line vector cross a corner or degrade into hunter behavior.

The lower click-selection branch of arsenal `FUN_1008_5770` is active in the native child window.
Its five visible 35-pixel hit regions select entries relative to the same scrolling origin used by
`FUN_1008_4976`, retain the original one-pixel dead boundaries between rows, and remain disabled
while the attract-mode demo owns the controls.

The companion arrow branch of `FUN_1008_5770` and `FUN_1008_5a04` is restored too. Its recovered
diamond hit target scrolls the five-row viewport independently of selection, captures the mouse,
waits 500 ms, then repeats at 100 ms until release. The original viewport bounds and attract-mode
input lock are retained.

Arsenal `FUN_1008_520c`'s message-review region is no longer replaced by modal gameplay popups.
Ordinary action feedback and newly discovered laws enter the recovered twenty-message
history and render word-wrapped in the original y=95..193 panel. The two configured history keys
navigate that display in place; lifecycle notices such as death and victory remain modal.
The backing store now also follows `FUN_1008_55e0`: it retains at most twenty entries, evicts the
oldest before the twenty-first append, and copies at most 99 characters into each 100-byte slot.
`FUN_1008_554e`, `FUN_1008_55be`, and `FUN_1008_5588` now govern the review cursor exactly: a new
entry is immediately current, Previous moves toward older entries, and Next may advance one step
past the newest entry to the original blank live-message slot.

The remaining selector-1010 class-specific callbacks have now been audited against their raw
16-bit instructions. `1010:0b2c`, `1010:235c`, and `1010:26dc` are the already represented modal
sign/casino notice paths; `1010:0c26`, `1010:0cd4`, and `1010:39a4` provide the reconstructed demo
command streams and their exact initial-ten/Shift-two/ordinary-one tick cadence. `1010:596a` is the
implemented retained-child-board transporter dispatch. `1010:23aa` confirms that casino wagers
accept only item classes 2 through 11, 15, and 19, which the native wager gate and rejection fixture
pin directly. `1010:d848` confirms level twelve's class-19 impact roll: one yields class 16, two
through four yield class 1, and five or six yield class 2. `1010:20c2` is the represented poker
straight/wild-card helper; `1010:282c` and `1010:2874` only invalidate and label the casino card
display. Finally, `1010:0d26` and `1010:0d2c` are constant accessors, while `1010:117c` is an empty
override. None of these audited slots leaves an additional gameplay branch disconnected.

The original `MAINICON` group resource points to extracted `RT_ICON` id 1. The native main window
now constructs its large and small class icons directly from that exact 32×32 16-color payload via
`CreateIconFromResourceEx`, replacing the generic Windows application icon without redrawing or
converting the original artwork.

The About command now uses the exact `HURLABOUT` resource dimensions, caption, font, button geometry,
and four text strings painted by the indirect dialog procedure at `1008:3d78`. This removes the
invented reconstruction/version prose and restores the original 1997 copyright and development
credit verbatim.

The `BALLSHELP` dialog is restored as the original twelve-page Help and Information browser rather
than a one-page invented MessageBox. Its eleven-control resource, page clamp, first/previous/next/
last dispatch, category targets (Elements, Commands, Laws & Tips, High Scores, and Registration),
and `Page %d of %d` indicator follow `1010:94e0` through `1010:9c6c`. The displayed prose is drawn
from the original 1080/1010 segment strings; the live High Scores page uses the reconstructed table.
As in `FUN_1008_6f46`, the C/F1 shortcut opens page six directly while the system-menu command opens
the first narrative page.

The complete fourteen-entry board-class dispatch corpus has now been checked slot by slot rather
than inferred from reachable native behavior. Every class-specific `+0x1c` post-turn target is
represented: `1008:e636`, `1010:1180`, `1010:37a2`, `1010:3d50`, `1010:4692`, `1010:518c`,
`1010:5a46`, `1010:6308`, `1010:6f7a`, `1010:8f56`, `1010:c9f8`, `1010:d06a`, `1010:d71e`, and
`1010:d968`. The `+0x14` graph handlers agree with the retained-board transition graph; the three
exceptions `1010:379e`, `1010:3d4c`, and `1010:d964` are literal `retf 6` no-ops. The nine concrete
`+0x34` sign handlers agree with `SignpostText`, including the coordinate-selected Sewer City signs.
The only special item slots are the reconstructed Smart Card, casino wager, Moon Key, and Emerald
handlers; the only special treasure slots are Gauntlet distribution and the final-arena class-19
roll; and the Power Plant alone overrides repellent, Smart Bomb, and pre-actor processing. All other
entries resolve to the audited common terrain, actor, drawing, accessor, or destructor routines.
`disassemble_callbacks.py` now enumerates those concrete targets so future parity work starts from
the same complete evidence set.

Final completion audit: the original authorization is retained beside the source executable; all
41 extracted NE shell/media resources and all 13 decoded gameplay-data artifacts match their
recorded SHA-256 hashes; the optimized `/W4` CMake build is warning-free; the regression executable
passes its full fixture corpus; and `dumpbin` identifies the deliverable as machine `8664` (x64),
Windows GUI subsystem, with a 64-bit image base. The reconstruction covers application-owned shell
and gameplay behavior. Win16 startup glue, memory-model helpers, exception machinery, and the
statically linked compiler library are deliberately supplied by the modern Visual C++ runtime and
Win32 API rather than transliterated as game code.

Post-audit gameplay corrections from live testing are pinned at the instruction level. Actor
movement now follows `FUN_1008_91ec`, whose legal destinations include ordinary passable terrain
plus tiles `0x09..0x17` and `0x87..0xa1`; spawned groups can therefore leave intact portal/base
machinery. Accepted input no longer rewinds an already active `FUN_1008_8b2c`/`8be6` transient
effect, so rapid movement cannot hold bubble, construction, or destruction animation on frame one.
Finally, generic-depth drain dispatch follows its two virtual slots: inherited `+0x18` sends
upward `0x23..0x26` drains to the retained parent, while `FUN_1008_dbdc` at `+0x14` sends down-tile
`0x27` to the next generated board (`0x28..0x2a` retain the callback's parent/error branch).
`FUN_1008_0630` then erases the depth-one `0x23` entry beneath the newly placed vessel, leaving the
first playable stage without an up drain. Regression fixtures cover portal escape, animation
completion under repeated movement, automatic down-drain traversal, and the erased root entry.

## Branch-return correction (2026-09-08)

The casino report exposed an omission in the earlier graph audit: common return slot `+0x18`
was not applied to every native class. `1008:ba30` dispatches all `0x23..0x26` tiles to that slot;
`1008:b882` follows the retained parent and searches its original numbered down connection with
`1008:b9ba`. The casino, Power Plant, hub, and other previously omitted classes now use this path.
Root boards do not invent a parent. Return validation no longer moves data out of live history
before confirming that the landing tile exists.

`1010:596a` is the sole derived return override: the shared selector-1058 board's `0x23` and
`0x24` lead to selectors 1060 and 1068 respectively, landing on `0x27` in either parent.
`1010:6e3c` enters 1058 through `0x24`, unlike 1060's `0x23`, and its second down exit constructs
the Power Plant (`1008:e07a`). `1010:582e` supplies 1058's previously omitted down edge to c80e.
These routes now retain the correct hub ancestry rather than pushing a spurious return ancestor.
Common returns restore the latest shared instance, and descendants of 1058 use one cache identity
regardless of which arm was used. Recoverable older phantom-parent stacks are unwound through
their retained hub; old route-specific descendant cache keys remain recognized.

Regression coverage walks into real casino and shared-branch tiles, verifies reciprocal landing
tiles and retained mutations, crosses to an unvisited sibling, traverses both newly restored exits,
round-trips a graph save, and returns through recursive and maze descendants. All common classes
are tested against all four blue return tile IDs. The c80e down callback uses an explicit test tile
because that constructor does not initially place a down drain. Existing standalone navigation
fixtures now explicitly model a parent and expect a parent request rather than a hardcoded hub ID.

## Rendering restoration, first batch (2026-09-08)

The native side panel now uses the original 0x99/0x9a/0x9b scroll states and 0x96 selection
marker from `1008:4976`, its layered ARSENAL label and inventory separators, and the 0x154
health icons from `1008:4f4e`. Scores receive grouped digits and the original yellow/shadow
treatment. Item descriptor word zero is retained for presentation: kinds 0/1 omit quantity,
while counted entries use the recovered two-line positioning and blue quantity text.
Modern Courier New currently substitutes for the original font setup; exact header typography
and the remaining status-panel border details are not claimed as pixel-equivalent.

The casino `1010:23ce` label now includes the ordinary-table 0xf0 strip and the three trailing
wild-table cells, in addition to the already reconstructed rank label. Offscreen GDI tests compare
health, scroll states, the unobscured selection marker, and every table-label cell against the
original bitmap atlas. This is a partial rendering milestone: the casino scrolling announcement
callback, illustrated help-page callbacks, high-score page, and full message-panel/header styling
remain open. No gameplay or save-format changes are intended in this batch.

## Casino marquee and message continuation (2026-09-09)

`1010:2874` now has a native announcement setter, with the five original strings at 3208,
2f14, 2f7a, 30c0 and 30d2 connected to the table state changes in 1c1e/1180. The 26fe
thirteen-character scrolling window is drawn at (216,156) on an opaque black background;
ordinary/instruction/losing text is yellow, winning text red, and closed-table text blue.
Animation advances on the existing timer, not in paint, so extra repaints do not change its
speed. The native fixed-pitch font remains a modern substitute. Marquee presentation is not
serialized: loading restarts the instruction/closed announcement for those states and the
welcome announcement otherwise, including a restored result hand. In-session board revisits
retain the announcement with their board. This save-presentation limitation remains open.

The original 520c "[M]ore" indicator is restored when another history entry exists, and removed
at the final or past-end entry. Full message-history coloring, precise text wrapping and header
typography, illustrated help, and high-score page rendering still remain open. Tests cover all
five marquee windows, wrap/reset, input-independent animation without RNG/turn consumption,
opaque lead-in rendering, paint purity, and continuation-state boundaries.

## Illustrated help and high-score presentation (2026-09-09)

The Elements, Weapons, Keyboard Commands and High Scores pages now use a dedicated
444x348 drawing surface instead of one wrapped text paragraph. Sprite compositions and
positions are based on `1010:a16e`, `b638`, and `9cb2`; the eight weapon rows, portal,
crud and treasure examples, number-pad caps, arrows, and Shift/A/Z caps use the original
atlas. The high-score page follows `1010:a64a`'s ten rows and independently colored
rank/name/score/skill columns, including grouped digits and the existing ten-million-point
hero bonus. The High Scores menu now opens that page instead of a MessageBox.

The dialog reserves enough client space for the logical page canvas; its size accounts
for modern non-client borders. GDI state is restored after rendering and clipping confines
each page. Offscreen regression snapshots for pages 4/5/6/10 were visually inspected.
These pages restore the missing illustrations and structured presentation, not complete
pixel parity: modern fonts and wrapped text remain, the animated help examples are static,
and narrative/registration/credits pages still use the existing text renderer. Header and
full message-panel styling, original help animation, remaining page-specific typography,
and saved marquee presentation remain open.

## Header and message panel (2026-09-09)

The side-panel caption now follows `1008:4e5e`'s separate D/S initials, RAIN/TORM endings,
average-character-width placement, red text and copyright mark. The generic centered caption
is removed. `1008:520c` now has a dedicated renderer using an eight-pixel character grid,
fourteen-pixel lines, explicit newlines, word wrapping, gray reviewed messages and navy current
messages. The review boundary follows 554e/5588: new batches reset it, forward navigation advances
it, and backward navigation does not. The continuation marker's M is black for reviewed text and
blue otherwise. Clearing message history resets the boundary.

Offscreen panel output was visually inspected, with tests for review-state transitions, GDI state
restoration and very narrow bounds. Modern Courier New sizes remain substitutes for original
font selection; the exact Win16 long-word overflow behavior is not copied (native rendering wraps
oversized words safely). Remaining presentation work includes help-example animation, the other
help pages' exact layout/fonts and saved marquee state, rather than the former plain-text header
and message panel.

## Moon Room chambers and animated help examples (2026-09-10)

The Moon Room (selector 1060) map and constructor match the extracted room. The
missing right-hand wall tiles were instead caused by an omitted runtime guard:
`1010:65e9..6624` excludes both an intact seal (0x37) and a switch-opened seal
(0x5c) before releasing four actors and clearing the chamber corners. The native
routine excluded only 0x5c, so the upper chamber broke on the first world turn.
The lower chamber also tested the wrong sentinel: `1010:676f` checks (31,24),
not (31,22). Both now follow the disassembly. Tests cover both chambers' intact,
switch-opened and destroyed seals, one-shot release, and the lower sentinel.
Already-cleared walls in existing saves are not repaired automatically: that
would also overwrite legitimate trap destruction. Test a newly generated room.

Help pages 4 and 5 now animate the drain, central treasure, portal center,
fireball and radio-controlled-bomb examples using `1010:afd8`'s atlas sequences.
The help dialog uses a 200ms presentation timer (matching the native board timer)
and buffered painting; animation consumes no gameplay turns or random numbers.
Pixel tests compare ten frames against the original atlas. Original randomized
treasure sparkles, actor/player examples, flashing note border, and animation
on the other help pages remain open, as do the previously documented typography
and saved casino-marquee presentation differences. This is not full help parity.

## Board visibility, entity priority and animation (2026-09-10)

`1008:b09e` and `9e68` call `917e` before displaying ordinary overlays: the
underlying tile must have descriptor flag bit zero set. The native renderer
omitted this gate, exposing pickups through rocks and walls. It now hides these
overlays without deleting them; clearing the terrain reveals them. Transient
effects 0x14..0x1f use the separate `8be6` path and retain their ability to render
on solid terrain, including construction flashes and door effects.

The original full-paint and timer paths differ: `5e70` paints the player before
actors/projectiles, but `8f32` explicitly excludes the player's square, while
the timer's `9680` draws the player last through `974c`. The remake had no such
projectile exclusion. Its full-frame renderer now consistently draws the player
last, including over actors, effects and casino graphics. Fireball/radio-bomb
animation follows `9d9c`'s actor-occupancy exclusion. Other projectile/actor overlap
ordering is unchanged; this is not a claim that every original repaint timing
artifact has been reproduced.

Deterministic `9e68` cycles are restored for colorful pickups, blinking overlay
0x2b and marker 0x2d. Fireballs/radio bombs use `9d9c`'s four-frame sequences.
No RNG is sampled while painting. Pixel regressions cover concealed/revealed
pickup types, solid-terrain effects, ten animation frames, actor occupancy and
the player's priority over all five projectile types. Original randomized
sparkles and the remaining terrain/help animation differences remain open.

## Priority audit fixes: Moon Room construction and effect lifetimes (2026-09-10)

The late Moon Room event no longer uses the level-three placement routine.
`1010:6344..63df` rolls columns 1..36 and rows 1..32, excludes the player via
`ce4e`, and requires an empty 3x3 footprint via `c1e6(force=0)`. All three
structures are staged before committing terrain/effects and marking the event
complete. The original infinite retry is bounded at 4096 random attempts per
structure, followed by a scan for a valid footprint; if three cannot be placed,
the board stays unchanged and the event remains pending. This intentional safety
fallback avoids hanging on packed boards. The event also restores the original
obstacle-break divisor change from 80 to 5, alongside its existing spawn-rate change.

Transient overlays now carry independent five-frame ages. A new action arms only
new effects, while older effects finish normally; rendering and expiration use each
overlay's own frame. This preserves complete effect sequences while retaining the
native nonblocking UI, rather than reproducing Win16's blocking `8b2c` message loop.
Persistent pickups and casino-card overlays are not expired by the effect timer.

DAH64 version 24 appends one age byte per overlay. The historical six-byte overlay
records and all preceding offsets remain unchanged. Versions 2..23 migrate the
old shared animation frame to each transient overlay. Individual ages survive
save/load and board copies; invalid ages and ages on persistent overlays are rejected.
Regression fixtures using offsets from the end of the file account for the new tail.
Tests cover overlapping batches, independent rendering, completion, save migration,
invalid ages, terrain/player exclusion, full-range placement, packed-board retry,
atomic three-structure construction and one-shot event completion.

## Remaining rendering audit and control surrounds (2026-09-10)

The parent window no longer paints the gaps around the arsenal and board black.
Its class background and paint path use light gray with sunken bevels around each
child. The shared surround renderer excludes child interiors and restores the DC;
pixel tests verify the gray separator, contrasting edges and intact child pixels.

The board now reconstructs the incremental `1008:9786` terrain sequences on every
full repaint: drains, machinery/portals, moving barriers, transporters, special
doors and animated late-game tiles. Actor types 2 and 3 use `9c5a`'s additional
frames instead of constant sprites. Treasure sparkles also animate. Conditional
updates retain their last frame within the original 120-tick cycle. Cosmetic
chance decisions use an isolated deterministic hash with the original probability
denominators; they deliberately do not reproduce the original shared gameplay RNG
stream. No repaint advances gameplay or animation time.

DAH64 version 25 appends announcement (uint16) and scroll position (uint32), so
casino win/loss messages, marquee position and result lifetime survive saves.
Versions through 24 still load; their absent announcement cannot be recovered and
uses the previous status-based fallback. Invalid announcement/scroll values are
rejected. The six-byte new tail is included in binary regression fixture offsets.

All twelve help pages now have an explicit gray-canvas layout instead of falling
back to the dialog's default-font paragraph. Text is shared with offscreen tests.
Elements, keyboard, registration and preview illustrations animate; enemy demos,
treasure sparkles and the colored note border are restored. The original `b4ae`
preview page is reconstructed from c4d6..c668, removing the erroneous merged
acknowledgements draft. Offscreen snapshots cover all twelve pages. This addresses
the audit's missing rendering categories, not exact Win16 typography: narrative
wrapping and some illustration spacing are adapted for modern fonts; cosmetic
random sequences are intentionally isolated, and the historical registration
submission workflow is not newly implemented by these rendering changes.

## Transition arrow animation correction (2026-09-11)

The terrain-animation restoration had reversed the two drain sprite families.
Raw disassembly at `1008:9933..998e` assigns forward tiles `0x27..0x2a`
sprites `0x2b..0x2f`, and return tiles `0x23..0x26` sprites `0x41..0x45`.
Corrected that visual mapping without changing transition destinations or saved
boards. Pixel regression coverage now checks all eight tile IDs across all 120
presentation ticks against these original sprite sequences.

## Startup notice, tunnel levers and difficulty audit (2026-09-11)

Restored the original modal welcome/copyright notice at `1008:00c4`, invoked
from original WinMain at `1008:0077`. It appears after the main window is shown;
the board timer pauses while a modal dialog disables its owner, so the attract
sequence does not run behind the startup notice. Typography uses the native
Win64 message box, not a bitmap approximation of the original Windows dialog.

The two selector-1058 tunnel switches incorrectly restored hard-coded assorted
terrain on alternate activations. `1010:5a46` actually calls `ce88`/`cafa` to
clear and frame each enclosure, retaining the doorway markers. Both 3x3 grids
are corrected. The vertical switch now uses its original construction effect
0x16 (not 0x14), and both switches queue original sound 0x16. Tests alternate
each lever four times with immediate movement versus six animation ticks of
waiting and compare complete board state/RNG. The result is timing-independent,
and overlapping effects expire. No save-format change or board reset is needed.

Difficulty is not yet fully faithful: see `DIFFICULTY-AUDIT-2026-09-11.md`.
The isolated 100-board/skill save-load probe passes, but original-derived
comparison found fixed Beginner populations/rates in late branches and the
turn-six/depth-six spawn-update mismatch. Those balance changes are audit
findings, not silently included in this visual/lever repair.

## Persistent Power Plant targets and difficulty corrections (2026-09-11)

Power Plant targets now retain their own coordinates, hit counters and defeated
flags, matching the per-target arrays used by `1008:ec6e` and `1010:00ec`.
Neither damage nor movement order is selected by the target's current board half.
Missile/radio impacts, missiles intercepted during target movement, awakening,
destruction and victory all resolve the target by identity. Defeated identities
cannot be reused when another target occupies the same coordinates. Each target
awards its kill bonus only once, and both must be defeated for victory.

DAH64 version 26 appends ten bytes: two uint16 columns, two uint16 rows, and two
uint8 defeated flags. Invalid coordinates/flags and overlapping live origins are
rejected. Older saves remain readable: target identity is inferred from remaining
target tiles and the old half-board convention without resetting the board or
counters. Previously misattributed hits cannot be reconstructed after both bosses
have crossed; old mid-fight saves therefore have a best-effort migration, whereas
new saves retain exact target identity. Saved defeated coordinates may coincide
with the surviving target's later location.

The five groups in `DIFFICULTY-AUDIT-2026-09-11.md` are corrected: maze actor counts,
types and transporter attempts; maze/alien-maze spawn and break rates; recursive
board pickup/structure counts and depth-dependent escalation; alien-maze captain
count; and the generic turn-six spawn update. Runtime formulas use existing skill
and depth fields, preserving board layouts and progression. Older procedural saves
have their erroneous spawn chance reconstructed from persisted escalation bits;
population changes apply only to newly generated boards.

Regression cases cover bosses in reversed vertical order, both bosses in one half,
actual movement across the midpoint onto a missile, all nine hit tiles, ineffective
weapons, separate ten-hit defeats and score awards, save/reload, surviving bosses
crossing old defeated positions, malformed coordinates, and version-25 migration.
Difficulty coverage includes all 100 board-class/skill save round trips, exact
maze populations at all skills, recursive pickup and escalation formulas, and
turn-six updates at all nine depths without clobbering later escalation.

Final verification: GameDataRegression and AuditRegression pass in both Release
and Debug builds. Additional target tests prevent moving through a dormant boss
and prevent missing target graphics from being treated as a victory. No existing
user save, score table or key-binding sidecar was modified during verification.

## Energy Sphere coordinates and rare procedural structures (2026-09-11)

Corrected the Energy Sphere destruction tile and construction effect to column
14, row 32. Original `1008:ed34..ed42` indexes column 14 with offset 0x38 and
row 32 with offset 0x40; the remake had transposed these coordinates. A direct
damage regression checks both the cleared tile and the untouched transposed
tile, plus the construction effect coordinates.

Restored `FUN_1008_d4e0`'s rare type-0x1a guarded structure selection at depths
five through nine: after the placement gate succeeds, roll random(20), selecting
0x1a on 20 and 0x18 otherwise, before drawing coordinates. Depths one through
four consume no selection roll. Tests check the original LCG-derived selection
and RNG state at every depth, and actual rare structure generation at all four
skills. Existing saved layouts are preserved; this applies to newly generated
boards. Previously destroyed spheres in saved games are not retroactively edited.

The board-history and cache limits remain unchanged, as requested.

GameDataRegression and AuditRegression pass in Release and Debug. The previous
depth-five navigation test froze an outcome from the incomplete RNG sequence;
it now exercises both valid original destinations across 40 seeds. The Release
executable is copied to `reconstructed/bin/DrainStorm.exe`.

## Actual attract-mode splash panel (2026-09-11)

The previously added modal welcome notice was not the splash requested by the
user. Removed that automatic message box. The original graphic is a 240x144
child panel over the demo board, created by `1010:7816` and painted by
`1010:78e0` (missing from the Ghidra function export; inspected in raw disassembly).
It is now composited after the board in the existing double-buffered paint path.

Recovered the gray bevels, embossed red DRAIN STORM lettering, TM, Version 1.02,
blue 1997 Dave Horner copyright, and the original atlas-backed New Game, Load
Game and Help buttons. Fonts and metric-derived title placement follow
`1010:76fe..77aa` and `1010:7ae5..7cbe`; native Courier New rasterization may
differ from Windows 3.x. Button backgrounds use sprites 0x1b0/0x1b1, with animated
player, squid and bulldozer icons from `1010:8216`. Mouse clicks invoke the existing
new-game, load-game and help workflows, with capture and pressed artwork.

Placement follows `1008:198e`: (12,12) for classes 5/10, (228,12) for class 6,
(24,276) for class 16, and (144,276) otherwise. Among the four demo classes,
class 10 therefore uses the upper-left position while 1/2/9 use the lower one.
Bounds are derived from the current board on each draw, so demo changes reposition
the panel without stale state. Gameplay and loaded saves never display it.

Regression checks cover all placement cases, button hit rectangles, atlas pixels
at multiple animation frames and pressed states, unchanged gameplay RNG, and
pixel-exact removal during gameplay. Visual snapshots `demo-panel-2.bmp` and
`demo-panel-10.bmp` are emitted in the test build directory for inspection.
Both snapshots were visually inspected. GameDataRegression and AuditRegression
pass in Release and Debug; the verified Release build is published to
`reconstructed/bin/DrainStorm.exe`. No user saves or sidecars were changed.

## Drain-entry animation and splash capture behavior (2026-09-14)

Recovered the missing `1008:b78a` drain-entry drawing sequence invoked from
`1008:ba30`: five 50ms frames, sprites 0x30..0x34 for forward drains and
0x35..0x39 for return drains. The Win64 shell renders a presentation-only source
board snapshot while destination state is committed atomically using the existing
navigation code. A separate 50ms timer completes the five-frame sequence; the
200ms world-presentation timer and keyboard/board mouse actions pause during it.
The snapshot is not serialized. New game, successful load, and demo restart clear
it, so saves made mid-animation retain the actual destination, not stale visuals.

Splash button capture now updates the pressed artwork when dragged outside/back
inside the armed button. Demo cycling pauses while a button is held, preventing
the panel from moving out from under a pending click. Capture loss releases the
button state. The original panel fonts, graphics and position rules are unchanged.

Tests compare all five frames for all eight drain tiles against the original
atlas, check termination and removal of source graphics, preserve destination RNG
and animation state during the effect, and exercise forward/return navigation,
save/load during the effect, and cancellation by a new game.

Further missing legacy functionality identified: `1010:7e18` paints a timed
"Register Now!" reminder, driven by `1010:81d4`/`808a`. Help page eleven also has
an original clickable registration action (`1010:aba0`/`ac1c`, opening `dd30`).
These obsolete registration features remain intentionally absent; they are not
missing combat or progression logic. The accepted history/cache limits remain
unchanged. This targeted pass is not a complete differential playthrough audit.

GameDataRegression and AuditRegression pass in Release and Debug. The updated
Release executable is copied to `reconstructed/bin/DrainStorm.exe` and its hash
matches the build output. No user saves, scores or key bindings were modified.

## Player-hit rendering priority (2026-09-14)

The type-0x18 contact effect was already emitted on both collision paths and
mapped to original sprites 0x3a..0x3e, but the final player pass covered it.
The player pass now selects the newest active hit effect on the player's current
square instead of the normal vessel/wreck sprite. This preserves the original
`1008:8be6` hit-pass exception while keeping ordinary actors, projectiles and
other effects below the player. Moving away leaves the effect on its original
square; expiry restores normal player rendering. Tests cover all five frames
with living/dead players, simultaneous actors/projectiles, expiry, and an adjacent
hit effect that must not replace the player's sprite.

This change is limited to layering. The previously identified transient-effect
timing and immediate fatal-hit modal-dialog behavior remain separate follow-ups.

## Full-audit repair completion (2026-09-28)

The confirmed P-001 through P-080 backlog now has implementation dispositions in
`audit/REPAIR_STATUS.md`. This supersedes historical open-repair notes above;
broader verification limits and intentional changes remain in `audit/COMPATIBILITY.md`.

Fatal playable hits keep all five visible damage frames before publishing Game Over
and restarting the demo. `1008:e636` now retains spawn words in actual event order,
including breaches after the arena lever; ring turning follows the lever branch,
the Sphere latch drives wake-up/repair gates, and the lower target rolls before its
delay check. Native format 32 preserves these two spawn words without changing
legacy actor/overlay record sizes. Old formats retain their best-effort inferred
history; missing event chronology cannot be reconstructed from their flags alone.

Gameplay animation restores the shared LCG at timer boundaries, following terrain,
actor and linked-list traversal/visibility gates. Per-object sprite caches keep
arbitrary repaints free of RNG mutations and machinery freezes after Sphere shutdown.
The arsenal/help emboss passes and list bevels are restored. Sidecar and file-dialog
failures are distinguished from first-run defaults and Cancel. The graphical score
table shares the signed ranking calculation, including both end-game bonuses.

Release regressions include atlas pixel checks, all board/skill save combinations,
legacy migrations, malformed-sidecar preservation, and hidden production-window
input routing (ordered replay, retained Shift and chained drain transitions).
No synchronized original/native playthrough or complete combat-reachability proof
has been performed.
