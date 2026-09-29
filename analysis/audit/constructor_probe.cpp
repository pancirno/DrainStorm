#include <fstream>
#include <iostream>
#include <iterator>
#include "application_state.h"

int wmain(int argc, wchar_t** argv) {
    if (argc != 2) return 2;
    const std::filesystem::path assets(argv[1]);
    for (std::size_t level = 1; level <= 9; ++level) {
        if (level == 8) continue; // This class does not consume its map resource.
        drain_storm::GameData board;
        if (!board.Load(assets, level, 1)) return 2;
        std::ifstream file(assets / L"levels" / (L"level_0" + std::to_wstring(level) + L".map"),
            std::ios::binary);
        const std::string map{std::istreambuf_iterator<char>(file), {}};
        if (map.size() != 36 * 41) return 2;
        std::size_t index = 0, mismatches = 0;
        for (unsigned column = 0; column < 40; ++column) {
            for (unsigned row = 0; row < 36; ++row) {
                const char ch = map[row * 41 + column];
                if (ch != 'g' && ch != 'm' && ch != 'd' && ch != 'z') continue;
                const unsigned type = ch == 'g' ? 0 : ch == 'm' ? 1 : ch == 'd' ? 2 : 4;
                if (index >= board.actors_.size()) return 2;
                const auto& actor = board.actors_[index++];
                mismatches += actor.column != column || actor.row != row || actor.type != type;
            }
        }
        std::cout << "map-actor-order class=" << level << " map_actors=" << index
            << " mismatched_indices=" << mismatches << '\n';
    }
    drain_storm::GameData hub;
    if (!hub.Load(assets, 3, 1)) return 2;
    std::vector<unsigned> randomResults;
    std::uint32_t state = 1;
    for (unsigned count = 0; count < 100000; ++count) {
        state = state * 0x343fdu + 0x269ec3u;
        randomResults.push_back((state >> 16) & 0x7fff);
        if (state == hub.randomState_) break;
    }
    if (state != hub.randomState_ || randomResults.size() < 40 || hub.actors_.size() < 40) return 2;
    unsigned mismatches = 0, nativeOnes = 0, originalOnes = 0;
    for (std::size_t index = 0; index < 40; ++index) {
        const auto native = hub.actors_[hub.actors_.size() - 40 + index].type;
        const unsigned expected = randomResults[randomResults.size() - 40 + index] % 3 == 0;
        mismatches += native != expected; nativeOnes += native == 1; originalOnes += expected;
    }
    std::cout << "hub-ring native_type1=" << nativeOnes << " original_type1=" << originalOnes
        << " mismatched_actors=" << mismatches << '\n';
    // Original d4e0 prefix, up to guarded-object placement. Earlier fixed
    // nonzero terrain remains nonzero through the original's later phases.
    for (unsigned seed = 1; seed <= 128; ++seed) {
        drain_storm::GameData rng; rng.randomState_ = seed;
        std::array<std::array<std::uint16_t, 36>, 40> terrain{};
        for (int i = 0; i < 14; ++i) if (rng.NextRandom(5) > 1) {
            rng.NextRandom(38); rng.NextRandom(34);
        }
        for (int i = 0; i < 2; ++i) {
            if (rng.NextRandom(5) > 1) {
                const int x = rng.NextRandom(37), y = rng.NextRandom(33);
                if (terrain[x][y] == 0 && terrain[x+1][y] == 0
                    && terrain[x][y+1] == 0 && terrain[x+1][y+1] == 0) {
                    terrain[x][y] = 0x7f; terrain[x+1][y] = 0x80;
                    terrain[x][y+1] = 0x81; terrain[x+1][y+1] = 0x82;
                }
            }
            if (rng.NextRandom(12) > 4) { rng.NextRandom(38); rng.NextRandom(34); }
        }
        int watchX = -1, watchY = -1;
        for (int i = 0; i < 13; ++i) if (rng.NextRandom(3) > 1) {
            const auto type = rng.NextRandom(20) == 20 ? 0x1a : 0x18;
            const int x = rng.NextRandom(36), y = rng.NextRandom(32);
            bool occupied = false;
            for (int a = x; a < x+3; ++a) for (int b = y; b < y+3; ++b)
                if (terrain[a][b]) { occupied = true; watchX = a; watchY = b; }
            if (occupied) continue;
            const std::uint16_t tiles[3][3] = {{8,9,10},{11,static_cast<std::uint16_t>(type),12},{13,14,15}};
            for (int a = 0; a < 3; ++a) for (int b = 0; b < 3; ++b)
                terrain[x+a][y+b] = tiles[b][a];
        }
        if (watchX < 0) continue;
        drain_storm::GameData actual;
        if (!actual.Load(assets, 25, seed, 4)) return 2;
        if (actual.level_[watchX][watchY] != terrain[watchX][watchY]) {
            std::cout << "procedural-overwrite depth=9 skill=4 seed=" << seed
                << " cell=" << watchX << ',' << watchY << " actual=" << actual.level_[watchX][watchY]
                << " expected=" << terrain[watchX][watchY] << '\n';
            break;
        }
    }
    return 0;
}
