#include <iostream>
#include "application_state.h"

int wmain(int argc, wchar_t** argv) {
    if (argc != 2) return 2;
    const std::filesystem::path assets(argv[1]);
    for (int tableIndex = 1; tableIndex <= 5; ++tableIndex) {
        drain_storm::GameData board;
        if (!board.Load(assets, 17, 1)) return 2;
        board.inventory_.clear();
        unsigned seed = 1;
        for (; seed < 10000; ++seed) {
            auto oracle = board; oracle.randomState_ = seed;
            if (oracle.NextRandom(4) != 1 && oracle.NextRandom(5) == tableIndex) break;
        }
        if (seed == 10000) return 2;
        board.randomState_ = seed;
        board.ResolveOverlayEffect(3);
        constexpr int expected[] = {13, 12, 7, 11, 8};
        std::cout << "type3-reward roll=" << tableIndex << " seed=" << seed
            << " item=" << (board.inventory_.empty() ? -1 : board.inventory_.front().type)
            << " expected=" << expected[tableIndex - 1] << '\n';
    }
    drain_storm::ApplicationState state;
    state.resourceDirectory_ = assets.parent_path();
    state.demoMode_ = false; state.soundEnabled_ = false;
    auto& board = state.gameData_;
    if (!board.Load(assets, 17, 1)) return 2;
    for (auto& col : board.level_) col.fill(0);
    board.projectiles_.clear();
    board.playerColumn_ = 10; board.playerRow_ = 10;
    board.health_ = 1; board.terminalDeathEnabled_ = true;
    board.actors_ = {{0, 11, 10, 0, 0, 0}};
    board.overlays_ = {{9, 11, 10}};
    if (!state.MovePlayer(1, 0)) return 2;
    std::cout << "fatal-hit-life-pickup health=" << state.Health()
        << " game_over_notice=" << !state.TakeLifecycleMessage().empty()
        << " expected_notice=1\n";
    return 0;
}
