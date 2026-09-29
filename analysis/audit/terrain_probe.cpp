#include <iostream>
#include "application_state.h"

int wmain(int argc, wchar_t** argv) {
    if (argc != 2) return 2;
    const std::filesystem::path assets(argv[1]);
    {
        drain_storm::GameData treasure;
        if (!treasure.Load(assets, 17, 1)) return 2;
        int mismatches = 0, empty = 0;
        for (unsigned seed = 1; seed <= 128; ++seed) {
            treasure.randomState_ = seed;
            auto oracle = treasure;
            const int expected = oracle.NextRandom(5) <= 4 ? 1 : 2;
            const int actual = treasure.RollObjectTreasure(19);
            mismatches += actual != expected;
            empty += actual < 0;
        }
        std::cout << "procedural-key-treasure cases=128 mismatches=" << mismatches
            << " empty=" << empty << " expected_both=0\n";
    }
    for (int strength : {1, 2, 3}) {
        drain_storm::GameData board;
        if (!board.Load(assets, 17, 1)) return 2;
        for (auto& col : board.level_) col.fill(0);
        board.overlays_.clear(); board.actors_.clear(); board.projectiles_.clear();
        board.level_[10][10] = 0x7f; board.level_[11][10] = 0x80;
        board.level_[10][11] = 0x81; board.level_[11][11] = 0x82;
        std::vector<drain_storm::Projectile> fragments;
        board.DamageBoardSquare(10, 10, strength, &fragments);
        int remaining = 0;
        for (int col = 10; col <= 11; ++col)
            for (int row = 10; row <= 11; ++row) remaining += board.level_[col][row] != 0;
        std::cout << "blast-chest strength=" << strength << " remaining_tiles=" << remaining
            << " expected=0\n";
    }
    drain_storm::GameData board;
    if (!board.Load(assets, 17, 1)) return 2;
    for (auto& col : board.level_) col.fill(0);
    board.actors_.clear(); board.overlays_.clear(); board.projectiles_.clear();
    board.level_[10][10] = 0x10; board.level_[11][11] = 0x19;
    board.pendingSound_ = drain_storm::GameplaySound::None;
    board.DamageBoardSquare(10, 10, 1);
    std::cout << "reinforced-bomb overlays=" << board.overlays_.size()
        << " sound=" << static_cast<int>(board.TakeSoundEvent()) << " expected=1,12\n";
    board.level_[10][10] = 0;
    std::uint16_t solid = 0;
    for (std::uint16_t tile = 1; tile < board.tiles_.size(); ++tile)
        if ((board.tiles_[tile].flags & 5u) == 4u) { solid = tile; break; }
    if (!solid) return 2;
    board.level_[11][10] = solid;
    board.projectiles_ = {{1, 10, 10, 1, 0}};
    board.AdvanceProjectiles();
    std::cout << "indestructible-impact tile=" << solid << " overlays=" << board.overlays_.size()
        << " sound=" << static_cast<int>(board.TakeSoundEvent()) << " expected=1,12\n";
    return 0;
}
