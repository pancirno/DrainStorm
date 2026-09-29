#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <string>

namespace drain_storm {

struct HighScoreEntry {
    std::string name;
    std::uint32_t score{};
    std::uint16_t skill{1};
    bool heroBonus{};
    bool occupied{};
    bool sphereBonus{};
};

class HighScoreTable {
public:
    static constexpr std::size_t kEntryCount = 10;
    [[nodiscard]] static std::int32_t RankedScore(std::uint32_t score, bool heroBonus,
                                                bool sphereBonus) noexcept;

    bool Load(const std::filesystem::path& path);
    bool Save(const std::filesystem::path& path) const;
    bool Submit(std::string name, std::uint32_t score, std::uint16_t skill,
                bool heroBonus = false, bool sphereBonus = false);
    [[nodiscard]] bool Qualifies(std::uint32_t score, bool heroBonus = false,
                                 bool sphereBonus = false) const noexcept;
    [[nodiscard]] const std::array<HighScoreEntry, kEntryCount>& Entries() const noexcept {
        return entries_;
    }
    [[nodiscard]] std::wstring Format() const;

private:
    std::array<HighScoreEntry, kEntryCount> entries_{};
};

} // namespace drain_storm
