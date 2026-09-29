# Findings — baseline bc22292

2026-09-14. Audit only; no implementation fixes made.

## P-001 — Fresh child construction loses its advanced RNG state

**Confirmed, high repair priority; gameplay-parity defect.** No claim that this
alone makes a game unwinnable.

Current code: `application_state.cpp:889` loads a fresh child using the parent's
RNG state. Its constructor consumes random draws. At line 894,
`child.AdoptPlayerStateFrom(gameData_)` calls `game_data.cpp:2253`, which replaces
that resulting RNG state with the parent's pre-construction value.

Original evidence: the procedural exit handler `1008:dbdc` constructs its child
through `d4e0`/`1010:4044`. Random calls use `1008:3c88` and the common runtime
generator `1000:1f6e`; they consume one shared stream. The original has no
per-board RNG field to restore after constructing a child. Revisit/adoption and
fresh construction therefore need different RNG handling in the remake.

Isolated probe: generate a depth-one board with seed 1, select its down exit,
construct an equivalent depth-two board independently from the resulting parent
RNG, and compare with the application transition:

```text
new-child parent_rng=3571223223 constructor_final_rng=4123114318 actual_transition_rng=3571223223
```

The child layout is generated, but subsequent randomness resumes from before
that generation. This affects subsequent gameplay rolls and later generation.
Inspect every fresh-child path, including shared-board construction, when fixing.

Proposed coverage: assert constructor-final RNG after fresh transitions for all
classes/skills; assert current global/player RNG carries through cached revisits
without restoring an old board-local stream. Preserve existing layouts/saves.

## P-002 — Cached procedural exits consume an unnecessary destination roll

**Confirmed, medium repair priority; RNG/branch-dispatch parity defect.**

Current code: `game_data.cpp:4126` rolls `NextRandom(8)` on every depth-five to
depth-eight exit activation. Only afterward does `HandleBoardTransition` look
up the cached child by path. It may ignore the newly requested class and reuse
the old child, but the random draw has already changed the gameplay stream.

Original raw instructions `1008:dc03..dc10` check the cached child pointer and
jump directly to `dcdb` when it exists. The depth/destination roll at `dc1d`
is reached only when the child must first be created.

Probe creates and caches a depth-six child of a depth-five board, returns to the
parent and dispatches the same exit again without actor movement:

```text
cached-exit before_rng=1345925429 after_dispatch_rng=3559661348 requested_class=22 actual_cached_class=22
```

Even where the roll selects the same class, consuming it is incorrect. The
probe deliberately forces the initial child choice to isolate repeated traversal.

Proposed coverage: revisit the same procedural child repeatedly at depths 5–8;
verify stable child identity and no destination-selection RNG consumption. Test
both possible first-created child classes and save/load between traversals.

## P-003 — Rare recursive branch omits its forward drain

**Confirmed, high repair priority; missing progression connection.**

Original `1010:ce62` occasionally creates a `c80e` board (native class 8).
After construction, raw instructions `cef0..ceff` dereference the child's column
array, select column 20 and write tile `0x27` at row 5. This is a caller-specific
addition, not part of the generic class-8 constructor.

Current `GameData::DetectBoardTransition` selects class 8, but neither it nor
`ApplicationState::HandleBoardTransition` performs this patch. The resulting
board has no forward drain. Backtracking remains possible; this is not evidence
of an unavoidable game-wide softlock.

Probe dispatches the real class-11 exit using seed 10 after board generation:

```text
rare-c80e seed=10 request=8 destination=8 tile20_5=3 down_exits=0
```

Expected: tile `(20,5)` is `0x27`. Existing branch regression fixtures inject a
class-8 down tile (`audit_regression.cpp`, `edge.source == 8`), masking this
constructor/caller mismatch. Add a test that enters through class 11 without
injecting tiles, uses the generated exit, and returns through both cached boards.

## P-004 — Exhausted factory selection lacks its recursive fallback

**Confirmed, medium repair priority; intermittently inactive forward drain.**

Original `1010:ce62` calls `1008:2b54` on its special factory-selection roll.
When both one-time factories are used, `2b54` returns null. Instructions
`cf32..cf34` then branch to `cf46`, constructing another recursive `cb9c` board.

Current class-11 dispatch checks `genericRuntimeFlags_ & 0x30` inside the
special-roll arm but leaves the target zero when both flags are set. Its normal
recursive fallback is only in the outer `else`, so this roll does nothing.

```text
exhausted-factories seed=5 request=0 destination=11 tile20_5=0 down_exits=1
```

The destination shown is the unchanged source board, not a newly created child.
Expected: request class 11 with recursive-child code 2, then create/cache it.
Later attempts can choose another roll; this is not a permanent blocked exit.
Cover neither/one/both factories used, all selection arms, and cached revisits.

## P-005 — End-of-game and quit reset to gameplay instead of the demo

**Confirmed, medium repair priority; lifecycle/presentation mismatch.**

Original `1008:1b22` handles the victory/death notice and high score, frees the
old board and calls demo initialization `1008:169c`. The accepted quit paths in
`1008:1efe` also end with `169c` (after saving or score submission as applicable).

Current `ApplicationState::HandleEndState` calls `NewGame(difficulty)` in both
end-state branches, immediately creating playable class 17. `QuitCurrentGame`
uses the same reset. The remake should return to attract/demo mode and wait for
the player to start another game.

Isolated end-state probe:

```text
death demo=0 next_board=17 notice=1
victory demo=0 next_board=17 notice=1
```

The notices are present; this is not the previously fixed missing-game-over
message. Resetting the board before presenting the notice also discards any
pending fatal-hit effect; its exact original presentation sequence needs the
separate animation review. High-score values are captured before reset, so this
finding does not assert score loss.

Cover death, victory and both accepted quit choices, score submission, cancelled
quit, and explicitly starting a game from the resulting demo. `RestartDemo`
currently assumes demo mode is already active; merely calling it from gameplay
without setting the mode would be insufficient.

## P-006 — Hub graph construction is deferred and reordered

**Confirmed, high repair priority; board-generation/RNG parity defect.**

Original hub exit handler `1010:4382` constructs missing connected boards before
dispatching the selected exit. Raw instructions confirm this order:

1. `43cc`: `5fc4`, native class 6.
2. `4434`: `6a56`, native class 7.
3. `449d`: `0ea0`, casino class 2.
4. `4507`: `cb9c`, recursive class 11.
5. `4575`: `55ac`, shared class 5; `4584..45ca` links it to both arms.

Only at `45ce..461c` does the handler select the requested child. Existing
pointers skip construction, so this is first-exit graph initialization, not
regeneration on every visit. Each direct child receives hub depth + 1; the shared
board receives hub depth + 2.

Current `ApplicationState::HandleBoardTransition` constructs only the requested
board. The other arm, casino, recursive board and shared room are generated on
later visits, using whatever random state exists then. Thus generation order and
the layouts of random-containing rooms depend on traversal and intervening play
instead of being fixed together on the first hub exit.

Probe loads a fresh hub for each exit, dispatches its existing tile without
injecting a destination, then counts these five board classes across the current
board, history and both caches:

```text
hub-first-exit tile=39 destination=6 connected_boards=1 expected=5
hub-first-exit tile=40 destination=7 connected_boards=1 expected=5
hub-first-exit tile=41 destination=2 connected_boards=1 expected=5
hub-first-exit tile=42 destination=11 connected_boards=1 expected=5
```

This is distinct from P-001: preserving the selected constructor's final RNG
alone still omits/reorders the other constructors. Repair and test them together.
Verify all four first-exit choices, constructor order, depths, single shared-room
identity, no regeneration on revisits, and persistence before visiting siblings.
This finding does not assert a blocked route or broken shared-return direction.

## Reviewed return behavior — no additional discrepancy in this slice

- Raw `1008:b882` takes the parent's pointer and stored exit selector, then
  `b9ba` searches column-major for tile `0x27 + selector`. The remake's retained
  return tile and `PositionPlayerAtTile` use the corresponding tile and scan
  order on valid graph paths. Missing-tile fallback and legacy-save recovery
  are not covered by this parity verdict.
- `1010:596a` chooses the first parent for blue `0x23`, second for `0x24`, and
  searches that parent's `0x27`. The current class-5 dispatch and explicit
  shared-return arm agree. Full save/revisit identity and legitimate traversal
  remain separate checks.
- `1008:2b54` rejects already-used factory choices by drawing again and marks
  the selected flag before construction. Current selection mirrors that order;
  P-004 concerns the caller's missing null-result fallback. Player-state adoption
  carries the two global flag bits while retaining local runtime bits. Full
  save/load propagation remains pending.

## P-007 — Missile fragments wait until the next projectile update

**Confirmed, high repair priority; combat turn-order defect.**

Original `1008:77d0` iterates against the live projectile count. On missile/actor
impact it removes the missile, appends eight fragments through `7f48`/`8e22`,
decrements the loop index, and continues processing. `7f8e` compacts the array;
the newly appended fragments therefore move and collide in this same update.

Current `AdvanceProjectiles` accumulates fragments in `spawnedProjectiles` and
appends them only after its loop. They remain at the impact point until the next
accepted action. This changes blast reach, enemy survival and subsequent actor
movement; capacity handling also needs to respect the original live count.

`combat_probe.cpp`: a missile at (10,10) hits an enemy at (11,10); another is at
(12,10). After one projectile update:

```text
missile-chain surviving_actors=1 fragments_at_impact=8 expected_actors=0 expected_unmoved=0
```

Cover actor/terrain impacts, multiple missiles, appended-fragment ordering and
the 150-projectile cap. Do not automatically advance fragments created in the
later actor phase: original `8310` occurs after the projectile update.

## P-008 — Casino deflection rejects a turn the original accepts

**Confirmed, medium repair priority; failed-action turn-order defect.**

