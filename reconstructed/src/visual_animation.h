#pragma once

#include <cstdint>

namespace drain_storm::presentation {
// Repeatable cosmetic randomness. Never advances the gameplay RNG on paint.
inline bool VisualChance(std::uint32_t frame, std::uint32_t salt, unsigned divisor) {
    std::uint32_t value = frame + salt * 0x9e3779b9u;
    value = (value ^ (value >> 16)) * 0x7feb352du;
    value = (value ^ (value >> 15)) * 0x846ca68bu;
    return ((value ^ (value >> 16)) % divisor) == 0;
}
}
