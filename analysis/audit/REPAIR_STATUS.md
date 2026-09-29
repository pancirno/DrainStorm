# Audit repair status

The source audit in `FINDINGS.md` describes the frozen pre-repair baseline.
This file tracks changes made after that audit. An item absent from the
addressed list remains open; passing tests does not imply all 80 findings are
closed.

## Addressed in the first repair pass

- P-001–P-004: fresh/cached branch RNG, rare forward drain and exhausted
  factory fallback.
- P-005 and P-054: demo restart on death, victory, quit, and the final script
  command. Fatal-hit frame lifetime is covered below.
- P-006: hub branches are constructed in original order before departure.
- P-007–P-011: live same-turn missile fragments, casino deflection turn order,
  reverse Smart Bomb captain splitting, Bronze Key quadrant/loot traversal,
  and radio-blast visitation order.
- P-012, P-018: hub perimeter enemy mix and initial room-specific difficulty
  parameters.
- P-013, P-015, P-016: inline column-major map parsing, linked-list-head
  overlay traversal, and immediate mine-blast captain replacements. Saved
  overlay vectors retain their existing insertion-order representation.
- P-014: native saves accept legitimate off-board Gauntlet pickups.
- P-017: final-arena escalation scans the correct 120 centers and stores its
  spawn/group tiers only after the actor turn. Save version 31 retains both
  values. Raw `1010:d71e` writes the spawn/group fields but never changes
  the break divisor; the constructor's value of 100 remains throughout.
- P-019: Gauntlet transporter and late-flood hazards draw column before row;
  out-of-board original candidates consume RNG but are not written unsafely.
- P-020, P-021: procedural late thresholds and hub lever geometry, barrier
  opening, effects, and actor release coordinates.
- P-022, P-023: alarm and guarded structure placement use the original
  coordinate draw order. Alarm placements restore nine construction effects,
  sound, and player-overlap budget handling.
- P-024: hub structure origins, persistent runtime spawn threshold and all
  three zero-center reward branches follow the original RNG order. Save
  version 31 retains the threshold; older saves derive the former turn-based
  value. The unsafe original retry loop remains capped at 20 attempts.
- P-025, P-077: F1 help dispatch and help-dialog window-close handling.
- P-026, P-028, P-037: type-three pickup table, whole-chest blast callback,
  and procedural chest loot override.
- P-027: a fatal playable hit latches death for the accepted turn, so a
  subsequently collected life cannot suppress the original game-over path.
- P-029: resisted structure hits and indestructible solid impacts show their
  explosion effect and sound; original bomb/missile resistance notices and
  hidden-rock treasure message are restored from the executable strings.
- P-030, P-032, P-033: casino bonus openings use the original map coordinates;
  poker classification preserves ace-high straights, qualifying pairs, and
  the four-wild-card quirk; retaliation scales with elapsed board turns.
- P-031: lost top-edge casino chests release captain defenders, and damaged
  machines/portals are reconstructed on the 200-turn upkeep. The chest-count
  observation is saved in version 29; older saves derive it from terrain.
- P-034: casino machine cycles advance before the player interaction and once
  more after a qualifying win, retain an active cycle's strength, and restore
  the left/right directional gates. Mode-three captains from the left machine
  spawn at the original remote gate.
- P-035: eight-way weapons and both missile-fragment paths enumerate column
  velocity before row velocity, retaining capacity and RNG order.
- P-036: maze signs, moving-boss identification, post-bet casino ticker, and
  Moon Room switch/portal sound choices match the inspected callbacks.
- P-038, P-039, P-042: fortress seal, maze key overlay, and collision-checked
  procedural chest/guarded-object placement.
- P-043, P-044: extra procedural chest-room eligibility uses depth and skill;
  both room phases discard tenth-attempt candidates, and exhausting the
  5x5-room budget ends that phase as in the original branch logic.
- P-046: configurable movement dispatches before overlapping command keys
  and literal N/L/C/F1 shortcuts, retaining the original binding precedence.
- P-040: common mine-resolution tail now follows each derived room callback;
  a real alien-maze sentinel regression covers the one-turn gate delay.
- P-041: Statue Room special-wave actors honor the 300-actor limit while
  retaining the original type-roll RNG draws.
- P-045, P-048: bubble-hit effect/sound survives occupied terrain, and
  Gauntlet flood paints backward actor-removal effects (except index zero).
- P-047: Breakout's two switch flags persist independently of damaged tiles
  in save version 30; its hidden mechanism requires actual movement and
  plays the construction sound.