Original `1008:1b22` first validates the requested destination using `27b8`,
advances projectiles, and only then invokes pre-movement callback `1010:22c6`.
When the force line cancels northward movement, `22c6` tests the remaining
horizontal component; if blocked, it zeros that component too. The accepted
world turn continues as a stationary action.

Current `MovePlayer` deflects before its final passability check and returns
false if the remaining horizontal square is blocked. The same input costs no
turn. On a cleared casino fixture, player (20,4), rock (21,4), open (21,3), and
command (+1,-1):

```text
casino-deflection accepted=0 turns=0 expected_accepted=1 expected_turns=1
```

Cover initial target blocked/open, deflected horizontal blocked/open, straight
north and diagonal commands, all skills, and projectile changes to terrain
between initial validation and deflection. Fixture uses ordinary passable/rock
tiles; no claim that its exact arrangement occurs in every generated casino.

## P-009 — Common Smart Bomb processes captains in reverse RNG order

**Confirmed, medium repair priority; enemy replacement/RNG assignment defect.**

Original `1008:ca6a` traverses the initial actor array from last to first. It
splits each captain through `bc2c` before removing that captain; appended
lieutenants survive this backward traversal. Current common Smart Bomb loop
traverses actors from first to last and defers all replacements until afterward.
The special Power Plant path already uses backward iteration.

With seed 1, two captains at (10,10) and (30,10), the original assigns the first
draw (7) to the right captain and the second (2) to the left. The remake does
the reverse. Counts below are after the accepted item turn; surviving groups
remain separated spatially:

```text
smart-bomb left=7 right=2 expected_left=2 expected_right=7
```

Test multiple captains, mixed actor types and near-capacity arrays. Preserve the
original append-before-removal capacity semantics, not just the draw order.

## P-010 — Chest key-turn quadrants and loot traversal are transposed

**Confirmed, medium repair priority; rendering and reward-placement defect.**

Raw `1008:bc64` places effects at `bd10..bd67`: top-left `0x19`, top-right
`0x1a`, bottom-left `0x1b`, bottom-right `0x1c`. Current `keyEffects[col][row]`
swaps the middle two. Probe for chest origin (10,10):

```text
chest-effect column=10 row=10 type=25
chest-effect column=11 row=10 type=27
chest-effect column=10 row=11 type=26
chest-effect column=11 row=11 type=28
```

Expected top-right type 26 and bottom-left type 27. In addition, original
`bde0..be55` invokes treasure callbacks column-major (column in local `bp-4`,
row in `si`); current code is row-major. The second and third randomized loot
results therefore belong to different squares. Test all four selected chest
corners, each class-specific treasure callback, and retained-key outcomes.

## P-011 — Radio-blast visitation order changes randomized drops

**Confirmed, medium repair priority; combat/reward RNG assignment defect.**

Original `1008:a7a8` takes row then column. Its diagonal sequence is NW, NE,
SW, SE; each successful diagonal first extends horizontally, then vertically.
Its straight sequence is N, W, E, S. Current `ApplyRadioExplosion` uses NW,
SW, NE, SE, extends vertically first, and visits W, N, S, E afterward.
The footprint agrees, but ordered damage callbacks and RNG-dependent drops do not.

Probe places lieutenants NE (11,9) and SW (9,11) of blast center (10,10), with
a seed where only the first lieutenant drops treasure:

```text
radio-drop column=9 row=11 expected_column=11 expected_row=9
```

Preserve exact call order, including blocked propagation and nested sphere
explosions. `a980` intentionally does not split captains; that current behavior
agrees and is not a missing feature.

## P-012 — Hub perimeter enemy mixture uses the wrong roll

**Confirmed, high repair priority; difficulty/balance defect.**

Original `1010:4044` uses `random(3) == 1` for type 1 in each of the four
ten-actor perimeter runs. Current class-3 constructor uses `NextRandom(2) > 1`.
Type 1 therefore has probability 1/2 instead of 1/3, at all four skills.

The constructor probe isolates the final forty random draws from seed 1 and
compares actor types against the original predicate:

```text
hub-ring native_type1=25 original_type1=15 mismatched_actors=20
```

Draw count is unchanged here; the distribution and individual actor types are
wrong. Test all forty ordered actor records, not just total population.

## P-013 — Map-defined actors are initialized in the wrong order

**Confirmed, medium repair priority; shared parser/actor scheduling defect.**

Original `1008:b164` clears the map, then parses with column outermost and row
innermost. Actor creation calls `80c0` inline. Current map parser uses row then
column and pushes actors in that order. Since `8310` processes actors in array
order, identical starting coordinates do not imply identical behavior.

The constructor probe reads actual map resources and compares the original
column-major actor sequence with the corresponding native prefix:

```text
map-actor-order class=3 map_actors=21 mismatched_indices=21
map-actor-order class=5 map_actors=4 mismatched_indices=4
```

Other map-consuming classes tested had no map actor markers; class 8 correctly
does not consume its nominal resource. Map-defined overlays and inline structure
expansion also need order-preserving treatment. Original spaces leave existing
expanded tiles alone, whereas the remake expands large markers after parsing;
check overlapping non-space markers before asserting geometry parity.

## P-014 — Legitimate Gauntlet states produce unreadable native saves

**Confirmed, high repair priority; save/resume defect.**

Original `1010:4f76` draws column `random(24)+10`, row `random(7)+column-4`,
then adds the overlay through `1008:af5a`, which does not reject off-board
coordinates. Rows 36/37 are possible. The remake preserves this generation but
`ReadGame` rejects every overlay with row >= 36. `WriteGame` still succeeds.

An in-memory sweep of all 25 classes, all four skills, seeds 1..128 found:

```text
roundtrip class=4 skill=1 failed=18/128 first_seed=2 first_invalid_overlays=1
roundtrip class=4 skill=2 failed=20/128 first_seed=2 first_invalid_overlays=1
roundtrip class=4 skill=3 failed=22/128 first_seed=2 first_invalid_overlays=1
roundtrip class=4 skill=4 failed=25/128 first_seed=2 first_invalid_overlays=1
roundtrip total=12800 failures=85
```

All other tested constructor states round-tripped. Graph saves embed these same
payloads, so retaining such a Gauntlet in history/cache can invalidate loading
the entire expedition. Reconcile generation with safe persistence without
inventing a collectible in a different square or changing RNG consumption.
`roundtrip_probe.cpp` writes only memory streams, not player files.

## P-015 — Overlay lists use insertion order instead of original head order

**Confirmed, medium repair priority; pickup/message and effect-order defect.**

Original `1008:af5a` inserts each overlay at the linked-list head. `a0ca` walks
from that head when collecting, so co-located pickups resolve newest-first.
The remake appends to a vector and `ResolveOverlaysAtPlayer` walks oldest-first.

Actual Moon Room constructor `1010:5fc4` adds types 13, 3, 9 at (31,10).
Original collection starts with type 9 (extra life), then 3, then 13. Current
collection starts with 13. The probe uses the real constructor stack:

```text
moon-stack first_is_extra_life=0 expected=1
```

This also affects ordering of randomized stacked rewards and list-driven
rendering/mine processing. Do not merely reverse one pickup loop without
auditing other readers and existing saved-vector ordering. Exact-cell collection
and no collection on Wait are already correct and are not being reopened.

## P-016 — Multiple mine blasts incorrectly protect newly split lieutenants

**Confirmed, high repair priority; overlapping-blast combat defect.**

Original `1008:aacc` snapshots actor count for each affected square. Captain
splitting via `bc2c` appends replacements immediately; they are excluded from
that square's initial-count loop but visible to a subsequent mine blast.
Current `ResolveTriggeredMines` defers all replacements until every triggered
mine has finished. Later overlapping blasts cannot hit them.

Probe: captain (10,10), triggered mines (10,10) and (11,10), seed 1:

```text
overlapping-mines surviving_actors=7 expected=0
```

Both orders cover the captain's square, so this counterexample is independent
of the linked-list ordering defect P-015. Cover overlaps, non-overlaps, multiple
captains, drop/score order and the original actor-cap behavior.

## P-017 — Final arena counts the wrong structure coordinates

**Confirmed, high repair priority; initial and escalating combat difficulty.**

Original `1010:d33a` builds 120 centers at columns 3..36 step 3, rows
2..29 step 3, initializes spawn chance/group/break divisor to `0/1/100`.
Raw `1010:d71e` scans those centers (origin column + 1, origin row + 1).
Current `StructureSpawnParameters` scans columns 1..28 step 3 and rows
2..35 step 3: no initial structure centers are counted. All four skills
therefore start at the below-ten-survivors tier:

```text
skill=1 chance=9  group=2 break=20 true_centers=120 expected=0,1,100
skill=2 chance=10 group=3 break=20 true_centers=120 expected=0,1,100
skill=3 chance=11 group=3 break=20 true_centers=120 expected=0,1,100
skill=4 chance=12 group=3 break=20 true_centers=120 expected=0,1,100
```

`runtime_probe.cpp` loads the real constructor for this diagnostic. Preserve
the original post-actor update timing as well: `d71e` stores parameters for
subsequent turns; recomputing them before spawning changes threshold timing.
Zero chance is a roll threshold, not necessarily a reason to omit RNG draws.

## P-018 — Several room-specific difficulty parameters are missing

**Confirmed, high repair priority; board-wide combat balance.**

The base constructor `1008:7560` initializes byte offsets `4a/4c/4e` to
`10/1/20`; derived constructors override them. The native parameter helpers
omit the following overrides. Here `s` is difficulty 1..4; triples mean
spawn threshold / maximum group / actor break divisor, not percentages.

