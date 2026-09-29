# Original-versus-Win64 parity audit

Started 2026-09-14. Status: **source-level review complete** as of 2026-09-23.

The review records eighty confirmed findings (P-001–P-080). It compares
exported and discovered original routines, gameplay virtual targets, core
resources and the frozen native implementation, with focused in-memory probes.
All ledger entries have a disposition. This is a completed code review, not a
claim of original/native execution equivalence or a completed game playthrough.
No fix or replacement build is included; the production baseline is unchanged.

## Frozen baseline

- Commit: `bc22292c810a3928b07b53ae8149eef139de20dc`.
- Delivered executable: `reconstructed/bin/DrainStorm.exe`.
- SHA-256: `91D5975F3F93DF8692CFFFBDA0EFBFD8169E1074396D3FBC485FFBF69169022E`.
- Original `Drain Storm/DRAIN.EXE` SHA-256:
  `36fa1a907e94d5e4b2a4562f366c497a89c076c9f5438603c5a3c9273a2d1988`.
- Working tree clean before audit artifacts were added.
- GameDataRegression and AuditRegression rerun successfully in Release and Debug.
- No gameplay files, executable, user saves, scores, or key bindings changed in this pass.

## Evidence and inventory

Run `build_inventory.py` with Python to regenerate the mechanical indexes.
It uses the NE header/segment/relocation tables, the Ghidra function export,
and explicit original-address comments in current C++ source.

- `function-inventory.csv`: 467 exported functions plus discovered addresses;
  737 indexed addresses total, **not 737 proven functions**.
- `internal-fixups.csv`: internal pointer/far-call candidates from original NE
  all 17 segments. Twenty-two apparent direct-call targets lack a function export.
  This is a byte-pattern index, not instruction-boundary proof: `1008:6804`
  is now rejected in the review ledger because its supposed call is a string
  segment PUSH preceded by a jump-displacement byte equal to the CALL opcode.
- `entry-points.csv`: the NE entry table (one entry at `1000:0706`).
- `gameplay-vtables.csv`: all 320 slots of 16 constructor-established gameplay
  vtables, resolving to 121 distinct targets. Maze classes and procedural depths
  share tables. This closes the concrete gameplay virtual-target discovery gap,
  not a proof of behavioral equivalence.
- `classify_virtual_stubs.py` and `virtual-stubs.csv`: complete instructions for
  45 proven empty or constant-return virtual callbacks. These are not missing
  gameplay features. All discovered gameplay virtual targets now have evidence
  in the review ledger or this stub classification, including partial reviews;
  this does not mean that every callback has passed parity review.
- `inventory-summary.json`: counts and original-file hash.
- `review-ledger.csv`: human-reviewed scope and dispositions, independent of
  the generated index. Regeneration does not overwrite review judgments.
- `FINDINGS.md`: confirmed discrepancies and reproducible observations.
- `progression_probe.cpp`: isolated in-memory RNG/transition diagnostics.
- `combat_probe.cpp`: isolated combat and failed-action diagnostics; build and
  run with the same forced include, objects and asset argument as the progression probe.
- `constructor_probe.cpp`: actual-resource actor ordering and hub RNG diagnostics;
  same build/link settings and asset argument.
- `roundtrip_probe.cpp`: 12,800 in-memory constructor/save/load cases (25 classes,
  four skills, 128 seeds), with the same build/link settings and asset argument.
  No player saves or sidecars are read or written by these probes.
- `runtime_probe.cpp`: room difficulty parameters, final-arena center counting,
  procedural escalation boundaries and Gauntlet hazard coordinates; same build settings.
- `pickup_probe.cpp`: all five type-3 treasure outcomes and same-turn fatal-hit
  plus life pickup; same build settings, in-memory state only, sound disabled.
- `terrain_probe.cpp`: explosive chest destruction and resisted-hit presentation;
  same build settings and in-memory isolation.
- `casino_probe.cpp`: actual-map bonus gates, upkeep, retaliation scaling and
  isolated held-hand payouts; same build settings, no UI or player-file writes.
- `poker_rules_probe.py`: finite original/native classifier-model comparison
  over 22,958 legal rank/suit classes, including wild-card tables and original
  quirks. Does not execute the original binary or test dealing/RNG.
- `presentation_probe.cpp`: actual-map sign text, tracked boss identification
  and first-bet ticker diagnostics, plus offscreen effect/actor layering;
  uses the same in-memory build settings and never launches a window.
- `demo_script_probe.py`: exact command/shift comparison against all four
  original selector-1080 attract scripts. All 453 command pairs match.
  This does not prove demo transition timing or resulting world-state parity.
- `map_parser_probe.py`: independent inline-parser terrain model versus native
  two-pass parsing for all nine supplied maps: zero terrain differences.
  Moon/Statue constructor force-zero footprints are also empty as expected.
  Actor and overlay ordering remain separate confirmed discrepancies.
- `resource_text_probe.py`: all 47 fortune literals exactly match the original
  selector-1020 fixed-width records; all 31 WAV payloads match original NE
  resources. Sound dispatch indexes 26/27 are swapped (P-058).
  Fortune recall formatting is a separate check.
