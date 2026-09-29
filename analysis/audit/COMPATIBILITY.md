# Compatibility and validation boundaries

These are not all repair requests or user-approved deviations. They separate
platform choices and safer failure behavior from confirmed gameplay findings.

## Saves and settings

The remake does not import the original DAH-format saved graph or BB.BIN
sidecar. It has separate versioned native graph, score and keyboard formats.
Its round-trip tests prove native state persistence, not original-byte
compatibility. Export/import of original saves would be additional functionality.

Native loading preserves the live game and message history on failure or
cancellation, unlike the original's destructive early reset. Native save
dialogs also confirm overwrites. These are safer differences, not evidence
that the native format reproduces Win16 object layouts. Both implementations
write directly to the selected destination rather than atomically replacing
it. A canceled save during Quit is ignored by both; see FINDINGS.md.

The native read path validates signatures, versions, counts and records;
the original raw reads perform less validation. Deliberately malformed native
files are not specifications for original gameplay. P-014 remains a genuine
normal-state problem: native-generated off-board Gauntlet overlays can make
the remake reject its own save.

Original settings and scores are re-read at several New/Load/Help entry points;
the native application caches them after startup. Startup read errors and
high-score error-page handling are restored under P-078. External edits while
the game is open are not synchronized.

## Timing and presentation

The original short-effect driver requests a 1 ms timer, pumps only timer
messages, paints five phases, and returns before further input is processed.
The remake keeps effects alive across normal 200 ms animation callbacks and
continues accepting actions. Requested Win16 timer periods are not reliable
wall-clock durations; no claim of a literal 5 ms original animation is made.
The user explicitly requested visible damage animations and preservation of
animation progress while moving. Do not undo that requirement in pursuit of
the old blocking loop. Retain completion/lifecycle ordering (P-005), correct
layering (P-049), and the separately identified drain-input issue (P-072).

Sound priority promotion agrees, but native playback starts only the final
queued sound at the end of a turn, whereas the original starts each priority
increase immediately. No audible-timing equivalence is claimed. Modern fonts
and reflow differ; absent sprites/embossing/text are separately listed in P-071.
The consistently player-last native compositor follows the user's explicit
priority request; original initial WM_PAINT ordering differed from timer paint.

The native disabled-owner timer guard freezes board/demo animation during modal
dialogs. Original `5f92` continues its board/help/demo callbacks; `2d02`'s app+6
flag is consumed by the paint path (`5e70`), not by the demo command dispatcher
or cadence callback (`6f46`, `1010:0cd4`). This is a scheduling difference,
not proof that real gameplay turns run beneath every original modal dialog.

Casino result expiry uses 102 callbacks from zero in both implementations,
but original `26fe` owns a process-global counter reset only when expiry fires.
Native `casinoResultTicks_` belongs to a saved board and resets on every new
round/result. Ending an expedition before expiry can leave the original counter
partly advanced for a later casino. Native animation callbacks also replace
original paint-driven ticking. Do not describe equal thresholds as equal timing.

The original seeds its shared RNG once with a truncated 16-bit time value.
Native startup uses tick-count-derived 32-bit seeds and derives fresh seeds
for New/RestartDemo. Matching the two programs by wall-clock start time is
not a valid reproducibility procedure. P-001 and P-064 concern subsequent
stream ownership/call-order differences, not a demand to reuse an old clock.
The P-064 repair restores gameplay animation's shared RNG draws on controlled
timer callbacks and caches their sprites for side-effect-free repaint. Help
illustrations retain isolated presentation randomness; arbitrary repaint count
does not change gameplay. No wall-clock stream equivalence is claimed.

## What isolated probes do not establish

Original startup also shows a first-launch welcome/terms notice and opens Help
when the stored launch counter is one. The remake has no first-launch counter
or automatic Help opening. The welcome text points to the bundled README and
keyboard help; both remain accessible. This is distinct from the board-area
splash and the intentionally omitted registration prompts. Restoring a
one-time onboarding flow is a product choice, not restoring a recurring popup.

Original entity occupancy is a mutable bit map: removing or moving one record
clears its old bit even if another record shares the square, until a refresh.
Native queries scan live vectors. The source-level difference is established
in a652/a704/7f8e/8032/8310; its additional gameplay consequences beyond
P-055/P-076 have not been reproduced in a legitimate original execution.
The stationary-projectile overlap concern in FINDINGS.md is also unresolved.

The idol's one-shot gate is an original saved flag at +0x60, versus the native
repaired corner tile at (19,18). Ordinary repair geometry, replacement tiles,
score and consumption agree. A valid gameplay path that removes/reverts that
corner and repeats the reward has not been established; malformed-state
mutation alone is not evidence of an exploitable original/remake difference.

Native graph tests use real connection tiles but teleport to them. They do not
prove combat reachability, ammunition sufficiency, absence of every soft-lock,
or that a player can complete an entire expedition. Original instructions,
not existing native regressions, are the parity specification. No original
binary playthrough or synchronized original/native trace has been performed.
An inventory entry or reviewed helper is not a claim of full game equivalence.