| Class | Original initial triple | Native initial triple | Original constructor |
| --- | --- | --- | --- |
| 1 | 0 / 1 / 5 | 0 / 1 / 20 | 1008:e07a |
| 2 | 4 / 1 / 20 | 10 / 1 / 20 | 1010:0ea0 |
| 4 | (s+3)*2 / 1 / 20 | 10 / 1 / 20 | 1010:4f76 |
| 5 | s+10 / 1 / (85-10*s) | 10 / 1 / 20 | 1010:55ac |
| 7 | 3 / 3 / 15 | 10 / 1 / 15 | 1010:6a56 |
| 8 | 6 / 1 / 25 | 6 / 1 / 20 | 1010:c80e |
| 9 | 4 / 1 / 75 | 10 / 1 / 20 | 1010:d8f4 |
| 10 | s*3 / (s+2) / 20 | 10 / 1 / 20 | 1010:33b4 |

All native entries reproduced with `runtime_probe.cpp` at all four skills.
Class 4's chance happens to match at skill 2 only. Class 12 is covered by
P-017. Class 7's `1010:6f7a` later sets threshold 25 after its special spawn
divisor becomes zero; the native helper remains at 10 and group 1. Its
separate special spawning logic does not replace common `1008:81dc` rolls.
Do not overwrite correctly reconstructed later transitions for classes 1/8.

## P-019 — Gauntlet runtime hazards transpose column and row

**Confirmed, medium repair priority; destroyed-transporter and flood events.**

Original `1010:518c` draws column `random(24)+10`, then row
`random(7)+column-4`. Direct column-pointer indexing and `af5a` arguments
agree. Both seven-hazard transporter events and the 24-hazard late flood
use this ordering. Native `AdvanceBoardRuntime` labels the first draw row
and places the hazard with transposed coordinates.

```text
gauntlet-hazard actual=26,28 expected=28,26
```

The probe destroys the northeast transporter on a real class-4 board,
isolates other events and sets RNG seed 1. Both compared coordinates are
in bounds. Original random rows can also exceed the board bounds; fixing
parity must not introduce unsafe writes (see the related save issue P-014).

## P-020 — Procedural late escalations fire one turn early

**Confirmed, medium repair priority; timed spawning.**

Original `1008:dd76` uses strict comparisons at `(depth+10)*30` and
`(depth+9)*50`. Current generic-board runtime uses `>=`. In-memory boundary
probes at depth 1, with earlier phase bits already set, report:

```text
threshold turn=330 flags=7 expected=3
threshold turn=500 flags=15 expected=7
```

These events should first execute at counter 331 and 501. The earlier
50/150 checks and the separate recursive class-11 checks already use `>`.

## P-021 — Hub lever opens the wrong rectangle and leaves machinery barriers intact

**Confirmed, high repair priority; hub mechanism functionality.**

Original `1010:4692` clears columns 30..31, rows 28..31 via `1008:ce88`,
then restores eight explicit edge tiles. Native code clears columns 28..31,
rows 29..30 instead. Its following eight assignments match the original,
but cannot repair the wrong cleared rectangle. The eight `af5a` edge effects
also transpose their coordinates.

Two subsequent `1008:bb7e` operations are omitted: animated zero-to-zero
refresh over columns 30..31, rows 29..30, and removal of tile `0x5f` over
columns 30..34, rows 19..23. The latter opens the activated structure's
enclosure. Finally, all three released groups appear at (21,32), not the
original (32,21), despite the native structure center itself being correct.

Real class-3 map, player placed on the lever, other periodic spawning isolated:

```text
hub-lever actors_at=21,32 expected=32,21 barriers_before=16 after=16 expected_after=0
```

Original `bb7e` and `ce88` column-pointer indexing establishes argument order;
this is not inferred from decompiler variable names. Repair terrain, effects,
barrier removal and released actor coordinates together. Add an actual-map
lever regression, not just a synthetic center-tile assertion.

## P-022 — Alarm-room structure placement drops helper behavior

**Confirmed, medium repair priority; placement and missing presentation.**

Original `1010:c9f8` draws column from `random(36)`, row from `random(32)`
and calls `1008:cf50`. Current class-8 runtime swaps those axes and builds
tiles inline. It omits `cf50`'s nine type-`0x16` overlays and sound `0x16`
for every successful placement. Original player-overlap rejection does not
consume its shared attempt budget; native rejection does.

An isolated open-terrain callback probe with seed 1 places the original first
center at (7,5). Native leaves that cell empty and produces no overlays:

```text
alarm expected_first_center=7,5 actual_tile=0 expected_tile=24 overlays=0 expected_at_least=9
```

The synthetic terrain isolates the helper difference; it is not presented as
an original-game screenshot or complete run. Check exact exhaustion behavior
against raw loop branches when replacing the placement budget implementation.

## P-023 — Common late structure placement also transposes coordinates

**Confirmed, medium repair priority; recursive and procedural escalation layouts.**

`PlaceRandomGuardedStructure` reverses the original column `random(36)` and
row `random(32)` draws. This affects class 11 (`1010:d06a`) and classes
17..25 (`1008:dd76`), independently of P-020's timing. The helper's comment
also incorrectly labels the original ranges. On open terrain, seed 1, the
original first center is (7,5); the native helper leaves that cell empty.
Unlike class 8 (P-022), this helper does produce construction effects/sound.
Its bounded retry cap is a separate native safety adaptation; do not restore
an unbounded UI hang merely to match the original failure path.

## P-024 — Hub structure replenishment omits reward and escalation branches

**Confirmed, high repair priority; constructor placement and ongoing hub rules.**

Raw `1010:4a83..4a90` adds 11 to the column roll and 6 to the row roll.
Native `PlaceLevelThreeStructure` adds 10 and 5, shifting every candidate
up/left, including the constructor's three calls. Availability decisions and
subsequent RNG consumption can consequently diverge, not just final graphics.

For runtime calls only, original `4bed..4c12` can increment the stored spawn
threshold, capped at 75, with a difficulty-dependent roll. Native drops this
entire branch and models hub threshold solely from turn count.

When the pre-placement center count is zero, `4c12..4e2c` additionally:

- Can change tile (2,9) from `0x60` to `0x5c` on `random(5)==5`.
- Can change tile (31,4) from `0x5f` to `0x5c` on `random(12)==3`.
- On `random(10)==2`, searches for space, clears a 2x2 region, creates a
  `c452` chest/transporter structure and adds construction effects.

All three branches, their effects and their random draws are absent. Restore
the actual runtime-only branch conditions rather than adding reward rolls
to constructor calls. Raw original retry logic has an unsafe/nonterminating
failure condition; preserve native responsiveness while matching successful
paths and document the safety adaptation separately.

## P-025 — F1 help dispatch is misread as the letter P

**Confirmed, medium repair priority; help access from the attract screen.**

`1008:6f46` permits literal key codes `0x70`, `0x4e`, `0x4c` through the
non-root/demo gate. `0x70` is `VK_F1`, not uppercase P (`0x50`). Its later
dispatch sets help-page index 1; C sets page 6. Current `MainWindowProcedure`
permits P instead and uses it to navigate message history. F1 is ignored in
the demo. In gameplay it is grouped with C and opens page 6 rather than 1.
Correct both the gate and page selection, plus the misleading source comment.
This finding is based on original numeric key comparisons and native event
dispatch, not a GUI test. The obsolete R registration shortcut is excluded.

## P-026 — Type-3 treasure table is shifted and can award nothing

**Confirmed, high repair priority; lost pickups and wrong reward distribution.**

Raw `1008:a2a1..a2b1` indexes selector-1080 words at `1518 + 2*random(5)`.
The actual five words at `151a..1522` are `13,12,7,11,8`. Current
`ResolveOverlayEffect(3)` uses `12,7,11,8,ffff`, shifting all rewards and
replacing the last with an unsupported item. The separate one-in-four
extra-life branch agrees. Types 1/2 tables were also checked against their
original data and match, rather than being assumed affected.

`pickup_probe.cpp` isolates each non-life table result:

```text
roll=1 seed=4  item=12 expected=13
roll=2 seed=2  item=7  expected=12
roll=3 seed=1  item=11 expected=7
roll=4 seed=5  item=8  expected=11
roll=5 seed=10 item=-1 expected=8
```

`-1` means no inventory entry. Smart Bomb (13) disappears from this reward
table; the radio-bomb result (8) and its companion detonator are lost on roll
5. Raw `a3d0` quantity branches otherwise agree for these valid item types.

## P-027 — A same-turn life pickup cancels an already-fatal hit

**Confirmed, medium repair priority; death-state semantics.**

Original `1008:2920` sets application `f4=0` when playable health reaches
zero. Later pickup `1008:a16a` type 9 increments health (`f0`) but does not
reset `f4`. End-of-turn `1008:1b22` checks that death latch. Current code
has no equivalent latched death state: `HandleEndState` checks only final
`Health()==0`, allowing a later same-turn pickup to resurrect the player.

Probe: health 1, ordinary enemy and type-9 pickup together one cell east,
move onto them through `ApplicationState::MovePlayer`:

```text
fatal-hit-life-pickup health=1 game_over_notice=0 expected_notice=1
```

This is distinct from P-005's wrong board after death and the visual lifetime
question. Preserve original ordering; do not simply skip all pickups/turn
processing on the first hit, because the original completes the turn before
the lifecycle decision.

## P-028 — Explosive chest impacts bypass the chest-opening callback

**Confirmed, high repair priority; incomplete destruction and missing loot.**

Raw `1008:7ef6..7f13` invokes vtable `+38` (`bc64`) with impact class 4
when the struck tile is `7f..82`. `bc64` normalizes the corner, clears all
four chest cells, creates four explosion effects and rolls the board-specific
`+3c` treasure callback for each cell. Native `DamageBoardSquare` instead
clears only the directly struck cell; no chest callback or treasure rolls run.

`terrain_probe.cpp` hits the northwest corner of a correctly formed 2x2 chest:

```text
blast-chest strength=1 remaining_tiles=3 expected=0
blast-chest strength=2 remaining_tiles=3 expected=0
blast-chest strength=3 remaining_tiles=3 expected=0
```

