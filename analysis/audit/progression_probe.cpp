#include <iostream>
#include "application_state.h"

int wmain(int argc, wchar_t** argv) {
    if (argc != 2) return 2;
    const std::filesystem::path assets(argv[1]);
    drain_storm::ApplicationState state;
    state.resourceDirectory_ = assets.parent_path();
    if (!state.gameData_.Load(assets, 17, 1) || !state.gameData_.PositionPlayerAtTile(0x27)) return 2;
    const auto parentRng = state.gameData_.randomState_;
    drain_storm::GameData expectedChild;
    if (!expectedChild.Load(assets, 18, parentRng, state.gameData_.Difficulty(),
        state.gameData_.boardDepth_ + 1)) return 2;
    state.gameData_.pendingTransition_ = 18;
    state.HandleBoardTransition();
    if (state.gameData_.LevelNumber() != 18) return 2;
    std::cout << "new-child parent_rng=" << parentRng
        << " constructor_final_rng=" << expectedChild.randomState_
        << " actual_transition_rng=" << state.gameData_.randomState_ << '\n';

    // Prepare a cached depth-five exit. Bypass the first destination roll so
    // the experiment isolates repeated traversal, not constructor selection.
    state.boardHistory_.clear(); state.currentBoardPath_.clear(); state.cachedBoards_.clear();
    if (!state.gameData_.Load(assets, 21, 1) || !state.gameData_.PositionPlayerAtTile(0x27)) return 2;
    state.gameData_.pendingTransition_ = 22;
    state.HandleBoardTransition();
    if (state.gameData_.LevelNumber() != 22 || !state.gameData_.PositionPlayerAtTile(0x23)
        || !state.ActivateTransporter(true) || state.gameData_.LevelNumber() != 21) return 2;
    const auto before = state.gameData_.randomState_;
    state.gameData_.DetectBoardTransition();
    const auto request = state.gameData_.pendingTransition_;
    const auto after = state.gameData_.randomState_;
    state.HandleBoardTransition();
    std::cout << "cached-exit before_rng=" << before << " after_dispatch_rng=" << after
        << " requested_class=" << request << " actual_cached_class=" << state.gameData_.LevelNumber() << '\n';
    for (bool exhausted : {false, true}) {
        drain_storm::ApplicationState branch;
        branch.resourceDirectory_ = assets.parent_path();
        if (!branch.gameData_.Load(assets, 11, 1)
            || !branch.gameData_.PositionPlayerAtTile(0x27)) return 2;
        unsigned seed = 1;
        for (; seed < 10000; ++seed) {
            auto oracle = branch.gameData_;
            oracle.randomState_ = seed;
            const int first = oracle.NextRandom(8);
            if ((!exhausted && first == 8)
                || (exhausted && first != 8 && oracle.NextRandom(3) == 2)) break;
        }
        if (seed == 10000) return 2;
        branch.gameData_.randomState_ = seed;
        branch.gameData_.genericRuntimeFlags_ = exhausted ? 0x30 : 0;
        branch.gameData_.DetectBoardTransition();
        const auto request = branch.gameData_.pendingTransition_;
        branch.HandleBoardTransition();
        unsigned exits = 0;
        for (const auto& column : branch.gameData_.level_)
            for (const auto tile : column) exits += tile == 0x27;
        std::cout << (exhausted ? "exhausted-factories" : "rare-c80e")
            << " seed=" << seed << " request=" << request
            << " destination=" << branch.gameData_.LevelNumber()
            << " tile20_5=" << branch.gameData_.TileAt(20, 5)
            << " down_exits=" << exits << '\n';
    }
    for (bool victory : {false, true}) {
        drain_storm::ApplicationState end;
        end.resourceDirectory_ = assets.parent_path();
        if (!end.gameData_.Load(assets, 1, 1)) return 2;
        end.gameData_.health_ = victory ? 3 : 0;
        end.gameData_.victoryTriggered_ = victory;
        end.HandleEndState();
        std::cout << (victory ? "victory" : "death") << " demo=" << end.IsDemoMode()
            << " next_board=" << end.gameData_.LevelNumber()
            << " notice=" << !end.pendingLifecycleMessage_.empty() << '\n';
    }
    for (unsigned tile = 0x27; tile <= 0x2a; ++tile) {
        drain_storm::ApplicationState hub;
        hub.resourceDirectory_ = assets.parent_path();
        if (!hub.gameData_.Load(assets, 3, 1)
            || !hub.gameData_.PositionPlayerAtTile(static_cast<std::uint16_t>(tile))) return 2;
        hub.gameData_.DetectBoardTransition();
        hub.HandleBoardTransition();
        unsigned connected = 0;
        for (std::size_t level : {6u, 7u, 2u, 11u, 5u}) {
            bool present = hub.gameData_.LevelNumber() == level || hub.sharedBoards_.contains(level);
            for (const auto& entry : hub.cachedBoards_) present |= entry.board.LevelNumber() == level;
            for (const auto& entry : hub.boardHistory_) present |= entry.board.LevelNumber() == level;
            connected += present;
        }
        std::cout << "hub-first-exit tile=" << tile << " destination=" << hub.gameData_.LevelNumber()
            << " connected_boards=" << connected << " expected=5\n";
    }
    // All four original scripts end in an unshifted wait. The shift callback
    // advances the script cursor and restarts on the following sentinel,
    // before 6f46 dispatches that final wait to the newly current board.
    for (const auto [level, length] : {std::pair{1u, 41u}, {2u, 139u},
                                      {10u, 21u}, {9u, 252u}}) {
        drain_storm::ApplicationState demo;
        demo.resourceDirectory_ = assets.parent_path();
        if (!demo.gameData_.Load(assets, level, 1)) return 2;
        demo.demoMode_ = true;
        demo.demoStep_ = length - 1;
        demo.demoTick_ = 0;
        demo.soundEnabled_ = false;
        demo.AdvanceAnimation();
        std::cout << "demo-sentinel source=" << level << " step=" << demo.demoStep_
            << " expected_step=0 current_class=" << demo.gameData_.LevelNumber() << '\n';
    }
    drain_storm::ApplicationState sphereScore;
    sphereScore.resourceDirectory_ = assets.parent_path();
    sphereScore.demoMode_ = false;
    sphereScore.soundEnabled_ = false;
    if (!sphereScore.gameData_.Load(assets, 1, 1)) return 2;
    for (int index = 0; index < 10; ++index)
        if (!sphereScore.highScores_.Submit("Audit", 1'000'000, 1, false)) return 2;
    if (!sphereScore.gameData_.DamageBoardSquare(12, 12, 2)) return 2;
    const auto sphereRawScore = sphereScore.gameData_.score_;
    sphereScore.gameData_.health_ = 0;
    sphereScore.HandleEndState();
    std::cout << "sphere-score raw=" << sphereRawScore
        << " qualifies=" << sphereScore.HasPendingHighScore() << " expected=1\n";
    return 0;
}
