# Drain Storm x64 reconstruction

This directory contains the native x64 reconstruction of the original Win16 game.

The implementation is behavior-led: routines are identified in the Ghidra project and then
reimplemented against the current Win32 API. Decompiler output is evidence, not source code.

## Reconstruction status

The September 2026 audit found remaining behavior differences; reconstruction coverage is not
yet a claim of full gameplay equivalence. The first audit-fix build corrects fireball splash and
terrain impacts, bubble absorption, mouse input on child boards, Fortress placement coordinates
and difficulty scaling, special-key labels, and bounded message-history batching. The follow-up
fixes restore exact-cell pickup collection, per-actor movement/collision order, depth-aware cannon
eligibility, and casino rejected-turn/message behavior. All findings listed in that audit are now
addressed; this still does not certify every original instruction or board as equivalent.
See [`../analysis/AUDIT-2026-09-07.md`](../analysis/AUDIT-2026-09-07.md).

The full follow-up audit's 80 confirmed findings now have repair dispositions in
[`../analysis/audit/REPAIR_STATUS.md`](../analysis/audit/REPAIR_STATUS.md).
The final repair pass restores fatal-hit completion, Power Plant event ordering,
shared animation RNG on timer ticks, Sphere machinery shutdown, UI embossing and
read-error notices. Broader playthrough verification and intentional compatibility
choices remain documented separately; this is not a claim of instruction-for-instruction parity.

The application shell, attract-mode demo, randomized new-game path, board graph, gameplay systems,
all fourteen board-class callback tables, persistence, scores, original dialogs, bitmap, icon, and
sound resources are reconstructed. The intentionally excluded routines are the bundled 16-bit C/C++
runtime and Win16 compatibility plumbing; their native Visual C++ and Win32 equivalents provide the
same infrastructure in the x64 build. The callback-level evidence and implementation history are in
[`../analysis/RECONSTRUCTION.md`](../analysis/RECONSTRUCTION.md).

## Configure and build

Open this folder in Visual Studio as a CMake project, select an x64 configuration, and build the
`DrainStorm` target.

Enable `BUILD_TESTING` (the default) to build `DrainStormGameDataTests`; it validates resource
loading, reconstructed spawn coordinates, passability, actor turns, and inventory selection.
It also covers graph-save traversal and native high-score persistence, ordering, display, and
corrupt-file rejection.
The regression suite now uses isolated temporary assets and sidecars, preserving player saves,
scores and keyboard settings. On MSVC, `DrainStormAuditTests` adds deterministic tests for the
audit repairs. It includes original-atlas pixel checks and hidden production Win32
window tests for deferred keyboard/mouse input. Run both with
`ctest --test-dir <build-directory> --output-on-failure`.
The audit tests also cover the follow-up fixes, message ordering, depth save/load, and child revisits.
The September 8 transition fix restores common blue-drain returns (including the casino),
both shared-parent landing points, and the missing shared-board and Power Plant exits.
Walking-entry regressions cover branching, retained child state, recursive/maze returns, and graph saves.

New board saves use DAH64 version 32. Construction depth, branch state, independent
effects, global Sphere state and event-ordered spawn parameters are retained. Older native saves remain
readable; depth is inferred from procedural level and retained ancestry when available. Legacy
standalone special-board saves did not store enough information to recover their exact depth and
use the root-depth fallback. Earlier builds cannot read newly written version-32 saves.

Original resources extracted from `DRAIN.EXE` are kept under `assets/original`. Their offsets,
lengths, flags, and SHA-256 hashes are recorded in `assets/original/manifest.json`.

Controls: eight-direction movement uses the arrow/navigation keys;
Alt+Up/Alt+Down selects arsenal items; hold Shift with a movement direction (or Shift-click an
adjacent board direction) to activate the selected weapon. The original default `A` and `Z` keys
also move the arsenal selection, `T` toggles sound, and `U`/`D` activate upward/downward
transporters while standing on them. `F` recalls collected Drain Storm laws in reverse discovery
order, matching the original law-book binding.
Right-click identifies board contents with the recovered `FUN_1008_60f4` descriptions, and numpad
5 waits a turn. The original `N`, `L`, `S`, `C`, and `F1` shell shortcuts are active.
`P` and `M` review previous and newer gameplay messages.
Options → Keyboard Commands can rebind all 19 entries in the recovered command table. Bindings are
stored in the portable `DRAIN64.KEY` sidecar and restored on the next launch.

The File menu provides New, Open, Save, and High Scores commands. New expeditions that end with a
qualifying score prompt for the player's name and are inserted into the persistent ten-entry
`DRAIN64.SCO` table beside the native executable.

Projectile impacts preserve the original actor transformations: bubbles fossilize eligible
creatures, lieutenants can drop treasure, and captains split into subordinate enemies.