The input impact classes are bombs/missiles/radio blasts; the chest helper is
always passed class 4. Do not confuse that helper argument with a fireball:
original fireballs are consumed by solid terrain without terrain damage.
Coordinate and loot-order defects in the separate Bronze Key path remain P-010.

## P-029 — Resisted projectile impacts omit original effects and feedback

**Confirmed, medium repair priority; missing animation, sound and messages.**

Original `1008:bfc4` adds an explosion overlay and sound 12 before resistance
checks. Native structure damage returns on a resisted hit before adding either.
The random resisted ordinary bomb/reinforced missile branches also omit their
original messages at `1008:d440` and `d466` (bomb/missile had no effect).

Separately, `1008:77d0` adds the same effect/sound when a bomb or missile hits
an indestructible solid tile (descriptor bit 4). Native calls
`DamageBoardSquare`, which returns at that flag check, then removes the
projectile silently. Terrain must remain intact, but impact feedback must not
be suppressed. The hidden-rock treasure message at `1008:d0a0` is also absent
from native `DamageBoardSquare` despite the drop itself being implemented.

These are presentation/feedback defects, not a request to make protected
terrain destructible. `terrain_probe.cpp` isolates reinforced-bomb and
indestructible-wall hits without accepting another game turn.

## P-030 — Casino bonuses open transposed doors and treasure-room regions

**Confirmed, high repair priority; earned reward access remains blocked.**

Raw `1010:1786..17ea` addresses columns 13/35, row 19 for the first two
bonus doors. Native changes (19,13)/(19,35). Final bonus `184c..18dd`
clears columns 7..9 and 32..34, rows 2..4; native swaps axes again.
Matching overlays are transposed with the terrain. Actual casino map, held
Royal Flush, each bonus stage isolated in `casino_probe.cpp`:

```text
bonus stage=0 original_door_tile=55 expected=92
bonus stage=1 original_door_tile=55 expected=92
bonus final blocked_original_cells=18 expected=0
```

Here 55 is the retained wall and 92 is the expected key door (`0x5c`). Final
state still marks the casino closed/completed, despite retaining its original
18 blocked treasure-room cells. Test actual access locations, not only flags.

## P-031 — Casino defense and periodic rebuilding are omitted

**Confirmed, high repair priority; missing room-specific gameplay.**

Original `1010:1180..1207` tracks remaining top-edge chests at columns
11..29 step 3, row 0. A decrease spawns 1..2 type-3 captains from each intact
center (14,6), (21,6), (28,6), then updates the previous count at `+e6`.
Native has neither that tracking state nor the callback branch.

Original `120c..12a0` also rebuilds missing corner machines and those three
top structures every 200 board turns, using forced `cf50` placement and
construction effects. Native omits it. Isolated actual-map probes:

```text
casino-chest-defense actors=0 expected_min=3
casino-rebuild tile=0 expected=30
```

First probe removes one chest origin; second destroys center (4,33) and
sets counter 200. Existing retaliation is advanced by `28ac` at `12a2`
**before** processing the player's casino interaction; native advances it
afterward. Preserve this ordering when restoring the missing runtime stages.

## P-032 — Casino pays low pairs and rejects mixed-suit ace-high straights

**Confirmed, medium repair priority; wrong payout classification.**

Raw `1010:1f9a` distinguishes qualifying pairs (J/Q/K/A at `2060..2074`)
from lower pairs, and `1da4` pays only the qualifying classification on an
ordinary table. Native pays every pair. Original `20c2` represents the ace
at both straight endpoints. Native straight search stops at king-high;
its separate Royal Flush check catches only the same-suit case.

Hold-all ordinary-table probes with one unit bet:

```text
poker low-pair payout=1 expected=0
poker ace-high-straight payout=0 expected=8
```

Hands are 2/2/4/8/10 and A/10/J/Q/K, with mixed suits. The ordinary/wild
payout constants inspected in `1da4` agree.

Extended `poker_rules_probe.py` translates the raw classifiers `1f9a`,
`20c2`, `21f0`, `224a` and native evaluator, testing all 22,958 realizable
rank-multiset/non-wild-suit-class combinations across ordinary, deuces-wild
and sevens-wild tables. It reports 2,017 payout differences: 1,980 low-pair
cases, 23 ordinary/wild ace-high cases, and 14 four-wild-card cases. This is
a finite classifier-model comparison, not an emulator or dealing/RNG test.

The four-wild case is an **original quirk**, not ideal poker: `1f9a` returns
four-of-a-kind as soon as an absent lower rank plus four wilds equals four,
before encountering the real singleton's five-of-a-kind. For four deuces
and a three, the later straight-flush branch consequently pays 20, while
native correctly recognizes mathematical five-of-a-kind and pays 25.
Keep this distinction explicit when choosing strict parity versus a rules fix.
The application-level held-hand probe also covers this case and the wild
ace-high hand A/2/10/J/Q (mixed non-wild suits), expected return 4.

## P-033 — Casino retaliation uses score instead of elapsed board turns

**Confirmed, medium repair priority; incorrect difficulty escalation.**

Raw `1010:16ec..170f` compares the board's signed 32-bit counter at
`+3e/+40` to 500 and 1500. Current runtime checks player `score_` instead.
The bet-size and multiplier increments and cap 3 agree. A held ordinary
Flush (multiplier 10), bet 1, demonstrates both directions:

```text
retaliation late=0 mode=3 expected=2  [turn=1, score=2000]
retaliation late=1 mode=2 expected=3  [turn=1601, score=0]
```

Correct the source comment as well: these are time thresholds, not score
thresholds. Retain strict `>` comparisons and original callback scheduling.

## P-034 — Casino retaliation loses gates and changes an active cycle

**Confirmed, medium repair priority; room mechanics and scheduling.**

Original `1010:28ac` restores the machine's surrounding boundary, then
explicitly writes directional door `0x3e` at (4,31) on the left machine and
`0x3f` at (36,30) on the right (`2bf8..2c18`, `2c4f..2c72`). Native restores
only generic border tiles, permanently replacing those doors with walls.

`28b5..28c0` sets the retaliation mode only when the cycle is idle. A win
during an active cycle calls this same function again (`171b`), advancing
the existing cycle a second time after the regular pre-interaction call.
Native overwrites its mode and performs only one post-interaction step.
An active mode-1 cycle at state 2 plus a held flush win should remain mode 1
and reach state 4, not switch to mode 2 and reach state 3.

The mode-3 left-machine captain spawn is also different: original raw
`2a38..2a50` explicitly spawns at (36,30), whereas native puts all actor
types at the left center (4,33). This asymmetric original location should
not be silently replaced with a more intuitive common-center spawn.

## P-035 — Eight-way shots and missile fragments use transposed launch order

**Confirmed, medium repair priority; ordered combat and capacity parity.**

Raw `1008:22ca..2300` (eight bombs), `2305..233b` (eight missiles), and
`7f48..7f85` (missile fragments) enumerate column velocity in the outer
loop and row velocity in the inner loop, omitting (0,0). `8e22` stores the
two arguments at projectile offsets +6/+8 respectively. Native enumerates
row first for all three paths. The original order begins (-1,-1), (-1,0),
(-1,1); native begins (-1,-1), (0,-1), (1,-1).

This changes which direction gets the remaining slots near the 150-shot
limit and which enemy receives each RNG outcome when the shots hit. It is
distinct from P-007's deferred fragment scheduling. The combat probe fires
each eight-way item in an empty central region and reports its second
projectile direction; both currently return (0,-1), expected (-1,0).

## P-036 — Remaining signpost, identification and casino prompt mismatches

**Confirmed, lower repair priority; missing or misrouted presentation.**

- Maze classes 13–15 share sign callback `1010:902e`, displaying the literal
  at `1010:905c`, **The Labyrinth.** Native instead returns the class-16
  message, **Things may not be as they appear...** Class 9 has an empty sign
  callback; native's extra class-9 Labyrinth string is not that callback.
- Original `1010:0b4e` identifies either moving boss using its tracked 3x3
  bounds at +54/+58, selecting names at `1010:0e0c` and `0e3c`. Despite a
  comment claiming the same behavior, native `IdentifySquare` checks fixed
  corner rectangles and never consults the tracked boss bounds. Ordinary
  boss tiles therefore fall through to `???` away from those corners.
  Even at initial positions, target 0 incorrectly says **Power Plant**
  instead of **Gormaug**, and target 1 says **Gormaug** instead of **Gorloth**.
- First casino bet invokes `2874` with `1010:31b6`: **You may continue
  betting, or pull the lever to deal.** Native changes the lever and state
  but retains the initial "Place your bet" ticker. This prompt is not the
  maximum-bet warning; it belongs to the normal first-bet transition.
- Moon Room seal switches request sound 22 (Construction) at `1010:656c`
  and `65d7`; native requests sound 12 (BoardBreak). The late three-portal
  branch `633a..63df` emits construction overlays but no sound call; native
  adds sound 22. Other inspected Moon Room trap sounds and spawn coordinates
  agree. These are event-selection differences, not sample-asset differences.

`presentation_probe.cpp` inspects actual constructor signs and initial boss
positions, and makes a normal first casino bet in isolated memory. No GUI
or player files are touched. Most other fixed room-sign literals checked
against their original binary strings match; do not replace those again.

## P-037 — Procedural chests use the common room's treasure table

**Confirmed, medium repair priority; missing virtual loot override.**

Procedural classes 17–25 use `1008:df74` at vtable slot +3c, not common
`bed2`. Its entire raw body gives a Bronze Key opening type-1 treasure on
random(5) outcomes 1–4, type-2 on outcome 5, and no reward for other impact
classes. Native `RollObjectTreasure` lacks this override and uses random(9):
four type-1 outcomes, four type-2 outcomes, and one empty outcome. Thus key
chests can reveal fewer than four pickups even after P-010's traversal fix.

