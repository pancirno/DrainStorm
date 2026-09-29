#include <iostream>
#include <sstream>
#include "application_state.h"

int wmain(int argc, wchar_t** argv) {
    if (argc != 2) return 2;
    const std::filesystem::path assets(argv[1]);
    unsigned total = 0, failures = 0;
    for (std::size_t level = 1; level <= 25; ++level) {
        for (int skill = 1; skill <= 4; ++skill) {
            unsigned failed = 0, firstSeed = 0, invalidOverlays = 0;
            for (unsigned seed = 1; seed <= 128; ++seed) {
                drain_storm::GameData board;
                if (!board.Load(assets, level, seed, skill)) return 2;
                std::stringstream stream(std::ios::in | std::ios::out | std::ios::binary);
                if (!board.WriteGame(stream)) return 2;
                auto restored = board;
                ++total;
                if (!restored.ReadGame(stream)) {
                    ++failed; ++failures;
                    if (firstSeed == 0) {
                        firstSeed = seed;
                        for (const auto& overlay : board.overlays_)
                            invalidOverlays += overlay.column >= 40 || overlay.row >= 36;
                    }
                }
            }
            if (failed) std::cout << "roundtrip class=" << level << " skill=" << skill
                << " failed=" << failed << "/128 first_seed=" << firstSeed
                << " first_invalid_overlays=" << invalidOverlays << std::endl;
        }
    }
    std::cout << "roundtrip total=" << total << " failures=" << failures << std::endl;
    return 0;
}