- `maze_constructor_probe.cpp`: alien-maze initial terrain postcondition,
  all 16 wall-neighbor masks against an instruction-derived oracle, and
  class-11 sign placement. Same isolated build settings as the runtime probe.
- `ui_state_probe.cpp`: inventory viewport endpoint handling and fortune recall
  after an intervening new discovery; in-memory only with normal item-use code.
  `resource_text_probe.py` also verifies all sprite/tile/overlay/item table bytes
  and all eight directional missile records against the original image.
- `help_text_probe.py`: compares five long help narratives and eight weapon
  descriptions, and decodes the hidden attribution using the original nibble/XOR
  algorithm. Page 2 omits a short phrase; other tested text matches. This is
  not a pixel-layout test. Also verifies both bundled text files byte-for-byte
  and distinguishes reward-specific long names from abbreviated arsenal names.
- `near_call_probe.py`: follows direct control flow and all 23 explicitly
  bounded switch tables from exported and manually reviewed game/UI roots.
  It finds no near CALL instructions, unresolved indirect jumps, or undecodable
  instructions on those paths. This is not execution coverage or a proof that
  every arbitrary pointer candidate is a function.
- `dialog_probe.cpp`: exercises the real Help callback in a never-shown
  test dialog; verifies the window-close/registration-page routing defect.
  Link with comdlg32 in addition to the usual probe libraries; no asset
  argument. Does not call the game's entry point or open a visible game window.
- `graph_save_probe.cpp`: 1,024 save/load checkpoints across casino, both hub
  arms, shared-room descendants and return routes (16 seeds, four skills).
  All loads, navigation checks, current-board payload comparisons and shared
  descendant identity checks passed. The newly created temporary save folder
  and its probe file were removed; no player saves were used. Navigation
  teleports to real drains and is not an end-to-end gameplay test.
  An additional 704 maze checkpoints cover the three-maze chain, Alien Maze,
  side branch, returns, revisits and factory-flag propagation. All checks pass.
- `COMPATIBILITY.md`: save-format, timing, RNG initialization and runtime
  validation boundaries; these are not blanket user-approved deviations.

An implementation link is **not** a parity verdict. Unexported call targets are
not automatically missing functionality: several already have implementations.
Pointer candidates may be data, tables or instruction interiors. No export size
is treated as a reliable function end. The control-flow probe closes the
near-call/switch-table discovery gap for reviewed game/UI roots. Arbitrary
pointer candidates still require instruction/data classification; they must
not be counted as missing functions.

## Review coverage and validation limits

Coverage snapshot, 2026-09-23: every exported routine in game/UI selectors
1008/1010/1018 has an individual ledger entry. All selector-1000 runtime
exports are classified; the 91 selector-11d0 exports are Windows API imports.
All 22 apparent unexported direct-call targets have review dispositions,
including the disproved CALL at 1008:6804. The 23 bounded game/UI switch
tables have been followed without finding additional near-call routines.

No ledger row remains partially reviewed. Dispositions include accepted
platform/product differences, confirmed defects, and source differences whose
effect in legitimate play remains unverified. A ledger row is an audit of its
recorded scope, not a proof that all 737 indexed addresses are separate
functions or that every input sequence behaves identically. Arbitrary pointer
candidates beyond reviewed bounded switch tables are not exhaustively proved
to be code or data.

Unfinished validation is explicitly listed in `COMPATIBILITY.md` and the open
verification section of `FINDINGS.md`: an original Win16 execution trace,
legitimate end-to-end playthrough, and source-level overlap/boundary concerns
that have not been reproduced in normal play. Those are not silently counted
as confirmed findings.

Existing branch tests cover hub/casino returns, both shared-parent arms and
shared descendants, and selected recursive/maze routes. Some fixtures inject
tiles or choose branches directly; they do not prove that legitimate play can
reach those exits or obtain sufficient resources. Existing tests are corroborating
evidence, not the original specification. No full playthrough was performed here.

Accepted differences: history/cache limits, native runtime/Win64 infrastructure,
and obsolete registration workflows. Native font rasterization remains a known
visual adaptation. Shared-RNG divergence is **not** automatically accepted merely
because cosmetic RNG has intentionally been isolated.

## Probe reproduction

`progression_probe.cpp` uses the existing `reconstructed/tests/audit_access.h`
forced include, `reconstructed/src` include directory, and the Release audit
test objects for application_state, game_data, bitmap_surface and high_scores.
Compile with the project's MSVC C++20 /MD, UNICODE, _UNICODE, NOMINMAX and
WIN32_LEAN_AND_MEAN settings; link user32, gdi32 and winmm. Pass
`reconstructed/assets/original/game-data` as its sole argument.
The probe makes no saves and does not launch or control the game.
The baseline run was compiled to `reconstructed/build-release/audit_progression_probe.exe`.

Output captured in FINDINGS.md. A normal exit means the diagnostic ran, **not**
that the reported discrepancies are fixed.