Restore this override together with P-028's explosive chest callback:
procedural explosive chests intentionally produce no loot and consume no
loot RNG. Otherwise adding the currently omitted callback will incorrectly
give them the common room's explosion rewards. Gauntlet and Main Portal
loot overrides were checked against `1010:54ec` and `d848` and match.

The terrain probe compares 128 seeded key-loot calls with the original
five-outcome formula; its expected mismatch and empty counts are both zero.

## P-038 — Xoid Fortress clears seals instead of activating them

**Confirmed, medium repair priority; room-specific combat progression.**

When one of the four sentinel centers disappears, `1010:37a2` calls
`bb5a` with old tile 0x21, replacement 0x20, effects disabled (`3894..389c`).
It then changes 0x20 to 0x22 inside columns 9..12, rows 19..22. Native
instead removes every 0x21 to empty, losing the newly activated portals.
The four sentinels, Emerald pickup rectangle, one-shot flags and second
replacement rectangle otherwise agree with the inspected raw body.

The actual-resource runtime probe destroys a sentinel and counts remaining
active 0x20 seals. Do not test only the opened flag or Emerald availability.

## P-039 — Alien maze completion places its Moon Key at swapped coordinates

**Confirmed, medium repair priority; quest reward placement.**

Original `1010:3d50` completes the three quadrant triggers, activates the
core rectangle, then calls `af5a` with Moon Key type 4 at **(20,18)**
(`3e64..3e75`). `af5a`'s final argument is the column; its preceding
argument is the row. Native uses **(18,20)**. The metal door at (20,16),
lieutenant spawn at (20,18), spawn quantity and quadrant trigger locations
agree. The runtime probe activates the three real-map sentinels and reports
the resulting key coordinate.

## P-040 — Triggered mines run before derived room callbacks instead of after

**Confirmed, high repair priority; shared turn ordering.**

Concrete callbacks such as `1010:3d50`, `37a2` and `d968` perform their
room-specific work, then call common `1008:ba30`. That helper increments
the turn counter and resolves pending mine markers through `aacc` before
sign/transition handling. Native calls `ResolveTriggeredMines()` at the
very start of every post-actor `AdvanceBoardRuntime`, before selecting the
derived room branch. Room triggers therefore see mine damage one turn
earlier and RNG-dependent room actions exchange order with mine outcomes.

An isolated actual-map alien-maze probe queues a pending mine at (5,29),
whose blast can destroy the first sentinel's 3x3 structure. On a successful
blast, original keeps its chamber-active flag until the next callback;
native clears it immediately and reveals that quadrant during this callback.
This is separate from P-016's captain replacement order *within* mine blasts.
Fix by recovering the shared callback tail, not by adding per-room delays.

## P-041 — Statue Room special spawning bypasses the original actor limit

**Confirmed, medium repair priority; combat population and cap semantics.**

Original `1010:6f7a` spawns its four special portal actors through common
`1008:80c0`, which stops at 300 live actors. Native's corresponding loop
uses unconditional `actors_.push_back`, unlike its other guarded spawn
helpers. It can therefore add four more actors to an already full board.
The isolated actual-map runtime probe starts with 300 actors, chooses a seed
whose special-wave roll succeeds, and reports 304 instead of 300.

Preserve the caller's RNG consumption even when capacity prevents insertion;
the type roll still precedes `80c0` in the original. Audit captain replacement
paths against the same rule when repairing P-009 and P-016.

## P-042 — Procedural guarded structures overwrite occupied terrain

**Confirmed, high repair priority; damaged generated structures.**

Original `1008:d4e0` places guarded 3x3 objects through `c1e6` with
force zero. `c1e6..c240` rejects the entire placement if any covered cell
is occupied or outside the board. Native's constructor-local
`placeLargeObject` writes all nine cells without checking occupancy.
It can overwrite part of an earlier chest or another guarded structure.

The constructor probe models the original prefix through this placement
phase. At class 25 (depth 9), skill 4, seed 1, native tile (31,19) is 15,
where the original retains chest tile 127. Later original construction
phases cannot overwrite that occupied cell. This is an actual-resource
native comparison against an instruction-derived prefix model, not execution
of the original binary. Restore the all-or-nothing force-zero check;
do not globally prohibit overwrites at deliberately forced call sites.

The early procedural chest placements omit the analogous `c452` check:
original requires all four cells of the 2x2 chest to be empty; native only
checks bounds. Two overlapping chest attempts can therefore leave a partial
chest too. The prefix oracle now models both helper contracts. This second
path is confirmed by source comparison; the cited seed-1 example above
specifically reproduces the guarded-structure overwrite.

The same force-zero contract is also omitted in class 11 (`1010:cb9c`):
all three random-object branches call `c1e6` with zero, while native
`placeRandomObject`/`placeLargeObject` overwrite any existing footprint.

## P-043 — Procedural chest-room eligibility ignores difficulty

**Confirmed, medium repair priority; difficulty and generation RNG.**

Original `1008:d948` tests `depth > 5 - skill` before its one-in-four
roll permitting an extra 6x6 chest room. Native tests `depth > 4` for
every skill. Skill 1 agrees; skills 2, 3 and 4 should become eligible
at depths 4, 3 and 2 respectively, rather than depth 5.

The missing roll also shifts subsequent RNG even when no room would be
chosen. This is a direct raw-branch/native-expression comparison; it is
not a measured frequency claim about complete generated maps.

## P-044 — Procedural room retries change original termination semantics

**Confirmed, lower repair priority; generation parity / original quirk.**

Original 5x5 room placement decrements its ten-attempt budget after each
candidate (`1008:d849`) and exits the entire room-building phase when
the budget reaches zero (`d85b..d861`, target `d948`). Thus even a valid
tenth candidate is discarded; ten failures also cancel remaining rooms.
Native accepts the tenth candidate and, after ten failures, tries the next
requested room. The 6x6 chest-room phase likewise discards a valid tenth
candidate (`d9f3`, `da02..da08`), while native accepts it.

These branches are confirmed in raw instructions and the native loops.
A full-map seed reproducer has not yet been established. Treat the
tenth-attempt discard as an original quirk when deciding strict parity;
do not silently label the native behavior a deliberate adaptation.

## P-045 — Bubble hits on nonempty terrain lose their animation and sound

**Confirmed, medium repair priority; combat presentation.**

Original `1008:77d0`'s direct bubble-hit branch queues overlay 0x16 and
sound 9 for actor classes 0/1 before checking whether the underlying tile
is empty. Only the fossilized terrain write is conditional on tile zero.
Native `AdvanceProjectiles` places all three operations inside that check,
so the enemy disappears without its bubble-hit animation or pop sound.

The combat probe puts an ordinary enemy on drain tile 0x23 and advances
a moving bubble into it. The enemy is removed and the drain correctly
remains 35, but effects=0 and pop=0, instead of one effect and a pop.
This is an isolated collision fixture, not a recorded original-game run.
Keep the terrain guard while moving the presentation outside it.

## P-046 — Fixed commands override valid custom keyboard bindings

**Confirmed, medium repair priority; input dispatch.**

Original `1008:6f46` tests configurable movement and command bindings
before literal N/L/C/F1 handling. Native `MainWindowProcedure` handles
N/L/C/F1 first, then sound and other commands, and movement last.
The native binding dialog and `SetKeyBindings` permit these keys and
duplicates. Assigning movement to C therefore opens help instead of moving;
assigning a movement key also to sound toggles sound instead of moving.
Original movement takes precedence in both examples. This is established
by dispatch and validation source comparison, not GUI input injection.

Native swaps internal diagonal binding slots 5/6 together with their defaults
and labels; that alone does not change default movement and is not a bug.
The omitted literal R registration shortcut remains an accepted exclusion.

## P-047 — Breakout switches lose state and the hidden mechanism ignores action type

**Confirmed, medium repair priority; room runtime.**

Original `1010:5a46` retains switch flags at +0x54/+0x56, independent of
the wall tiles being toggled. Native infers the flags from (31,3) and (34,6).
After opening the first tunnel and clearing its anchor tile, the next toggle
opens it again instead of restoring its framed perimeter. The runtime probe
reports tile 54 instead of 56. Restoring explicit flags also requires saving
them; do not infer them from potentially damaged terrain on reload.

The original leading action gate (`5a4c..5a61`) also covers the hidden
17-cell mechanism at `5d64..5e0b`: it requires a nonzero movement vector
and a movement action. Native gates only the two visible switches, then
activates the hidden mechanism even on a wait or item action. A probe with
the first breach already recorded reports transformed=1 on a stationary
callback, where original skips it. Native also omits the mechanism's
construction sound at `5e01..5e0b`. The 17 cells and framed switch geometry
otherwise agree with the inspected routine and `cafa` helper.

## P-048 — Gauntlet flood deletes enemies without their removal effects

**Confirmed, lower repair priority; missing animation.**

After its 800-turn threshold, original `1010:518c` visits actor indices
count-1 down through 1, adds effect 0x16 at each coordinate, then clears
the actor count. Native simply clears `actors_` after placing flood terrain.
The removal effects are missing. Original's omission of an effect for index
zero is a documented quirk, not a reason to add an effect to every actor
when implementing strict parity. This is separate from P-019's swapped
flood coordinates. The comparison is source-level, without a new GUI trace.

## P-049 — Actors and projectiles cover the separate transient-effect pass

**Confirmed, medium repair priority; effect layering.**

Original `1008:1b22` calls `8b2c` after the world/room update. That routine
drives five `8be6` passes which directly paint transient sprites over the
board, with no actor-occupancy rejection. Native `DrawLevel` paints these
effects in the ordinary overlay loop, before actors and projectiles, so a
surviving or newly spawned entity hides them. The separate player-hit
exception is already implemented and is not reported missing here.

An offscreen atlas comparison places effect 0x16 on an actor square.
Native differs from the original first-frame sprite 0x5b in 127 of 144
pixels. No game window is launched. Keep collectible-under-terrain rules
separate from this late effect pass; moving every overlay above actors is
not an equivalent repair.

