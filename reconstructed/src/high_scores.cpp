#include "high_scores.h"

#include <algorithm>
#include <array>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace drain_storm {
namespace {

constexpr std::array<char, 5> kSignature = {'D', 'S', 'H', '6', '4'};
constexpr std::uint16_t kVersion = 4;
constexpr std::size_t kStoredNameLength = 32;
constexpr std::size_t kOriginalNameLength = 12;

template <typename T>
bool Read(std::istream& stream, T& value) {
    stream.read(reinterpret_cast<char*>(&value), sizeof(value));
    return stream.good();
}

template <typename T>
bool Write(std::ostream& stream, const T& value) {
    stream.write(reinterpret_cast<const char*>(&value), sizeof(value));
    return stream.good();
}

std::string NormalizeName(std::string name) {
    name.erase(std::remove_if(name.begin(), name.end(), [](unsigned char character) {
        return character < 0x20 || character == 0x7f;
    }), name.end());
    if (name.size() > kOriginalNameLength) {
        name.resize(kOriginalNameLength);
    }
    return name;
}

} // namespace

std::int32_t HighScoreTable::RankedScore(std::uint32_t score, bool heroBonus,
                         bool sphereBonus) noexcept {
    // FUN_1010_ed28 adds five million for the Sphere flag at +0x14 and ten
    // million for the victory flag at +0x16, using a signed 32-bit ranking.
    const std::uint32_t adjusted = score + (sphereBonus ? 5'000'000u : 0u)
        + (heroBonus ? 10'000'000u : 0u);
    return static_cast<std::int32_t>(adjusted);
}

bool HighScoreTable::Load(const std::filesystem::path& path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) {
        std::error_code error;
        if (std::filesystem::exists(path, error) || error) return false;
        entries_ = {};
        return true;
    }
    std::array<char, kSignature.size()> signature{};
    std::uint16_t version{};
    std::uint16_t count{};
    stream.read(signature.data(), static_cast<std::streamsize>(signature.size()));
    if (!stream || signature != kSignature || !Read(stream, version)
        || (version < 1 || version > kVersion)
        || !Read(stream, count) || count > kEntryCount) {
        return false;
    }
    std::array<HighScoreEntry, kEntryCount> restored{};
    for (std::size_t index = 0; index < count; ++index) {
        std::array<char, kStoredNameLength> name{};
        stream.read(name.data(), static_cast<std::streamsize>(name.size()));
        if (!stream || name.back() != '\0' || !Read(stream, restored[index].score)
            || !Read(stream, restored[index].skill)) {
            return false;
        }
        restored[index].name = NormalizeName(name.data());
        restored[index].occupied = true;
        // Version 1 accidentally stored the active native board selector in
        // this field. Preserve valid 1..4 values and safely map selectors that
        // cannot be an original Skill value back to EZ.
        if (version == 1 && (restored[index].skill < 1 || restored[index].skill > 4)) {
            restored[index].skill = 1;
        }
        if (version >= 3) {
            std::uint8_t heroBonus{};
            if (!Read(stream, heroBonus) || heroBonus > 1) return false;
            restored[index].heroBonus = heroBonus != 0;
        }
        if (version >= 4) {
            std::uint8_t sphereBonus{};
            if (!Read(stream, sphereBonus) || sphereBonus > 1) return false;
            restored[index].sphereBonus = sphereBonus != 0;
        }
        if (restored[index].skill < 1 || restored[index].skill > 4) return false;
        if (RankedScore(restored[index].score, restored[index].heroBonus,
                        restored[index].sphereBonus) < 0
            || (index != 0
                && RankedScore(restored[index - 1].score, restored[index - 1].heroBonus,
                               restored[index - 1].sphereBonus)
                    < RankedScore(restored[index].score, restored[index].heroBonus,
                                  restored[index].sphereBonus))) {
            return false;
        }
    }
    entries_ = std::move(restored);
    return true;
}

bool HighScoreTable::Save(const std::filesystem::path& path) const {
    std::ofstream stream(path, std::ios::binary | std::ios::trunc);
    if (!stream) {
        return false;
    }
    stream.write(kSignature.data(), static_cast<std::streamsize>(kSignature.size()));
    const std::uint16_t count = static_cast<std::uint16_t>(std::count_if(
        entries_.begin(), entries_.end(), [](const HighScoreEntry& entry) {
            return entry.occupied;
        }));
    if (!Write(stream, kVersion) || !Write(stream, count)) {
        return false;
    }
    for (std::size_t index = 0; index < count; ++index) {
        std::array<char, kStoredNameLength> name{};
        const std::string normalized = NormalizeName(entries_[index].name);
        std::copy(normalized.begin(), normalized.end(), name.begin());
        stream.write(name.data(), static_cast<std::streamsize>(name.size()));
        if (!stream || !Write(stream, entries_[index].score)
            || !Write(stream, entries_[index].skill)
            || !Write(stream, static_cast<std::uint8_t>(entries_[index].heroBonus ? 1 : 0))
            || !Write(stream, static_cast<std::uint8_t>(entries_[index].sphereBonus ? 1 : 0))) {
            return false;
        }
    }
    return stream.good();
}

bool HighScoreTable::Qualifies(std::uint32_t score, bool heroBonus,
                               bool sphereBonus) const noexcept {
    // FUN_1010_ec10 initializes empty records to score -1 and FUN_1010_ec94
    // compares candidate and stored scores as signed Win16 long values.
    const std::int32_t candidate = RankedScore(score, heroBonus, sphereBonus);
    const std::int32_t lowest = !entries_.back().occupied
        ? -1 : RankedScore(entries_.back().score, entries_.back().heroBonus,
                           entries_.back().sphereBonus);
    return candidate > lowest;
}

bool HighScoreTable::Submit(std::string name, std::uint32_t score, std::uint16_t skill,
                            bool heroBonus, bool sphereBonus) {
    if (!Qualifies(score, heroBonus, sphereBonus) || skill < 1 || skill > 4) {
        return false;
    }
    HighScoreEntry candidate{NormalizeName(std::move(name)), score, skill,
                             heroBonus, true, sphereBonus};
    const std::int32_t candidateScore = RankedScore(score, heroBonus, sphereBonus);
    auto position = std::find_if(entries_.begin(), entries_.end(), [candidateScore](const HighScoreEntry& entry) {
        return candidateScore > (!entry.occupied
            ? -1 : RankedScore(entry.score, entry.heroBonus, entry.sphereBonus));
    });
    if (position == entries_.end()) {
        return false;
    }
    std::move_backward(position, entries_.end() - 1, entries_.end());
    *position = std::move(candidate);
    return true;
}

std::wstring HighScoreTable::Format() const {
    std::wostringstream output;
    output << L"DRAIN STORM HIGH SCORES\n\n";
    bool any = false;
    for (std::size_t index = 0; index < entries_.size(); ++index) {
        const HighScoreEntry& entry = entries_[index];
        if (!entry.occupied) {
            continue;
        }
        any = true;
        output << std::setw(2) << index + 1 << L". "
               << std::wstring(entry.name.begin(), entry.name.end())
               << L"   " << RankedScore(entry.score, entry.heroBonus,
                                         entry.sphereBonus) << L"   Skill "
               << (entry.skill == 1 ? L"EZ" : std::to_wstring(entry.skill)) << L'\n';
    }
    if (!any) {
        output << L"No expeditions have been recorded yet.";
    }
    return output.str();
}

} // namespace drain_storm
