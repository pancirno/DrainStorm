#include <iostream>
#include "application_state.h"

// Independent c662 candidate masks, ordered by original c9ce(row,column)
// calls: east, west, south, north. Zero eligible candidates retains tile 3.
static unsigned OriginalWall(unsigned mask) {
    bool candidates[11];
    std::fill(std::begin(candidates), std::end(candidates), true);
    const auto disable = [&candidates](std::initializer_list<int> list) {
        for (int index : list) candidates[index] = false;
    };
    if (mask & 1) disable({1,3,4,7}); else disable({2,5,6,8,9,10});
    if (mask & 2) disable({1,2,5,6}); else disable({3,4,7,8,9,10});
    if (mask & 4) disable({0,4,5,8}); else disable({2,3,6,7,9,10});
    if (mask & 8) disable({0,2,3,9}); else disable({4,5,6,7,8,10});
    constexpr unsigned tiles[] = {0x36,0x37,0x38,0x39,0x3b,0x3a,0x3c,0x3d,0x3e,0x3f,0x40};
    for (int i = 0; i < 11; ++i) if (candidates[i]) return tiles[i];
    return 3;
}

int wmain(int argc, wchar_t** argv) {
    if (argc != 2) return 2;
    drain_storm::GameData board;
    if (!board.Load(argv[1], 16, 1)) return 2;
    unsigned ordinaryOutsideEntry = 0, dormantWalls = 0;
    for (unsigned x = 0; x < 40; ++x) for (unsigned y = 0; y < 36; ++y) {
        if (x >= 5 && x <= 9 && y >= 5 && y <= 9) continue;
        ordinaryOutsideEntry += board.level_[x][y] >= 0x36 && board.level_[x][y] <= 0x40;
        dormantWalls += board.level_[x][y] == 0x5d;
    }
    std::cout << "alien-constructor ordinary_walls_outside_entry=" << ordinaryOutsideEntry
        << " expected=0 dormant_walls=" << dormantWalls << '\n';
    unsigned mismatches = 0;
    for (unsigned mask = 0; mask < 16; ++mask) {
        for (auto& column : board.level_) column.fill(0);
        board.level_[10][10] = 0x5d;
        if (mask & 1) board.level_[11][10] = 0x5d;
        if (mask & 2) board.level_[9][10] = 0x5d;
        if (mask & 4) board.level_[10][11] = 0x5d;
        if (mask & 8) board.level_[10][9] = 0x5d;
        board.ActivateLevelSixteenRegion(9, 9, 11, 11);
        const unsigned expected = OriginalWall(mask);
        mismatches += board.level_[10][10] != expected;
        std::cout << "wall-mask=" << mask << " native=" << board.level_[10][10]
            << " original=" << expected << '\n';
    }
    std::cout << "wall-orientation mismatches=" << mismatches << " cases=16\n";
    if (!board.Load(argv[1], 11, 1)) return 2;
    unsigned signs = 0;
    for (const auto& column : board.level_)
        for (unsigned tile : column) signs += tile == 0x7e;
    std::cout << "procedural-class11 signs=" << signs
        << " expected_nonhub=0 expected_hub=1\n";
    return 0;
}