The raw effect driver also confirms the lifecycle concern under P-005:
it completes the five-frame batch before end-state dialogs/reset. Native
arms asynchronous effects and immediately handles/reset end state. A
nonblocking UI can retain that ordering without copying Win16's busy loop.
No exact millisecond claim is made from Win16's requested timer interval.

## P-050 — Power Plant progression gates are replaced by a first-turn shortcut

**Confirmed, high repair priority; final-room progression and combat balance.**

Raw `1008:e636` keeps separate one-shot flags. The security-door event
checks (23,29) != 0x61, releases actors in the right half, sets spawn
chance to skill+9 and group size to max(skill+2,4). The second event checks
(21,13) != 0x5c, forcibly reconstructs four corner portals and changes
group size to skill+5. Native performs both events when turnCounter==1,
regardless of the watched tiles, and never retains those separate flags.
Its corner update also only changes surviving 0x1f centers rather than
reconstructing the original forced 3x3 objects.

The arena lever's original coordinate is **(1,19)** (`e717/e726` and
constructor `e07a`'s overlay call). Native places and checks it at **(19,1)**.
The runtime probe with initially intact gates changes the corner center
from 31 to 29, although original leaves it dormant. Entering (1,19) does
not set the native arena flag. Restore the three independent event gates
and persist them; adjusting a turn threshold cannot reproduce this logic.

## P-051 — Power Plant Smart Bomb shielding never switches off

**Confirmed, high repair priority; weapon behavior.**

Original `1008:eec2` protects the central rectangle only while the arena
flag at +0x46 is zero (`eee7..eeec`). After the lever, those actors instead
receive the ordinary one-in-two resistance roll. Native checks coordinates
unconditionally, even when `levelOneArenaEntered_` is true.

The isolated probe sets the arena flag, puts one lieutenant at (12,12),
and chooses a seed whose first resistance roll succeeds in killing it.
Native keeps one actor; original would keep zero. Captain replacement
capacity/order remain covered by P-009/P-016 rather than being declared
correct here. The original mitigation-versus-shield message precedence
and rectangle bounds otherwise agree.

## P-052 — Power Plant starts with two lieutenants instead of captain groups

**Confirmed, high repair priority; constructor difficulty and RNG.**

Original `1008:e07a` makes two `80c0` calls at (33,15) and (34,20),
each with actor type 3, behavior 2 and quantity `NextRandom(5)+2*skill`.
Native inserts one type-2 actor at each coordinate and omits both rolls.
At skill 1 the original therefore creates 6..14 captains across the two
locations; the actual-map probe finds zero captains and two lieutenants.
Besides substantially reducing combat difficulty, the missing rolls shift
the constructor's final gameplay RNG state. This is separate from P-001's
later player-state adoption problem and P-018's spawn-rate defaults.

## P-053 — Boss defensive waves have the wrong population, types and RNG

**Confirmed, high repair priority; final-boss combat.**

Original `1010:00ec`'s non-adjacent defensive wave creates eight actors
around the 3x3 perimeter, with no center actor. Corners choose type 2/0,
edge middles choose type 1/0. Every actor has behavior 0, and each type
roll is followed by `NextRandom(1)` for quantity. Native creates nine
actors including the center, uses type 2/0 everywhere, omits quantity
rolls, and assigns behavior 2 through its shared local spawn lambda.
That behavior mismatch also affects the four-corner close-player branch.

An isolated native callback probe forces the one-in-fifty wave: count=9,
center=1 and nonhunter=9, versus original 8/0/0. Preserve the apparently
redundant range-one rolls because they advance the original gameplay RNG.

The same callback's moving-boss charge check also evaluates its adjacent
radio-charge RNG for charges *inside* the new boss footprint in native
code. Original only rolls for charges outside that footprint, after the
inside branch has failed. Native can consequently add an extra effect and
advance RNG unnecessarily for an interior charge. Boss hit count/score
thresholds inspected in `ec6e` and this callback agree for the first nine
damaging hits; that does not make the surrounding callback correct.

## P-054 — Demo restarts after, rather than during, its final command

**Confirmed, low repair priority; attract-mode timing and initial state.**

All 453 stored command/shift pairs match, but the sentinel ordering does not.
Original `0c26`, `2d80`, `38f6` and `dafe` increment the cursor when queried
for shift state, detect the next record's shift=2 sentinel, and rebuild the
demo before returning. `1008:6f46` then dispatches the already-read command
against the newly current board. All four last commands are unshifted waits.

Native `AdvanceAnimation` executes that wait on the old board, returns with
the cursor at the script length, and restarts on the next animation tick
without executing the wait on the new board. The isolated boundary probe
finds cursors 41/139/21/252 after the final command, instead of the original's
reset cursor 0. Thus the next demo's initial world state is missing that
first wait/turn, in addition to the one-tick changeover difference. This is
not a missing script or a blocker to starting normal gameplay.

## P-055 — Actor entry omits projectile avoidance and excludes tile 8

**Confirmed, high repair priority; enemy movement and combat.**

Original `1008:91ec` uses actor type and occupancy flags, not just terrain
passability. Types 2 and 3 entering an empty terrain cell with a projectile
roll `NextRandom(2)` and reject on 2. On other actor-passable terrain they
reject projectile-occupied cells unconditionally. `a652` establishes bit 4
for all projectile records. Native `AdvanceActors::canEnter` never checks
projectiles or makes that random draw, eliminating this defensive behavior
and changing the shared random stream.

The same predicate's original machinery range is inclusive 8..0x17
(`92ef` compares 8 with signed JL; `9309` compares 0x17 with JLE).
Native uses `tile > 8`, making the top-left corner tile impassable.

The isolated vector-actor probe demonstrates all three cases: a type-0 actor
fails to enter tile 8 (column 10 instead of 11); a type-2 actor enters a
bubble-occupied drain tile and consumes the bubble (column 11 instead of
10); the same happens on tile 0 with a seed whose original avoidance roll
is 2. Actor occupancy bitmap/self-cell behavior remains a separate open
comparison; this finding does not assume that native vector scans reproduce
the original bitmap's stacked-actor semantics.

## P-056 — Energy Sphere destruction does not disable structure spawning

**Confirmed, high repair priority; final-fight difficulty and shared state.**

Original sphere destruction writes application word `+0xfe = 1` at
`1008:ed80`. `1008:81dc` tests that word before its entire scan: once the
sphere is destroyed, ordinary machinery spawns stop on every board, and
the scan consumes no RNG. This is separate from the victory flag at +0x100
and does not require first defeating both bosses.

Native sphere destruction clears the tiles, awards points and queues the
message, but records no shared sphere-destroyed latch. Its general
`SpawnStructureActors` has no corresponding gate. A probe first destroys
the actual sphere through `DamageBoardSquare`, then isolates one surviving
type-0x20 spawner: native creates one enemy and advances RNG, versus zero
enemies and unchanged RNG in the original. Fixing only the Power Plant's
tile test would not preserve this application-wide state on room transitions
and save/load. The separate boss defensive waves are not suppressed by this
particular original gate.

The early native `levelNumber==1 && turnCounter<=1` return also omits the
original chance rolls at zero spawn probability. That is another RNG effect
of P-050's incorrect initial spawn gating, not evidence that the original
skips those rolls.

## P-057 — The active five-million-point Sphere ranking bonus is missing

**Confirmed, medium repair priority; high-score qualification and ordering.**

The ledger previously described `1010:ed28`'s second bonus as an unused
historical flag. That assessment was incorrect: `1008:ed80` sets the
application's Energy Sphere flag +0xfe, and the death/victory and quit-without-
saving paths pass that exact word to `1018:0252`. The score record stores it
at +0x14; `ed28` adds 5,000,000 when it equals 1. The separate +0x16 victory
flag adds another 10,000,000. Both bonuses affect ranking; raw gameplay
score remains separate.

Native only retains the victory bonus. Someone who destroys the Sphere and
then dies or quits loses the five-million ranking credit; a normal victory
after destroying it also ranks five million too low. This follows from the
missing shared state in P-056 but requires changes to qualification, pending
score submission and persistent score records as well as gameplay state.

An in-memory probe fills the leaderboard with ten ordinary one-million
scores, destroys the real Sphere and invokes death handling. Native does
not offer score submission even though the original effective score exceeds
five million. No user score file is read or written.

## P-058 — Two boss sound resources are swapped

**Confirmed, low repair priority; audio presentation.**

Original `1008:2eba` resolves sound index 26 to `SNDYEOW` and index 27 to
`SNDWAKEUP`. Native `GameplaySound` and `PlayPendingGameplaySound` reverse
both names. Consequently the inspected boss-hit path queues index 26 but
plays the wake-up recording, while awakening queues index 27 and plays the
hit recording. Correct both the enum names and call-site intent while
preserving original numeric priorities.

The resource probe compares every entry: these are the only two dispatch
differences. All 31 WAV assets are present and byte-identical to their
original NE resources; no replacement or missing audio asset is needed.

## P-059 — Maze entry omits the initial nearby-wall reveal

**Confirmed, low repair priority; initial maze presentation.**

After converting visible wall tiles 0x36..0x40 to hidden 0x2b..0x35,
original `1010:86a0` converts the walls in the entry's surrounding 3x3
back to visible forms before returning. Native classes 13..15 perform
the hide pass but omit this final reveal. Their runtime reveal only occurs
after a subsequent accepted turn.

With seed 1, the runtime probe finds respectively 3, 3 and 4 hidden
neighbor walls for the three maze classes; the original postcondition is
zero. Maze passage generation and constructor coordinate/RNG sequences
otherwise agree in the reviewed paths, subject to overlay ordering P-015
and the separate wall-shaping helper dependency.

## P-060 — Generated branch levels always receive the introductory sign

**Confirmed, low repair priority; branch-specific constructor state.**