- P-049: transient impact/effect sprites paint in their late pass above
  actors and projectiles, while ordinary pickups remain beneath them.
- P-050–P-054: Power Plant progression gates, Smart Bomb shielding, initial
  captain groups, boss defensive waves, and demo script timing. The remaining
  e636 order review restores last-writer spawn words (including late breaches),
  lever-before-ring-turn ordering, the global Sphere gate, and the lower boss's
  random draw before its delay check. Save version 32 persists both spawn words;
  older saves retain the previous inferred priority because event history was
  not recorded.
- P-055, P-069, P-076: actor entry now follows the original empty-cell
  projectile dodge, machinery tile-eight boundary, and own-square occupancy;
  living bosses also respect the defeated boss's retained footprint.
- P-056: Sphere destruction now suppresses the global structure scan and is
  propagated through board transitions and version-28 native saves. Older
  non-Power-Plant saves cannot always reconstruct this historical global flag.
- P-057: Sphere flag adds the five-million-point ranking bonus on death, quit,
  and victory while raw gameplay score remains unchanged. Native score tables
  now persist the flag in format version 4 and still load older formats.
- P-058, P-075: boss sound indices and full pickup-message item names.
- P-059, P-061, P-062: maze entrances reveal nearby walls immediately;
  alien-maze walls stay dormant until region activation; all three wall
  shaping paths use the original east/west/south/north neighbor order.
- P-060: class-11 keeps the reserved entry cell empty on ordinary generated
  descendants; only the hub-created branch receives its introductory sign.
- P-063: the casino paints all five undealt card backs without overlay
  markers; dealt/redrawn cards clear, show backs, then reveal their faces
  over the original five-phase transient lifetime.
- P-064: gameplay animation uses the original shared LCG at controlled timer
  callbacks, traversing terrain column-major, actors in creation order and
  overlays head-first. Occupancy/player/terrain gates preserve draw counts;
  individual enemy and treasure frame caches remove synchronized hash choices.
  Machinery retains its last frame after Sphere shutdown. Paint has no RNG
  side effects; cosmetic cache fields do not change actor/overlay save records.
- P-065–P-067: fortune discovery preserves recall order and numbered/empty
  prompts; endpoint selection normalizes the arsenal viewport; its fifth
  mouse row includes the bottom boundary without selecting a hidden sixth.
- P-068, P-073: captain replacements pursue instead of wandering; casino
  type-two terrain identifies as Poker Table and transient effects fall
  through right-click identification to the underlying tile.
- P-070: sound-toggle and hidden copyright notices enter the arsenal message
  history without interrupting play with modal dialogs.
- P-071: weapon help has its continuous floor strip, narrative page
  2 includes its transition phrase, and command help restores nine atlas
  keycaps plus live sound state. Arsenal title fragments, help headings and
  page labels restore the white/black emboss passes; the list restores its
  vertical bevel edges around both list and score/health status (the
  message-panel bevel was already present).
- P-072: key and board-click actions received during the five-frame drain
  animation are queued and replayed after the destination board is painted;
  replay preserves Shift state and pauses again if an action starts another
  transition. Hidden production Win32-window regressions now verify keyboard/
  mouse ordering, captured Shift, and pause/resume across a second transition.
  Human wall-clock/playthrough verification remains distinct from these tests.
- P-074, P-080: consuming an item preserves an already-visible arsenal
  viewport; system-arrow selection requires the Alt-context flag.
- P-079: help illustrations use the same facing/power-up/attack/wreck sprite
  selector as gameplay without mutating the live player's animation state.
- P-005: fatal hits retain the playable board for all five damage-effect
  frames, reject further gameplay actions, then publish Game Over and reset
  to the demo. Timer dispatch presents the deferred lifecycle message.
- P-078: absent sidecars remain valid first-run defaults; present unreadable
  or corrupt sidecars report an error without startup writes. High-score
  help retains its read-error state until a successful score save. Common
  file dialogs distinguish Cancel from CommDlgExtendedError failures.

Cleanup also routes the graphical high-score page through the same signed
ranking calculation as the table, including the previously missing Sphere
bonus, and updates legacy-save fixtures to truncate at actual format boundaries.

All confirmed P-001 through P-080 repair findings now have an implementation
disposition in this tracker. The Release `GameDataRegression` and
`AuditRegression` suites pass, including original-atlas pixel checks, save
migrations and native input routing. This closes the confirmed repair backlog,
not the broader verification questions or intentional compatibility choices
listed in `FINDINGS.md` and `COMPATIBILITY.md`. No synchronized Win16/Win64
playthrough or proof of full-game combat reachability is claimed.
