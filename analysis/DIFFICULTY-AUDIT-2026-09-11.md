# Difficulty audit — 2026-09-11

## Implementation update

The five discrepancy groups below are now corrected. Maze populations/type rolls,
extra captains, transporter attempts, alien-maze captains, recursive pickup and
structure attempts, spawn probabilities and break divisors now use the recovered
skill/depth formulas. Recursive escalation deadlines and final construction count
also use actual depth (`400+30*depth`, `(depth+13)*50`, `depth+5`) rather than
the former depth-three constants; the deadlines use the original strict `>` test.
The generic procedural update now runs once at turn six on every depth.

Runtime corrections apply to retained boards without regeneration. Older saves'
incorrect procedural spawn parameter is repaired from skill, depth, turn and the
persisted escalation bits. New population rules apply only when constructing a
new board; existing saved actors and collected-item layouts are preserved.

Permanent tests cover all 100 board/skill constructor and save/load combinations,
exact maze/alien-maze populations, recursive pickup rolls, phase-specific spawn
and break formulas, turn-six behavior at all nine procedural depths, and legacy
spawn-parameter migration. The Power Plant damage/identity defect is also fixed
in the same change, with separate damage, movement, victory and save regressions.

The remaining caveats below describe the original audit's scope, not outstanding
work on these five groups. This does not certify every function in the game.

## Original audit (before these fixes)

Scope: compare the four selectable skills with the extracted executable and
current implementation. This is an audit, not a claim of complete gameplay parity.
Difficulty behavior was not changed in the accompanying startup/lever fix.

## Confirmed working plumbing

- The dialog maps Beginner, Intermediate, Advanced and Drain Death to 1..4.
- New Game validates the selection and stores it; player-state adoption carries
  it into child/restored boards. The score-submission path records that skill.
- An isolated in-memory probe loaded all 25 board classes at each of the four
  skills, saved and reloaded them: all 100 retained the selected difficulty.
- Existing regressions cover selected-skill New Game, invalid values, skill-four
  save/load, Gauntlet pickup scaling and Moon Room actor scaling.
- Current formulas include hub replenishment `(6-skill)*10`, Gauntlet hazard
  rolls `random(2*skill)>1`, Moon Room's `3*skill` repeated actor groups, and
  difficulty-scaled Power Plant interception/spawn rolls. Relevant originals:
  `1010:4692`, `1010:518c`, `1010:5fc4`, `1010:00ec`.
- Casino machine modes cap at three, including skill four. This is intentional
  in the recovered machine sequence, not proof that Drain Death was ignored.

## Confirmed incomplete behavior

### 1. Maze enemy populations remain fixed at Beginner values

`GameData::Load`, classes 13..15, unconditionally creates six type-0 actors.
Original `1010:86a0` creates `(skill+2)*2` actors, selecting type 0 when
`random(skill)<2`, otherwise type 2. It also creates a type-3 actor when maze
stage > 1 or skill > 2. The current constructor omits this additional actor.

Probe, seed 1: all three maze classes at all four skills produced exactly six
actors, with zero type-2 and zero type-3 actors. Original expected totals:

| Skill | First maze | Second/third maze |
| --- | ---: | ---: |
| 1 | 6 | 7 |
| 2 | 8 | 9 |
| 3 | 11 | 11 |
| 4 | 13 | 13 |

The same original constructor makes `skill` transporter placement attempts;
the remake makes only one. Attempts need not succeed, so final transporter
counts cannot be inferred directly from skill.

### 2. Maze runtime rates ignore their skill/stage formulas

At the end of `1010:86a0`, structure-spawn chance is `(skill+mazeStage)*2`
and terrain-break divisor is `(6-skill)*10`. The current spawn dispatcher has
no class-13..15 branch and uses its default chance of 10; actor terrain breaking
likewise uses its default divisor of 20. This can make lower skills harder in
some respects and higher skills easier in others.

### 3. Recursive branch construction is specialized to skill one/depth three

Original `1010:cb9c` uses `random(skill+3)` for pickup count and
`boardDepth+skill-1` structure attempts. The current class-11 constructor uses
`random(4)` and three attempts. Its spawn chance starts at a fixed 1 rather
than the constructor's depth-derived value. Its break handling also omits the
original initial `max(10,100-boardDepth-15*skill)` divisor.
This affects both selected difficulty and deeper recursive exploration.

### 4. Alien-maze initialization is also partially fixed at skill one

Original `1010:3a96` creates `skill` type-3 actors, sets spawn chance to
`(skill+2)*2` and break divisor to `max(20,(6-skill)*10)`.
The remake's class-16 constructor creates one actor and its runtime uses
default spawn/break values rather than dedicated class-16 formulas.

### 5. Procedural spawn-rate update tests depth instead of turn count

Original `1008:dd76` tests the board turn counter (`+0x3e/+0x40`) for exactly
six, then sets chance to `min(skill+5, boardDepth+skill-1)`.
The remake tests `depth == 6` on every turn through 65535. Consequently the
update is missing on other depths and repeatedly overwrites later escalation
on depth six. This is a progression/time-scaling bug as well as a difficulty
discrepancy.

## Limits and follow-up

These findings establish that all four skill settings are not yet faithfully
implemented. They are not an exhaustive inventory of every conditional in the
binary. In particular, unlisted callbacks and constructor RNG order deserve
continued comparison. The generic constructor also omits the original rare
type-0x1a structure branch at depth >= 5; that is a separate progression issue.

The earlier Power Plant target-identity issue remains outside this change.
No full playthrough or statistical difficulty/balance certification was done.

Before changing these formulas, add original-derived regression oracles for
all four skills and affected board classes. Existing saved boards must retain
their collected items and layout; newly constructed boards should use corrected
population rules, while runtime fixes should apply without regenerating saves.