Original `1010:cb9c` reserves an empty entry and its (+2,+2) square,
but writes signpost 0x7e there only when its extra constructor argument
equals one. The hub's `4382` path supplies one; factory/ChestRoom and
other generated descendants supply zero. Native class 11 always writes
the sign and does not receive this constructor argument. The isolated
maze-constructor probe finds one sign in the generic class-11 constructor,
where non-hub creation requires zero. Preserve the empty-square test even
when suppressing the sign: the original performs that test unconditionally.

## P-061 — Alien maze walls are converted before their dormant-state pass

**Confirmed, high repair priority; wrong initial terrain and activation.**

Original `1010:3a96` leaves maze walls as tile 3 after recursive carving,
adds its chambers, then converts every 3 to dormant alien wall 0x5d
and every empty square to 0x5e. Only the entry rectangle (5,5)..(9,9)
is activated and wall-shaped through `3ece` before return. Raw instructions
`3b5a..3d11` confirm there is no intervening whole-board `c662` call.

Native class 16 additionally calls `autotileRegion` on the entire maze
before the 3-to-0x5d pass. Already shaped walls 0x36..0x40 escape that
pass and start as ordinary walls instead of dormant alien terrain.
At seed 1 the native constructor leaves **661 ordinary wall tiles outside
the entry**, where the original postcondition is zero; only 16 dormant
wall tiles remain. This is a terrain-state error, not just a sprite choice.
The independent `maze_constructor_probe.cpp` checks that postcondition.

## P-062 — Maze wall shaping transposes row and column neighbors

**Confirmed, medium repair priority; wall graphics and tile identity.**

Original `1008:c662` passes `(row,column)` to `c9ce`. Its four candidate
elimination groups test east, west, south, north in that order. Native
maze constructors and `ActivateLevelSixteenRegion` apply those same
groups to south, north, east, west instead. The resulting 0x36..0x40
wall forms are transposed, including straight segments, corners and tees.

An independent instruction-derived candidate oracle compared with the
actual native activation helper differs for **12 of all 16 cardinal
neighbor masks**. For example, east+west neighbors produce native tile
55 instead of original 54. The candidate table itself agrees; its spatial
arguments do not. Fix all three duplicated shaping implementations.
Separate region-boundary and already-shaped-center handling remain review
items; these are not required to reproduce the orientation defect.

## P-063 — Casino card backs and deal/redraw animation are omitted

**Confirmed, medium repair priority; missing casino presentation.**

Original `1010:23ce` paints all five card slots independently of overlay
nodes: backs in statuses 0/1, faces in statuses 2/3/4. `1008:8be6`
separately dispatches transient type 0x1e to `1010:2cac`: phase 1 clears
the card rectangle, phase 3 paints the back, and phase 5 invalidates the
cards to reveal the final face. It then removes the transient node.

Native `DrawLevel` requires a persistent 0x1e marker before painting any
card, but creates those markers only on dealing, after switching to status
2. Thus a fresh/reset table has no card backs; the back-painting branch
does not cover the ordinary pre-deal state. Type 0x1e is explicitly excluded
from native transient aging and has no `2cac` phase implementation, so
dealt/redrawn faces appear immediately without the original animation.

The presentation probe reports zero initial drawing markers (the original
paints five backed slots), then five markers still present after six
animation ticks following a real deal. Repair must separate persistent
card-slot painting from transient flip effects; merely aging/removing the
current markers would make the final faces disappear too.

## P-064 — Animation randomness no longer advances the shared gameplay stream

**Confirmed parity difference; deliberate implementation choice, not yet
accepted as an audit exception.**

Original `1008:9786`, `9c5a`, and `9e68` call the same `3c88` random
routine used by construction and combat. Examples include skull terrain
on even animation ticks, captain/lieutenant animation decisions, and
treasure sparkles. These calls occur during `9680`'s animation callback,
with visibility/occupancy gates and traversal order affecting draw count.

Native `TerrainSprite`, `ActorSprite`, and overlay drawing instead use
`presentation::VisualChance`, a pure frame/salt hash. Neither painting nor
`AdvanceAnimation` consumes the corresponding gameplay draws. This makes
repainting deterministic, but also changes later combat, loot and generated
boards relative to the original after an otherwise identical timed sequence.
Captain/lieutenant visual choices additionally share one type-specific salt,
so same-type actors are synchronized instead of drawing separately.

This is established by source and original call-path comparison, not a
timed original-executable replay. It needs an explicit parity decision:
retain isolated cosmetic randomness as an accepted modernization, or restore
the original shared-stream effects at controlled animation ticks. Do not
blindly introduce RNG side effects on arbitrary Win64 repaint requests.

The same terrain review also confirms that original `9786` skips machinery
center animation after app+fe is set (Sphere destroyed). Native has neither
that shared latch nor the animation gate; this extends P-056's affected
consumers beyond spawning.

## P-065 — Reading a new fortune interrupts recall order and loses numbering

**Confirmed, low repair priority; fortune-message behavior.**

Original `1008:2c08` appends a newly discovered law without changing the
recall cursor at app+e8. Native item case 15 resets `lawRecallCursor_` to
zero whenever it discovers a new law. With three known laws and cursor 2,
reading a fourth should leave cursor 2; the next recall should use index 1.
The native probe instead observes cursor 0 and then recalls index 3.

Original `2c84` also formats recalled messages as `#%d - %s` and uses
`You haven't read any fortunes yet.` for an empty list. Native recall
omits the numbered prefix and substitutes different empty-list wording.
All 47 actual fortune texts match (resource probe); the discrepancy is
the recall behavior and surrounding prompt, not missing fortune content.

## P-066 — Arsenal selection does not reveal an offscreen endpoint item

**Confirmed, low repair priority; keyboard/scroll interaction.**

Original `1008:29e8` and `2a98` normalize the inventory viewport after
their conditional previous/next selection change, including when selection
is already first/last. Native `SelectPreviousItem`/`SelectNextItem` return
early at those boundaries and leave an independently scrolled selection
offscreen. The isolated UI-state probe scrolls away from the first item:
previous leaves `firstVisibleItem=1` instead of zero. At the last of twelve
items with viewport zero, next leaves zero instead of seven.

Do not change the selected item at the boundaries; restore visibility as
the original does. This is separate from the accepted message-history limits.

## P-067 — Bottom arsenal-row clicks can select the hidden sixth item

**Confirmed, low repair priority; inventory mouse hit testing.**

Original `1008:5770` explicitly assigns y=384..420 inclusive to the fifth
visible item. The first four rows each have a one-pixel separator, but
the last row extends to the bottom boundary. Native uses uniform modulo-36
rows throughout: y=419 is incorrectly rejected as a separator, and y=420
maps to visible index 5 (the hidden sixth item), if that item exists.
The UI-state probe exhaustively compares y=240..420 with the original
intervals and isolates these two differing coordinates.

## P-068 — Captain replacements wander instead of pursuing the player

**Confirmed, medium repair priority; enemy behavior.**

Original `1008:bc2c` creates its random number of type-2 replacements through
`80c0` with behavior zero. Native captain replacement paths in Smart Bomb,
triggered mines, projectile kills and radio blasts all use behavior two.
The native actor dispatcher treats two as random wandering rather than the
original zero's pursuit behavior. The combat probe's two-captain Smart Bomb
fixture produces nine non-hunter replacements; the original requires zero.
This is separate from replacement scheduling and capacity issues P-009/P-016.

## P-069 — A surviving boss can overlap the defeated boss's remains

**Confirmed, medium repair priority; boss movement and terrain integrity.**

Original `1010:09c0` tests the other boss's retained coordinates regardless
of its defeated flag. Its movement caller `00ec` rejects candidate centers
within three cells on both axes. Native constructs the collision-target list
only from undefeated bosses, so the defeated boss no longer blocks movement.
An isolated combat probe places centers at (10,10) and (13,10), with the
second defeated and its corpse tiles present. A rightward movement attempt
moves the survivor to (11,10), versus the original's retained (10,10), and
can overwrite the corpse footprint. This is an instruction-derived comparison,
not a recorded playthrough of the original executable.

## P-070 — Auxiliary notices interrupt play with a modal dialog

**Confirmed, low repair priority; message delivery.**

Original `1008:6cdc` decodes the hidden attribution and forwards it through
`67e6` into the arsenal message history. Native's matching keyboard gesture
uses `MessageBoxW` instead. The decoded original text exactly matches the
native literal, as verified by `help_text_probe.py`; delivery is the mismatch.
The original sound-toggle path in `6f46` likewise sends its notice through
`67e6`, whereas native uses a modal dialog. These notices should not require
acknowledgement or suspend play, and should remain available in message history.

## P-071 — Some static UI details still omit original drawing operations

**Confirmed, low repair priority; cosmetic fidelity.**

Original arsenal title painter `1008:4e5e` sends all four title fragments
through embossed-text helper `3d50`. Native `GameData::DrawArsenal` uses
single-pass `TextOutW`, omitting the highlight/shadow passes. Its list and
status painters also omit the original vertical bevel edges (the row dividers
are present). Original weapon help `1010:b638` draws a continuous three-cell
floor strip at y=52..304 in 12-pixel steps; native draws floor cells only at
the eight weapon-row positions, leaving gaps between them.

The long help narrative on page 2 additionally omits `Here's a look:`;
pages 1/3/8/9 and all eight weapon descriptions match their original text
after whitespace normalization. These are distinct from native font
rasterization differences and do not change game rules.

Further help comparison: page 7 (`1010:9eec`) omits all nine atlas keycaps
and their separately drawn key labels, replacing the original 30-pixel
rows with plain text. Its sound row also omits the live `(currently ON)` /
`(currently OFF)` suffix selected at `a03e`. The help header's page label
(`99ba`) and page titles also use single-pass text instead of the original
embossing. These are missing drawing/content operations, not font metrics.

## P-072 — Drain animation discards input instead of deferring it

**Confirmed, medium repair priority; transition/action timing.**

Original `1008:b78a` completes its five 50ms frames before returning to
the transition caller. Its inner loop only retrieves timer messages; gameplay
keyboard/mouse actions are not dispatched until after that animation.
Native `HandleBoardTransition` immediately installs the destination, retaining
the old board solely as a drawing snapshot. Crucially, its window handlers
check `IsDrainAnimating()` and return without acting on keyboard and board
mouse input. These messages are therefore consumed and lost, not deferred.

The initial UI-state probe bypassed those window handlers and could advance
the destination through direct API calls. That is **not evidence of normal
controls advancing unseen gameplay**; that earlier interpretation is withdrawn.
The confirmed source-level discrepancy is discarded versus queued input.
Defer input/action dispatch through transition presentation while preserving
the existing protection against acting on an unseen board.

## P-073 — Identification misses casino callbacks and calls effects treasure

**Confirmed, low repair priority; right-click descriptions.**

Original `1008:60f4` delegates terrain type 2 to the board's description
callback; casino `1010:26dc` supplies `Poker Table.`. Native only applies
that text to tiles 0x6d/0x6e and misses type 2. All 65 type-2 squares in
the actual casino map fail this comparison in the UI-state probe.

The original overlay switch only describes explicitly listed pickup/control
types and skips unrecognized nodes to continue with terrain. Native's default
returns `Treasure` for every other overlay, including transient explosions.
The probe places effect 0x14 on an empty cell and receives `Treasure` instead
of falling through to `Empty`. The player/actor/projectile precedence is
otherwise retained in the inspected ordinary paths. Boss labels remain P-036.

## P-074 — Consuming an item unnecessarily scrolls the arsenal

Priority: low. Original `1008:1fe2`'s item-use tail, after `2766`, selects
the bubbler if present and changes the viewport only if that selection is
outside its five visible rows. Native `ConsumeSelectedItem` always anchors
the selected item in a newly calculated viewport. In `ui_state_probe.cpp`,
consuming slot 1 moves the bubbler from slot 3 to slot 2; viewport 1 already
contains it. Native changes the viewport to 0; original retains 1.
Record compaction and fallback weapon choice themselves agree. This is a
scroll-position discrepancy, not lost inventory or an extra pickup.

## P-075 — Pickup messages use abbreviated arsenal names

Priority: low. `1008:a3d0` constructs its quantity-aware messages from a
separate set of long names, not the item descriptor's short UI label.
Native `AddRandomizedInventoryItem` uses `ItemName` for both. Five reward
names consequently differ: `Triple-Shot Cannon` / `3x Bubbler`, `Bomb Burst`
/ `8x Bomb`, `Missile Burst` / `8x Missile`, `Radio Controlled Bomb` /
`Radio bomb`, and `Fortune Cookie` / `Cookie` (five distinct item IDs;
the singular/plural forms share this cause). The prefix and plural suffix
rules themselves match. `help_text_probe.py` extracts the original operands
and compares them with the native descriptor bytes. Quantities, the radio
detonator addition, and the cookie's extra treasure callback are separate
and agree in this helper.

## P-076 — Actor entry incorrectly exempts the actor's own square

Priority: medium. Original `1008:91ec` rejects cells with occupancy bit 2,
including the current actor's square when its bit is set. Native
`AdvanceActors::canEnter` skips the current actor in its entity scan.
With one behavior-2 actor at (10,10), empty terrain, no projectiles, a
current occupancy bitmap, and RNG seed 6, the original's first zero-vector
attempt must retry and its next attempt moves to (11,10). The native probe
stops at (10,10), consuming only the first two rolls. This changes subsequent
gameplay RNG as well as that turn's movement.

Evidence: complete `8310`, `91ec`, `a652` bodies and the independent
eight-attempt oracle in `combat_probe.cpp`; not an original-binary execution.
The broader lossy bitmap behavior with stacked actors remains an open
comparison. Fixing projectile avoidance (P-055) alone does not fix this.

## P-077 — Help's window-close action opens the registration page

Priority: medium. Original help dispatcher `1010:94e0` handles `WM_CLOSE`
through `975c`, which calls `EndDialog`. Native `HelpDialogProcedure` has
no `WM_CLOSE` branch. Windows' default dialog handling posts the Cancel
command (ID 2), which this dialog uses for its Register button. The native
command handler consequently switches to page 11 instead of closing.

`dialog_probe.cpp` includes the actual production callback in a standalone
console diagnostic, creates a never-shown dialog with the real ID-2/ID-100
controls, sends `WM_CLOSE`, and dispatches its queued messages:
`hidden=1 page_after_close=11 still_window=1`. It destroys that test window
afterward and never enters the game's startup or touches player files.
The original evidence is the complete `94e0`/`975c` bodies. The explicit
Close button (ID 100) works; this finding concerns the window-close action,
not an assertion that Escape had different behavior in the original.

## P-078 — Persistent-data and file-dialog read errors are silently hidden

Priority: low; error reporting, not normal gameplay. Original `1010:edea`
distinguishes a missing sidecar from other open failures, and `1018:0000`
displays a read-error notice. Native `HighScoreTable::Load` and
`LoadKeyBindings` treat every open failure like an absent file;
`LoadOriginalResources` silently substitutes defaults even on rejected data.
The high-score help page also displays the cached/default table rather than
the original `a64a` read-error page. A permissions/sharing/corruption problem
can therefore look like scores or bindings were never saved.

Likewise original `1008:0c70` / `0d46` query `CommDlgExtendedError` after
the common dialog returns false. Native `ChooseGameFile` treats false as
Cancel without distinguishing a dialog failure. Ordinary game-file read/write
failures after a successful file selection do have native error messages,
as do high-score/key-binding write failures; those are not missing.
Evidence is the complete original I/O bodies and native call-chain inspection.
No player file was damaged or permissions changed to exercise this path.

## P-079 — Help player illustrations ignore the actual player sprite state

**Confirmed, low repair priority; help animation fidelity.**

Original `1010:afd8` calls `1008:2d50` for the player illustrations on
pages 2, 6 and 12 (and the registration page). That helper selects facing,
power-up, firing and zero-health frames from the application state.
Native `ApplicationState::DrawHelpPage` always draws `0x152 + frame % 8`
on those three retained pages. The animated illustration exists, but its
state-dependent branches are absent. This is separate from the correctly
prioritized damage effect in the actual board renderer. Evidence: complete
helper/callback bodies and native call sites; no pixel-equivalence claim.

## P-080 — System-arrow handler does not check the Alt modifier

**Confirmed, low repair priority; input routing.**

Original `1008:73ce` moves the arsenal selection for virtual keys `0x26`
and `0x28` only when the system-key flags include `0x2000` (Alt context),
and only outside demo mode. Native `MainWindowProcedure` tests the key code
on every `WM_SYSKEYDOWN` and calls the selection wrapper without checking
the Alt-context bit in `lParam`. The demo suppression is retained. This can
move the selection when a system-key arrow reaches the main window without
Alt context, such as through menu/focus routing. Evidence: both callbacks
and the native selection wrapper; no broader menu-playthrough claim.

## Open verification items, not additional confirmed bugs

- Alien-maze region activation uses whole-board neighbor checks in native
  code, whereas original `c9ce` rejects neighbors outside the active rectangle.
  The runtime probe compared all four real-map activation rectangles against
  copies with exterior neighbors masked out: zero interior tile differences
  across 2,048 cases (128 seeds, four skills, four regions). A gameplay-reachable boundary difference is not established;
  do not treat this source-level concern as a confirmed visible defect.

- `AdvanceProjectiles` skips all zero-vector records, while original `77d0`
  still processes their collision/terrain checks. A synthetic stationary mine
  sharing a square with an actor survives in the remake but is consumed with
  the actor in the original projectile phase. Probe output: `surviving_actors=1
  shots=1 expected_both=0`. Legitimate-state reachability and the distinct later
  `8310` mine-trigger path need review before elevating this to a gameplay bug.

- `1008:3a6e` is now resolved: raw instructions concatenate eleven embedded
  copyright/title/contact strings from selector 1080 and XOR signed bytes,
  starting at 0xff. The supplied original's strings produce the required
  0xe4. This is an embedded-branding integrity gate, not another quest or
  player prerequisite. Its absence does not block parity for the supplied
  unmodified original; retain the attribution itself. No finding is added.
- Constructor exit reachability and ammunition sufficiency have not been proved.
- Native quit-after-save-dialog-cancel/failure discards the expedition, but
  original `1008:1efe` also ignores `134e`'s result and restarts. This is an
  inherited data-loss risk, not a newly introduced parity mismatch. A safe
  behavior improvement should be identified as such, not attributed to original code.
- Rapid transient-effect timing and modal scheduling are now compared in
  COMPATIBILITY.md; original/native wall-clock equivalence is not established.
  The hit-sprite priority itself is fixed in the baseline and must not be
  reported as still missing. P-005 and P-049 remain separate lifecycle/layer defects.
- The original timed registration reminder and registration form action remain
  intentionally omitted, as do the user-accepted history/cache-limit changes.

## Inventory limitation discovered

Twenty-two apparent direct far-call targets found through NE relocations are
absent from the Ghidra function export. One (`1008:6804`) is now disproved:
at relocation site `633b`, the preceding apparent CALL opcode is actually
part of a jump displacement; the relocation belongs to a segment PUSH for
a string. The mechanical index intentionally remains distinct from the
review ledger's instruction-boundary judgments. Other entries include restored routines such as
the splash painter; absence from the export does not imply absence from the
remake. All 22 now have explicit review dispositions, including the raw demo
mouse handler at `1010:8310`. The 23 bounded switch tables have also been
followed by `near_call_probe.py`. Arbitrary pointer candidates and runtime
validation remain distinct coverage questions; this is not a parity pass.
