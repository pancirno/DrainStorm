#include "game_data.h"

#include "bitmap_surface.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace drain_storm {
namespace {

constexpr char kSaveSignature[] = {'D', 'A', 'H', '6', '4'};
constexpr std::uint16_t kSaveVersion = 32;
constexpr std::uint32_t kMaximumSavedRecords = 10000;

bool IsTransientOverlay(const BoardOverlay& overlay) noexcept {
    return (overlay.type >= 0x14 && overlay.type <= 0x1d) || overlay.type == 0x1f
        || (overlay.type == 0x1e && overlay.animationFrame != 0);
}

constexpr std::array<std::wstring_view, 47> kLaws = {
    L"Going mining? You may find treasures in Crud.",
    L"Beware the Captains of Gorloth.\n\nDestroy them at a distance.",
    L"Lost? Be sure to read signposts.", L"They say Sewer Squids are dumb.",
    L"Warning: Lieutenants are clever. They'll side-step your attack.",
    L"They say the Demon Lord's Lieutenants can't be fossilized.",
    L"Bulldozers can ruin a perfectly good decoy.",
    L"The Bottomless Abyss hides important treasure!", L"Only the powerful will survive the Abyss.",
    L"Don't dilly-dally in the Drain (you'll be sorry).", L"Some Lieutenants carry treasure.",
    L"Some walls can't be destroyed.", L"Play at The Demon-Dust Casino.",
    L"At the casino, you may be punished for winning.",
    L"Don't stand on the betting square when being attacked!",
    L"They say the casino won't accept Smart Bombs.", L"Don't bet Repellent at the casino.",
    L"Jacks or better wins! Unless you're playing wild cards.",
    L"Casino treasures are yours... just keep winning.",
    L"Only big winners can get past the casino force field.",
    L"Confucius say, \"He who not use decoy not live long.\"",
    L"The Wise will tell: Practice using decoys- they're helpful.", L"Bubbles kill...\nBut not everything.",
    L"Use repellent to keep those pesky monsters away.",
    L"For best results, open treasure chests with a key.", L"If the portal is reinforced, try a missile.",
    L"Warriors tell: Mines can be helpful in the Power Plant.",
    L"Keys can open doors... but so can a missile.", L"The Moon Room is filled with treasure.",
    L"Beware the cursed Moon Room. There are traps.",
    L"The tips of the moon can be very helpful... but beware.", L"The key to the Moon lies in the Maze.",
    L"There's a special key to getting big treasure.",
    L"Rumors say the Xoids posess the Emerald of Gorloth.", L"Repairing the statue may help you on your way.",
    L"Gorloth the Terrible is missing his emerald-eye.",
    L"Smart Card carriers are permitted into the Power Plant.", L"A Smart Card was lost in the Main Portal Room.",
    L"Smart Bombs lose power in the presence of Gorloth.",
    L"Rumors say Gorloth himself guards the Power Plant.",
    L"They say Bombs and Mines only humor the Demon Lord.",
    L"Wise men warn: Repellent aids little near the Demon Lord.",
    L"They say no one can withstand the blast of a Radio Bomb.",
    L"A lever shuts down the Power Plant's force field.",
    L"They say Gorloth's brother is home for a visit.",
    L"For best results, open treasure chests with a key.", L"Lost? Be sure to read signposts."
};

template <typename T>
bool WriteValue(std::ostream& stream, const T& value) {
    static_assert(std::is_trivially_copyable_v<T>);
    stream.write(reinterpret_cast<const char*>(&value), sizeof(value));
    return stream.good();
}

template <typename T>
bool ReadValue(std::istream& stream, T& value) {
    static_assert(std::is_trivially_copyable_v<T>);
    stream.read(reinterpret_cast<char*>(&value), sizeof(value));
    return stream.good();
}

template <typename T>
bool WriteRecords(std::ostream& stream, const std::vector<T>& records) {
    const auto count = static_cast<std::uint32_t>(records.size());
    return WriteValue(stream, count)
        && (records.empty()
            || (stream.write(reinterpret_cast<const char*>(records.data()),
                             static_cast<std::streamsize>(records.size() * sizeof(T))), stream.good()));
}

template <typename T>
bool ReadRecords(std::istream& stream, std::vector<T>& records) {
    std::uint32_t count{};
    if (!ReadValue(stream, count) || count > kMaximumSavedRecords) {
        return false;
    }
    records.resize(count);
    if (!records.empty()) {
        stream.read(reinterpret_cast<char*>(records.data()),
                    static_cast<std::streamsize>(records.size() * sizeof(T)));
    }
    return stream.good();
}

// Preserve the historical six-byte overlay records. Version 24 appends ages
// after the existing save payload, independently of native struct padding.
bool WriteRecords(std::ostream& stream, const std::vector<BoardOverlay>& records) {
    if (!WriteValue(stream, static_cast<std::uint32_t>(records.size()))) return false;
    for (const auto& overlay : records)
        if (!WriteValue(stream, overlay.type) || !WriteValue(stream, overlay.column)
            || !WriteValue(stream, overlay.row)) return false;
    return true;
}

bool ReadRecords(std::istream& stream, std::vector<BoardOverlay>& records) {
    std::uint32_t count{};
    if (!ReadValue(stream, count) || count > kMaximumSavedRecords) return false;
    records.assign(count, {});
    for (auto& overlay : records)
        if (!ReadValue(stream, overlay.type) || !ReadValue(stream, overlay.column)
            || !ReadValue(stream, overlay.row)) return false;
    return true;
}

// Presentation caches must not alter the original twelve-byte actor payload.
bool WriteRecords(std::ostream& stream, const std::vector<BoardActor>& records) {
    if (!WriteValue(stream, static_cast<std::uint32_t>(records.size()))) return false;
    for (const auto& actor : records)
        if (!WriteValue(stream, actor.type) || !WriteValue(stream, actor.column)
            || !WriteValue(stream, actor.row) || !WriteValue(stream, actor.deltaColumn)
            || !WriteValue(stream, actor.deltaRow) || !WriteValue(stream, actor.behavior)) return false;
    return true;
}

bool ReadRecords(std::istream& stream, std::vector<BoardActor>& records) {
    std::uint32_t count{};
    if (!ReadValue(stream, count) || count > kMaximumSavedRecords) return false;
    records.assign(count, {});
    for (auto& actor : records)
        if (!ReadValue(stream, actor.type) || !ReadValue(stream, actor.column)
            || !ReadValue(stream, actor.row) || !ReadValue(stream, actor.deltaColumn)
            || !ReadValue(stream, actor.deltaRow) || !ReadValue(stream, actor.behavior)) return false;
    return true;
}

bool WriteOverlayFrames(std::ostream& stream, const std::vector<BoardOverlay>& records) {
    for (const auto& overlay : records)
        if (!WriteValue(stream, overlay.animationFrame)) return false;
    return true;
}

std::vector<std::uint8_t> ReadFile(const std::filesystem::path& path) {
    std::ifstream stream(path, std::ios::binary | std::ios::ate);
    if (!stream) {
        return {};
    }
    const std::streamsize size = stream.tellg();
    if (size <= 0) {
        return {};
    }
    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(size));
    stream.seekg(0);
    if (!stream.read(reinterpret_cast<char*>(bytes.data()), size)) {
        return {};
    }
    return bytes;
}

std::uint16_t ReadU16(const std::vector<std::uint8_t>& bytes, std::size_t offset) {
    return static_cast<std::uint16_t>(bytes[offset])
        | static_cast<std::uint16_t>(bytes[offset + 1] << 8);
}

// Direct assignments reconstructed from FUN_1008_b164. Characters that create dynamic objects
// leave the underlying board square empty and are rendered by separate overlay lists in the game.
std::optional<int> DecodeLevelCharacter(std::uint8_t character) {
    switch (character) {
    case ' ':
    case '.': return 0;
    case '&': return 0x43;
    case '(': return 0x38;
    case ')': return 0x39;
    case '*': return 0x03;
    case '+': return 0x40;
    case '-': return 0x36;
    case '/': return 0x41;
    case '0': return 0x01;
    case ';': return 0x6d;
    case '<': return 0x23;
    case '>': return 0x27;
    case '@': return 0x07;
    case 'B': return 0x5d;
    case 'C': return 0x5c;
    case 'E': return 0x3c;
    case 'I': return 0xb1;
    case 'K': return 0x7e;
    case 'O': return 0xb0;
    case 'S': return 0x6c;
    case 'T': return 0x3f;
    case 'U': return 0xaf;
    case 'W': return 0x6e;
    case 'Z': return 0x20;
    case 'b': return 0x60;
    case 'c': return 0x5f;
    case 'n': return 0x6f;
    case 'o': return 0x02;
    case 's': return 0x68;
    case 'w': return 0x3e;
    case '{': return 0x3a;
    case '|': return 0x37;
    case '}': return 0x3b;
    case '~': return 0x3d;

    // Object/signpost constructors in FUN_1008_b164; the static square remains empty.
    case '$': case '?': case 'D': case 'G': case 'R':
    case 'd': case 'g': case 'm': case 't': case 'z':
        return 0;
    default:
        return std::nullopt;
    }
}

std::uint16_t TerrainSprite(std::uint16_t tile, std::uint16_t initial,
                           std::uint32_t frame) {
    // 1008:9786. Convert incremental GDI updates to a complete frame by
    // retaining the last applicable update within the original 120-tick cycle.
    std::uint16_t sprite = initial;
    for (unsigned tick = 0; tick <= frame % 120; ++tick) {
        if ((tile >= 0x23 && tile <= 0x26) || (tile >= 0x27 && tile <= 0x2a)) {
            // 1008:9933..998e: forward 0x27..2a uses 0x2b..2f;
            // return 0x23..26 uses 0x41..45, not the other way around.
            const int base = tile <= 0x26 ? 0x41 : 0x2b;
            sprite = static_cast<std::uint16_t>(base + (tick % 8 < 4 ? 1 + tick % 4 : 0));
        } else if (tile >= 0x51 && tile <= 0x54) sprite = tick % 9 * 4 + tile + 0x5e;
        else if (tile >= 0x41 && tile <= 0x44) {
            constexpr std::uint16_t horizontal[] = {0xa8, 0xaa, 0xa9, 0xab, 0xa7, 0xab, 0xa9, 0xaa};
            constexpr std::uint16_t vertical[] = {0xa8, 0xac, 0xad, 0xae, 0xa7, 0xae, 0xad, 0xac};
            const auto phase = (tick + (tile <= 0x42 ? 4 : 0)) % 8;
            sprite = (tile & 1) ? horizontal[phase] : vertical[phase];
        } else if (tile == 0x49) {
            if (tick % 10 == 1) sprite = 0x70;
            if (tick % 10 == 5) sprite = 0x71;
        } else if (tile == 0x18 || tile == 0x19 || tile == 0x1e || tile == 0x1f) {
            if (tick % 5 == 1) sprite = 0x6d;
            if (tick % 5 == 2) sprite = 0x6e;
        } else if (tile == 0x1a || tile == 0x1b) {
            constexpr std::uint16_t frames[] = {0x6d, 0x6f, 0x73};
            sprite = frames[tick % 3];
        } else if (tile == 0x1c || tile == 0x1d) sprite = tick % 2 ? 0x6d : 0x6f;
        else if (tile == 0x20 || tile == 0x22) {
            const unsigned period = tile == 0x20 ? 20 : 6;
            if (tick % period == 0) sprite = 0x29;
            if (tick % period == period / 2) sprite = 0x2a;
        } else if (tile == 0x59 && tick % 10 == 0) sprite = tick % 40 / 10 + 0xd3;
        else if (tile == 0x5b && tick % 2 == 0) sprite = tick % 8 / 2 + 0xd8;
        else if (tile == 0xab) {
            if (tick % 4 == 0) sprite = 0x1c5;
            if (tick % 4 == 1) sprite = 0x1c6;
        } else if (tile == 0xad) sprite = 0x1c8 + tick % 3;
        else if (tile == 0xb0 && tick % 2 == 0) sprite = 0x1d5 + tick % 6 / 2;
    }
    return sprite;
}

std::uint16_t ActorSprite(std::uint16_t type, std::uint32_t frame) {
    // Direct translations of FUN_1008_ced2, FUN_1008_cf26, and FUN_1008_cefc,
    // selected by FUN_1008_8ea4 for the four map actor classes.
    switch (type) {
    case 0: {
        const std::uint16_t phase = static_cast<std::uint16_t>(frame % 10);
        return phase > 5 ? static_cast<std::uint16_t>(11 - phase)
                         : static_cast<std::uint16_t>(phase + 1);
    }
    case 1: {
        constexpr std::uint16_t frames[] = {10, 11, 10, 12};
        return frames[frame % 4];
    }
    case 2: return 7; // Randomized frames are retained by each actor on ticks.
    case 3: return 0x10;
    case 4: {
        constexpr std::uint16_t frames[] = {0x0d, 0x0e, 0x0f, 0x0e};
        return frames[frame % 4];
    }
    default: return 0;
    }
}

} // namespace

bool GameData::Load(const std::filesystem::path& gameDataDirectory, std::size_t levelNumber,
                    std::uint32_t initialRandomState, int difficulty, int boardDepth,
                    bool introductorySign) {
    if (levelNumber < 1 || levelNumber > 25 || difficulty < 1 || difficulty > 4
        || boardDepth < 0 || boardDepth > 0x7fff) {
        return false;
    }
    const std::vector<std::uint8_t> spriteBytes = ReadFile(gameDataDirectory / L"sprite_map.bin");
    const std::vector<std::uint8_t> tileBytes = ReadFile(gameDataDirectory / L"tile_descriptors.bin");
    const std::vector<std::uint8_t> overlaySpriteBytes = ReadFile(
        gameDataDirectory / L"overlay_sprites.bin");
    const std::vector<std::uint8_t> itemDescriptorBytes = ReadFile(
        gameDataDirectory / L"item_descriptors.bin");
    const std::wstring levelFileName = L"level_0" + std::to_wstring(levelNumber) + L".map";
    std::vector<std::uint8_t> levelBytes;
    if (levelNumber >= 10) {
        levelBytes.assign(kBoardRows * (kBoardColumns + 1), static_cast<std::uint8_t>(' '));
        for (std::size_t row = 0; row < kBoardRows; ++row) {
            levelBytes[row * (kBoardColumns + 1) + kBoardColumns] = 0;
        }
    } else {
        levelBytes = ReadFile(gameDataDirectory / L"levels" / levelFileName);
    }
    if (spriteBytes.size() % 10 != 0 || tileBytes.size() % 6 != 0 ||
        overlaySpriteBytes.size() % 2 != 0 || itemDescriptorBytes.size() % 34 != 0 ||
        levelBytes.size() != kBoardRows * (kBoardColumns + 1)) {
        return false;
    }

    sprites_.clear();
    for (std::size_t offset = 0; offset < spriteBytes.size(); offset += 10) {
        sprites_.push_back({ReadU16(spriteBytes, offset), ReadU16(spriteBytes, offset + 2),
                            ReadU16(spriteBytes, offset + 4), ReadU16(spriteBytes, offset + 6),
                            ReadU16(spriteBytes, offset + 8)});
    }
    tiles_.clear();
    for (std::size_t offset = 0; offset < tileBytes.size(); offset += 6) {
        tiles_.push_back({ReadU16(tileBytes, offset), ReadU16(tileBytes, offset + 2),
                          ReadU16(tileBytes, offset + 4)});
    }
    overlaySprites_.clear();
    for (std::size_t offset = 0; offset < overlaySpriteBytes.size(); offset += 2) {
        overlaySprites_.push_back(static_cast<std::int16_t>(ReadU16(overlaySpriteBytes, offset)));
    }
    itemSprites_.clear();
    itemDisplayKinds_.clear();
    itemNames_.clear();
    for (std::size_t offset = 0; offset < itemDescriptorBytes.size(); offset += 34) {
        itemDisplayKinds_.push_back(ReadU16(itemDescriptorBytes, offset));
        itemSprites_.push_back(ReadU16(itemDescriptorBytes, offset + 2));
        std::wstring name;
        for (std::size_t character = offset + 4;
             character < offset + 34 && itemDescriptorBytes[character] != 0; ++character) {
            name.push_back(static_cast<wchar_t>(itemDescriptorBytes[character]));
        }
        itemNames_.push_back(std::move(name));
    }
    // Inventory layout initialized by FUN_1008_169c. Randomized stock quantities use
    // the lowest value in their original inclusive ranges until saved-game RNG is connected.
    inventory_ = {{0, 0}, {4, 19}, {1, 3}, {3, 19}, {2, 3}, {5, 9},
                  {6, 9}, {8, 7}, {9, 2}, {0x13, 5}, {0x0e, 1}, {0x0b, 4}};
    selectedItem_ = 0;
    firstVisibleItem_ = 0;
    animationFrame_ = 0;
    transientOverlayFrame_ = 0;
    playerAttackFrame_ = 0;
    randomState_ = initialRandomState;
    turnCounter_ = 0;
    score_ = 0;
    health_ = 3;
    fatalHitLatched_ = false;
    playerFacesRight_ = true;
    repellentEnergy_ = 0;
    victoryTriggered_ = false;
    pendingMessages_.clear();
    pendingDialogMessage_.clear();
    pendingTransition_ = 0;
    discoveredLaws_.fill(false);
    discoveredLawOrder_.clear();
    lawRecallCursor_ = 0;
    levelNumber_ = levelNumber;
    boardDepth_ = static_cast<std::uint16_t>(boardDepth != 0 ? boardDepth
        : (levelNumber >= 17 ? levelNumber - 16 : 1));
    depthFromSave_ = false;
    difficulty_ = difficulty;
    levelFourNortheastTriggered_ = false;
    levelFourSouthwestTriggered_ = false;
    levelFourFloodTriggered_ = false;
    levelFourCountdown_ = 24;
    levelTwoSequenceState_ = -1;
    levelTwoSequenceMode_ = std::min(difficulty_, 3);
    casinoTopChestCount_ = 7;
    levelSevenStructureActive_.fill(true);
    levelSevenSpawnDivisor_ = 25;
    levelSevenEndPhase_ = false;
    levelFiveProbeColumn_ = 0;
    levelFiveProbeRow_ = 0;
    levelFiveFirstProbeBroken_ = false;
    levelFiveSecondProbeBroken_ = false;
    levelFiveProbeTransformed_ = false;
    levelFiveFirstSwitchActive_ = false;
    levelFiveSecondSwitchActive_ = false;
    levelSixStructuresSpawned_ = false;
    levelSixUpperTransporterTriggered_ = false;
    levelSixLowerTransporterTriggered_ = false;
    levelSixCenterOpened_ = false;
    levelEightAlarmTriggered_ = false;
    levelThreeSpawnChance_ = static_cast<std::uint16_t>(difficulty_ + 24);
    levelTwelveSpawnChance_ = 0;
    levelTwelveMaximumGroup_ = 1;
    levelOneSpawnChance_ = 0;
    levelOneMaximumGroup_ = 1;
    levelElevenRuntimeFlags_ = 0;
    levelTenEmeraldClaimed_ = false;
    levelTenBarrierOpened_ = false;
    levelSixteenChambersActive_.fill(true);
    levelSixteenCoreActivated_ = false;
    genericSpawnChance_ = levelNumber >= 17 ? (difficulty_ + 5) * 3 : 10;
    genericBreakDivisor_ = levelNumber >= 17 ? (8 - difficulty_) * 15 : 20;
    genericRuntimeFlags_ = 0;
    levelOneTargetHits_.fill(0);
    levelOneTargetColumns_ = {36, 36};
    levelOneTargetRows_ = {1, 32};
    levelOneTargetDefeated_.fill(0);
    levelOneFirstTargetActivationTurn_ = 0;
    levelOneSecurityGateOpened_ = false;
    levelOneCornerPortalsActivated_ = false;
    energySphereDestroyed_ = false;
    levelOneArenaEntered_ = false;
    casinoStatus_ = 0;
    SetCasinoAnnouncement(0);
    casinoBetCount_ = 0;
    casinoBetItemType_ = 0xffff;
    casinoDeck_.fill(0);
    casinoRanks_.fill(0);
    casinoSuits_.fill(0);
    casinoHeld_.fill(0);
    casinoDeckIndex_ = 0;
    casinoWildRank_ = 0;
    casinoBonusStage_ = 0;
    casinoResultTicks_ = 0;

    overlays_.clear();
    actors_.clear();
    terrainAnimation_.assign(kBoardColumns, {});
    bool foundPlayerStart = false;
    for (auto& column : level_) column.fill(0);
    const auto placeLargeObject = [this](std::uint16_t centerTile,
                                         std::size_t column, std::size_t row) {
        if (column + 2 >= kBoardColumns || row + 2 >= kBoardRows) return;
        const bool oddPattern = (centerTile & 1u) != 0;
        const std::uint16_t firstTile = oddPattern ? 0x10 : 8;
        level_[column][row] = firstTile;
        level_[column + 1][row] = static_cast<std::uint16_t>(firstTile + 1);
        level_[column + 2][row] = static_cast<std::uint16_t>(firstTile + 2);
        level_[column][row + 1] = static_cast<std::uint16_t>(firstTile + 3);
        level_[column + 1][row + 1] = centerTile;
        level_[column + 2][row + 1] = static_cast<std::uint16_t>(firstTile + 4);
        level_[column][row + 2] = static_cast<std::uint16_t>(firstTile + 5);
        level_[column + 1][row + 2] = static_cast<std::uint16_t>(firstTile + 6);
        level_[column + 2][row + 2] = static_cast<std::uint16_t>(firstTile + 7);
    };
    const auto placeLargeObjectIfEmpty = [this, &placeLargeObject](
            std::uint16_t centerTile, std::size_t column, std::size_t row) {
        if (column + 2 >= kBoardColumns || row + 2 >= kBoardRows) return;
        for (std::size_t offsetColumn = 0; offsetColumn < 3; ++offsetColumn)
            for (std::size_t offsetRow = 0; offsetRow < 3; ++offsetRow)
                if (level_[column + offsetColumn][row + offsetRow] != 0) return;
        placeLargeObject(centerTile, column, row);
    };
    // b164 clears the board and then scans columns outermost, rows innermost.
    // Non-space symbols overwrite earlier inline expansions; spaces leave
    // already-expanded object squares intact.
    for (std::size_t row = 0; row < kBoardRows; ++row)
        if (levelBytes[row * (kBoardColumns + 1) + kBoardColumns] != 0)
            return false;
    for (std::size_t column = 0; column < kBoardColumns; ++column) {
        for (std::size_t row = 0; row < kBoardRows; ++row) {
            const std::size_t rowOffset = row * (kBoardColumns + 1);
            const std::uint8_t character = levelBytes[rowOffset + column];
            if (character == ' ') continue;
            if (character == '<') {
                playerColumn_ = column;
                playerRow_ = row;
                foundPlayerStart = true;
            }
            // FUN_1008_b164 creates these through FUN_1008_af5a.  Its node layout is
            // row, column, type; FUN_1008_b09e later maps the type to a sprite.
            if (character == '$' || character == '?') {
                overlays_.push_back({static_cast<std::uint16_t>(character == '$' ? 1 : 2),
                                     static_cast<std::uint16_t>(column),
                                     static_cast<std::uint16_t>(row)});
            }
            switch (character) {
            case 'g': actors_.push_back({0, static_cast<std::uint16_t>(column),
                                         static_cast<std::uint16_t>(row)}); break;
            case 'm': actors_.push_back({1, static_cast<std::uint16_t>(column),
                                         static_cast<std::uint16_t>(row)}); break;
            case 'd': actors_.push_back({2, static_cast<std::uint16_t>(column),
                                         static_cast<std::uint16_t>(row)}); break;
            case 'z': actors_.push_back({4, static_cast<std::uint16_t>(column),
                                         static_cast<std::uint16_t>(row)}); break;
            case 'D': placeLargeObjectIfEmpty(0x1a, column, row); continue;
            case 'G': placeLargeObjectIfEmpty(0x18, column, row); continue;
            case 'R': placeLargeObjectIfEmpty(0x1c, column, row); continue;
            case 't':
                if (column + 1 < kBoardColumns && row + 1 < kBoardRows
                    && level_[column][row] == 0 && level_[column + 1][row] == 0
                    && level_[column][row + 1] == 0
                    && level_[column + 1][row + 1] == 0) {
                    level_[column][row] = 0x7f;
                    level_[column + 1][row] = 0x80;
                    level_[column][row + 1] = 0x81;
                    level_[column + 1][row + 1] = 0x82;
                }
                continue;
            default: break;
            }
            const std::optional<int> tile = DecodeLevelCharacter(
                character);
            if (!tile || *tile < 0 || static_cast<std::size_t>(*tile) >= tiles_.size()) {
                return false;
            }
            level_[column][row] = static_cast<std::uint16_t>(*tile);
        }
    }
    if (levelNumber == 1) {
        // Post-map construction translated from FUN_1008_e07a (selector 1038).
        level_[12][12] = 0x51; level_[13][12] = 0x52;
        level_[12][13] = 0x53; level_[13][13] = 0x54;

        const auto placeNineTileBlock = [this](std::size_t column, std::size_t row,
                                                std::uint16_t first) {
            for (std::size_t offsetRow = 0; offsetRow < 3; ++offsetRow) {
                for (std::size_t offsetColumn = 0; offsetColumn < 3; ++offsetColumn) {
                    level_[column + offsetColumn][row + offsetRow] = static_cast<std::uint16_t>(
                        first + offsetRow * 3 + offsetColumn);
                }
            }
        };
        const auto placeBf2aBlock = [this](std::size_t column, std::size_t row) {
            constexpr std::uint16_t tiles[3][3] = {
                {0x45, 0x46, 0x47}, {0x48, 0x49, 0x4d}, {0x4e, 0x4f, 0x50}};
            for (std::size_t offsetRow = 0; offsetRow < 3; ++offsetRow) {
                for (std::size_t offsetColumn = 0; offsetColumn < 3; ++offsetColumn) {
                    level_[column + offsetColumn][row + offsetRow] =
                        tiles[offsetRow][offsetColumn];
                }
            }
        };
        placeBf2aBlock(7, 7);
        placeBf2aBlock(7, 16);
        placeBf2aBlock(16, 7);
        placeBf2aBlock(16, 16);

        for (std::size_t column = 10; column <= 15; ++column) {
            level_[column][8] = (column & 1u) == 0 ? 0x43 : 0x41;
            level_[column][17] = (column & 1u) == 0 ? 0x43 : 0x41;
        }
        for (std::size_t row = 10; row <= 15; ++row) {
            level_[8][row] = (row & 1u) == 0 ? 0x44 : 0x42;
            level_[17][row] = (row & 1u) == 0 ? 0x44 : 0x42;
        }
        placeLargeObject(0x1f, 2, 2);
        placeLargeObject(0x1f, 2, 22);
        placeLargeObject(0x1f, 22, 22);
        placeLargeObject(0x1f, 22, 2);

        // FUN_1008_80c0 stores a 12-byte actor record: type, column, row,
        // column velocity, row velocity, and behavior. These four runs form
        // the clockwise moving ring from FUN_1008_e07a.
        for (int column = 14; column > 10; --column) {
            actors_.push_back({3, static_cast<std::uint16_t>(column), 10, 1, 0, 3});
        }
        for (int row = 14; row > 10; --row) {
            actors_.push_back({3, 15, static_cast<std::uint16_t>(row), 0, 1, 3});
        }
        for (int column = 11; column < 15; ++column) {
            actors_.push_back({3, static_cast<std::uint16_t>(column), 15, -1, 0, 3});
        }
        for (int row = 11; row < 15; ++row) {
            actors_.push_back({3, 10, static_cast<std::uint16_t>(row), 0, -1, 3});
        }

        overlays_.push_back({0x2d, 1, 19});
        // FUN_1008_c1e6 and FUN_1008_80c0 receive row before column.
        placeLargeObject(0x1d, 32, 14);
        for (int index = 0, count = NextRandom(5) + 2 * difficulty_; index < count; ++index)
            actors_.push_back({3, 33, 15, 0, 0, 2});
        placeLargeObject(0x1d, 33, 19);
        for (int index = 0, count = NextRandom(5) + 2 * difficulty_; index < count; ++index)
            actors_.push_back({3, 34, 20, 0, 0, 2});
        level_[14][32] = 0x23;
        for (std::size_t row = 29; row < 32; ++row) level_[23][row] = 0x61;
        level_[20][28] = 0xab;
        level_[20][27] = 0xad;
        placeLargeObject(0x1a, 4, 31);
        placeNineTileBlock(36, 1, 0x90);
        placeNineTileBlock(36, 32, 0x90);
    }
    if (levelNumber == 2) {
        // Initial selector-1040 construction translated from FUN_1010_0ea0.
        // Original board helpers receive row before column.
        for (std::uint16_t row = 18; row <= 20; ++row) {
            overlays_.push_back({3, 14, row});
            overlays_.push_back({3, 34, row});
        }
        placeLargeObject(0x1e, 35, 27);
        placeLargeObject(0x1e, 3, 32);

        const auto placeBf2a = [this](std::size_t column, std::size_t row) {
            constexpr std::uint16_t values[3][3] = {
                {0x45, 0x46, 0x47}, {0x48, 0x49, 0x4d}, {0x4e, 0x4f, 0x50}};
            for (std::size_t offsetRow = 0; offsetRow < 3; ++offsetRow) {
                for (std::size_t offsetColumn = 0; offsetColumn < 3; ++offsetColumn) {
                    level_[column + offsetColumn][row + offsetRow] = values[offsetRow][offsetColumn];
                }
            }
        };
        placeBf2a(7, 2);
        placeBf2a(32, 2);

        const auto placeTransporter = [this](std::size_t column, std::size_t row) {
            level_[column][row] = 0x7f;
            level_[column + 1][row] = 0x80;
            level_[column][row + 1] = 0x81;
            level_[column + 1][row + 1] = 0x82;
        };
        for (std::size_t column = 11; column < 30; column += 3) {
            placeTransporter(column, 0);
        }
        for (std::size_t column = 13; column < 28; column += 7) {
            placeLargeObject(0x1f, column, 5);
        }

        overlays_.push_back({0x26, 23, 26});
        overlays_.push_back({0x27, 24, 26});
        overlays_.push_back({0x28, 23, 27});
        overlays_.push_back({0x29, 24, 27});
        overlays_.push_back({0x2c, 26, 26});
        for (std::size_t index = 0; index < 6; ++index) {
            level_[17 + index * 3][35 - index] = static_cast<std::uint16_t>(99 + index);
        }
        for (std::size_t index = 0; index < 5; ++index) {
            level_[20 + index * 3][35 - index] =
                static_cast<std::uint16_t>(NextRandom(4) + 0x68);
        }
        level_[20][27] = static_cast<std::uint16_t>(NextRandom(4) + 0x68);
        level_[20][26] = static_cast<std::uint16_t>(NextRandom(6) + 0x62);
        level_[28][27] = static_cast<std::uint16_t>(NextRandom(4) + 0x68);
        level_[28][26] = static_cast<std::uint16_t>(NextRandom(6) + 0x62);
        // FUN_1010_1c1e selects ordinary poker or a deuce/seven-wild table.
        if (NextRandom(2) == 2) {
            casinoWildRank_ = static_cast<std::uint16_t>(NextRandom(2) * 5 - 3);
        }
    }
    if (levelNumber == 3) {
        // FUN_1010_4044 begins with three calls to FUN_1010_4a0e. Each call
        // searches at most twenty candidates in the recovered inner range and
        // rejects transporters, existing 3x3 objects, and the player's square.
        PlaceLevelThreeStructure(false);
        PlaceLevelThreeStructure(false);
        PlaceLevelThreeStructure(false);

        // Fixed and randomized portion of selector-1048 construction from
        // FUN_1010_4044. The actor-count roll has an upper bound equal to the
        // selected difficulty, and FUN_1008_80c0 creates that many actors.
        const int initialActors = NextRandom(difficulty_);
        for (int index = 0; index < initialActors; ++index) {
            actors_.push_back({3, 28, 1, 0, 0, 0});
        }
        overlays_.push_back({9, 36, 0});
        if (NextRandom(3) > 1) overlays_.push_back({3, 35, 0});
        if (NextRandom(2) == 2) overlays_.push_back({3, 34, 0});
        if (NextRandom(3) > 1) overlays_.push_back({3, 0, 8});
        if (NextRandom(3) < 3) overlays_.push_back({3, 0, 9});

        const auto placeBf2a = [this](std::size_t column, std::size_t row) {
            constexpr std::uint16_t values[3][3] = {
                {0x45, 0x46, 0x47}, {0x48, 0x49, 0x4d}, {0x4e, 0x4f, 0x50}};
            for (std::size_t offsetRow = 0; offsetRow < 3; ++offsetRow) {
                for (std::size_t offsetColumn = 0; offsetColumn < 3; ++offsetColumn) {
                    level_[column + offsetColumn][row + offsetRow] = values[offsetRow][offsetColumn];
                }
            }
        };
        placeBf2a(31, 5);
        placeBf2a(31, 9);

        level_[36][3] = 0x69;
        level_[36][4] = 0x6b;
        level_[35][4] = 0x6c;
        level_[36][12] = 0x6a;
        level_[35][12] = 0x6c;
        level_[36][13] = 0x69;
        for (std::size_t index = 0; index < 6; ++index) {
            level_[38][1 + index * 3] = static_cast<std::uint16_t>(99 + index);
        }
        level_[35][8] = 0x29;
        level_[0][35] = 0x88;
        level_[1][35] = 0x89;
        level_[2][27] = 0x2a;
        overlays_.push_back({0x2d, 38, 19});
        placeLargeObject(0x1e, 31, 20);
        level_[3][17] = 0x27;
        level_[32][31] = 0x28;

        // Four vector-driven perimeter runs. The original chooses type 0 or 1
        // independently for every actor.
        for (int column = 37; column >= 28; --column) {
            actors_.push_back({static_cast<std::uint16_t>(NextRandom(3) == 1),
                               static_cast<std::uint16_t>(column), 24, 1, 0, 3});
        }
        for (int row = 33; row >= 24; --row) {
            actors_.push_back({static_cast<std::uint16_t>(NextRandom(3) == 1), 39,
                               static_cast<std::uint16_t>(row), 0, 1, 3});
        }
        for (int column = 30; column < 40; ++column) {
            actors_.push_back({static_cast<std::uint16_t>(NextRandom(3) == 1),
                               static_cast<std::uint16_t>(column), 35, -1, 0, 3});
        }
        for (int row = 26; row < 36; ++row) {
            actors_.push_back({static_cast<std::uint16_t>(NextRandom(3) == 1), 28,
                               static_cast<std::uint16_t>(row), 0, -1, 3});
        }
    }
    if (levelNumber == 4) {
        // Selector-1050 constructor (FUN_1010_4f76). FUN_1008_bb5a clears
        // tile 0x21 inside the original 36-by-33 subregion before placing exits.
        for (std::size_t column = 0; column <= 35; ++column) {
            for (std::size_t row = 0; row <= 32; ++row) {
                if (level_[column][row] == 0x21) {
                    level_[column][row] = 0;
                }
            }
        }
        level_[33][5] = 0x7f;
        level_[34][5] = 0x80;
        level_[33][6] = 0x81;
        level_[34][6] = 0x82;
        level_[5][29] = 0x7f;
        level_[6][29] = 0x80;
        level_[5][30] = 0x81;
        level_[6][30] = 0x82;

        // The original creates difficulty+5 scattered type-1/type-2 pickups.
        for (int index = 0; index < difficulty_ + 5; ++index) {
            const int column = NextRandom(24) + 10;
            const int row = NextRandom(7) + column - 4;
            const std::uint16_t type = NextRandom(7) < 2 ? 2 : 1;
            overlays_.push_back({type, static_cast<std::uint16_t>(column),
                                 static_cast<std::uint16_t>(row)});
        }
    }
    if (levelNumber == 5) {
        // Selector-1058 constructor from FUN_1010_55ac.
        level_[3][33] = 0x23;
        constexpr std::pair<std::size_t, std::size_t> structures[] = {
            {18, 18}, {21, 18}, {19, 15}};
        for (const auto& [column, row] : structures) {
            placeLargeObject(0x19, column, row);
            for (std::size_t offsetColumn = 0; offsetColumn <= 2; offsetColumn += 2) {
                for (std::size_t offsetRow = 0; offsetRow <= 2; offsetRow += 2) {
                    actors_.push_back({1, static_cast<std::uint16_t>(column + offsetColumn),
                                       static_cast<std::uint16_t>(row + offsetRow), 0, 0, 0});
                }
            }
        }

        // The constructor probes for adjacent tile-3 squares and retains the
        // coordinates for its runtime mechanism. Reproduce the probe so later
        // random construction consumes the same global sequence.
        for (int attempt = 0; attempt < 4096; ++attempt) {
            const std::size_t column = static_cast<std::size_t>(NextRandom(39) - 1);
            const std::size_t row = static_cast<std::size_t>(NextRandom(36) - 1);
            if (level_[column][row] == 3 && level_[column + 1][row] == 3) {
                levelFiveProbeColumn_ = static_cast<std::uint16_t>(column);
                levelFiveProbeRow_ = static_cast<std::uint16_t>(row);
                break;
            }
        }
        level_[35][3] = 0x24;
        overlays_.push_back({0x2d, 37, 2});
        overlays_.push_back({0x2d, 37, 3});
        overlays_.push_back({0x0f, 39, 5});
        level_[32][7] = 0x27;

        for (int index = 0; index < difficulty_ + 5; ++index) {
            const std::uint16_t column = static_cast<std::uint16_t>(NextRandom(30));
            const std::uint16_t row = static_cast<std::uint16_t>(36 - NextRandom(5));
            const std::uint16_t type = NextRandom(5) == 2 ? 2 : 1;
            overlays_.push_back({type, column, row});
        }
    }
    if (levelNumber == 6) {
        // Selector-1060 constructor from FUN_1010_5fc4.
        overlays_.push_back({0x0d, 31, 10});
        overlays_.push_back({3, 31, 10});
        overlays_.push_back({9, 31, 10});
        overlays_.push_back({0x0a, 31, 23});
        overlays_.push_back({3, 31, 23});
        overlays_.push_back({9, 31, 23});
        overlays_.push_back({0x2d, 25, 9});
        overlays_.push_back({0x2d, 25, 24});
        level_[10][17] = 0x59;
        level_[10][16] = 0x62;
        level_[10][18] = 0x62;
        placeLargeObject(0x1f, 17, 14);
        placeLargeObject(0x1f, 17, 19);

        // FUN_1008_80c0 repeats each call difficulty*3 times.
        for (std::uint16_t coordinate = 10; coordinate < 31; coordinate += 5) {
            for (int repeat = 0; repeat < difficulty_ * 3; ++repeat) {
                actors_.push_back({4, 5, coordinate, 0, 0, 2});
            }
            for (int repeat = 0; repeat < difficulty_ * 3; ++repeat) {
                actors_.push_back({4, 34, coordinate, 0, 0, 2});
            }
            for (int repeat = 0; repeat < difficulty_ * 3; ++repeat) {
                actors_.push_back({4, coordinate, 5, 0, 0, 2});
            }
            for (int repeat = 0; repeat < difficulty_ * 3; ++repeat) {
                actors_.push_back({4, coordinate, 31, 0, 0, 2});
            }
        }
        level_[5][5] = 0x23;
        level_[5][31] = 0x28;
        level_[33][3] = 0x27;
        level_[8][19] = 0x7e;
        level_[3][11] = 0x7e;
    }
    if (levelNumber == 7) {
        // Selector-1068 constructor from FUN_1010_6a56. The original data
        // segment stores DAT_1080_192a=19 (column) and DAT_1080_192c=18 (row).
        constexpr std::size_t centerColumn = 19;
        constexpr std::size_t centerRow = 18;
        std::uint16_t deviceTile = 0x70;
        for (std::size_t offsetRow = 0; offsetRow < 3; ++offsetRow) {
            for (std::size_t offsetColumn = 0; offsetColumn < 3; ++offsetColumn) {
                level_[centerColumn + offsetColumn][centerRow + offsetRow] = deviceTile++;
            }
        }
        const auto addActors = [this](std::uint16_t type, std::uint16_t column,
                                      std::uint16_t row, int count) {
            for (int index = 0; index < count; ++index) {
                actors_.push_back({type, column, row, 0, 0, 0});
            }
        };
        addActors(0, 18, 17, NextRandom(3) + difficulty_);
        addActors(1, 22, 17, NextRandom(3) + difficulty_);
        addActors(0, 22, 21, NextRandom(3) + difficulty_);
        addActors(1, 18, 21, NextRandom(3) + difficulty_);
        addActors(2, 18, 19, NextRandom(3) + difficulty_ + 3);
        addActors(2, 22, 19, NextRandom(3) + difficulty_ + 3);

        placeLargeObject(0x1f, 8, 7);
        placeLargeObject(0x1f, 30, 7);
        placeLargeObject(0x1f, 8, 29);
        placeLargeObject(0x1f, 30, 29);
        for (std::size_t column = 18; column <= 22; ++column) {
            level_[column][30] = (column & 1u) == 0 ? 0x43 : 0x41;
        }
        level_[20][33] = 0x28;
        overlays_.push_back({3, 18, 31});
        overlays_.push_back({3, 22, 31});
        placeLargeObject(0x1e, 30, 1);
        level_[2][27] = 0x27;
    }
    if (levelNumber == 8) {
        // Procedural board constructor FUN_1010_c80e starts from a blank board;
        // the eighth extracted map resource is not consumed by this class.
        for (BoardColumn& column : level_) {
            column.fill(0);
        }
        actors_.clear();
        overlays_.clear();
        const auto setIfEmpty = [this](int column, int row, std::uint16_t tile) {
            if (column >= 0 && row >= 0 && column < static_cast<int>(kBoardColumns) &&
                row < static_cast<int>(kBoardRows) && level_[column][row] == 0) {
                level_[column][row] = tile;
            }
        };
        // Exact FUN_1008_9504/FUN_1008_9594 terrain-growth sequence.
        for (int growth = 0; growth < 20; ++growth) {
            const int centerColumn = NextRandom(40) - 1;
            const int centerRow = NextRandom(36) - 1;
            const int upwardExtent = NextRandom(20);
            const int downwardExtent = NextRandom(20);
            int leftExtent = NextRandom(4) - 2;
            int rightExtent = NextRandom(4) - 2;
            for (int column = centerColumn - upwardExtent - 1;
                 column <= centerColumn + downwardExtent + 1; ++column) {
                for (int row = centerRow - leftExtent; row <= centerRow + rightExtent; ++row) {
                    setIfEmpty(column, row, 3);
                }
                int leftChange = 0;
                int rightChange = 0;
                if (column < centerColumn) {
                    leftChange = NextRandom(4) - 2;
                    rightChange = NextRandom(4) - 2;
                } else {
                    rightChange = 1 - NextRandom(4);
                    leftChange = 1 - NextRandom(4);
                }
                leftExtent += leftChange;
                rightExtent += rightChange;
            }
        }
        for (std::size_t column = 8; column <= 32; ++column) {
            for (std::size_t row = 8; row <= 27; ++row) {
                level_[column][row] = 0;
            }
        }
        for (std::size_t column = 19; column <= 21; ++column) {
            for (std::size_t row = 6; row <= 8; ++row) {
                level_[column][row] = 0;
            }
        }
        level_[20][7] = 0x23;
        playerColumn_ = 20;
        playerRow_ = 7;
        foundPlayerStart = true;
        for (std::size_t column = 12; column <= 27; column += 5) {
            for (std::size_t row = 12; row <= 22; row += 5) {
                level_[column][row] = 0x7f;
                level_[column + 1][row] = 0x80;
                level_[column][row + 1] = 0x81;
                level_[column + 1][row + 1] = 0x82;
            }
        }
    }
    if (levelNumber == 9) {
        // Selector-1078 starting-board constructor (FUN_1010_d8f4).
        placeLargeObject(0x1e, 31, 32);
        placeLargeObject(0x1f, 5, 5);
    }
    if (levelNumber == 10) {
        // Procedural root/child class FUN_1010_33b4. It starts with five
        // FUN_1008_9594 terrain-growth seeds, each using extent five.
        const auto setIfEmpty = [this](int column, int row, std::uint16_t tile) {
            if (column >= 0 && row >= 0 && column < static_cast<int>(kBoardColumns)
                && row < static_cast<int>(kBoardRows) && level_[column][row] == 0) {
                level_[column][row] = tile;
            }
        };
        for (int growth = 0; growth < 5; ++growth) {
            const int centerColumn = NextRandom(40) - 1;
            const int centerRow = NextRandom(36) - 1;
            int firstExtent = NextRandom(5);
            int secondExtent = NextRandom(5);
            int leftExtent = NextRandom(4) - 2;
            int rightExtent = NextRandom(4) - 2;
            for (int column = centerColumn - firstExtent - 1;
                 column <= centerColumn + secondExtent + 1; ++column) {
                for (int row = centerRow - leftExtent; row <= centerRow + rightExtent; ++row) {
                    setIfEmpty(column, row, 3);
                }
                int leftChange{};
                int rightChange{};
                if (column < centerColumn) {
                    leftChange = NextRandom(4) - 2;
                    rightChange = NextRandom(4) - 2;
                } else {
                    rightChange = 1 - NextRandom(4);
                    leftChange = 1 - NextRandom(4);
                }
                leftExtent += leftChange;
                rightExtent += rightChange;
            }
        }
        // 1010:340a: column first, both rolls one-based; count scales with skill.
        for (int index = 0; index < (difficulty_ + 2) * 2; ++index) {
            const int column = NextRandom(38);
            const int row = NextRandom(34);
            setIfEmpty(column, row, 0x20);
        }
        const auto fillRectangle = [this](std::uint16_t tile, std::size_t left,
                                           std::size_t top, std::size_t right,
                                           std::size_t bottom) {
            for (std::size_t column = left; column <= right; ++column) {
                for (std::size_t row = top; row <= bottom; ++row) {
                    level_[column][row] = tile;
                }
            }
        };
        const auto borderRectangle = [this](std::uint16_t tile, std::size_t left,
                                             std::size_t top, std::size_t right,
                                             std::size_t bottom) {
            for (std::size_t column = left; column <= right; ++column) {
                level_[column][top] = tile;
                level_[column][bottom] = tile;
            }
            for (std::size_t row = top; row <= bottom; ++row) {
                level_[left][row] = tile;
                level_[right][row] = tile;
            }
        };
        // FUN_1008_ce88 receives max-row, max-column, min-row, min-column.
        // Keep that original row-before-column ordering when translating the
        // Xoid Fortress chamber rather than transposing its far corner.
        fillRectangle(0, 4, 14, 17, 27);
        level_[10][20] = 0x83; level_[11][20] = 0x84;
        level_[10][21] = 0x85; level_[11][21] = 0x86;
        borderRectangle(0x60, 5, 15, 16, 26);
        borderRectangle(0x60, 3, 18, 5, 23);
        fillRectangle(0, 5, 19, 5, 22);
        borderRectangle(0x60, 16, 18, 18, 23);
        fillRectangle(0, 16, 19, 16, 22);
        borderRectangle(0x60, 8, 13, 13, 15);
        fillRectangle(0, 9, 15, 12, 15);
        borderRectangle(0x60, 8, 26, 13, 28);
        fillRectangle(0, 9, 26, 12, 26);
        level_[4][19] = 0x7e;
        for (std::uint16_t column = 9; column < 13; ++column) {
            overlays_.push_back({static_cast<std::uint16_t>(NextRandom(2) == 1 ? 1 : 2),
                                 column, 27});
            overlays_.push_back({static_cast<std::uint16_t>(NextRandom(2) == 1 ? 1 : 2),
                                 column, 14});
        }
        for (std::uint16_t row = 20; row < 23; ++row) {
            overlays_.push_back({static_cast<std::uint16_t>(NextRandom(2) == 1 ? 1 : 2),
                                 17, row});
            overlays_.push_back({static_cast<std::uint16_t>(NextRandom(2) == 1 ? 1 : 2),
                                 4, row});
        }
        overlays_.push_back({10, 17, 19});
        placeLargeObject(0x1f, 4, 14);
        placeLargeObject(0x1f, 15, 14);
        placeLargeObject(0x1f, 15, 25);
        placeLargeObject(0x1f, 4, 25);
        level_[9][19] = 0x21; level_[12][19] = 0x21;
        level_[12][22] = 0x21; level_[9][22] = 0x21;
        level_[8][18] = static_cast<std::uint16_t>(NextRandom(3) < 2 ? 0x21 : 0x60);
        level_[13][18] = static_cast<std::uint16_t>(NextRandom(3) < 2 ? 0x21 : 0x60);
        level_[13][23] = static_cast<std::uint16_t>(NextRandom(3) < 2 ? 0x21 : 0x60);
        level_[8][23] = static_cast<std::uint16_t>(NextRandom(3) < 2 ? 0x21 : 0x60);
        level_[7][17] = 0x21; level_[14][17] = 0x21;
        level_[14][24] = 0x21; level_[7][24] = 0x21;
        const std::size_t startColumn = static_cast<std::size_t>(NextRandom(10) + 25);
        const std::size_t startRow = static_cast<std::size_t>(NextRandom(34));
        level_[startColumn][startRow] = 0x23;
        playerColumn_ = startColumn;
        playerRow_ = startRow;
        foundPlayerStart = true;
    }
    if (levelNumber == 11) {
        // Procedural FUN_1010_cb9c class created by selector-1048's fourth edge.
        // Population depends on both retained depth and selected skill.
        const auto setIfEmpty = [this](int column, int row, std::uint16_t tile) {
            if (column >= 0 && row >= 0 && column < static_cast<int>(kBoardColumns)
                && row < static_cast<int>(kBoardRows) && level_[column][row] == 0) {
                level_[column][row] = tile;
            }
        };
        const int pickupCount = NextRandom(difficulty_ + 3);
        for (int index = 0; index < pickupCount; ++index) {
            const std::uint16_t column = static_cast<std::uint16_t>(NextRandom(38));
            const std::uint16_t row = static_cast<std::uint16_t>(NextRandom(34));
            const std::uint16_t type = NextRandom(3) < 2 ? 2 : 7;
            overlays_.push_back({type, column, row});
        }
        for (int index = 0;; ++index) {
            if (NextRandom(10) <= index) {
                break;
            }
            level_[NextRandom(38)][NextRandom(34)] = 0xaf;
        }
        const auto placeRandomObject = [this, &placeLargeObjectIfEmpty](bool secondVariant) {
            const std::size_t column = static_cast<std::size_t>(NextRandom(36));
            const std::size_t row = static_cast<std::size_t>(NextRandom(32));
            const bool alternate = NextRandom(4) == 3;
            const std::uint16_t type = secondVariant
                ? static_cast<std::uint16_t>(alternate ? 0x1c : 0x19)
                : static_cast<std::uint16_t>(alternate ? 0x1b : 0x19);
            placeLargeObjectIfEmpty(type, column, row);
        };
        for (int index = 0; index < boardDepth_ + difficulty_ - 1; ++index) {
            if (NextRandom(5) < 3) {
                if (NextRandom(5) > 3) {
                    placeRandomObject(false);
                } else if (NextRandom(5) > 3) {
                    placeRandomObject(true);
                }
            } else {
                const std::size_t column = static_cast<std::size_t>(NextRandom(36));
                const std::size_t row = static_cast<std::size_t>(NextRandom(32));
                const std::uint16_t type = NextRandom(4) == 3 ? 0x1a : 0x18;
                placeLargeObjectIfEmpty(type, column, row);
            }
        }
        const int growthCount = NextRandom(3) + 1;
        for (int growth = 0; growth < growthCount; ++growth) {
            const int centerColumn = NextRandom(40) - 1;
            const int centerRow = NextRandom(36) - 1;
            const int firstExtent = NextRandom(8);
            const int secondExtent = NextRandom(8);
            int leftExtent = NextRandom(4) - 2;
            int rightExtent = NextRandom(4) - 2;
            for (int column = centerColumn - firstExtent - 1;
                 column <= centerColumn + secondExtent + 1; ++column) {
                for (int row = centerRow - leftExtent; row <= centerRow + rightExtent; ++row) {
                    setIfEmpty(column, row, 3);
                }
                int leftChange{};
                int rightChange{};
                if (column < centerColumn) {
                    leftChange = NextRandom(4) - 2;
                    rightChange = NextRandom(4) - 2;
                } else {
                    rightChange = 1 - NextRandom(4);
                    leftChange = 1 - NextRandom(4);
                }
                leftExtent += leftChange;
                rightExtent += rightChange;
            }
        }
        for (int attempt = 0; attempt < 4096; ++attempt) {
            const std::size_t column = static_cast<std::size_t>(NextRandom(37));
            const std::size_t row = static_cast<std::size_t>(NextRandom(33));
            if (level_[column][row] == 0 && level_[column + 2][row + 2] == 0) {
                level_[column][row] = 0x23;
                if (introductorySign) level_[column + 2][row + 2] = 0x7e;
                playerColumn_ = column;
                playerRow_ = row;
                foundPlayerStart = true;
                break;
            }
        }
        for (int attempt = 0; attempt < 4096; ++attempt) {
            const std::size_t column = static_cast<std::size_t>(NextRandom(38));
            const std::size_t row = static_cast<std::size_t>(NextRandom(34));
            if (level_[column][row] == 0) {
                level_[column][row] = 0x27;
                break;
            }
        }
    }
    if (levelNumber == 12) {
        // Final arena class FUN_1010_d33a. Like the other procedural classes it
        // starts from the zero-filled board rather than an embedded map segment.
        const auto patternedBorder = [this](std::size_t left, std::size_t top,
                                             std::size_t right, std::size_t bottom) {
            for (std::size_t column = left; column <= right; ++column) {
                level_[column][top] = 0x36;
                level_[column][bottom] = 0x36;
            }
            for (std::size_t row = top; row <= bottom; ++row) {
                level_[left][row] = 0x37;
                level_[right][row] = 0x37;
            }
            level_[left][top] = 0x38;
            level_[right][top] = 0x39;
            level_[left][bottom] = 0x3a;
            level_[right][bottom] = 0x3b;
        };
        patternedBorder(1, 0, 38, 35);
        for (std::size_t row = 0; row < kBoardRows; ++row) {
            if (level_[0][row] == 0) level_[0][row] = 0x21;
            if (level_[39][row] == 0) level_[39][row] = 0x21;
        }
        patternedBorder(4, 32, 18, 33);
        patternedBorder(21, 32, 35, 33);
        level_[24][33] = 0x7e;
        level_[23][33] = 0x3b;
        level_[25][33] = 0x3a;
        level_[23][32] = 0x3f;
        level_[25][32] = 0x3f;
        level_[15][33] = 0x23;
        level_[14][33] = 0x3b;
        level_[16][33] = 0x3a;
        level_[14][32] = 0x3f;
        level_[16][32] = 0x3f;
        playerColumn_ = 15;
        playerRow_ = 33;
        foundPlayerStart = true;
        level_[30][33] = 0x27;
        level_[29][33] = 0x3b;
        level_[31][33] = 0x3a;
        level_[29][32] = 0x3f;
        level_[31][32] = 0x3f;
        for (std::size_t column = 2; column < 36; column += 3) {
            for (std::size_t row = 1; row < 29; row += 3) {
                placeLargeObject(NextRandom(25) < 2 ? 0x19 : 0x18, column, row);
            }
        }
        const std::uint16_t excludedColumn = static_cast<std::uint16_t>(NextRandom(12) * 3);
        const std::uint16_t excludedRow = static_cast<std::uint16_t>(NextRandom(10) * 3 - 1);
        overlays_.push_back({0x0e, excludedColumn, excludedRow});
        for (int index = 0; index < 12; ++index) {
            std::uint16_t column{};
            std::uint16_t row{};
            do {
                column = static_cast<std::uint16_t>(NextRandom(12) * 3);
                row = static_cast<std::uint16_t>(NextRandom(10) * 3 - 1);
            } while (column == excludedColumn && row == excludedRow);
            overlays_.push_back({static_cast<std::uint16_t>(NextRandom(5) == 5 ? 2 : 1),
                                 column, row});
        }
        for (const std::size_t column : {2u, 19u, 36u}) {
            level_[column][32] = 0x7f;
            level_[column + 1][32] = 0x80;
            level_[column][33] = 0x81;
            level_[column + 1][33] = 0x82;
        }
    }
    if (levelNumber >= 13 && levelNumber <= 15) {
        // FUN_1010_86a0 stages one through three. FUN_1008_c5b0 builds a
        // randomized depth-first maze whose passage width shrinks at stage three.
        const int stage = static_cast<int>(levelNumber - 12);
        const int passageWidth = stage < 3 ? 2 : 1;
        for (BoardColumn& column : level_) column.fill(3);

        struct MazeNode { int column; int row; };
        const auto clearBlock = [this, passageWidth](int column, int row) {
            for (int offsetColumn = 0; offsetColumn < passageWidth; ++offsetColumn) {
                for (int offsetRow = 0; offsetRow < passageWidth; ++offsetRow) {
                    const int targetColumn = column + offsetColumn;
                    const int targetRow = row + offsetRow;
                    if (targetColumn >= 0 && targetRow >= 0
                        && targetColumn < static_cast<int>(kBoardColumns)
                        && targetRow < static_cast<int>(kBoardRows)) {
                        level_[targetColumn][targetRow] = 0;
                    }
                }
            }
        };
        const auto carveBetween = [this, passageWidth](MazeNode from, MazeNode to) {
            const int deltaColumn = (to.column > from.column) - (to.column < from.column);
            const int deltaRow = (to.row > from.row) - (to.row < from.row);
            while (from.column != to.column || from.row != to.row) {
                from.column += deltaColumn;
                from.row += deltaRow;
                if (deltaColumn == 0) {
                    for (int offset = 0; offset < passageWidth; ++offset) {
                        level_[from.column + offset][from.row] = 0;
                    }
                } else {
                    for (int offset = 0; offset < passageWidth; ++offset) {
                        level_[from.column][from.row + offset] = 0;
                    }
                }
            }
        };
        const auto carveMaze = [this, passageWidth, &clearBlock, &carveBetween](
                                  auto&& self, MazeNode node) -> void {
            constexpr std::array<MazeNode, 4> directions = {
                MazeNode{-1, 0}, MazeNode{0, 1}, MazeNode{0, -1}, MazeNode{1, 0}};
            std::vector<MazeNode> children;
            for (const MazeNode direction : directions) {
                const MazeNode child{node.column + direction.column * (passageWidth + 1),
                                     node.row + direction.row * (passageWidth + 1)};
                if (child.column < 0 || child.row < 0
                    || child.column >= static_cast<int>(kBoardColumns)
                    || child.row >= static_cast<int>(kBoardRows)
                    || level_[child.column][child.row] != 3) {
                    continue;
                }
                carveBetween(node, child);
                clearBlock(child.column, child.row);
                children.push_back(child);
            }
            if (children.empty()) return;
            std::size_t childIndex = static_cast<std::size_t>(NextRandom(
                static_cast<int>(children.size())) - 1);
            for (std::size_t visited = 0; visited < children.size(); ++visited) {
                self(self, children[childIndex]);
                childIndex = (childIndex + 1) % children.size();
            }
        };
        clearBlock(1, 1);
        carveMaze(carveMaze, {1, 1});

        const auto isWall = [this](int column, int row) {
            return column >= 0 && row >= 0 && column < static_cast<int>(kBoardColumns)
                && row < static_cast<int>(kBoardRows) && level_[column][row] >= 3
                && level_[column][row] <= 0x40;
        };
        constexpr std::array<std::uint16_t, 11> wallTiles = {
            0x36, 0x37, 0x38, 0x39, 0x3b, 0x3a, 0x3c, 0x3d, 0x3e, 0x3f, 0x40};
        for (int column = 0; column < static_cast<int>(kBoardColumns); ++column) {
            for (int row = 0; row < static_cast<int>(kBoardRows); ++row) {
                if (level_[column][row] != 3) continue;
                std::array<bool, 11> candidate{};
                candidate.fill(true);
                const auto disable = [&candidate](std::initializer_list<int> indices) {
                    for (const int index : indices) candidate[static_cast<std::size_t>(index)] = false;
                };
                if (isWall(column + 1, row)) disable({1, 3, 4, 7});
                else disable({2, 5, 6, 8, 9, 10});
                if (isWall(column - 1, row)) disable({1, 2, 5, 6});
                else disable({3, 4, 7, 8, 9, 10});
                if (isWall(column, row + 1)) disable({0, 4, 5, 8});
                else disable({2, 3, 6, 7, 9, 10});
                if (isWall(column, row - 1)) disable({0, 2, 3, 9});
                else disable({4, 5, 6, 7, 8, 10});
                const auto selected = std::find(candidate.begin(), candidate.end(), true);
                if (selected != candidate.end()) {
                    level_[column][row] = wallTiles[static_cast<std::size_t>(
                        std::distance(candidate.begin(), selected))];
                }
            }
        }

        for (std::size_t column = 11; column <= 25; ++column)
            for (std::size_t row = 11; row <= 25; ++row) level_[column][row] = 0;
        const auto patternedBorder = [this](std::size_t left, std::size_t top,
                                             std::size_t right, std::size_t bottom) {
            for (std::size_t column = left; column <= right; ++column) {
                level_[column][top] = level_[column][bottom] = 0x36;
            }
            for (std::size_t row = top; row <= bottom; ++row) {
                level_[left][row] = level_[right][row] = 0x37;
            }
            level_[left][top] = 0x38; level_[right][top] = 0x39;
            level_[left][bottom] = 0x3a; level_[right][bottom] = 0x3b;
        };
        patternedBorder(13, 13, 23, 23);
        level_[18][23] = level_[13][18] = level_[23][18] = level_[18][13] = 0;
        for (const auto [column, row] : std::array<std::pair<std::size_t, std::size_t>, 4>{
                 std::pair{14u, 14u}, std::pair{20u, 14u},
                 std::pair{14u, 20u}, std::pair{20u, 20u}}) {
            placeLargeObject(0x1a, column, row);
        }
        for (std::uint16_t column = 17; column < 20; ++column) {
            overlays_.push_back({1, column, 17});
            overlays_.push_back({1, column, 19});
        }
        overlays_.push_back({1, 17, 18});
        overlays_.push_back({1, 19, 18});
        const auto placeRandomStructure = [this, &placeLargeObject](int columnRange,
                                                                    int columnOffset,
                                                                    int rowOffset) {
            const std::size_t column = static_cast<std::size_t>(NextRandom(columnRange) + columnOffset);
            const std::size_t row = static_cast<std::size_t>(NextRandom(14) + rowOffset);
            placeLargeObject(0x18, column, row);
        };
        placeRandomStructure(6, 0, 0);
        placeRandomStructure(6, 0, 18);
        placeRandomStructure(9, 27, 0);
        placeRandomStructure(9, 27, 18);
        for (int index = 0; index < (difficulty_ + 2) * 2; ++index) {
            const std::uint16_t type = NextRandom(difficulty_) < 2 ? 0 : 2;
            const auto column = static_cast<std::uint16_t>(NextRandom(39));
            const auto row = static_cast<std::uint16_t>(NextRandom(35));
            actors_.push_back({type, column, row, 0, 0, 0});
        }
        if (stage > 1 || difficulty_ > 2) {
            const auto column = static_cast<std::uint16_t>(NextRandom(39));
            const auto row = static_cast<std::uint16_t>(NextRandom(35));
            actors_.push_back({3, column, row, 0, 0, 0});
        }
        const auto addGridOverlays = [this, passageWidth](std::uint16_t type) {
            const int count = NextRandom(3);
            for (int index = 0; index < count; ++index) {
                const int stride = passageWidth + 1;
                const int columnGrid = NextRandom(40 / stride);
                const int columnOffset = NextRandom(passageWidth);
                const int rowGrid = NextRandom(12);
                const int rowOffset = NextRandom(passageWidth);
                const auto column = static_cast<std::uint16_t>(columnGrid * stride - columnOffset);
                const auto row = static_cast<std::uint16_t>(rowGrid * stride - rowOffset);
                overlays_.push_back({type, column, row});
            }
        };
        addGridOverlays(7);
        addGridOverlays(2);
        // 1010:86a0 makes one non-retrying attempt per skill level.
        for (int attempt = 0; attempt < difficulty_; ++attempt) {
            const std::size_t transporterColumn = static_cast<std::size_t>(NextRandom(13) * 3 - 2);
            const std::size_t transporterRow = static_cast<std::size_t>(NextRandom(12) * 3 - 2);
            if (transporterColumn + 1 < kBoardColumns && transporterRow + 1 < kBoardRows
                && level_[transporterColumn][transporterRow] == 0
                && level_[transporterColumn + 1][transporterRow] == 0
                && level_[transporterColumn][transporterRow + 1] == 0
                && level_[transporterColumn + 1][transporterRow + 1] == 0) {
                level_[transporterColumn][transporterRow] = 0x7f;
                level_[transporterColumn + 1][transporterRow] = 0x80;
                level_[transporterColumn][transporterRow + 1] = 0x81;
                level_[transporterColumn + 1][transporterRow + 1] = 0x82;
            }
        }
        std::size_t startColumn{};
        std::size_t startRow{};
        for (int attempt = 0; attempt < 4096; ++attempt) {
            const int columnGrid = NextRandom(3);
            const int columnOffset = NextRandom(passageWidth + 1);
            const int rowGrid = NextRandom(11);
            const int rowOffset = NextRandom(passageWidth + 1);
            startColumn = static_cast<std::size_t>(
                columnGrid * (passageWidth + 1) - columnOffset);
            startRow = static_cast<std::size_t>(rowGrid * (passageWidth + 1) - rowOffset);
            if (startColumn < kBoardColumns && startRow < kBoardRows
                && level_[startColumn][startRow] == 0
                && (stage != 1 || (startColumn + passageWidth + 1 < kBoardColumns
                    && startRow + passageWidth + 1 < kBoardRows
                    && level_[startColumn + passageWidth + 1][startRow] == 0
                    && level_[startColumn][startRow + passageWidth + 1] == 0))) break;
        }
        level_[startColumn][startRow] = 0x23;
        playerColumn_ = startColumn; playerRow_ = startRow; foundPlayerStart = true;
        if (stage == 1) {
            level_[startColumn + passageWidth + 1][startRow] = 0x28;
            level_[startColumn][startRow + passageWidth + 1] = 0x7e;
        }
        for (int attempt = 0; attempt < 4096; ++attempt) {
            const int columnGrid = NextRandom(3);
            const int columnOffset = NextRandom(2);
            const int rowGrid = NextRandom(12);
            const int rowOffset = NextRandom(2);
            const std::size_t column = static_cast<std::size_t>(
                (columnGrid + 10) * 3 - columnOffset);
            const std::size_t row = static_cast<std::size_t>(rowGrid * 3 - rowOffset);
            if (column < kBoardColumns && row < kBoardRows && level_[column][row] == 0) {
                level_[column][row] = 0x27;
                break;
            }
        }
        for (BoardColumn& column : level_)
            for (std::uint16_t& tile : column)
                if (tile >= 0x36 && tile <= 0x40) tile = static_cast<std::uint16_t>(tile - 0x0b);
        // 1010:86a0 reveals the immediate 3x3 neighborhood at the entrance
        // before the first player turn, after hiding the rest of the maze.
        for (int column = static_cast<int>(playerColumn_) - 1;
             column <= static_cast<int>(playerColumn_) + 1; ++column) {
            for (int row = static_cast<int>(playerRow_) - 1;
                 row <= static_cast<int>(playerRow_) + 1; ++row) {
                if (column < 0 || row < 0 || column >= static_cast<int>(kBoardColumns)
                    || row >= static_cast<int>(kBoardRows)) continue;
                std::uint16_t& tile = level_[column][row];
                if (tile >= 0x2b && tile <= 0x35)
                    tile = static_cast<std::uint16_t>(tile + 0x0b);
            }
        }
    }
    if (levelNumber == 16) {
        // FUN_1010_3a96: a width-one randomized maze converted to the alien
        // 0x5d/0x5e terrain set, with only its entry chamber converted back.
        for (BoardColumn& column : level_) column.fill(3);
        struct MazeNode { int column; int row; };
        const auto clearCell = [this](MazeNode node) {
            if (node.column >= 0 && node.row >= 0
                && node.column < static_cast<int>(kBoardColumns)
                && node.row < static_cast<int>(kBoardRows)) {
                level_[node.column][node.row] = 0;
            }
        };
        const auto carveBetween = [this](MazeNode from, MazeNode to) {
            const int deltaColumn = (to.column > from.column) - (to.column < from.column);
            const int deltaRow = (to.row > from.row) - (to.row < from.row);
            while (from.column != to.column || from.row != to.row) {
                from.column += deltaColumn;
                from.row += deltaRow;
                level_[from.column][from.row] = 0;
            }
        };
        const auto carveMaze = [this, &clearCell, &carveBetween](auto&& self,
                                                                  MazeNode node) -> void {
            constexpr std::array<MazeNode, 4> directions = {
                MazeNode{-1, 0}, MazeNode{0, 1}, MazeNode{0, -1}, MazeNode{1, 0}};
            std::vector<MazeNode> children;
            for (const MazeNode direction : directions) {
                const MazeNode child{node.column + direction.column * 2,
                                     node.row + direction.row * 2};
                if (child.column < 0 || child.row < 0
                    || child.column >= static_cast<int>(kBoardColumns)
                    || child.row >= static_cast<int>(kBoardRows)
                    || level_[child.column][child.row] != 3) continue;
                carveBetween(node, child);
                clearCell(child);
                children.push_back(child);
            }
            if (children.empty()) return;
            std::size_t childIndex = static_cast<std::size_t>(
                NextRandom(static_cast<int>(children.size())) - 1);
            for (std::size_t visited = 0; visited < children.size(); ++visited) {
                self(self, children[childIndex]);
                childIndex = (childIndex + 1) % children.size();
            }
        };
        clearCell({1, 1});
        carveMaze(carveMaze, {1, 1});

        const auto isWall = [this](int column, int row) {
            return column >= 0 && row >= 0 && column < static_cast<int>(kBoardColumns)
                && row < static_cast<int>(kBoardRows) && level_[column][row] >= 3
                && level_[column][row] <= 0x40;
        };
        const auto autotileRegion = [this, &isWall](std::size_t left, std::size_t top,
                                                    std::size_t right, std::size_t bottom) {
            constexpr std::array<std::uint16_t, 11> tiles = {
                0x36, 0x37, 0x38, 0x39, 0x3b, 0x3a, 0x3c, 0x3d, 0x3e, 0x3f, 0x40};
            for (std::size_t column = left; column <= right; ++column) {
                for (std::size_t row = top; row <= bottom; ++row) {
                    if (level_[column][row] != 3) continue;
                    std::array<bool, 11> candidate{};
                    candidate.fill(true);
                    const auto disable = [&candidate](std::initializer_list<int> indices) {
                        for (const int index : indices)
                            candidate[static_cast<std::size_t>(index)] = false;
                    };
                    if (isWall(static_cast<int>(column) + 1, static_cast<int>(row))) disable({1,3,4,7});
                    else disable({2,5,6,8,9,10});
                    if (isWall(static_cast<int>(column) - 1, static_cast<int>(row))) disable({1,2,5,6});
                    else disable({3,4,7,8,9,10});
                    if (isWall(static_cast<int>(column), static_cast<int>(row) + 1)) disable({0,4,5,8});
                    else disable({2,3,6,7,9,10});
                    if (isWall(static_cast<int>(column), static_cast<int>(row) - 1)) disable({0,2,3,9});
                    else disable({4,5,6,7,8,10});
                    const auto selected = std::find(candidate.begin(), candidate.end(), true);
                    if (selected != candidate.end()) {
                        level_[column][row] = tiles[static_cast<std::size_t>(
                            std::distance(candidate.begin(), selected))];
                    }
                }
            }
        };
        const auto fillRectangle = [this](std::uint16_t tile, std::size_t left,
                                           std::size_t top, std::size_t right,
                                           std::size_t bottom) {
            for (std::size_t column = left; column <= right; ++column)
                for (std::size_t row = top; row <= bottom; ++row) level_[column][row] = tile;
        };
        fillRectangle(0, 17, 15, 23, 21);
        for (std::size_t column = 18; column <= 22; ++column) {
            level_[column][16] = level_[column][20] = 3;
        }
        for (std::size_t row = 16; row <= 20; ++row) {
            level_[18][row] = level_[22][row] = 3;
        }
        fillRectangle(0, 5, 27, 9, 31); placeLargeObject(0x18, 6, 28);
        fillRectangle(0, 31, 27, 35, 31); placeLargeObject(0x18, 32, 28);
        fillRectangle(0, 31, 5, 35, 9); placeLargeObject(0x18, 32, 6);
        fillRectangle(0, 5, 5, 9, 9);
        level_[6][6] = 0x7e;
        level_[8][8] = 0x23;
        playerColumn_ = 8; playerRow_ = 8; foundPlayerStart = true;
        for (int index = 0; index < difficulty_; ++index) {
            const std::uint16_t actorColumn = static_cast<std::uint16_t>(NextRandom(28) + 10);
            const std::uint16_t actorRow = static_cast<std::uint16_t>(NextRandom(24) + 10);
            actors_.push_back({3, actorColumn, actorRow, 0, 0, 0});
        }
        for (BoardColumn& column : level_) {
            for (std::uint16_t& tile : column) {
                if (tile == 3) tile = 0x5d;
                else if (tile == 0) tile = 0x5e;
            }
        }
        for (std::size_t column = 5; column <= 9; ++column) {
            for (std::size_t row = 5; row <= 9; ++row) {
                if (level_[column][row] == 0x5e) level_[column][row] = 0;
                else if (level_[column][row] == 0x5d) level_[column][row] = 3;
            }
        }
        autotileRegion(5, 5, 9, 9);
    }
    if (levelNumber >= 17 && levelNumber <= 25) {
        // FUN_1008_d4e0 at depths one through nine.
        const int depth = static_cast<int>(levelNumber - 16);
        const int selectedDifficulty = difficulty_;
        const auto setIfEmpty = [this](int column, int row, std::uint16_t tile) {
            if (column >= 0 && row >= 0 && column < static_cast<int>(kBoardColumns)
                && row < static_cast<int>(kBoardRows) && level_[column][row] == 0) {
                level_[column][row] = tile;
            }
        };
        for (int index = 0; index < depth + selectedDifficulty + 1; ++index) {
            if (NextRandom(5) > 1) {
                const auto column = static_cast<std::uint16_t>(NextRandom(38));
                const auto row = static_cast<std::uint16_t>(NextRandom(34));
                overlays_.push_back({1, column, row});
            }
        }
        const int transporterAttempts = depth < 4 ? 1 : 2;
        for (int attempt = 0; attempt < transporterAttempts; ++attempt) {
            if (NextRandom(5) > 5 - selectedDifficulty) {
                const std::size_t column = static_cast<std::size_t>(NextRandom(37));
                const std::size_t row = static_cast<std::size_t>(NextRandom(33));
                if (column + 1 < kBoardColumns && row + 1 < kBoardRows
                    && level_[column][row] == 0 && level_[column + 1][row] == 0
                    && level_[column][row + 1] == 0
                    && level_[column + 1][row + 1] == 0) {
                    level_[column][row] = 0x7f; level_[column + 1][row] = 0x80;
                    level_[column][row + 1] = 0x81; level_[column + 1][row + 1] = 0x82;
                }
            }
            if (NextRandom(12) > (6 - selectedDifficulty) * 2) {
                const auto column = static_cast<std::uint16_t>(NextRandom(38));
                const auto row = static_cast<std::uint16_t>(NextRandom(34));
                overlays_.push_back({2, column, row});
            }
        }
        int guardedObjectAttempts = depth + selectedDifficulty;
        if (selectedDifficulty < 2 && guardedObjectAttempts > 4) guardedObjectAttempts = 4;
        for (int index = 0; index < guardedObjectAttempts; ++index) {
            if (NextRandom(3) > 1) {
                const auto type = SelectProceduralStructureType(depth);
                const std::size_t column = static_cast<std::size_t>(NextRandom(36));
                const std::size_t row = static_cast<std::size_t>(NextRandom(32));
                placeLargeObjectIfEmpty(type, column, row);
            }
        }
        const auto fixedPickupColumn = static_cast<std::uint16_t>(NextRandom(38));
        const auto fixedPickupRow = static_cast<std::uint16_t>(NextRandom(34));
        overlays_.push_back({1, fixedPickupColumn, fixedPickupRow});
        const int typeSevenAttempts = depth < 5 ? 1 : 2;
        for (int attempt = 0; attempt < typeSevenAttempts; ++attempt) {
            if (NextRandom(selectedDifficulty + 2) <= selectedDifficulty) {
                const auto column = static_cast<std::uint16_t>(NextRandom(38));
                const auto row = static_cast<std::uint16_t>(NextRandom(34));
                overlays_.push_back({7, column, row});
            }
        }
        for (int attempt = 0; attempt < depth / 3 + selectedDifficulty; ++attempt) {
            if (NextRandom(3) > 1) {
                const auto column = static_cast<std::uint16_t>(NextRandom(38));
                const auto row = static_cast<std::uint16_t>(NextRandom(34));
                actors_.push_back({0, column, row, 0, 0, 0});
            }
            if (depth > 3 && NextRandom(4) == 4) {
                const auto column = static_cast<std::uint16_t>(NextRandom(38));
                const auto row = static_cast<std::uint16_t>(NextRandom(34));
                actors_.push_back({0, column, row, 0, 0, 0});
            }
        }

        const int growthCount = NextRandom(3) + 1;
        for (int growth = 0; growth < growthCount; ++growth) {
            const int centerColumn = NextRandom(40) - 1;
            const int centerRow = NextRandom(36) - 1;
            const int firstExtent = NextRandom(8);
            const int secondExtent = NextRandom(8);
            int leftExtent = NextRandom(4) - 2;
            int rightExtent = NextRandom(4) - 2;
            for (int column = centerColumn - firstExtent - 1;
                 column <= centerColumn + secondExtent + 1; ++column) {
                for (int row = centerRow - leftExtent; row <= centerRow + rightExtent; ++row)
                    setIfEmpty(column, row, 3);
                int leftChange{};
                int rightChange{};
                if (column < centerColumn) {
                    leftChange = NextRandom(4) - 2;
                    rightChange = NextRandom(4) - 2;
                } else {
                    rightChange = 1 - NextRandom(4);
                    leftChange = 1 - NextRandom(4);
                }
                leftExtent += leftChange;
                rightExtent += rightChange;
            }
        }
        int roomCount{};
        if (NextRandom(selectedDifficulty + 2) <= selectedDifficulty) {
            const int upperBound = depth < 4 ? 2 : selectedDifficulty / 3 + 3;
            roomCount = NextRandom(upperBound) - 1;
        }
        bool stopBuildingRooms = false;
        for (int room = 0; room < roomCount && !stopBuildingRooms; ++room) {
            for (int attempt = 0; attempt < 10; ++attempt) {
                const std::size_t left = static_cast<std::size_t>(NextRandom(33) + 1);
                const std::size_t top = static_cast<std::size_t>(NextRandom(29) + 1);
                bool clear = left + 4 < kBoardColumns && top + 4 < kBoardRows;
                for (std::size_t column = left; clear && column <= left + 4; ++column)
                    for (std::size_t row = top; row <= top + 4; ++row)
                        clear = clear && (level_[column][row] == 0 || level_[column][row] == 3);
                // 1008:d849 consumes the last budget unit before accepting
                // the candidate. Ten attempts also end this entire phase.
                if (attempt == 9) { stopBuildingRooms = true; break; }
                if (!clear) continue;
                for (std::size_t column = left; column <= left + 4; ++column)
                    for (std::size_t row = top; row <= top + 4; ++row) level_[column][row] = 0;
                for (std::size_t column = left; column <= left + 4; ++column)
                    level_[column][top] = level_[column][top + 4] = 3;
                for (std::size_t row = top; row <= top + 4; ++row)
                    level_[left][row] = level_[left + 4][row] = 3;
                for (std::size_t column = left + 1; column < left + 4; ++column) {
                    for (std::size_t row = top + 1; row < top + 4; ++row) {
                        if (NextRandom(4) == 4) {
                            overlays_.push_back({static_cast<std::uint16_t>(NextRandom(5) == 5 ? 2 : 1),
                                                 static_cast<std::uint16_t>(column),
                                                 static_cast<std::uint16_t>(row)});
                        }
                        const int actorRoll = NextRandom(7);
                        const std::uint16_t actorType = actorRoll < 2
                            ? static_cast<std::uint16_t>(NextRandom(5) < 2 ? 1 : 2) : 0;
                        actors_.push_back({actorType, static_cast<std::uint16_t>(column),
                                           static_cast<std::uint16_t>(row), 0, 0, 0});
                    }
                }
                break;
            }
        }
        int transporterRoomRange = 1;
        if (depth > 5 - selectedDifficulty && NextRandom(4) == 4)
            transporterRoomRange = 2;
        const int transporterRoomCount = NextRandom(transporterRoomRange) - 1;
        for (int room = 0; room < transporterRoomCount; ++room) {
            for (int attempt = 0; attempt < 10; ++attempt) {
                const std::size_t left = static_cast<std::size_t>(NextRandom(32) + 1);
                const std::size_t top = static_cast<std::size_t>(NextRandom(28) + 1);
                bool clear = left + 5 < kBoardColumns && top + 5 < kBoardRows;
                for (std::size_t column = left; clear && column <= left + 5; ++column)
                    for (std::size_t row = top; row <= top + 5; ++row)
                        clear = clear && level_[column][row] == 0;
                // 1008:d9f3 also discards a valid tenth candidate.
                if (attempt == 9) break;
                if (!clear) continue;
                for (std::size_t column = left; column <= left + 5; ++column)
                    for (std::size_t row = top; row <= top + 5; ++row) level_[column][row] = 0;
                for (std::size_t column = left; column <= left + 5; ++column)
                    level_[column][top] = level_[column][top + 5] = 3;
                for (std::size_t row = top; row <= top + 5; ++row)
                    level_[left][row] = level_[left + 5][row] = 3;
                const std::size_t transporterColumn = left + 2;
                const std::size_t transporterRow = top + 2;
                level_[transporterColumn][transporterRow] = 0x7f;
                level_[transporterColumn + 1][transporterRow] = 0x80;
                level_[transporterColumn][transporterRow + 1] = 0x81;
                level_[transporterColumn + 1][transporterRow + 1] = 0x82;
                for (std::size_t column = left + 1; column < left + 5; ++column) {
                    for (std::size_t row = top + 1; row < top + 5; ++row) {
                        const bool borderInterior = column == left + 1 || column == left + 4
                            || row == top + 1 || row == top + 4;
                        if (borderInterior && NextRandom(2) == 2) {
                            actors_.push_back({2, static_cast<std::uint16_t>(column),
                                               static_cast<std::uint16_t>(row), 0, 0, 0});
                        }
                    }
                }
                break;
            }
        }
        for (int attempt = 0; attempt < 4096; ++attempt) {
            const std::size_t column = static_cast<std::size_t>(NextRandom(37));
            const std::size_t row = static_cast<std::size_t>(NextRandom(33));
            if (level_[column][row] == 0 && level_[column + 2][row + 2] == 0) {
                level_[column][row] = 0x23;
                if (depth == 1) level_[column + 2][row + 2] = 0x7e;
                playerColumn_ = column; playerRow_ = row; foundPlayerStart = true;
                break;
            }
        }
        for (int attempt = 0; attempt < 4096; ++attempt) {
            const std::size_t column = static_cast<std::size_t>(NextRandom(38));
            const std::size_t row = static_cast<std::size_t>(NextRandom(34));
            if (level_[column][row] == 0) { level_[column][row] = 0x27; break; }
        }
    }
    if (!foundPlayerStart && levelNumber == 1) {
        // The selector-1038 board constructor places its start tile after parsing the map
        // (FUN_1008_e07a: column 14, row 32).  Preserve that constructor step here.
        playerColumn_ = 14;
        playerRow_ = 32;
        level_[playerColumn_][playerRow_] = 0x23;
    } else if (!foundPlayerStart) {
        playerColumn_ = 0;
        playerRow_ = 0;
    }
    return true;
}

bool GameData::LoadDemoGame(const std::filesystem::path& gameDataDirectory, int difficulty,
                            std::uint32_t initialRandomState) {
    if (difficulty < 1 || difficulty > 4) {
        return false;
    }
    // FUN_1008_169c consumes five inventory rolls before selecting one of four
    // starting board classes. Preserve that shared RNG chain into the constructor.
    std::uint32_t state = initialRandomState;
    const auto roll = [&state](int upperBound) {
        state = state * 0x343fdu + 0x269ec3u;
        return static_cast<int>(((state >> 16u) & 0x7fffu)
            % static_cast<std::uint32_t>(upperBound)) + 1;
    };
    const std::uint16_t typeFourQuantity = static_cast<std::uint16_t>(roll(5) + 18);
    const std::uint16_t typeThreeQuantity = static_cast<std::uint16_t>(roll(5) + 18);
    const std::uint16_t typeFiveQuantity = static_cast<std::uint16_t>(roll(5) + 8);
    const std::uint16_t typeSixQuantity = static_cast<std::uint16_t>(roll(5) + 8);
    const std::uint16_t typeElevenQuantity = static_cast<std::uint16_t>(roll(3) + 3);
    constexpr std::array<std::size_t, 4> startingLevels = {2, 1, 10, 9};
    const std::size_t levelNumber = startingLevels[static_cast<std::size_t>(roll(4) - 1)];
    if (!Load(gameDataDirectory, levelNumber, state, difficulty)) {
        return false;
    }
    inventory_ = {{0, 0}, {4, typeFourQuantity}, {1, 3}, {3, typeThreeQuantity}, {2, 3},
                  {5, typeFiveQuantity}, {6, typeSixQuantity}, {8, 7}, {9, 2}, {0x13, 5},
                  {0x0e, 1}, {0x0b, typeElevenQuantity}};
    return true;
}

bool GameData::LoadNewGame(const std::filesystem::path& gameDataDirectory, int difficulty,
                           std::uint32_t initialRandomState) {
    if (difficulty < 1 || difficulty > 4) {
        return false;
    }
    // FUN_1008_0630 is the playable New Game path. It begins with the generic
    // depth-one class and a compact three-slot arsenal: Bubble, Radio bomb,
    // and either Mines or Bombs. These rolls precede procedural construction.
    std::uint32_t state = initialRandomState;
    const auto roll = [&state](int upperBound) {
        state = state * 0x343fdu + 0x269ec3u;
        return static_cast<int>(((state >> 16u) & 0x7fffu)
            % static_cast<std::uint32_t>(upperBound)) + 1;
    };
    const std::uint16_t radioBombQuantity = static_cast<std::uint16_t>(roll(5) + 3);
    const bool startsWithMines = roll(2) == 1;
    const std::uint16_t secondaryType = startsWithMines ? 2 : 3;
    const std::uint16_t secondaryQuantity = static_cast<std::uint16_t>(
        startsWithMines ? roll(2) : roll(5) + 3);
    if (!Load(gameDataDirectory, 17, state, difficulty)) {
        return false;
    }
    // FUN_1008_0630 positions the vessel at b946's depth-one entry and then
    // overwrites that 0x23 square with zero. Only child stages keep an up drain.
    if (level_[playerColumn_][playerRow_] == 0x23) {
        level_[playerColumn_][playerRow_] = 0;
    }
    inventory_ = {{0, 0}, {4, radioBombQuantity},
                  {secondaryType, secondaryQuantity}};
    return true;
}

bool GameData::SaveGame(const std::filesystem::path& path) const {
    std::ofstream stream(path, std::ios::binary | std::ios::trunc);
    if (!stream) {
        return false;
    }
    return WriteGame(stream);
}

bool GameData::WriteGame(std::ostream& stream) const {
    stream.write(kSaveSignature, sizeof(kSaveSignature));
    if (!WriteValue(stream, kSaveVersion)) {
        return false;
    }
    const auto levelNumber = static_cast<std::uint16_t>(levelNumber_);
    const auto selectedItem = static_cast<std::uint32_t>(selectedItem_);
    const auto playerColumn = static_cast<std::uint16_t>(playerColumn_);
    const auto playerRow = static_cast<std::uint16_t>(playerRow_);
    const std::uint8_t facesRight = playerFacesRight_ ? 1 : 0;
    const std::uint8_t northeast = levelFourNortheastTriggered_ ? 1 : 0;
    const std::uint8_t southwest = levelFourSouthwestTriggered_ ? 1 : 0;
    const std::uint8_t flood = levelFourFloodTriggered_ ? 1 : 0;
    const std::uint8_t endPhase = levelSevenEndPhase_ ? 1 : 0;
    if (!WriteValue(stream, levelNumber) || !WriteValue(stream, selectedItem)
        || !WriteValue(stream, animationFrame_) || !WriteValue(stream, randomState_)
        || !WriteValue(stream, turnCounter_) || !WriteValue(stream, score_)
        || !WriteValue(stream, playerColumn) || !WriteValue(stream, playerRow)
        || !WriteValue(stream, facesRight) || !WriteValue(stream, health_)
        || !WriteValue(stream, repellentEnergy_) || !WriteValue(stream, northeast)
        || !WriteValue(stream, southwest) || !WriteValue(stream, flood)
        || !WriteValue(stream, levelSevenSpawnDivisor_) || !WriteValue(stream, endPhase)
        || !WriteValue(stream, levelTwoSequenceState_)
        || !WriteValue(stream, levelTwoSequenceMode_)
        || !WriteValue(stream, levelFourCountdown_)
        || !WriteValue(stream, difficulty_)
        || !WriteValue(stream, levelFiveProbeColumn_)
        || !WriteValue(stream, levelFiveProbeRow_)
        || !WriteValue(stream, levelFiveFirstProbeBroken_)
        || !WriteValue(stream, levelFiveSecondProbeBroken_)
        || !WriteValue(stream, levelFiveProbeTransformed_)
        || !WriteValue(stream, levelSixStructuresSpawned_)
        || !WriteValue(stream, levelSixUpperTransporterTriggered_)
        || !WriteValue(stream, levelSixLowerTransporterTriggered_)
        || !WriteValue(stream, levelSixCenterOpened_)
        || !WriteValue(stream, levelEightAlarmTriggered_)
        || !WriteValue(stream, levelElevenRuntimeFlags_)) {
        return false;
    }
    for (const bool active : levelSevenStructureActive_) {
        const std::uint8_t value = active ? 1 : 0;
        if (!WriteValue(stream, value)) {
            return false;
        }
    }
    for (const bool discovered : discoveredLaws_) {
        const std::uint8_t value = discovered ? 1 : 0;
        if (!WriteValue(stream, value)) {
            return false;
        }
    }
    for (const BoardColumn& column : level_) {
        stream.write(reinterpret_cast<const char*>(column.data()),
                     static_cast<std::streamsize>(column.size() * sizeof(column[0])));
    }
    return stream.good() && WriteRecords(stream, overlays_) && WriteRecords(stream, actors_)
        && WriteRecords(stream, inventory_) && WriteRecords(stream, projectiles_)
        && WriteRecords(stream, discoveredLawOrder_) && WriteValue(stream, lawRecallCursor_)
        && WriteValue(stream, levelOneTargetHits_[0])
        && WriteValue(stream, levelOneTargetHits_[1])
        && WriteValue(stream, casinoStatus_) && WriteValue(stream, casinoBetCount_)
        && WriteValue(stream, casinoBetItemType_)
        && WriteValue(stream, casinoDeck_) && WriteValue(stream, casinoRanks_)
        && WriteValue(stream, casinoSuits_) && WriteValue(stream, casinoHeld_)
        && WriteValue(stream, casinoDeckIndex_) && WriteValue(stream, casinoWildRank_)
        && WriteValue(stream, casinoBonusStage_) && WriteValue(stream, casinoResultTicks_)
        && WriteValue(stream, genericSpawnChance_) && WriteValue(stream, genericBreakDivisor_)
        && WriteValue(stream, genericRuntimeFlags_)
        && WriteValue(stream, transientOverlayFrame_)
        && WriteValue(stream, levelOneFirstTargetActivationTurn_)
        && WriteValue(stream, static_cast<std::uint8_t>(levelOneArenaEntered_ ? 1 : 0))
        && WriteValue(stream, static_cast<std::uint8_t>(levelTenEmeraldClaimed_ ? 1 : 0))
        && WriteValue(stream, static_cast<std::uint8_t>(levelTenBarrierOpened_ ? 1 : 0))
        && WriteValue(stream, static_cast<std::uint8_t>(levelSixteenChambersActive_[0] ? 1 : 0))
        && WriteValue(stream, static_cast<std::uint8_t>(levelSixteenChambersActive_[1] ? 1 : 0))
        && WriteValue(stream, static_cast<std::uint8_t>(levelSixteenChambersActive_[2] ? 1 : 0))
        && WriteValue(stream, static_cast<std::uint8_t>(levelSixteenCoreActivated_ ? 1 : 0))
        && WriteValue(stream, boardDepth_)
        && WriteOverlayFrames(stream, overlays_)
        && WriteValue(stream, casinoAnnouncement_)
        && WriteValue(stream, static_cast<std::uint32_t>(casinoScrollOffset_))
        && WriteValue(stream, levelOneTargetColumns_)
        && WriteValue(stream, levelOneTargetRows_)
        && WriteValue(stream, levelOneTargetDefeated_)
        && WriteValue(stream, static_cast<std::uint8_t>(levelOneSecurityGateOpened_ ? 1 : 0))
        && WriteValue(stream, static_cast<std::uint8_t>(levelOneCornerPortalsActivated_ ? 1 : 0))
        && WriteValue(stream, static_cast<std::uint8_t>(energySphereDestroyed_ ? 1 : 0))
        && WriteValue(stream, casinoTopChestCount_)
        && WriteValue(stream, static_cast<std::uint8_t>(levelFiveFirstSwitchActive_ ? 1 : 0))
        && WriteValue(stream, static_cast<std::uint8_t>(levelFiveSecondSwitchActive_ ? 1 : 0))
        && WriteValue(stream, levelThreeSpawnChance_)
        && WriteValue(stream, levelTwelveSpawnChance_)
        && WriteValue(stream, levelTwelveMaximumGroup_)
        && WriteValue(stream, levelOneSpawnChance_)
        && WriteValue(stream, levelOneMaximumGroup_);
}

bool GameData::LoadGame(const std::filesystem::path& path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) {
        return false;
    }
    return ReadGame(stream);
}

bool GameData::ReadGame(std::istream& stream) {
    char signature[sizeof(kSaveSignature)]{};
    stream.read(signature, sizeof(signature));
    std::uint16_t version{};
    if (!stream || !std::equal(std::begin(signature), std::end(signature),
                               std::begin(kSaveSignature))
        || !ReadValue(stream, version) || version < 2 || version > kSaveVersion) {
        return false;
    }

    GameData restored = *this;
    if (version < 3) {
        restored.levelTwoSequenceState_ = -1;
        restored.levelTwoSequenceMode_ = 1;
    }
    if (version < 4) {
        restored.levelFourCountdown_ = 24;
    }
    if (version < 5) {
        restored.difficulty_ = 1;
    }
    if (version < 11) {
        restored.levelOneTargetHits_.fill(0);
    }
    if (version < 12) {
        restored.casinoStatus_ = 0;
        restored.casinoBetCount_ = 0;
        restored.casinoBetItemType_ = 0xffff;
    }
    if (version < 13) {
        restored.casinoDeck_.fill(0);
        restored.casinoRanks_.fill(0);
        restored.casinoSuits_.fill(0);
        restored.casinoHeld_.fill(0);
        restored.casinoDeckIndex_ = 0;
    }
    if (version < 14) restored.casinoWildRank_ = 0;
    if (version < 15) restored.casinoBonusStage_ = 0;
    if (version < 16) restored.casinoResultTicks_ = 0;
    std::uint16_t levelNumber{};
    std::uint32_t selectedItem{};
    std::uint16_t playerColumn{};
    std::uint16_t playerRow{};
    std::uint8_t facesRight{};
    std::uint8_t northeast{};
    std::uint8_t southwest{};
    std::uint8_t flood{};
    std::uint8_t endPhase{};
    if (!ReadValue(stream, levelNumber) || !ReadValue(stream, selectedItem)
        || !ReadValue(stream, restored.animationFrame_) || !ReadValue(stream, restored.randomState_)
        || !ReadValue(stream, restored.turnCounter_) || !ReadValue(stream, restored.score_)
        || !ReadValue(stream, playerColumn) || !ReadValue(stream, playerRow)
        || !ReadValue(stream, facesRight) || !ReadValue(stream, restored.health_)
        || !ReadValue(stream, restored.repellentEnergy_) || !ReadValue(stream, northeast)
        || !ReadValue(stream, southwest) || !ReadValue(stream, flood)
        || !ReadValue(stream, restored.levelSevenSpawnDivisor_) || !ReadValue(stream, endPhase)) {
        return false;
    }
    if (version >= 3 && (!ReadValue(stream, restored.levelTwoSequenceState_)
        || !ReadValue(stream, restored.levelTwoSequenceMode_))) {
        return false;
    }
    if (restored.levelTwoSequenceMode_ == 4) {
        restored.levelTwoSequenceMode_ = 3;
    }
    if (version >= 4 && !ReadValue(stream, restored.levelFourCountdown_)) {
        return false;
    }
    if (version >= 5 && !ReadValue(stream, restored.difficulty_)) {
        return false;
    }
    if (version >= 7 && (!ReadValue(stream, restored.levelFiveProbeColumn_)
        || !ReadValue(stream, restored.levelFiveProbeRow_)
        || !ReadValue(stream, restored.levelFiveFirstProbeBroken_)
        || !ReadValue(stream, restored.levelFiveSecondProbeBroken_)
        || !ReadValue(stream, restored.levelFiveProbeTransformed_))) {
        return false;
    }
    if (version >= 8 && (!ReadValue(stream, restored.levelSixStructuresSpawned_)
        || !ReadValue(stream, restored.levelSixUpperTransporterTriggered_)
        || !ReadValue(stream, restored.levelSixLowerTransporterTriggered_)
        || !ReadValue(stream, restored.levelSixCenterOpened_))) {
        return false;
    }
    if (version >= 9 && !ReadValue(stream, restored.levelEightAlarmTriggered_)) {
        return false;
    }
    if (version >= 10 && !ReadValue(stream, restored.levelElevenRuntimeFlags_)) {
        return false;
    }
    for (std::size_t index = 0; index < restored.levelSevenStructureActive_.size(); ++index) {
        std::uint8_t active{};
        if (!ReadValue(stream, active) || active > 1) {
            return false;
        }
        restored.levelSevenStructureActive_[index] = active != 0;
    }
    for (std::size_t index = 0; index < restored.discoveredLaws_.size(); ++index) {
        std::uint8_t discovered{};
        if (!ReadValue(stream, discovered) || discovered > 1) {
            return false;
        }
        restored.discoveredLaws_[index] = discovered != 0;
    }
    for (BoardColumn& column : restored.level_) {
        stream.read(reinterpret_cast<char*>(column.data()),
                    static_cast<std::streamsize>(column.size() * sizeof(column[0])));
    }
    if (!stream || !ReadRecords(stream, restored.overlays_) || !ReadRecords(stream, restored.actors_)
        || !ReadRecords(stream, restored.inventory_) || !ReadRecords(stream, restored.projectiles_)) {
        return false;
    }
    if (version >= 6) {
        if (!ReadRecords(stream, restored.discoveredLawOrder_)
            || !ReadValue(stream, restored.lawRecallCursor_)) {
            return false;
        }
    } else {
        restored.discoveredLawOrder_.clear();
        for (std::size_t law = 0; law < restored.discoveredLaws_.size(); ++law) {
            if (restored.discoveredLaws_[law]) {
                restored.discoveredLawOrder_.push_back(static_cast<std::uint8_t>(law));
            }
        }
        restored.lawRecallCursor_ = 0;
    }
    if (version >= 11 && (!ReadValue(stream, restored.levelOneTargetHits_[0])
        || !ReadValue(stream, restored.levelOneTargetHits_[1]))) {
        return false;
    }
    if (version >= 12 && (!ReadValue(stream, restored.casinoStatus_)
        || !ReadValue(stream, restored.casinoBetCount_)
        || !ReadValue(stream, restored.casinoBetItemType_))) {
        return false;
    }
    if (version >= 13 && (!ReadValue(stream, restored.casinoDeck_)
        || !ReadValue(stream, restored.casinoRanks_)
        || !ReadValue(stream, restored.casinoSuits_)
        || !ReadValue(stream, restored.casinoHeld_)
        || !ReadValue(stream, restored.casinoDeckIndex_))) {
        return false;
    }
    if (version >= 14 && !ReadValue(stream, restored.casinoWildRank_)) return false;
    if (version >= 15 && !ReadValue(stream, restored.casinoBonusStage_)) return false;
    if (version >= 16 && !ReadValue(stream, restored.casinoResultTicks_)) return false;
    if (version >= 17 && (!ReadValue(stream, restored.genericSpawnChance_)
        || !ReadValue(stream, restored.genericBreakDivisor_)
        || !ReadValue(stream, restored.genericRuntimeFlags_))) return false;
    if (version >= 18) {
        if (!ReadValue(stream, restored.transientOverlayFrame_)) return false;
    } else {
        restored.transientOverlayFrame_ = std::any_of(
            restored.overlays_.begin(), restored.overlays_.end(), [](const BoardOverlay& overlay) {
                return (overlay.type >= 0x14 && overlay.type <= 0x1d)
                    || overlay.type == 0x1f;
            }) ? 1 : 0;
    }
    if (version >= 19) {
        std::uint8_t arenaEntered{};
        if (!ReadValue(stream, restored.levelOneFirstTargetActivationTurn_)
            || !ReadValue(stream, arenaEntered) || arenaEntered > 1) return false;
        restored.levelOneArenaEntered_ = arenaEntered != 0;
    } else {
        restored.levelOneFirstTargetActivationTurn_ = 0;
        restored.levelOneArenaEntered_ = false;
        if (levelNumber == 1) {
            bool upperTargetWasActivated = false;
            for (std::size_t column = 0; column + 2 < kBoardColumns; ++column) {
                for (std::size_t row = 0; row < 18 && row + 2 < kBoardRows; ++row) {
                    upperTargetWasActivated = upperTargetWasActivated
                        || restored.level_[column][row] == 0x87
                        || restored.level_[column][row] == 0x99;
                }
            }
            if (upperTargetWasActivated) {
                restored.levelOneFirstTargetActivationTurn_ = restored.turnCounter_ > 12
                    ? restored.turnCounter_ - 12 : restored.turnCounter_;
            }
            for (std::size_t column = 10; column <= 15; ++column) {
                restored.levelOneArenaEntered_ = restored.levelOneArenaEntered_
                    || restored.level_[column][8] == 0x22
                    || restored.level_[column][17] == 0x22;
            }
        }
    }
    if (version >= 21) {
        std::uint8_t emeraldClaimed{};
        std::uint8_t barrierOpened{};
        if (!ReadValue(stream, emeraldClaimed) || emeraldClaimed > 1
            || !ReadValue(stream, barrierOpened) || barrierOpened > 1) return false;
        restored.levelTenEmeraldClaimed_ = emeraldClaimed != 0;
        restored.levelTenBarrierOpened_ = barrierOpened != 0;
    } else {
        // Versions through 20 predate the native 1010:37a2 callback. Leave
        // both phases armed so an already-breached old save catches up once.
        restored.levelTenEmeraldClaimed_ = false;
        restored.levelTenBarrierOpened_ = false;
    }
    if (version >= 22) {
        for (std::size_t index = 0; index < restored.levelSixteenChambersActive_.size(); ++index) {
            std::uint8_t active{};
            if (!ReadValue(stream, active) || active > 1) return false;
            restored.levelSixteenChambersActive_[index] = active != 0;
        }
        std::uint8_t coreActivated{};
        if (!ReadValue(stream, coreActivated) || coreActivated > 1) return false;
        restored.levelSixteenCoreActivated_ = coreActivated != 0;
    } else {
        // The 1010:3d50 phases were absent from versions through 21. Keep all
        // sentinels armed so old saves apply any already-earned openings once.
        restored.levelSixteenChambersActive_.fill(true);
        restored.levelSixteenCoreActivated_ = false;
    }
    restored.depthFromSave_ = version >= 23;
    if (version >= 23) {
        if (!ReadValue(stream, restored.boardDepth_) || restored.boardDepth_ > 0x7fff)
            return false;
    } else {
        restored.boardDepth_ = static_cast<std::uint16_t>(levelNumber >= 17 ? levelNumber - 16 : 1);
    }
    for (auto& overlay : restored.overlays_) {
        if (version >= 24) {
            if (!ReadValue(stream, overlay.animationFrame) || overlay.animationFrame > 5
                || (!IsTransientOverlay(overlay) && overlay.animationFrame != 0)) return false;
        } else if (IsTransientOverlay(overlay)) {
            overlay.animationFrame = restored.transientOverlayFrame_ != 0
                ? restored.transientOverlayFrame_ : 1;
        }
    }
    if (version >= 25) {
        std::uint32_t scroll{};
        if (!ReadValue(stream, restored.casinoAnnouncement_) || restored.casinoAnnouncement_ > 5
            || !ReadValue(stream, scroll)
            || scroll > restored.CasinoAnnouncementText().size() + 14) return false;
        restored.casinoScrollOffset_ = scroll;
    }
    if (version >= 26) {
        if (!ReadValue(stream, restored.levelOneTargetColumns_)
            || !ReadValue(stream, restored.levelOneTargetRows_)
            || !ReadValue(stream, restored.levelOneTargetDefeated_)) return false;
        for (std::size_t id = 0; id < 2; ++id)
            if (restored.levelOneTargetColumns_[id] > 37 || restored.levelOneTargetRows_[id] > 33
                || restored.levelOneTargetDefeated_[id] > 1)
                return false;
        if (restored.levelOneTargetColumns_[0] == restored.levelOneTargetColumns_[1]
            && restored.levelOneTargetRows_[0] == restored.levelOneTargetRows_[1]
            && !restored.levelOneTargetDefeated_[0] && !restored.levelOneTargetDefeated_[1]) return false;
    } else {
        // Older saves contain only half-board damage counters. Preserve these
        // as best possible; historical crossed-target identity is unrecoverable.
        restored.levelOneTargetColumns_ = {36, 36};
        restored.levelOneTargetRows_ = {1, 32};
        restored.levelOneTargetDefeated_.fill(0);
    }
    if (version >= 27) {
        std::uint8_t security{}, corners{};
        if (!ReadValue(stream, security) || !ReadValue(stream, corners)
            || security > 1 || corners > 1) return false;
        restored.levelOneSecurityGateOpened_ = security != 0;
        restored.levelOneCornerPortalsActivated_ = corners != 0;
    } else {
        // Previous native builds activated both events on the first turn.
        // Preserve that saved board state without replaying their actor release.
        const bool oldFirstTurn = levelNumber == 1 && restored.turnCounter_ > 1;
        restored.levelOneSecurityGateOpened_ = oldFirstTurn;
        restored.levelOneCornerPortalsActivated_ = oldFirstTurn;
    }
    if (version >= 28) {
        std::uint8_t destroyed{};
        if (!ReadValue(stream, destroyed) || destroyed > 1) return false;
        restored.energySphereDestroyed_ = destroyed != 0;
    } else {
        restored.energySphereDestroyed_ = levelNumber == 1
            && restored.level_[12][12] == 0;
    }
    if (version >= 29) {
        if (!ReadValue(stream, restored.casinoTopChestCount_)
            || restored.casinoTopChestCount_ > 7) return false;
    } else {
        // Older native saves did not retain the top-edge chest observation.
        // Start from the saved terrain so they do not trigger false defenses.
        restored.casinoTopChestCount_ = 0;
        if (levelNumber == 2)
            for (int column = 11; column < 30; column += 3)
                restored.casinoTopChestCount_ += restored.level_[column][0] == 0x7f;
    }
    if (version >= 30) {
        std::uint8_t first{}, second{};
        if (!ReadValue(stream, first) || !ReadValue(stream, second)
            || first > 1 || second > 1) return false;
        restored.levelFiveFirstSwitchActive_ = first != 0;
        restored.levelFiveSecondSwitchActive_ = second != 0;
    } else if (levelNumber == 5) {
        // Old native saves omitted the switch words. Infer a best-effort state
        // from the intact tunnel edge; a destroyed edge remains ambiguous.
        restored.levelFiveFirstSwitchActive_ = restored.level_[31][3] == 0x36;
        restored.levelFiveSecondSwitchActive_ = restored.level_[34][6] == 0x37;
    }
    if (version >= 31) {
        if (!ReadValue(stream, restored.levelThreeSpawnChance_)
            || restored.levelThreeSpawnChance_ > 75
            || !ReadValue(stream, restored.levelTwelveSpawnChance_)
            || restored.levelTwelveSpawnChance_ > 12
            || !ReadValue(stream, restored.levelTwelveMaximumGroup_)
            || restored.levelTwelveMaximumGroup_ < 1
            || restored.levelTwelveMaximumGroup_ > 3) return false;
    } else {
        // Earlier native saves reconstructed this parameter from elapsed
        // turns, so historical per-placement increases cannot be recovered.
        restored.levelThreeSpawnChance_ = static_cast<std::uint16_t>(
            restored.difficulty_ + (restored.turnCounter_ <= 6 ? 24 : 4));
        restored.levelTwelveSpawnChance_ = 0;
        restored.levelTwelveMaximumGroup_ = 1;
        if (levelNumber == 12) restored.RefreshFinalArenaSpawnParameters();
    }
    if (version >= 32) {
        if (!ReadValue(stream, restored.levelOneSpawnChance_)
            || restored.levelOneSpawnChance_ > 13
            || !ReadValue(stream, restored.levelOneMaximumGroup_)
            || restored.levelOneMaximumGroup_ < 1
            || restored.levelOneMaximumGroup_ > 9) return false;
    } else {
        // Older saves did not preserve the order of the three one-shot gates.
        restored.levelOneSpawnChance_ = static_cast<std::uint16_t>(restored.levelOneArenaEntered_
            ? restored.difficulty_ * 2 + 3
            : restored.levelOneSecurityGateOpened_ ? restored.difficulty_ + 9 : 0);
        restored.levelOneMaximumGroup_ = static_cast<std::uint16_t>(restored.levelOneArenaEntered_ ? 1
            : restored.levelOneCornerPortalsActivated_ ? restored.difficulty_ + 5
            : restored.levelOneSecurityGateOpened_ ? std::max(restored.difficulty_ + 2, 4) : 1);
    }
    if (levelNumber < 1 || levelNumber > 25 || playerColumn >= kBoardColumns
        || playerRow >= kBoardRows || facesRight > 1 || northeast > 1 || southwest > 1
        || flood > 1 || endPhase > 1 || restored.health_ < 0 || restored.health_ > 0x7fff
        || (restored.levelTwoSequenceState_ != -1
            && (restored.levelTwoSequenceState_ < 0 || restored.levelTwoSequenceState_ > 106))
        || restored.levelTwoSequenceMode_ < 1 || restored.levelTwoSequenceMode_ > 3
        || restored.levelFourCountdown_ > 24
        || restored.levelOneFirstTargetActivationTurn_ > restored.turnCounter_
        || restored.difficulty_ < 1 || restored.difficulty_ > 4
        || restored.levelFiveProbeColumn_ + 1 >= kBoardColumns
        || restored.levelFiveProbeRow_ >= kBoardRows
        || (restored.levelElevenRuntimeFlags_ >> 4u) > 4
        || restored.casinoStatus_ > 4 || restored.casinoBetCount_ > 20
        || (restored.casinoBetCount_ == 0 && restored.casinoBetItemType_ != 0xffff)
        || (restored.casinoBetCount_ != 0 && restored.casinoBetItemType_ >= 22)
        || restored.casinoDeckIndex_ > restored.casinoDeck_.size()
        || (restored.casinoWildRank_ != 0 && restored.casinoWildRank_ != 2
            && restored.casinoWildRank_ != 7)
        || restored.casinoBonusStage_ > 3
        || restored.casinoResultTicks_ > 101
        || restored.genericSpawnChance_ < 0 || restored.genericSpawnChance_ > 100
        || restored.genericBreakDivisor_ < 1 || restored.genericBreakDivisor_ > 120
        || restored.transientOverlayFrame_ > 5
        || (restored.genericRuntimeFlags_ & 0xc0u) != 0
        || (restored.casinoStatus_ != 3 && restored.casinoResultTicks_ != 0)
        || restored.lawRecallCursor_ > restored.discoveredLawOrder_.size()
        || (restored.inventory_.empty() ? selectedItem != 0
                                        : selectedItem >= restored.inventory_.size())) {
        return false;
    }
    if (version >= 13) {
        std::array<bool, 52> savedCards{};
        for (const std::uint16_t card : restored.casinoDeck_) {
            if (card >= savedCards.size() || (restored.casinoStatus_ >= 2 && savedCards[card])) {
                return false;
            }
            savedCards[card] = true;
        }
        for (std::size_t card = 0; card < restored.casinoRanks_.size(); ++card) {
            if (restored.casinoHeld_[card] > 1
                || (restored.casinoStatus_ >= 2
                    && (restored.casinoRanks_[card] < 1 || restored.casinoRanks_[card] > 13
                        || restored.casinoSuits_[card] < 1
                        || restored.casinoSuits_[card] > 4))) {
                return false;
            }
        }
        if (restored.casinoStatus_ >= 2 && restored.casinoDeckIndex_ < 5) return false;
    }
    std::array<bool, 47> orderedLaws{};
    for (const std::uint8_t law : restored.discoveredLawOrder_) {
        if (law >= orderedLaws.size() || orderedLaws[law]) {
            return false;
        }
        orderedLaws[law] = true;
    }
    if (orderedLaws != restored.discoveredLaws_) {
        return false;
    }
    for (const BoardColumn& column : restored.level_) {
        if (std::any_of(column.begin(), column.end(), [&](const std::uint16_t tile) {
                return tile >= restored.tiles_.size();
            })) {
            return false;
        }
    }
    const auto validCoordinate = [](const auto& record) {
        return record.column < kBoardColumns && record.row < kBoardRows;
    };
    const auto validOverlayCoordinate = [&](const BoardOverlay& overlay) {
        if (validCoordinate(overlay)) return true;
        // 1010:4f76 can place pickups just below the Gauntlet board. Keep
        // these unreachable records rather than changing their positions.
        return levelNumber == 4 && (overlay.type == 1 || overlay.type == 2)
            && overlay.column >= 11 && overlay.column <= 34
            && overlay.row >= kBoardRows && overlay.row <= 37;
    };
    if (!std::all_of(restored.overlays_.begin(), restored.overlays_.end(), validOverlayCoordinate)
        || !std::all_of(restored.actors_.begin(), restored.actors_.end(), validCoordinate)
        || !std::all_of(restored.projectiles_.begin(), restored.projectiles_.end(),
                        [](const Projectile& projectile) {
                            return projectile.column >= 0 && projectile.column < static_cast<int>(kBoardColumns)
                                && projectile.row >= 0 && projectile.row < static_cast<int>(kBoardRows);
                        })) {
        return false;
    }
    restored.levelNumber_ = levelNumber;
    if (version < 26) restored.ReconcileLevelOneTargets();
    if (version < 26 && levelNumber >= 17) {
        // Repair old turn-six/depth-six spawn values without regenerating any
        // saved terrain, inventory, actors, or completed escalation events.
        const int depth = levelNumber - 16;
        int chance = (restored.difficulty_ + 5) * 3;
        if (restored.turnCounter_ > 6)
            chance = std::min(restored.difficulty_ + 5, depth + restored.difficulty_ - 1);
        if (restored.genericRuntimeFlags_ & 1u) chance += 2;
        if (restored.genericRuntimeFlags_ & 2u) chance *= 2;
        if (restored.genericRuntimeFlags_ & 4u) chance = 25;
        if (restored.genericRuntimeFlags_ & 8u) chance = 40;
        restored.genericSpawnChance_ = chance;
    }
    restored.selectedItem_ = selectedItem;
    restored.firstVisibleItem_ = selectedItem > 4 ? selectedItem - 4 : 0;
    restored.playerColumn_ = playerColumn;
    restored.playerRow_ = playerRow;
    // DAH64 versions 2-19 accidentally stored vertical facing in this byte.
    // The original +0xee field is horizontal; old native saves safely resume
    // facing right because their byte cannot reconstruct that lost state.
    restored.playerFacesRight_ = version >= 20 ? facesRight != 0 : true;
    restored.levelFourNortheastTriggered_ = northeast != 0;
    restored.levelFourSouthwestTriggered_ = southwest != 0;
    restored.levelFourFloodTriggered_ = flood != 0;
    restored.levelSevenEndPhase_ = endPhase != 0;
    restored.pendingSound_ = GameplaySound::None;
    restored.pendingMessages_.clear();
    restored.pendingDialogMessage_.clear();
    restored.playerAttackFrame_ = 0;
    restored.terrainAnimation_.assign(kBoardColumns, {});
    if (version < 25) restored.SetCasinoAnnouncement(restored.casinoStatus_ == 4 ? 3 :
        restored.casinoStatus_ == 2 ? 1 : 0);
    *this = std::move(restored);
    return true;
}

void GameData::AdoptPlayerStateFrom(const GameData& source, bool keepConstructedRandomState) {
    inventory_ = source.inventory_;
    selectedItem_ = source.selectedItem_;
    firstVisibleItem_ = source.firstVisibleItem_;
    animationFrame_ = source.animationFrame_;
    if (!keepConstructedRandomState) randomState_ = source.randomState_;
    score_ = source.score_;
    difficulty_ = source.difficulty_;
    playerFacesRight_ = source.playerFacesRight_;
    health_ = source.health_;
    repellentEnergy_ = source.repellentEnergy_;
    discoveredLaws_ = source.discoveredLaws_;
    discoveredLawOrder_ = source.discoveredLawOrder_;
    lawRecallCursor_ = source.lawRecallCursor_;
    // FUN_1008_2b54's two global one-shot factory flags belong to the player
    // graph, while the low nibble remains local procedural-runtime state.
    genericRuntimeFlags_ = static_cast<std::uint8_t>(
        (genericRuntimeFlags_ & 0x0fu) | (source.genericRuntimeFlags_ & 0x30u));
    energySphereDestroyed_ = source.energySphereDestroyed_;
    pendingSound_ = GameplaySound::None;
    // Original notices belong to the player UI, not to the departing board.
    pendingMessages_ = source.pendingMessages_;
    pendingDialogMessage_ = source.pendingDialogMessage_;
    pendingTransition_ = 0;
}

bool GameData::PositionPlayerAtTile(std::uint16_t tile) noexcept {
    for (std::size_t column = 0; column < kBoardColumns; ++column) {
        for (std::size_t row = 0; row < kBoardRows; ++row) {
            if (level_[column][row] == tile) {
                playerColumn_ = column;
                playerRow_ = row;
                return true;
            }
        }
    }
    return false;
}

int GameData::TakeTransitionRequest() noexcept {
    const int request = pendingTransition_;
    pendingTransition_ = 0;
    return request;
}

void GameData::AdvanceAnimation() noexcept {
    AdvanceRandomAnimation();
    ++animationFrame_;
    if (levelNumber_ == 2) {
        // 26fe advances a thirteen-character window, including blank lead-in
        // and trailing space. Paint itself stays free of state changes.
        if (++casinoScrollOffset_ > CasinoAnnouncementText().size() + 14)
            casinoScrollOffset_ = 0;
    }
    // FUN_1008_8b2c/FUN_1008_8be6 painted five short frames and then removed
    // transient nodes 0x14..0x1d and 0x1f. The Win64 shell advances the same
    // sequence from WM_TIMER instead of blocking the message loop.
    std::erase_if(overlays_, [](BoardOverlay& overlay) {
        if (!IsTransientOverlay(overlay)) return false;
        if (overlay.animationFrame == 5) return true;
        ++overlay.animationFrame;
        return false;
    });
    transientOverlayFrame_ = 0;
    for (const auto& overlay : overlays_)
        if (IsTransientOverlay(overlay))
            transientOverlayFrame_ = std::max(transientOverlayFrame_, overlay.animationFrame);
    // FUN_1010_26fe leaves a resolved casino hand visible for 102 paint
    // callbacks, then invokes FUN_1010_1c1e to prepare a fresh table.
    if (levelNumber_ == 2 && casinoStatus_ == 3) {
        if (++casinoResultTicks_ > 101) ResetCasinoRound();
    }
}

void GameData::AdvanceRandomAnimation() noexcept {
    // 9680: terrain column-major, actor insertion order, overlay head-first.
    // Cache incremental randomized updates; WM_PAINT never rolls the LCG.
    const auto tick = animationFrame_ % 120;
    std::array<std::array<bool, kBoardRows>, kBoardColumns> occupied{};
    for (const auto& actor : actors_)
        if (actor.column < kBoardColumns && actor.row < kBoardRows)
            occupied[actor.column][actor.row] = true;
    for (const auto& shot : projectiles_)
        if (shot.column >= 0 && shot.column < static_cast<int>(kBoardColumns)
            && shot.row >= 0 && shot.row < static_cast<int>(kBoardRows))
            occupied[shot.column][shot.row] = true;
    for (std::size_t column = 0; column < kBoardColumns; ++column) {
        for (std::size_t row = 0; row < kBoardRows; ++row) {
            const auto tile = level_[column][row];
            auto& visual = terrainAnimation_[column][row];
            if (visual.tile != tile) visual = {tile, tiles_[tile].sprite};
            if (tile == 0 || (tile > 2 && tile < 8) || occupied[column][row]
                || (column == playerColumn_ && row == playerRow_)) continue;
            if (tile >= 0x18 && tile <= 0x22) {
                if (!energySphereDestroyed_)
                    visual.sprite = TerrainSprite(tile, visual.sprite, tick);
            } else if (tile == 0xaf && tick % 2 == 0) {
                visual.sprite = static_cast<std::uint16_t>(0x1d2 + NextRandom(2));
            } else if (tile >= 0x83 && tile <= 0x86) {
                visual.sprite = static_cast<std::uint16_t>(tile + (NextRandom(8) == 1 ? 0x103 : 0xff));
            } else if (tile >= 0xa2 && tile <= 0xaa && NextRandom(3) == 1) {
                const int offset = tile - 0xa2;
                visual.sprite = static_cast<std::uint16_t>(tile + (NextRandom(2) == 1 ? 0xa6
                    : offset < 2 ? 0xa2 : offset == 3 || offset == 4 ? 0xa1 : 0x99));
            }
        }
    }
    for (auto& actor : actors_) {
        if (actor.visualType != actor.type) {
            actor.visualType = actor.type;
            actor.visualSprite = actor.type == 2 ? 7 : actor.type == 3 ? 0x10 : 0xffff;
        }
        if (actor.type == 2) {
            const auto phase = tick % 15;
            if (phase == 0) actor.visualSprite = 7;
            else if (phase == 4 || phase == 6 || phase == 9 || phase == 11) {
                if (NextRandom(2) == 2)
                    actor.visualSprite = phase == 4 ? 8 : phase == 9 ? 9 : 7;
            }
        } else if (actor.type == 3) {
            if (tick % 6 == 0) actor.visualSprite = 0x10;
            else if (tick % 6 == 4 && NextRandom(2) == 2) actor.visualSprite = 0x11;
        }
    }
    for (auto node = overlays_.rbegin(); node != overlays_.rend(); ++node) {
        auto& overlay = *node;
        if (overlay.type != 1 || overlay.column >= kBoardColumns || overlay.row >= kBoardRows
            || occupied[overlay.column][overlay.row]
            || (overlay.column == playerColumn_ && overlay.row == playerRow_)
            || (tiles_[level_[overlay.column][overlay.row]].flags & 1u) == 0) continue;
        if (tick % 6 == 0) overlay.visualSprite = 0x46;
        else if ((tick % 6 == 4 || tick % 6 == 5) && NextRandom(3) == 1)
            overlay.visualSprite = tick % 6 == 4 ? 0x47 : 0x48;
    }
}

bool GameData::MovePlayer(int deltaColumn, int deltaRow) noexcept {
    pendingSound_ = GameplaySound::None;
    pendingMessages_.clear();
    pendingDialogMessage_.clear();
    // FUN_1008_1b22 updates vertical facing before validating the action, so
    // even a blocked vertical command turns the vessel.
    if (deltaColumn != 0) playerFacesRight_ = deltaColumn > 0;
    if (health_ == 0) {
        return false;
    }
    fatalHitLatched_ = false;
    int destinationColumn = static_cast<int>(playerColumn_) + deltaColumn;
    int destinationRow = static_cast<int>(playerRow_) + deltaRow;
    if (destinationColumn < 0 || destinationRow < 0 ||
        destinationColumn >= static_cast<int>(kBoardColumns) ||
        destinationRow >= static_cast<int>(kBoardRows)) {
        return false;
    }
    const std::uint16_t requestedTile = level_[static_cast<std::size_t>(destinationColumn)]
                                              [static_cast<std::size_t>(destinationRow)];
    if (requestedTile >= tiles_.size() || (tiles_[requestedTile].flags & 1u) == 0)
        return false;
    // 1b22 advances projectiles after validating the requested destination,
    // but before the casino's pre-movement deflection callback.
    AdvanceProjectiles();
    // Selector 1040 overrides the pre-movement vtable slot with FUN_1010_22c6.
    // Outside difficulty three, its horizontal force line prevents northward
    // entry into row three for columns 10..31. A diagonal command retains its
    // horizontal component when that square remains passable.
    if (levelNumber_ == 2 && difficulty_ != 3 && playerColumn_ > 9 && playerColumn_ < 32
        && destinationRow == 3 && deltaRow < 0) {
        pendingMessages_.emplace_back(L"A strange force deflects the Rubble Roadster.");
        deltaRow = 0;
        destinationRow = static_cast<int>(playerRow_);
        const std::uint16_t horizontalTile =
            level_[static_cast<std::size_t>(destinationColumn)]
                  [static_cast<std::size_t>(destinationRow)];
        if (horizontalTile >= tiles_.size()
            || (tiles_[horizontalTile].flags & 1u) == 0) {
            deltaColumn = 0;
            destinationColumn = static_cast<int>(playerColumn_);
        }
    }
    playerColumn_ = static_cast<std::size_t>(destinationColumn);
    playerRow_ = static_cast<std::size_t>(destinationRow);
    ResolveActorCollision();
    AdvanceBoardRuntime(true, deltaColumn, deltaRow, true);
    SpawnStructureActors();
    AdvanceActors();
    // 1008:1b22 dispatches a0ca only for actual movement, not Wait.
    if (deltaColumn != 0 || deltaRow != 0) ResolveOverlaysAtPlayer();
    AdvanceBoardRuntime(true, deltaColumn, deltaRow, false);
    // Every concrete +0x1c board callback ends in FUN_1008_ba30. An actual
    // move onto 0x23..0x26 dispatches +0x18 (up/parent); 0x27..0x2a
    // dispatches +0x14 (down/forward).
    if (deltaColumn != 0 || deltaRow != 0) DetectBoardTransition();
    // FUN_1008_ba30 invokes vtable +0x34 when an actual movement lands on a
    // signpost. The short preface enters FUN_1008_55e0's review stream;
    // the concrete board callback displays its sign through FUN_1008_67e6.
    if ((deltaColumn != 0 || deltaRow != 0)
        && level_[playerColumn_][playerRow_] == 0x7e) {
        pendingMessages_.push_back(L"You see here a signpost.\n\nIt reads:");
        const std::wstring_view sign = SignpostText();
        if (!sign.empty()) pendingMessages_.emplace_back(sign);
    }
    AdvanceRepellent();
    ArmTransientOverlayAnimation();
    return true;
}

void GameData::RefreshFinalArenaSpawnParameters() noexcept {
    // Raw 1010:d71e stores +0x4a/+0x4c after the common actor turn. The
    // center scan visits 120 sites, and +0x4c changes only below ten.
    int surviving = 0;
    for (int column = 3; column <= 36; column += 3)
        for (int row = 2; row <= 29; row += 3) {
            const auto tile = level_[column][row];
            surviving += tile == 0x18 || tile == 0x19;
        }
    if (surviving < 10) {
        levelTwelveSpawnChance_ = static_cast<std::uint16_t>(difficulty_ + 8);
        levelTwelveMaximumGroup_ = static_cast<std::uint16_t>(std::min(difficulty_ + 1, 3));
    } else if (surviving < 20) {
        levelTwelveSpawnChance_ = static_cast<std::uint16_t>(difficulty_ + 4);
    } else if (surviving < 30) {
        levelTwelveSpawnChance_ = 4;
    } else if (surviving < 60) {
        levelTwelveSpawnChance_ = 3;
    } else if (surviving < 100) {
        levelTwelveSpawnChance_ = 2;
    } else if (surviving < 120) {
        levelTwelveSpawnChance_ = 1;
    }
    // At 120 surviving centers the constructor's initial zero remains.
}

std::pair<int, int> GameData::StructureSpawnParameters() const noexcept {
    // FUN_1008_81dc gives every surviving 0x18..0x22 structure one spawn roll per
    // accepted turn.  Empty machinery (0x1e/0x1f) deliberately produces nothing.
    int spawnChance = 10;
    int maximumGroup = 1;
    if (levelNumber_ == 1) {
        // e636 writes these words in event order, including late breaches.
        return {levelOneSpawnChance_, levelOneMaximumGroup_};
    } else if (levelNumber_ == 2) {
        spawnChance = 4;
    } else if (levelNumber_ == 3) {
        // FUN_1010_4044 seeds offsets 0x4a/0x4c with difficulty+24 and
        // difficulty. 4692 lowers this stored value after turn six; later
        // replenishments can raise it independently, up to 75.
        spawnChance = levelThreeSpawnChance_;
        maximumGroup = difficulty_;
    } else if (levelNumber_ == 4) {
        spawnChance = (difficulty_ + 3) * 2;
    } else if (levelNumber_ == 5) {
        spawnChance = difficulty_ + 10;
    } else if (levelNumber_ == 6) {
        // FUN_1010_5fc4 initializes words 0x4a/0x4c to
        // (difficulty + 6) * 2 and 3. FUN_1010_6308 raises 0x4a to 50
        // after its late three-structure event.
        spawnChance = levelSixStructuresSpawned_ ? 50 : (difficulty_ + 6) * 2;
        maximumGroup = 3;
    } else if (levelNumber_ == 7) {
        spawnChance = levelSevenSpawnDivisor_ == 0 ? 25 : 3;
        maximumGroup = 3;
    } else if (levelNumber_ == 8) {
        spawnChance = levelEightAlarmTriggered_ ? 10 : 6;
        maximumGroup = levelEightAlarmTriggered_ ? 4 : 1;
    } else if (levelNumber_ == 9) {
        spawnChance = 4;
    } else if (levelNumber_ == 10) {
        spawnChance = difficulty_ * 3;
        maximumGroup = difficulty_ + 2;
    } else if (levelNumber_ == 11) {
        spawnChance = boardDepth_ / 2;
        maximumGroup = difficulty_;
        if ((levelElevenRuntimeFlags_ & 0x01u) != 0) spawnChance += 2;
        if ((levelElevenRuntimeFlags_ & 0x02u) != 0) spawnChance *= 2;
        if ((levelElevenRuntimeFlags_ & 0x04u) != 0) spawnChance = 25;
        if ((levelElevenRuntimeFlags_ & 0x08u) != 0) spawnChance = 50;
    } else if (levelNumber_ == 12) {
        spawnChance = levelTwelveSpawnChance_;
        maximumGroup = levelTwelveMaximumGroup_;
    } else if (levelNumber_ >= 13 && levelNumber_ <= 15) {
        spawnChance = (difficulty_ + static_cast<int>(levelNumber_ - 12)) * 2;
    } else if (levelNumber_ == 16) {
        spawnChance = (difficulty_ + 2) * 2;
    } else if (levelNumber_ >= 17 && levelNumber_ <= 25) {
        spawnChance = genericSpawnChance_;
        maximumGroup = difficulty_;
    }

    return {spawnChance, maximumGroup};
}

void GameData::SpawnStructureActors() noexcept {
    if (energySphereDestroyed_) return;
    const auto [spawnChance, maximumGroup] = StructureSpawnParameters();
    for (std::size_t column = 0; column < kBoardColumns; ++column) {
        for (std::size_t row = 0; row < kBoardRows; ++row) {
            const std::uint16_t tile = level_[column][row];
            if (tile < 0x18 || tile > 0x22) {
                continue;
            }
            int effectiveChance = spawnChance;
            if (tile == 0x21) {
                effectiveChance = 0;
            } else if (tile == 0x22) {
                effectiveChance *= 4;
            }
            if (actors_.size() >= 300 || NextRandom(100) > effectiveChance) {
                continue;
            }
            if (tile == 0x1e || tile == 0x1f || tile == 0x21) {
                continue;
            }

            std::uint16_t actorType = 4;
            if (tile != 0x20 && tile != 0x22) {
                const int primaryRoll = NextRandom(10);
                if (tile == 0x18 || tile == 0x19) {
                    actorType = primaryRoll < 9 ? 0 : 1;
                } else if (primaryRoll > 4) {
                    actorType = NextRandom(10) < 9 ? 0 : 1;
                } else if (tile == 0x1a || tile == 0x1b || NextRandom(10) > 3) {
                    actorType = 2;
                } else {
                    actorType = 3;
                }
            }
            const int group = NextRandom(maximumGroup);
            for (int index = 0; index < group && actors_.size() < 300; ++index) {
                actors_.push_back({actorType, static_cast<std::uint16_t>(column),
                                   static_cast<std::uint16_t>(row), 0, 0, 0});
            }
        }
    }
}

bool GameData::ActivateTransporter(bool upward) noexcept {
    if (health_ == 0 || pendingTransition_ != 0) {
        return false;
    }
    const std::uint16_t tile = level_[playerColumn_][playerRow_];
    if (upward ? (tile < 0x23 || tile > 0x26) : (tile < 0x27 || tile > 0x2a)) {
        return false;
    }
    DetectBoardTransition();
    return pendingTransition_ != 0;
}

GameplaySound GameData::TakeSoundEvent() noexcept {
    const GameplaySound event = pendingSound_;
    pendingSound_ = GameplaySound::None;
    return event;
}

void GameData::QueueSound(GameplaySound sound) noexcept {
    if (sound == GameplaySound::None) return;

    // FUN_1008_31d4 keeps one sound for the turn. Three event families promote
    // repeated calls to their pre-mixed two/three-event resources before the
    // recovered numeric priority comparison is made.
    if (sound == GameplaySound::Pop) {
        if (pendingSound_ == GameplaySound::Pop) sound = GameplaySound::DoublePop;
        else if (pendingSound_ == GameplaySound::DoublePop) sound = GameplaySound::TriplePop;
    } else if (sound == GameplaySound::DeadXoid) {
        if (pendingSound_ == GameplaySound::DeadXoid) sound = GameplaySound::DeadXoidTwo;
        else if (pendingSound_ == GameplaySound::DeadXoidTwo)
            sound = GameplaySound::DeadXoidThree;
    } else if (sound == GameplaySound::PlayerHit
               && pendingSound_ == GameplaySound::PlayerHit) {
        sound = GameplaySound::PlayerHitTwice;
    }
    if (pendingSound_ == GameplaySound::None
        || static_cast<int>(pendingSound_) < static_cast<int>(sound)) {
        pendingSound_ = sound;
    }
}

std::wstring GameData::TakeMessage() {
    if (pendingMessages_.empty()) return {};
    std::wstring message = std::move(pendingMessages_.front());
    pendingMessages_.erase(pendingMessages_.begin());
    return message;
}

std::vector<std::wstring> GameData::TakeMessages() {
    std::vector<std::wstring> messages = std::move(pendingMessages_);
    pendingMessages_.clear();
    return messages;
}

std::wstring GameData::TakeDialogMessage() {
    std::wstring message = std::move(pendingDialogMessage_);
    pendingDialogMessage_.clear();
    return message;
}

std::uint16_t GameData::PlayerSpriteIndex(bool shiftHeld) const noexcept {
    // Exact state branches of FUN_1008_2d50. Root death has a three-frame
    // wreck cycle; Shift uses a four-frame attack pose that advances on paint
    // and remains on its last frame until the key is released.
    if (health_ == 0 && terminalDeathEnabled_) {
        return static_cast<std::uint16_t>(0x3e + animationFrame_ % 3);
    }
    if (shiftHeld) {
        const std::uint16_t sprite = static_cast<std::uint16_t>(
            (playerFacesRight_ ? 0x166 : 0x162) + playerAttackFrame_);
        if (playerAttackFrame_ < 3) ++playerAttackFrame_;
        return sprite;
    }
    playerAttackFrame_ = 0;
    const std::uint16_t repellentOffset = repellentEnergy_ > 0 ? 0x18 : 0;
    return static_cast<std::uint16_t>((playerFacesRight_ ? 0x152 : 0x15a)
        + repellentOffset + animationFrame_ % 8);
}

std::size_t GameData::DiscoveredLawCount() const noexcept {
    return static_cast<std::size_t>(std::count(discoveredLaws_.begin(), discoveredLaws_.end(), true));
}

bool GameData::RecallDiscoveredLaw() noexcept {
    if (discoveredLawOrder_.empty()) {
        pendingMessages_.emplace_back(L"You haven't read any fortunes yet.");
        return true;
    }
    if (lawRecallCursor_ == 0 || lawRecallCursor_ > discoveredLawOrder_.size()) {
        lawRecallCursor_ = static_cast<std::uint32_t>(discoveredLawOrder_.size());
    }
    --lawRecallCursor_;
    pendingMessages_.emplace_back(L"#" + std::to_wstring(lawRecallCursor_ + 1)
        + L" - " + std::wstring(kLaws[discoveredLawOrder_[lawRecallCursor_]]));
    return true;
}

std::uint16_t GameData::TileAt(std::size_t column, std::size_t row) const {
    if (column >= kBoardColumns || row >= kBoardRows) {
        throw std::out_of_range("board coordinate");
    }
    return level_[column][row];
}

std::wstring GameData::IdentifySquare(std::size_t column, std::size_t row) const {
    // FUN_1008_60f4 shares FUN_1008_6048's null-parent gate.
    if (column >= kBoardColumns || row >= kBoardRows) return {};
    if (column == playerColumn_ && row == playerRow_)
        return L"The\nEffer-vessel.\n\n(You, bonehead.)";
    for (const BoardActor& actor : actors_) {
        if (actor.column != column || actor.row != row) continue;
        constexpr std::wstring_view names[] = {
            L"Sewer Squid", L"Bulldozer", L"Sewer Demon\n\nLieutenant of Gorloth",
            L"Sewer Demon\n\nCaptain of Gorloth",
            L"Xoid.\n\nPrimitive Dwellers of the Drain."};
        return actor.type < std::size(names) ? std::wstring(names[actor.type]) : L"???";
    }
    for (const Projectile& projectile : projectiles_) {
        if (projectile.column != static_cast<int>(column)
            || projectile.row != static_cast<int>(row)) continue;
        if (projectile.type == 1 && projectile.deltaColumn == 0 && projectile.deltaRow == 0) {
            const std::size_t mineCount = static_cast<std::size_t>(std::count_if(
                projectiles_.begin(), projectiles_.end(), [&](const Projectile& candidate) {
                    return candidate.type == 1 && candidate.column == projectile.column
                        && candidate.row == projectile.row && candidate.deltaColumn == 0
                        && candidate.deltaRow == 0;
                }));
            return L"Mines (" + std::to_wstring(mineCount) + L").";
        }
        constexpr std::wstring_view names[] = {
            L"Bubble", L"Bomb", L"Missile",
            L"Radio-Controlled Bomb\n\nUse detonator to explode.", L"Fireball"};
        return projectile.type < std::size(names)
            ? std::wstring(names[projectile.type]) : L"???";
    }
    // Selector 1038 overrides the square-description callback at 1010:0b4e.
    // The labels follow the tracked 3x3 target rectangles rather than their
    // current animation tile, so all nine squares identify as one object.
    if (levelNumber_ == 1) {
        for (std::size_t id = 0; id < 2; ++id) {
            if (levelOneTargetDefeated_[id]) continue;
            const int left = levelOneTargetColumns_[id];
            const int top = levelOneTargetRows_[id];
            if (static_cast<int>(column) >= left && static_cast<int>(column) <= left + 2
                && static_cast<int>(row) >= top && static_cast<int>(row) <= top + 2)
                return id == 0 ? L"Gormaug\nNecromancer\n\n\nGorloth's vile roommate."
                    : L"Gorloth\nDemon Lord";
        }
    }
    const std::uint16_t tile = level_[column][row];
    const std::uint16_t flags = tile < tiles_.size() ? tiles_[tile].flags : 0;
    // FUN_1008_60f4 only traverses overlay nodes when descriptor bit one is
    // clear. Intrinsically identifiable terrain (for example Crud under a
    // generated pickup) takes precedence over the node drawn on its square.
    if ((flags & 2u) == 0) {
        for (auto node = overlays_.rbegin(); node != overlays_.rend(); ++node) {
            const BoardOverlay& overlay = *node;
            if (overlay.column != column || overlay.row != row) continue;
            switch (overlay.type) {
            case 4: return L"Moon Key";
            case 0x0e: return L"Smart Card";
            case 0x2b: {
                if (levelNumber_ == 2 && row == 23 && column >= 18 && column <= 30
                    && column % 3 == 0) {
                    const std::size_t card = column / 3 - 6;
                    if (card < casinoHeld_.size()) {
                        return casinoHeld_[card] != 0 ? L"Card is Held" : L"Card is Unheld";
                    }
                }
                return L"Hold/Unhold Card Button";
            }
            case 0x2c: case 0x2d: return L"Lever.";
            case 0x26: case 0x27: case 0x28: case 0x29: return L"Betting Square.";
            default:
                if (overlay.type > 0 && overlay.type < 0x12) return L"Treasure";
                break;
            }
        }
    }
    if ((flags & 1u) == 0 && (flags & 2u) == 0) return {};
    if (tile == 0) return L"Empty";
    if (levelNumber_ == 2 && tile == 2) return L"Poker Table.";
    if (tile == 3) return L"Crud.";
    if (tile == 4) return L"Bulldozer trapped in bubble.";
    if (tile == 5 || tile == 6) return L"Sewer Squid trapped in bubble.";
    if (tile == 7) return L"Decoy";
    if ((tile >= 8 && tile <= 0x0f) || tile == 0x18 || tile == 0x1a
        || tile == 0x1c || tile == 0x1e || (tile >= 0x20 && tile <= 0x22))
        return L"Monster portal";
    if ((tile >= 0x10 && tile <= 0x17) || tile == 0x19 || tile == 0x1b
        || tile == 0x1d || tile == 0x1f) return L"Monster portal (Reinforced)";
    if (tile >= 0x23 && tile <= 0x26) return L"Drain from next level up.";
    if (tile >= 0x27 && tile <= 0x2a) return L"Drain to next level down.";
    if ((tile >= 0x36 && tile <= 0x40) || tile == 0x5d || tile == 0x5f || tile == 0x60)
        return L"Wall";
    if (tile >= 0x41 && tile <= 0x44) return L"Force Field";
    if (tile >= 0x45 && tile <= 0x50) return L"Pyramid";
    if (tile >= 0x51 && tile <= 0x58) return L"Power Plant";
    if (tile == 0x59) return L"Moon Door";
    if (tile == 0x5a) return L"Star Door";
    if (tile == 0x5b) return L"Door of Flame";
    if (tile == 0x5c) return L"Metal Door";
    if (tile == 0x61) return L"Security Door";
    if (tile == 0x62) return L"Door";
    if (tile >= 0x63 && tile <= 0x68) return L"6-sided die.";
    if (tile == 0x6d || tile == 0x6e) return L"Poker Table.";
    if (tile >= 0x70 && tile <= 0x7d) return L"Statue of Gorloth the Demon Lord.";
    if (tile >= 0x7f && tile <= 0x82) return L"Treasure Chest.";
    if (tile >= 0x83 && tile <= 0x86) return L"Giant Emerald.";
    if (tile >= 0xa2 && tile <= 0xaa) return L"Altar.";
    if (tile >= 0xab && tile <= 0xae) return L"Insert Smart Card Here.";
    if (tile == 0xaf) return L"Giant Skull";
    if (tile == 0xb0) return L"Fire";
    if (tile == 0xb1) return L"Torch";
    if (tile == 0x7e) return L"Signpost.";
    return L"???";
}

bool GameData::SelectPreviousItem() noexcept {
    const bool changed = selectedItem_ != 0;
    const std::size_t oldFirst = firstVisibleItem_;
    if (changed) --selectedItem_;
    if (selectedItem_ < firstVisibleItem_) firstVisibleItem_ = selectedItem_;
    return changed || firstVisibleItem_ != oldFirst;
}

bool GameData::SelectNextItem() noexcept {
    const bool changed = selectedItem_ + 1 < inventory_.size();
    const std::size_t oldFirst = firstVisibleItem_;
    if (changed) ++selectedItem_;
    if (selectedItem_ >= firstVisibleItem_ + 5) firstVisibleItem_ = selectedItem_ - 4;
    return changed || firstVisibleItem_ != oldFirst;
}

bool GameData::SelectVisibleArsenalItem(int clientY) noexcept {
    // The lower branch of FUN_1008_5770 maps five 35-pixel hit regions,
    // separated by one-pixel row boundaries, onto the scrolled arsenal.
    if (clientY <= 239 || clientY >= 421) return false;
    const int relativeY = clientY - 240;
    if (relativeY < 144 && relativeY % 36 == 35) return false;

    const std::size_t visibleRow = static_cast<std::size_t>(std::min(relativeY / 36, 4));
    const std::size_t inventoryIndex = firstVisibleItem_ + visibleRow;
    if (inventoryIndex >= inventory_.size()) return false;
    selectedItem_ = inventoryIndex;
    return true;
}

bool GameData::ScrollVisibleArsenal(bool upward) noexcept {
    // FUN_1008_5a04 scrolls the five-row viewport without changing selection.
    if (upward) {
        if (firstVisibleItem_ == 0) return false;
        --firstVisibleItem_;
        return true;
    }
    if (firstVisibleItem_ + 5 >= inventory_.size()) return false;
    ++firstVisibleItem_;
    return true;
}

void GameData::DrawArsenal(HDC destination, const BitmapSurface& atlas, int width,
                           int scrollDirection) const {
    const int saved = SaveDC(destination);
    const auto drawSprite = [&](int x, int y, std::size_t index) {
        if (index >= sprites_.size()) return;
        const auto& sprite = sprites_[index];
        atlas.DrawRegion(destination, x, y, sprite.sourceColumn * kCellSize,
            sprite.sourceRow * kCellSize, sprite.widthInCells * kCellSize,
            sprite.heightInCells * kCellSize);
    };
    SelectObject(destination, GetStockObject(DC_PEN));
    for (int y : {239, 275, 311, 348, 384, 421}) {
        SetDCPenColor(destination, RGB(0, 0, 0));
        MoveToEx(destination, 0, y, nullptr); LineTo(destination, width - 1, y);
        if (y != 421) {
            SetDCPenColor(destination, RGB(128, 128, 128));
            MoveToEx(destination, 0, y + 1, nullptr); LineTo(destination, width - 1, y + 1);
        }
    }
    // 4976 encloses the list: gray left/top edges, black right/bottom edges.
    SetDCPenColor(destination, RGB(128, 128, 128));
    MoveToEx(destination, 0, 421, nullptr); LineTo(destination, 0, 239);
    LineTo(destination, width - 1, 239);
    SetDCPenColor(destination, RGB(0, 0, 0));
    MoveToEx(destination, width - 1, 421, nullptr); LineTo(destination, width - 1, 239);
    // 4f4e uses the same inset around the score/health status region.
    MoveToEx(destination, 0, 85, nullptr); LineTo(destination, width - 1, 85);
    LineTo(destination, width - 1, 47);
    SetDCPenColor(destination, RGB(128, 128, 128));
    MoveToEx(destination, width - 1, 47, nullptr); LineTo(destination, 0, 47);
    LineTo(destination, 0, 85);
    // 4976: original three-state scroll control and layered ARSENAL heading.
    drawSprite(width - 25, 198, scrollDirection < 0 ? 0x99 : scrollDirection > 0 ? 0x9a : 0x9b);
    HFONT bold = CreateFontW(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        FIXED_PITCH | FF_MODERN, L"Courier New");
    HFONT normal = CreateFontW(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        FIXED_PITCH | FF_MODERN, L"Courier New");
    // 4e5e uses separate large initials and smaller word endings; positions
    // depend on each font's average character width, not a centered caption.
    HFONT initialFont = CreateFontW(-30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        FIXED_PITCH | FF_MODERN, L"Courier New");
    HFONT endingFont = CreateFontW(-16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        FIXED_PITCH | FF_MODERN, L"Courier New");
    SetBkMode(destination, TRANSPARENT);
    SetTextColor(destination, RGB(128, 0, 0));
    const auto embossedText = [&](int x, int y, const wchar_t* value, int length) {
        const auto ink = GetTextColor(destination);
        SetTextColor(destination, RGB(255, 255, 255));
        TextOutW(destination, x - 1, y - 1, value, length);
        SetTextColor(destination, RGB(0, 0, 0));
        TextOutW(destination, x + 1, y + 1, value, length);
        SetTextColor(destination, ink);
        TextOutW(destination, x, y, value, length);
    };
    SelectObject(destination, initialFont);
    TEXTMETRICW initialMetrics{}, endingMetrics{};
    GetTextMetricsW(destination, &initialMetrics);
    const int initialWidth = initialMetrics.tmAveCharWidth;
    embossedText(initialWidth, 0, L"D", 1);
    embossedText(initialWidth * 2 + 1, 15, L"S", 1);
    SelectObject(destination, endingFont);
    GetTextMetricsW(destination, &endingMetrics);
    embossedText(initialWidth * 2, 7, L"RAIN", 4);
    embossedText(initialWidth * 3 + 1, 22, L"TORM", 4);
    TextOutW(destination, (initialWidth + 1) * 3 + endingMetrics.tmAveCharWidth * 4,
        22, L"©", 1);
    SelectObject(destination, bold);
    SetBkMode(destination, TRANSPARENT);
    SetTextColor(destination, RGB(0, 0, 0));
    TextOutW(destination, 2, 205, L"ARSENAL", 7);
    SetTextColor(destination, RGB(255, 255, 255));
    TextOutW(destination, 0, 203, L"ARSENAL", 7);
    SetTextColor(destination, RGB(128, 128, 64));
    TextOutW(destination, 1, 204, L"ARSENAL", 7);
    // 4f4e: grouped, right-aligned score with shadow, and up to seven
    // vessel icons. Health above seven also receives a numeric label.
    std::wstring score = std::to_wstring(score_);
    for (int at = static_cast<int>(score.size()) - 3; at > 0; at -= 3) score.insert(at, L",");
    SIZE extent{};
    GetTextExtentPoint32W(destination, score.c_str(), static_cast<int>(score.size()), &extent);
    SetTextColor(destination, RGB(0, 0, 0));
    TextOutW(destination, width - extent.cx - 5, 48, score.c_str(), static_cast<int>(score.size()));
    SetTextColor(destination, RGB(255, 255, 0));
    TextOutW(destination, width - extent.cx - 6, 47, score.c_str(), static_cast<int>(score.size()));
    for (int index = 0; index < std::min(health_, 7); ++index) drawSprite(index * 13 + 2, 72, 0x154);
    if (health_ > 7) {
        const auto health = std::to_wstring(health_);
        SetTextColor(destination, RGB(0, 0, 255));
        TextOutW(destination, 95, 72, health.c_str(), static_cast<int>(health.size()));
    }
    SelectObject(destination, normal);
    // FUN_1008_4976 displays five inventory entries beginning at y=246, 36 pixels apart.
    constexpr std::size_t visibleItems = 5;
    const std::size_t displayedItems = std::min(visibleItems, inventory_.size());
    std::size_t firstItem = firstVisibleItem_;
    if (firstItem + displayedItems > inventory_.size()) {
        firstItem = inventory_.size() - displayedItems;
    }
    SetBkMode(destination, TRANSPARENT);
    for (std::size_t visible = 0; visible < displayedItems; ++visible) {
        const std::size_t inventoryIndex = firstItem + visible;
        const InventoryItem& item = inventory_[inventoryIndex];
        const int y = 246 + static_cast<int>(visible) * 36;
        if (item.type < itemSprites_.size()) {
            const std::uint16_t spriteIndex = itemSprites_[item.type];
            if (spriteIndex < sprites_.size()) {
                const SpriteRecord& sprite = sprites_[spriteIndex];
                atlas.DrawRegion(destination, 5, y,
                                 static_cast<int>(sprite.sourceColumn) * kCellSize,
                                 static_cast<int>(sprite.sourceRow) * kCellSize,
                                 static_cast<int>(sprite.widthInCells) * kCellSize,
                                 static_cast<int>(sprite.heightInCells) * kCellSize);
            }
        }
        if (item.type < itemNames_.size()) {
            const std::wstring& name = itemNames_[item.type];
            const bool counted = item.type < itemDisplayKinds_.size() && itemDisplayKinds_[item.type] > 1;
            SetTextColor(destination, RGB(0, 0, 0));
            TextOutW(destination, 36, y + (counted ? -5 : 2), name.c_str(), static_cast<int>(name.size()));
        }
        if (item.type < itemDisplayKinds_.size() && itemDisplayKinds_[item.type] > 1) {
            const auto quantity = std::to_wstring(item.quantity);
            SetTextColor(destination, RGB(0, 0, 255));
            TextOutW(destination, 36, y + 9, quantity.c_str(), static_cast<int>(quantity.size()));
        }
        if (inventoryIndex == selectedItem_) {
            drawSprite(width - 26, y + 13, 0x96);
            HPEN pen = CreatePen(PS_SOLID, 2, RGB(128, 0, 0));
            HGDIOBJ previousPen = SelectObject(destination, pen);
            HGDIOBJ previousBrush = SelectObject(destination, GetStockObject(HOLLOW_BRUSH));
            Rectangle(destination, 1, static_cast<int>(visible) * 182 / 5 + 240,
                width - 2, static_cast<int>(visible + 1) * 182 / 5 + 238);
            SelectObject(destination, previousBrush);
            SelectObject(destination, previousPen);
            DeleteObject(pen);
        }
    }
    RestoreDC(destination, saved);
    DeleteObject(normal);
    DeleteObject(bold);
    DeleteObject(initialFont);
    DeleteObject(endingFont);
}

bool GameData::MovePlayerToward(int column, int row) noexcept {
    // FUN_1008_6048 gates on demo state, not parent linkage. ApplicationState
    // rejects demo input. Clicking the vessel dispatches (0,0) as an
    // ordinary movement action, providing the mouse equivalent of Wait.
    if (column < static_cast<int>(playerColumn_) - 1 ||
        column > static_cast<int>(playerColumn_) + 1 ||
        row < static_cast<int>(playerRow_) - 1 ||
        row > static_cast<int>(playerRow_) + 1) {
        return false;
    }
    const int deltaColumn = column == static_cast<int>(playerColumn_)
        ? 0 : (column < static_cast<int>(playerColumn_) ? -1 : 1);
    const int deltaRow = row == static_cast<int>(playerRow_)
        ? 0 : (row < static_cast<int>(playerRow_) ? -1 : 1);
    return MovePlayer(deltaColumn, deltaRow);
}

bool GameData::UseSelectedItemToward(int column, int row) noexcept {
    if (column < static_cast<int>(playerColumn_) - 1
        || column > static_cast<int>(playerColumn_) + 1
        || row < static_cast<int>(playerRow_) - 1
        || row > static_cast<int>(playerRow_) + 1) {
        return false;
    }
    const int deltaColumn = column == static_cast<int>(playerColumn_)
        ? 0 : (column < static_cast<int>(playerColumn_) ? -1 : 1);
    const int deltaRow = row == static_cast<int>(playerRow_)
        ? 0 : (row < static_cast<int>(playerRow_) ? -1 : 1);
    return UseSelectedItem(deltaColumn, deltaRow);
}

void GameData::AddProjectile(std::uint16_t type, int column, int row,
                             int deltaColumn, int deltaRow) noexcept {
    if (projectiles_.size() < 150) {
        projectiles_.push_back({type, column, row, deltaColumn, deltaRow});
    }
}

void GameData::AwardActorScore(std::uint16_t type) noexcept {
    // FUN_1008_ad16.
    constexpr std::uint32_t values[] = {10, 15, 100, 250, 5};
    if (type < std::size(values)) {
        score_ += values[type];
    }
}

void GameData::AwardTileScore(std::uint16_t tile) noexcept {
    // FUN_1008_ad60. Center types 0x1e and 0x1f deliberately award nothing.
    switch (tile) {
    case 0x18: score_ += 100; break;
    case 0x19: score_ += 500; break;
    case 0x1a: score_ += 350; break;
    case 0x1b: score_ += 1000; break;
    case 0x1c: score_ += 2500; break;
    case 0x1d: score_ += 7500; break;
    case 0x20: score_ += 50; break;
    default:
        if (tile >= 0x87 && tile <= 0x8f) {
            score_ += 2500;
        }
        break;
    }
}

bool GameData::DamageBoardSquare(int column, int row, int strength,
                                 std::vector<Projectile>* spawnedProjectiles) noexcept {
    if (strength < 1 || column < 0 || row < 0 ||
        column >= static_cast<int>(kBoardColumns) || row >= static_cast<int>(kBoardRows)) {
        return false;
    }
    std::uint16_t& tile = level_[static_cast<std::size_t>(column)]
                                [static_cast<std::size_t>(row)];
    if (tile >= tiles_.size()) {
        return false;
    }
    const std::uint16_t flags = tiles_[tile].flags;
    if ((flags & 1u) != 0 || (flags & 4u) != 0) {
        return false;
    }

    if (levelNumber_ == 1 && tile >= 0x87 && tile <= 0x8f) {
        // Selector 1038's FUN_1008_ec6e recognizes the two mobile 3x3
        // targets by their tracked origins. Only missile (2) and radio-blast
        // (3) impact classes damage them; bubbles, bombs, and Fireballs do not.
        if (strength != 2 && strength != 3) return false;
        const int tileOffset = static_cast<int>(tile - 0x87);
        ReconcileLevelOneTargets();
        const std::size_t target = LevelOneTargetAt(column - tileOffset % 3, row - tileOffset / 3);
        if (target == 2) return false;
        ++levelOneTargetHits_[target];
        overlays_.push_back({0x14, static_cast<std::uint16_t>(column),
                             static_cast<std::uint16_t>(row)});
        if (levelOneTargetHits_[target] < 10) score_ += 2500;
        QueueSound(GameplaySound::Yeow);
        return true;
    }

    if (levelNumber_ == 1 && tile >= 0x51 && tile <= 0x54) {
        // Selector 1038 overrides vtable +0x40 with FUN_1008_ec6e. Impact
        // classes one through three destroy the complete 2x2 Energy Sphere;
        // the piercing class-four Fireball deliberately falls through.
        if (strength < 1 || strength > 3) return false;
        for (std::size_t objectColumn = 12; objectColumn <= 13; ++objectColumn) {
            for (std::size_t objectRow = 12; objectRow <= 13; ++objectRow) {
                level_[objectColumn][objectRow] = 0;
                overlays_.push_back({0x14, static_cast<std::uint16_t>(objectColumn),
                                     static_cast<std::uint16_t>(objectRow)});
            }
        }
        // FUN_1008_ec6e invokes four complete FUN_1008_a7a8 blasts centered
        // on the corners just outside the cleared sphere.
        for (const int centerColumn : {11, 14}) {
            for (const int centerRow : {11, 14}) {
                ApplyRadioExplosion(centerColumn, centerRow);
            }
        }
        // 1008:ed34 indexes column 14 (14 * 4), then row 32 (32 * 2).
        level_[14][32] = 0;
        overlays_.push_back({0x16, 14, 32});
        score_ += 20000;
        energySphereDestroyed_ = true;
        pendingMessages_.emplace_back(L"You destroyed the Energy Sphere!");
        QueueSound(GameplaySound::ExplosionLarge);
        return true;
    }

    if (tile == 0x5c) {
        overlays_.push_back({0x14, static_cast<std::uint16_t>(column),
                             static_cast<std::uint16_t>(row)});
        QueueSound(GameplaySound::BoardBreak);
        if (strength == 1) {
            return true;
        }
    }

    if (tile >= 8 && tile <= 0x17) {
        // FUN_1008_bfc4 converts any perimeter tile to its 3x3 origin.
        constexpr int originColumnDelta[8] = {0, -1, -2, 0, -2, 0, -1, -2};
        constexpr int originRowDelta[8] = {0, 0, 0, -1, -1, -2, -2, -2};
        const int patternOffset = static_cast<int>((tile - 8) & 7u);
        const int originColumn = column + originColumnDelta[patternOffset];
        const int originRow = row + originRowDelta[patternOffset];
        if (originColumn < 0 || originRow < 0 || originColumn + 2 >= static_cast<int>(kBoardColumns) ||
            originRow + 2 >= static_cast<int>(kBoardRows)) {
            return false;
        }
        const std::uint16_t center = level_[static_cast<std::size_t>(originColumn + 1)]
                                           [static_cast<std::size_t>(originRow + 1)];
        if (center < 0x18 || center > 0x1f) {
            return false;
        }
        // 1008:bfc4 paints and sounds the impact before either resistance
        // branch; an unsuccessful hit still has visible feedback.
        overlays_.push_back({0x14, static_cast<std::uint16_t>(column),
                             static_cast<std::uint16_t>(row)});
        QueueSound(GameplaySound::ExplosionSmall);
        const bool guarded = (center & 1u) != 0;
        if ((guarded && strength == 1) ||
            (((guarded && strength == 2) || (!guarded && strength == 1)) &&
             NextRandom(20) == 5)) {
            if (strength == 1)
                pendingMessages_.emplace_back(L"The bomb had no effect on the portal!");
            else if (strength == 2)
                pendingMessages_.emplace_back(L"The missile had no effect on the portal!");
            return true;
        }
        for (int objectRow = originRow; objectRow <= originRow + 2; ++objectRow) {
            for (int objectColumn = originColumn; objectColumn <= originColumn + 2; ++objectColumn) {
                level_[static_cast<std::size_t>(objectColumn)]
                      [static_cast<std::size_t>(objectRow)] = 0;
                overlays_.push_back({0x14, static_cast<std::uint16_t>(objectColumn),
                                     static_cast<std::uint16_t>(objectRow)});
                // Vtable slot +0x3c selects an optional recovered treasure for
                // every cleared object square from the impacting weapon class.
                const int dropType = RollObjectTreasure(strength);
                if (dropType >= 0) {
                    overlays_.push_back({static_cast<std::uint16_t>(dropType),
                                         static_cast<std::uint16_t>(objectColumn),
                                         static_cast<std::uint16_t>(objectRow)});
                }
            }
        }
        AwardTileScore(center);
        QueueSound(GameplaySound::BoardBreak);
        return true;
    }

    if (tile >= 0x7f && tile <= 0x82) {
        int chestColumn = column;
        int chestRow = row;
        if (tile == 0x80 || tile == 0x82) --chestColumn;
        if (tile == 0x81 || tile == 0x82) --chestRow;
        if (chestColumn < 0 || chestRow < 0
            || chestColumn + 1 >= static_cast<int>(kBoardColumns)
            || chestRow + 1 >= static_cast<int>(kBoardRows)) return false;
        for (int objectRow = chestRow; objectRow <= chestRow + 1; ++objectRow) {
            for (int objectColumn = chestColumn; objectColumn <= chestColumn + 1;
                 ++objectColumn) {
                level_[objectColumn][objectRow] = 0;
                overlays_.push_back({0x14, static_cast<std::uint16_t>(objectColumn),
                                     static_cast<std::uint16_t>(objectRow)});
                const int dropType = RollObjectTreasure(4);
                if (dropType >= 0)
                    overlays_.push_back({static_cast<std::uint16_t>(dropType),
                                         static_cast<std::uint16_t>(objectColumn),
                                         static_cast<std::uint16_t>(objectRow)});
            }
        }
        QueueSound(GameplaySound::BoardBreak);
        return true;
    }

    AwardTileScore(tile);
    overlays_.push_back({0x14, static_cast<std::uint16_t>(column),
                         static_cast<std::uint16_t>(row)});
    QueueSound(GameplaySound::BoardBreak);
    if (tile == 3 && NextRandom(50) == 1) {
        overlays_.push_back({1, static_cast<std::uint16_t>(column),
                             static_cast<std::uint16_t>(row)});
        pendingMessages_.emplace_back(L"There was treasure hidden under the rockpile!");
    }
    if (strength == 2 && spawnedProjectiles != nullptr) {
        for (int deltaColumn = -1; deltaColumn <= 1; ++deltaColumn) {
            for (int deltaRow = -1; deltaRow <= 1; ++deltaRow) {
                if (deltaColumn != 0 || deltaRow != 0) {
                    spawnedProjectiles->push_back(
                        {1, column, row, deltaColumn, deltaRow});
                }
            }
        }
    }
    tile = 0;
    return true;
}

int GameData::RollObjectTreasure(int impactClass) noexcept {
    if (levelNumber_ >= 17 && levelNumber_ <= 25) {
        if (impactClass != 19) return -1;
        return NextRandom(5) == 5 ? 2 : 1;
    }
    if (levelNumber_ == 4) {
        if (impactClass == 19) return NextRandom(2) == 1 ? 2 : 3;
        if (impactClass == 4) {
            const int roll = NextRandom(16);
            if (roll <= 2) return 1;
            if (roll <= 4) return 2;
            if (roll == 5) return 7;
        }
    } else if (levelNumber_ == 12) {
        if (impactClass == 19) {
            const int roll = NextRandom(6);
            return roll == 1 ? 16 : (roll <= 4 ? 1 : 2);
        }
    } else if (impactClass == 19) {
        const int roll = NextRandom(9);
        if (roll <= 4) return 1;
        if (roll <= 8) return 2;
    } else if (impactClass == 4) {
        const int roll = NextRandom(20);
        if (roll == 8) return 1;
        if (roll == 9) return 2;
        if (roll == 10) return 7;
    }
    return -1;
}

bool GameData::UseSelectedItem(int deltaColumn, int deltaRow) noexcept {
    if (deltaColumn != 0) playerFacesRight_ = deltaColumn > 0;
    if (health_ == 0 || selectedItem_ >= inventory_.size() ||
        (deltaColumn == 0 && deltaRow == 0)) {
        return false;
    }
    fatalHitLatched_ = false;
    pendingSound_ = GameplaySound::None;
    pendingMessages_.clear();
    pendingDialogMessage_.clear();
    const std::uint16_t type = inventory_[selectedItem_].type;
    const int playerColumn = static_cast<int>(playerColumn_);
    const int playerRow = static_cast<int>(playerRow_);
    bool used = projectiles_.size() < 150;
    bool consume = true;
    // 1008:1b22 runs a full world turn even when the item callback rejects use.
    const auto finishItemTurn = [this, deltaColumn, deltaRow]() noexcept {
        AdvanceProjectiles();
        AdvanceBoardRuntime(false, deltaColumn, deltaRow, true);
        SpawnStructureActors();
        AdvanceActors();
        AdvanceBoardRuntime(false, deltaColumn, deltaRow, false);
        AdvanceRepellent();
        ArmTransientOverlayAnimation();
        return true;
    };

    // Selector 1040 routes item use on its four betting squares through
    // FUN_1010_19a0 before the ordinary arsenal dispatch. The callback accepts
    // only original inventory classes 2..11, 15, and 19 and consumes one unit
    // as a bet without firing or placing that item.
    if (used && levelNumber_ == 2 && (playerColumn == 23 || playerColumn == 24)
        && (playerRow == 26 || playerRow == 27)) {
        const bool eligible = (type >= 2 && type <= 11) || type == 15 || type == 19;
        if (casinoStatus_ == 4) {
            pendingMessages_.emplace_back(L"Casino is closed.");
            return finishItemTurn();
        }
        if (casinoStatus_ == 2) {
            pendingMessages_.emplace_back(L"You have already bet. Pull the lever to draw cards.");
            return finishItemTurn();
        }
        if (!eligible) {
            pendingMessages_.emplace_back(L"The house won't cover your bet.");
            return finishItemTurn();
        }
        if (casinoStatus_ == 3) {
            ResetCasinoRound();
        }
        if (casinoBetCount_ != 0 && casinoBetItemType_ != type) {
            pendingMessages_.emplace_back(L"You can only bet one item type at a time.");
            return finishItemTurn();
        }
        if (casinoBetCount_ >= 20) {
            pendingMessages_.emplace_back(L"Maximum bet at this table is 20."
                              L" You may continue betting, or pull the lever to deal.");
            return finishItemTurn();
        }
        if (casinoBetCount_ == 0) {
            for (BoardOverlay& overlay : overlays_) {
                if (overlay.column == 26 && overlay.row == 26 && overlay.type == 0x2c) {
                    overlay.type = 0x2d;
                    break;
                }
            }
            casinoStatus_ = 1;
            casinoBetItemType_ = type;
            SetCasinoAnnouncement(5);
        }
        ++casinoBetCount_;
        pendingMessages_.emplace_back(L"You Bet a\n" + std::wstring(ItemName(type))
            + L"\nTotal Bet:\n" + std::to_wstring(casinoBetCount_) + L"(s).");
        ConsumeSelectedItem();
        QueueSound(GameplaySound::CasinoBet);
        return finishItemTurn();
    }
    if (used) switch (type) {
    case 0:
        // FUN_1008_1fe2 case 0 passes projectile type zero to FUN_1008_8e22.
        // Type four is reserved for the Fireball item (case 0x0b).
        AddProjectile(0, playerColumn, playerRow, deltaColumn, deltaRow);
        QueueSound(GameplaySound::SingleThuk);
        break;
    case 1: {
        // The six-word records at 1080:0016 define two parallel launch offsets for
        // each direction; the third projectile starts on the player's square.
        constexpr int launchTable[8][6] = {
            {-1, -1, 0, 1, 1, 0}, {-1, 0, 0, -1, 0, 1},
            {-1, 1, 0, -1, 1, 0}, {0, -1, -1, 0, 1, 0},
            {0, 1, -1, 0, 1, 0}, {1, -1, -1, 0, 0, 1},
            {1, 0, 0, -1, 0, 1}, {1, 1, 0, -1, -1, 0}};
        const int* launch = launchTable[0];
        for (const auto& record : launchTable) {
            if (record[0] == deltaRow && record[1] == deltaColumn) {
                launch = record;
                break;
            }
        }
        AddProjectile(0, playerColumn + launch[3], playerRow + launch[2], deltaColumn, deltaRow);
        AddProjectile(0, playerColumn, playerRow, deltaColumn, deltaRow);
        AddProjectile(0, playerColumn + launch[5], playerRow + launch[4], deltaColumn, deltaRow);
        QueueSound(GameplaySound::TripleThuk);
        break;
    }
    case 2: {
        const int column = playerColumn + deltaColumn;
        const int row = playerRow + deltaRow;
        if (column < 0 || row < 0 || column >= static_cast<int>(kBoardColumns) ||
            row >= static_cast<int>(kBoardRows)) {
            pendingMessages_.emplace_back(L"You can't deploy a decoy there.");
            used = false;
            break;
        }
        const std::uint16_t tile = level_[column][row];
        if (tile >= tiles_.size() || (tiles_[tile].flags & 1u) == 0 || tile != 0) {
            pendingMessages_.emplace_back(L"You can't deploy a decoy there.");
            used = false;
            break;
        }
        bool occupied = false;
        for (const BoardActor& actor : actors_) {
            occupied = occupied || (actor.column == column && actor.row == row);
        }
        if (occupied) {
            pendingMessages_.emplace_back(L"You can't deploy a decoy there.");
            used = false;
            break;
        }
        level_[column][row] = 7;
        QueueSound(GameplaySound::BombShot);
        break;
    }
    case 3: {
        const int column = playerColumn + deltaColumn;
        const int row = playerRow + deltaRow;
        const bool inBounds = column >= 0 && row >= 0
            && column < static_cast<int>(kBoardColumns)
            && row < static_cast<int>(kBoardRows);
        const bool passable = inBounds
            && level_[static_cast<std::size_t>(column)][static_cast<std::size_t>(row)]
                < tiles_.size()
            && (tiles_[level_[static_cast<std::size_t>(column)]
                           [static_cast<std::size_t>(row)]].flags & 1u) != 0;
        const bool occupied = std::any_of(actors_.begin(), actors_.end(),
            [column, row](const BoardActor& actor) {
                return actor.column == column && actor.row == row;
            });
        if (!passable || occupied) {
            pendingMessages_.emplace_back(L"You can't lay a mine there.");
            used = false;
            break;
        }
        AddProjectile(1, column, row, 0, 0);
        QueueSound(GameplaySound::BombShot);
        break;
    }
    case 4:
        AddProjectile(1, playerColumn, playerRow, deltaColumn, deltaRow);
        QueueSound(GameplaySound::BombShot);
        break;
    case 5:
        AddProjectile(2, playerColumn, playerRow, deltaColumn, deltaRow);
        QueueSound(GameplaySound::MissileLaunch);
        break;
    case 6:
        for (int column = -1; column <= 1; ++column) {
            for (int row = -1; row <= 1; ++row) {
                if (column != 0 || row != 0) {
                    AddProjectile(1, playerColumn, playerRow, column, row);
                }
            }
        }
        QueueSound(GameplaySound::BombShotEight);
        break;
    case 7:
        // FUN_1008_1fe2 case 7 is the missile counterpart to case 6: eight
        // type-2 projectiles leave the player's square in every direction.
        for (int column = -1; column <= 1; ++column) {
            for (int row = -1; row <= 1; ++row) {
                if (column != 0 || row != 0) {
                    AddProjectile(2, playerColumn, playerRow, column, row);
                }
            }
        }
        QueueSound(GameplaySound::MissileLaunchEight);
        break;
    case 8: {
        const int column = playerColumn + deltaColumn;
        const int row = playerRow + deltaRow;
        const bool inBounds = column >= 0 && row >= 0
            && column < static_cast<int>(kBoardColumns)
            && row < static_cast<int>(kBoardRows);
        const bool passable = inBounds
            && level_[static_cast<std::size_t>(column)][static_cast<std::size_t>(row)]
                < tiles_.size()
            && (tiles_[level_[static_cast<std::size_t>(column)]
                           [static_cast<std::size_t>(row)]].flags & 1u) != 0;
        if (!passable) {
            pendingMessages_.emplace_back(L"You can't lay a radio bomb there.");
            used = false;
            break;
        }
        AddProjectile(3, column, row, 0, 0);
        QueueSound(GameplaySound::BombShot);
        break;
    }
    case 9:
        QueueSound(DetonateCharges()
            ? GameplaySound::Explosion : GameplaySound::RadioBeep);
        break;
    case 10:
        // The rapid bomb weapon creates ten coincident type-1 records. They
        // separate only as the original turn-based projectile update runs.
        for (int projectile = 0; projectile < 10; ++projectile) {
            AddProjectile(1, playerColumn, playerRow, deltaColumn, deltaRow);
        }
        QueueSound(GameplaySound::BombShotEight);
        break;
    case 0x0b:
        AddProjectile(4, playerColumn, playerRow, deltaColumn, deltaRow);
        QueueSound(GameplaySound::SingleThuk);
        break;
    case 0x0c:
        // Repellent state at player offsets fa/fc. The board turn callback
        // FUN_1008_c55c drains two units after every accepted action.
        repellentEnergy_ = 100;
        break;
    case 0x0d: {
        // Selector 1038 overrides the Smart Bomb callback with FUN_1008_eec2:
        // the central force field protects its occupants and every other actor
        // independently has a one-in-two chance to resist the blast.
        std::vector<BoardActor> replacements;
        const bool hadActors = !actors_.empty();
        if (levelNumber_ == 1) {
            bool mitigated = false;
            bool shielded = false;
            for (std::size_t index = actors_.size(); index-- > 0;) {
                const BoardActor actor = actors_[index];
                if (!levelOneArenaEntered_ && actor.column > 8 && actor.column < 17
                    && actor.row > 8 && actor.row < 17) {
                    shielded = true;
                    continue;
                }
                if (NextRandom(2) == 2) {
                    mitigated = true;
                    continue;
                }
                if (actor.type == 3) {
                    const int count = NextRandom(7);
                    for (int replacement = 0; replacement < count; ++replacement) {
                        replacements.push_back(
                            {2, actor.column, actor.row, 0, 0, 0});
                    }
                }
                overlays_.push_back({0x14, actor.column, actor.row});
                actors_.erase(actors_.begin() + static_cast<std::ptrdiff_t>(index));
            }
            if (mitigated) {
                pendingMessages_.emplace_back(L"The Smart Bomb's effect is mitigated.");
            } else if (shielded) {
                pendingMessages_.emplace_back(L"The Force Field shields the blast!");
            }
        } else {
            // Common FUN_1008_ca6a removes every actor without score; captains
            // split before removal while the original array is traversed
            // backward. New lieutenants are appended but not visited again.
            for (std::size_t index = actors_.size(); index-- > 0;) {
                const BoardActor actor = actors_[index];
                if (actor.type == 3) {
                    const int count = NextRandom(7);
                    for (int replacement = 0;
                         replacement < count && actors_.size() < 300; ++replacement) {
                        actors_.push_back({2, actor.column, actor.row, 0, 0, 0});
                    }
                }
                overlays_.push_back({0x14, actor.column, actor.row});
                actors_.erase(actors_.begin() + static_cast<std::ptrdiff_t>(index));
            }
        }
        if (hadActors) {
            QueueSound(GameplaySound::BoardBreak);
        }
        actors_.insert(actors_.end(), replacements.begin(), replacements.end());
        break;
    }
    case 0x0e: {
        // Selector 1038 inherits the Power Plant item callback at 1010:0000.
        // A Smart Card is accepted only by the reader at (20,28); it animates
        // both reader halves and removes the three-cell security barrier.
        const int targetColumn = playerColumn + deltaColumn;
        const int targetRow = playerRow + deltaRow;
        if (levelNumber_ != 1 || targetColumn != 20 || targetRow != 28
            || level_[20][28] != 0xab) {
            pendingMessages_.emplace_back(L"Nothing happens.");
            used = false;
            break;
        }
        level_[20][28] = 0xac;
        level_[20][27] = 0xae;
        for (std::uint16_t row = 29; row <= 31; ++row) {
            level_[23][row] = 0;
            overlays_.push_back({0x1f, 23, row});
        }
        pendingMessages_.emplace_back(L"The card engages... The security door opens.");
        QueueSound(GameplaySound::SpecialDoorOpen);
        break;
    }
    case 0x0f: {
        // FUN_1008_2c08 draws one of 47 fixed 80-byte records from selector
        // 1020 and remembers its zero-based index in the player's law list.
        const std::size_t law = static_cast<std::size_t>(NextRandom(47) - 1);
        if (!discoveredLaws_[law]) {
            discoveredLaws_[law] = true;
            discoveredLawOrder_.push_back(static_cast<std::uint8_t>(law));
        }
        pendingMessages_.push_back(L"It's a fortune cookie...\n\nIt says:");
        pendingMessages_.emplace_back(kLaws[law]);
        break;
    }
    case 0x10:
    case 0x12:
        pendingMessages_.emplace_back(L"Nothing happens.");
        used = false;
        break;
    case 0x11: {
        // FUN_1010_68a8 is the Moon Key's board-specific interaction. Unlike
        // the Bronze Key (case 0x13), it is not accepted by common chests.
        const int targetColumn = playerColumn + deltaColumn;
        const int targetRow = playerRow + deltaRow;
        if (targetColumn < 0 || targetRow < 0
            || targetColumn >= static_cast<int>(kBoardColumns)
            || targetRow >= static_cast<int>(kBoardRows)) {
            pendingMessages_.emplace_back(L"Nothing happens.");
            used = false;
            break;
        }
        if (levelNumber_ == 6 && targetColumn == 10 && targetRow == 17
            && level_[10][17] == 0x59) {
            // Selector 1060 overrides the common callback at its Moon Room door.
            for (std::uint16_t row = 16; row <= 18; ++row) {
                level_[10][row] = 0;
                overlays_.push_back({0x1f, 10, row});
            }
            pendingMessages_.emplace_back(L"The key turns... The door opens.");
            QueueSound(GameplaySound::SpecialDoorOpen);
            break;
        }
        pendingMessages_.emplace_back(L"Nothing happens.");
        used = false;
        break;
    }
    case 0x14: {
        // Selector 1068 overrides vtable slot +0x24 with FUN_1010_7258.
        // The Emerald repairs the central idol and opens its five-square gate.
        const int targetColumn = playerColumn + deltaColumn;
        const int targetRow = playerRow + deltaRow;
        if (levelNumber_ != 7 || targetColumn < 19 || targetColumn > 21
            || targetRow < 18 || targetRow > 20 || level_[19][18] == 0x79) {
            pendingMessages_.emplace_back(L"Nothing happens.");
            used = false;
            break;
        }
        for (BoardColumn& boardColumn : level_) {
            for (std::uint16_t& tile : boardColumn) {
                if (tile == 0x70) tile = 0x79;
                else if (tile == 0x71) tile = 0x7a;
                else if (tile == 0x73) tile = 0x7b;
                else if (tile == 0x74) tile = 0x7c;
            }
        }
        for (std::uint16_t column = 18; column <= 22; ++column) {
            level_[column][30] = 0;
            overlays_.push_back({0x14, column, 30});
        }
        score_ += 3000;
        pendingMessages_.emplace_back(L"The emerald fuses itself to the eye socket...\n"
                          L"You have repaired the idol!");
        break;
    }
    case 0x13: {
        const int column = playerColumn + deltaColumn;
        const int row = playerRow + deltaRow;
        if (column < 0 || row < 0 || column >= static_cast<int>(kBoardColumns) ||
            row >= static_cast<int>(kBoardRows)) {
            used = false;
            break;
        }
        std::uint16_t& tile = level_[static_cast<std::size_t>(column)]
                                    [static_cast<std::size_t>(row)];
        // FUN_1008_bc64 recognizes the Bronze Key (class 0x13) against any
        // square of a 2x2 chest. It normalizes the selected corner, paints the
        // four key-turn effects, clears the chest, and calls vtable +0x3c once
        // for every revealed square.
        if (tile >= 0x7f && tile <= 0x82) {
            int chestColumn = column;
            int chestRow = row;
            if (tile == 0x80 || tile == 0x82) --chestColumn;
            if (tile == 0x81 || tile == 0x82) --chestRow;
            if (chestColumn < 0 || chestRow < 0
                || chestColumn + 1 >= static_cast<int>(kBoardColumns)
                || chestRow + 1 >= static_cast<int>(kBoardRows)) {
                used = false;
                break;
            }
            constexpr std::uint16_t keyEffects[2][2] = {
                {0x19, 0x1a}, {0x1b, 0x1c}};
            for (int objectColumn = chestColumn;
                 objectColumn <= chestColumn + 1; ++objectColumn) {
                for (int objectRow = chestRow;
                     objectRow <= chestRow + 1; ++objectRow) {
                    level_[objectColumn][objectRow] = 0;
                    overlays_.push_back({
                        keyEffects[objectRow - chestRow][objectColumn - chestColumn],
                        static_cast<std::uint16_t>(objectColumn),
                        static_cast<std::uint16_t>(objectRow)});
                    const int dropType = RollObjectTreasure(19);
                    if (dropType >= 0) {
                        overlays_.push_back({static_cast<std::uint16_t>(dropType),
                                             static_cast<std::uint16_t>(objectColumn),
                                             static_cast<std::uint16_t>(objectRow)});
                    }
                }
            }
            QueueSound(GameplaySound::Drill);
            if (NextRandom(5) == 5) {
                pendingMessages_.emplace_back(L"The chest opens. The key remains intact!");
                consume = false;
            } else {
                pendingMessages_.emplace_back(L"The chest opens.");
            }
            break;
        }
        if (tile != 0x5c) {
            // FUN_1008_1fe2 rejects ordinary terrain here. The recovered
            // class-0x13 treasure callback is used by the Bronze Key chest
            // path, but the key does not invoke it on ordinary object terrain.
            pendingMessages_.emplace_back(L"Nothing happens.");
            used = false;
            break;
        }
        tile = 0;
        overlays_.push_back({0x1d, static_cast<std::uint16_t>(column),
                             static_cast<std::uint16_t>(row)});
        // FUN_1008_1fe2 preserves the Bronze Key on the one-in-four result.
        consume = NextRandom(4) != 1;
        pendingMessages_.emplace_back(consume
            ? L"The door opens."
            : L"The door opens. The key remains intact!");
        QueueSound(GameplaySound::Drill);
        break;
    }
    default:
        used = false;
        break;
    }
    if (used && consume && type != 0 && type != 1) {
        ConsumeSelectedItem();
    }
    // FUN_1008_1b22 advances the complete world turn after FUN_1008_1fe2
    // regardless of whether that routine applied or consumed the selected
    // item. Rejected placements and inert keys are therefore not free probes.
    return finishItemTurn();
}

void GameData::ArmTransientOverlayAnimation() noexcept {
    // Original 8b2c completes each five-frame batch before accepting another
    // action. Our nonblocking renderer needs independent ages to preserve that
    // lifetime without rewinding old effects when a new action creates more.
    transientOverlayFrame_ = 0;
    for (auto& overlay : overlays_) {
        if (!IsTransientOverlay(overlay)) continue;
        if (overlay.animationFrame == 0) overlay.animationFrame = 1;
        transientOverlayFrame_ = std::max(transientOverlayFrame_, overlay.animationFrame);
    }
}

void GameData::ConsumeSelectedItem() noexcept {
    if (selectedItem_ >= inventory_.size()) {
        return;
    }
    InventoryItem& item = inventory_[selectedItem_];
    if (item.quantity > 0) {
        --item.quantity;
    }
    if (item.quantity != 0) {
        return;
    }
    inventory_.erase(inventory_.begin() + static_cast<std::ptrdiff_t>(selectedItem_));
    selectedItem_ = 0;
    for (std::size_t index = 0; index < inventory_.size(); ++index) {
        if (inventory_[index].type == 1) {
            selectedItem_ = index;
            break;
        }
    }
    if (selectedItem_ < firstVisibleItem_) firstVisibleItem_ = selectedItem_;
    else if (selectedItem_ >= firstVisibleItem_ + 5)
        firstVisibleItem_ = selectedItem_ - 4;
}

void GameData::AddInventoryItem(std::uint16_t type, std::uint16_t quantity) noexcept {
    if (quantity == 0) {
        return;
    }
    for (InventoryItem& item : inventory_) {
        if (item.type == type) {
            item.quantity = static_cast<std::uint16_t>(item.quantity + quantity);
            return;
        }
    }
    inventory_.push_back({type, quantity});
}

void GameData::MaintainCasinoStructures() noexcept {
    std::uint16_t remainingChests = 0;
    for (int column = 11; column < 30; column += 3)
        remainingChests += level_[column][0] == 0x7f;
    if (remainingChests < casinoTopChestCount_) {
        for (int column : {14, 21, 28}) {
            if (level_[column][6] != 0x1f) continue;
            const int count = NextRandom(2);
            for (int index = 0; index < count && actors_.size() < 300; ++index)
                actors_.push_back({3, static_cast<std::uint16_t>(column), 6, 0, 0, 0});
        }
    }
    casinoTopChestCount_ = remainingChests;
    if (turnCounter_ == 0 || turnCounter_ % 200u != 0) return;

    const auto rebuild = [this](std::uint16_t centerTile, int centerColumn,
                                int centerRow) {
        if (level_[centerColumn][centerRow] == centerTile) return;
        const int left = centerColumn - 1, top = centerRow - 1;
        const std::uint16_t first = (centerTile & 1u) != 0 ? 0x10 : 8;
        constexpr int offsets[3][3] = {{0, 1, 2}, {3, -1, 4}, {5, 6, 7}};
        for (int row = 0; row < 3; ++row) {
            for (int column = 0; column < 3; ++column) {
                level_[left + column][top + row] = offsets[row][column] < 0
                    ? centerTile : static_cast<std::uint16_t>(first + offsets[row][column]);
                overlays_.push_back({0x16, static_cast<std::uint16_t>(left + column),
                                     static_cast<std::uint16_t>(top + row)});
            }
        }
        QueueSound(GameplaySound::Construction);
    };
    rebuild(0x1e, 4, 33);
    rebuild(0x1e, 36, 28);
    for (int column : {14, 21, 28}) rebuild(0x1f, column, 6);
}

void GameData::AdvanceCasinoRetaliation(int requestedMode) noexcept {
    // 1010:28b5 accepts a new strength only while no machine cycle is active.
    if (levelTwoSequenceState_ == -1) {
        if (requestedMode == 0) return;
        levelTwoSequenceMode_ = requestedMode;
        levelTwoSequenceState_ = NextRandom(2) == 2 ? 0 : 100;
        return;
    }
    const auto clearEffects = [this](int left, int top, int right, int bottom) {
        overlays_.erase(std::remove_if(overlays_.begin(), overlays_.end(),
            [=](const BoardOverlay& overlay) {
                return overlay.column >= left && overlay.column <= right
                    && overlay.row >= top && overlay.row <= bottom;
            }), overlays_.end());
    };
    const auto setMachineBoundary = [this](int left, int top, int right, int bottom,
                                            bool restore) {
        for (int column = left; column <= right; ++column) {
            level_[column][top] = restore ? 0x36 : 0;
            level_[column][bottom] = restore ? 0x36 : 0;
        }
        for (int row = top; row <= bottom; ++row) {
            level_[left][row] = restore ? 0x37 : 0;
            level_[right][row] = restore ? 0x37 : 0;
        }
        if (restore) {
            level_[left][top] = 0x38;
            level_[right][top] = 0x39;
            level_[left][bottom] = 0x3a;
            level_[right][bottom] = 0x3b;
            // The original directional gates survive boundary restoration.
            if (left == 2) level_[4][31] = 0x3e;
            else level_[36][30] = 0x3f;
        }
    };
    const auto spawnMachineActors = [this](int column, int row) {
        const auto addActors = [this, column, row](std::uint16_t type, int count) {
            for (int index = 0; index < count && actors_.size() < 300; ++index) {
                actors_.push_back({type, static_cast<std::uint16_t>(column),
                                   static_cast<std::uint16_t>(row), 0, 0, 0});
            }
        };
        if (levelTwoSequenceMode_ == 1) {
            addActors(0, NextRandom(3));
            addActors(1, NextRandom(3));
        } else {
            addActors(0, NextRandom(6));
            addActors(1, NextRandom(6));
            addActors(2, levelTwoSequenceMode_ == 2 ? NextRandom(2)
                                                    : NextRandom(6) + 6);
            if (levelTwoSequenceMode_ == 3 && NextRandom(3) == 3) {
                const int captains = NextRandom(2);
                const int captainColumn = column == 4 ? 36 : column;
                const int captainRow = column == 4 ? 30 : row;
                for (int index = 0; index < captains && actors_.size() < 300; ++index)
                    actors_.push_back({3,
                        static_cast<std::uint16_t>(captainColumn),
                        static_cast<std::uint16_t>(captainRow), 0, 0, 0});
            }
        }
    };
    if (levelTwoSequenceState_ == 0 && level_[4][33] == 0x1e) {
        spawnMachineActors(4, 33);
    } else if (levelTwoSequenceState_ == 100 && level_[36][28] == 0x1e) {
        spawnMachineActors(36, 28);
    } else if (levelTwoSequenceState_ == 1) {
        setMachineBoundary(2, 31, 6, 35, false);
        clearEffects(2, 31, 6, 35);
    } else if (levelTwoSequenceState_ == 3 + levelTwoSequenceMode_) {
        setMachineBoundary(2, 31, 6, 35, true);
        clearEffects(2, 31, 6, 35);
        levelTwoSequenceState_ = -1;
    } else if (levelTwoSequenceState_ == 101) {
        setMachineBoundary(34, 26, 38, 30, false);
        clearEffects(34, 26, 38, 30);
    } else if (levelTwoSequenceState_ == 103 + levelTwoSequenceMode_) {
        setMachineBoundary(34, 26, 38, 30, true);
        clearEffects(34, 26, 38, 30);
        levelTwoSequenceState_ = -1;
    }
    if (levelTwoSequenceState_ != -1) ++levelTwoSequenceState_;
}

void GameData::ResetCasinoRound() noexcept {
    SetCasinoAnnouncement(0);
    casinoWildRank_ = 0;
    if (NextRandom(2) == 2) {
        casinoWildRank_ = static_cast<std::uint16_t>(NextRandom(2) * 5 - 3);
    }
    casinoStatus_ = 0;
    casinoBetCount_ = 0;
    casinoBetItemType_ = 0xffff;
    casinoHeld_.fill(0);
    casinoResultTicks_ = 0;
    overlays_.erase(std::remove_if(overlays_.begin(), overlays_.end(),
        [](const BoardOverlay& overlay) {
            const bool casinoCard = overlay.type == 0x1e && overlay.row == 17
                && overlay.column >= 17 && overlay.column <= 29
                && (overlay.column - 17) % 3 == 0;
            return casinoCard || overlay.type == 0x2b;
    }), overlays_.end());
}

void GameData::SetCasinoAnnouncement(std::uint16_t announcement) noexcept {
    casinoAnnouncement_ = announcement;
    casinoScrollOffset_ = 0;
}

std::wstring_view GameData::CasinoAnnouncementText() const noexcept {
    // Exact selector-1010 strings used by 2874 and rendered by 26fe.
    switch (casinoAnnouncement_) {
    case 1: return L"Move onto the red boxes to hold cards. Pull the lever when you are ready to draw.";
    case 2: return L"*** WINNER ***";
    case 3: return L"Casino Closed...";
    case 4: return L"Game Over. Play Again.";
    case 5: return L"You may continue betting, or pull the lever to deal.";
    default: return L"Five Card Draw Poker. Place your bet on the \"$\".";
    }
}

std::wstring GameData::CasinoAnnouncementWindow() const {
    std::wstring window(13, L' ');
    const auto text = CasinoAnnouncementText();
    for (std::size_t index = 0; index < window.size(); ++index) {
        const auto cursor = casinoScrollOffset_ + index;
        if (cursor >= 13 && cursor - 13 < text.size()) window[index] = text[cursor - 13];
    }
    return window;
}

void GameData::ResolveTriggeredMines() noexcept {
    std::vector<std::pair<int, int>> triggeredMines;
    for (auto node = overlays_.rbegin(); node != overlays_.rend(); ++node) {
        if (node->type == 0x24) {
            triggeredMines.emplace_back(node->column, node->row);
        }
    }
    overlays_.erase(std::remove_if(overlays_.begin(), overlays_.end(),
        [](const BoardOverlay& overlay) { return overlay.type == 0x24; }), overlays_.end());

    for (const auto [centerColumn, centerRow] : triggeredMines) {
        for (int column = centerColumn - 1; column <= centerColumn + 1; ++column) {
            for (int row = centerRow - 1; row <= centerRow + 1; ++row) {
                if (column < 0 || row < 0 || column >= static_cast<int>(kBoardColumns)
                    || row >= static_cast<int>(kBoardRows)) {
                    continue;
                }
                const std::uint16_t tile = level_[column][row];
                if (tile < tiles_.size()) {
                    const std::uint16_t flags = tiles_[tile].flags;
                    const bool passable = (flags & 1u) != 0;
                    if (passable || ((flags & 2u) != 0 && (flags & 4u) == 0)) {
                        overlays_.push_back({0x14, static_cast<std::uint16_t>(column),
                                             static_cast<std::uint16_t>(row)});
                        if (!passable) DamageBoardSquare(column, row, 1);
                    }
                }
                const std::size_t originalCount = actors_.size();
                std::size_t visited = 0;
                for (std::size_t actorIndex = 0;
                     visited < originalCount && actorIndex < actors_.size();) {
                    const BoardActor actor = actors_[actorIndex];
                    ++visited;
                    if (actor.column != column || actor.row != row) {
                        ++actorIndex;
                        continue;
                    }
                    const std::uint16_t actorType = actor.type;
                    AwardActorScore(actorType);
                    if (actorType == 2 && NextRandom(10) == 2) {
                        overlays_.push_back({1, static_cast<std::uint16_t>(column),
                                             static_cast<std::uint16_t>(row)});
                    }
                    if (actorType == 3) {
                        const int count = NextRandom(7);
                        for (int replacement = 0;
                             replacement < count && actors_.size() < 300; ++replacement) {
                            actors_.push_back({2, static_cast<std::uint16_t>(column),
                                               static_cast<std::uint16_t>(row), 0, 0, 0});
                        }
                    }
                    actors_.erase(actors_.begin()
                        + static_cast<std::ptrdiff_t>(actorIndex));
                }
            }
        }
        QueueSound(GameplaySound::BoardBreak);
    }
}

void GameData::AddRandomizedInventoryItem(std::uint16_t type) noexcept {
    // FUN_1008_a3d0 maps each recovered pickup type to its original randomized
    // stock count, then merges the result through FUN_1008_26ec.
    int quantity = 0;
    switch (type) {
    case 1: quantity = 1; break;
    case 2: quantity = NextRandom(2); break;
    case 3: case 4: quantity = NextRandom(10); break;
    case 5: case 6: case 8: case 10: case 11: quantity = NextRandom(8); break;
    case 7: quantity = NextRandom(5); break;
    case 12: quantity = NextRandom(2); break;
    case 13: quantity = 1; break;
    case 15:
        quantity = NextRandom(3);
        ResolveOverlayEffect(1);
        break;
    case 19:
        quantity = NextRandom(4) > 1 ? 1 : NextRandom(3);
        break;
    default:
        return;
    }
    AddInventoryItem(type, static_cast<std::uint16_t>(quantity));
    if (type == 8) {
        AddInventoryItem(9, 1);
    }
    if (quantity > 0) {
        std::wstring message = quantity == 1 ? L"You found a " : L"You found some ";
        const wchar_t* longName = nullptr;
        switch (type) {
        case 1: longName = L"Triple-Shot Cannon"; break;
        case 6: longName = L"Bomb Burst"; break;
        case 7: longName = L"Missile Burst"; break;
        case 8: longName = L"Radio Controlled Bomb"; break;
        case 15: longName = L"Fortune Cookie"; break;
        default: break;
        }
        message += longName != nullptr ? longName : ItemName(type);
        message += quantity == 1 ? L"!" : L"s!";
        pendingMessages_.push_back(std::move(message));
    }
}

void GameData::ResolveOverlayEffect(std::uint16_t type) noexcept {
    // Common vtable target 1008:a16a. Types 1-3 are randomized containers;
    // the remaining types have fixed effects or map directly to item classes.
    constexpr std::uint16_t typeOneItems[17] = {
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 2, 2, 15, 19};
    constexpr std::uint16_t typeTwoItems[20] = {
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 10, 10, 7, 8, 11};
    constexpr std::uint16_t typeThreeItems[5] = {13, 12, 7, 11, 8};

    QueueSound(type == 1 ? GameplaySound::TreasureTwo : GameplaySound::TreasureOne);
    switch (type) {
    case 1:
        if (std::none_of(inventory_.begin(), inventory_.end(),
                         [](const InventoryItem& item) { return item.type == 1; }) &&
            NextRandom(12) == 6 &&
            (boardDepth_ >= 3 || (boardDepth_ == 2 && NextRandom(5) == 5))) {
            AddRandomizedInventoryItem(1);
        } else if (NextRandom(30) == 12) {
            ResolveOverlayEffect(2);
        } else {
            AddRandomizedInventoryItem(typeOneItems[NextRandom(17) - 1]);
        }
        break;
    case 2:
        if (NextRandom(3) == 3 && boardDepth_ >= 3 &&
            std::none_of(inventory_.begin(), inventory_.end(),
                        [](const InventoryItem& item) { return item.type == 1; })) {
            AddRandomizedInventoryItem(1);
        } else if (NextRandom(20) == 12) {
            ResolveOverlayEffect(9);
        } else {
            AddRandomizedInventoryItem(typeTwoItems[NextRandom(20) - 1]);
        }
        break;
    case 3:
        if (NextRandom(4) == 1) {
            ResolveOverlayEffect(9);
        } else {
            AddRandomizedInventoryItem(typeThreeItems[NextRandom(5) - 1]);
        }
        break;
    case 4:
        AddInventoryItem(17, 1);
        pendingMessages_.push_back(L"You found the Moon Key!");
        break;
    case 5:
        AddInventoryItem(16, 1);
        pendingMessages_.push_back(L"You found the Star Key!");
        break;
    case 6:
        AddInventoryItem(18, 1);
        pendingMessages_.push_back(L"You found the Fire Key!");
        break;
    case 7: AddRandomizedInventoryItem(19); break;
    case 8:
        AddInventoryItem(20, 1);
        pendingMessages_.push_back(L"You found a giant emerald!");
        break;
    case 9:
        ++health_;
        pendingMessages_.push_back(L"You gained an extra life!");
        break;
    case 10: AddRandomizedInventoryItem(12); break;
    case 11: AddRandomizedInventoryItem(8); break;
    case 12: AddRandomizedInventoryItem(2); break;
    case 13: AddRandomizedInventoryItem(13); break;
    case 14:
        AddInventoryItem(14, 1);
        pendingMessages_.push_back(
            L"You found a\n\"Smart Card\"\nelectronic security card!");
        break;
    case 15: AddRandomizedInventoryItem(11); break;
    case 16: AddRandomizedInventoryItem(7); break;
    case 17:
        AddInventoryItem(21, 1);
        pendingMessages_.push_back(L"You found a Casino Chip!");
        break;
    default: break;
    }
}

void GameData::ResolveOverlaysAtPlayer() noexcept {
    // af5a inserts at the linked-list head; vector storage keeps insertion
    // order for save compatibility, so traverse the original head order here.
    for (std::size_t next = overlays_.size(); next > 0;) {
        const std::size_t index = --next;
        const BoardOverlay overlay = overlays_[index];
        // 1008:a108-a113 requires exact coordinate equality. Solid terrain
        // under a pickup must be cleared before the vessel can enter its cell.
        const bool atPlayer = overlay.column == playerColumn_ && overlay.row == playerRow_;
        if (atPlayer
            && overlay.type > 0 && overlay.type < 0x12) {
            const std::uint16_t type = overlay.type;
            overlays_.erase(overlays_.begin() + static_cast<std::ptrdiff_t>(index));
            ResolveOverlayEffect(type);
        }
    }
}

bool GameData::PlaceLevelThreeStructure(bool showConstructionEffects) noexcept {
    std::size_t existingStructures = 0;
    for (std::size_t column = 12; column < 29; ++column) {
        for (std::size_t row = 7; row < 23; ++row) {
            existingStructures += level_[column][row] == 0x18 ? 1u : 0u;
        }
    }
    if (existingStructures >= 3) {
        return false;
    }
    for (int attempt = 0; attempt < 20; ++attempt) {
        const std::size_t column = static_cast<std::size_t>(NextRandom(15) + 11);
        const std::size_t row = static_cast<std::size_t>(NextRandom(14) + 6);
        bool available = true;
        for (std::size_t candidateColumn = column;
             available && candidateColumn < column + 3; ++candidateColumn) {
            for (std::size_t candidateRow = row; candidateRow < row + 3; ++candidateRow) {
                const std::uint16_t tile = level_[candidateColumn][candidateRow];
                if ((tile >= 0x7f && tile <= 0x82) || (tile >= 8 && tile <= 0x17)
                    || (candidateColumn == playerColumn_ && candidateRow == playerRow_)) {
                    available = false;
                    break;
                }
            }
        }
        if (!available) {
            continue;
        }
        constexpr std::uint16_t objectTiles[3][3] = {
            {8, 9, 10}, {11, 0x18, 12}, {13, 14, 15}};
        for (std::size_t offsetRow = 0; offsetRow < 3; ++offsetRow) {
            for (std::size_t offsetColumn = 0; offsetColumn < 3; ++offsetColumn) {
                level_[column + offsetColumn][row + offsetRow] =
                    objectTiles[offsetRow][offsetColumn];
                if (showConstructionEffects) {
                    overlays_.push_back(
                        {0x16, static_cast<std::uint16_t>(column + offsetColumn),
                         static_cast<std::uint16_t>(row + offsetRow)});
                }
            }
        }
        if (showConstructionEffects) QueueSound(GameplaySound::Construction);
        if (showConstructionEffects) {
            if (levelThreeSpawnChance_ < 75
                && NextRandom(difficulty_ + 2) < difficulty_ + 1)
                ++levelThreeSpawnChance_;
            if (existingStructures == 0) {
                if (level_[2][9] == 0x60 && NextRandom(5) == 5) {
                    level_[2][9] = 0x5c;
                    overlays_.push_back({0x16, 2, 9});
                }
                if (level_[31][4] == 0x5f && NextRandom(12) == 3) {
                    level_[31][4] = 0x5c;
                    overlays_.push_back({0x16, 31, 4});
                }
                if (NextRandom(10) == 2) {
                    // 4c12 searches for a 2x2 site and replaces it with a
                    // c452 chest/transporter, even where ordinary terrain is
                    // present. Only large objects and the player bar it.
                    // Bound the original unsafe retry loop to keep the UI
                    // responsive when no site remains.
                    for (int chestAttempt = 0; chestAttempt < 20; ++chestAttempt) {
                        const int chestColumn = NextRandom(15) + 11;
                        const int chestRow = NextRandom(14) + 6;
                        bool chestAvailable = true;
                        for (int dx = 0; dx < 2; ++dx)
                            for (int dy = 0; dy < 2; ++dy) {
                                const int siteColumn = chestColumn + dx;
                                const int siteRow = chestRow + dy;
                                const auto tile = level_[siteColumn][siteRow];
                                if ((tile >= 0x7f && tile <= 0x82)
                                    || (tile >= 8 && tile <= 0x17)
                                    || (siteColumn == playerColumn_ && siteRow == playerRow_))
                                    chestAvailable = false;
                            }
                        if (!chestAvailable) continue;
                        level_[chestColumn][chestRow] = 0x7f;
                        level_[chestColumn + 1][chestRow] = 0x80;
                        level_[chestColumn][chestRow + 1] = 0x81;
                        level_[chestColumn + 1][chestRow + 1] = 0x82;
                        for (int dx = 0; dx < 2; ++dx)
                            for (int dy = 0; dy < 2; ++dy)
                                overlays_.push_back({0x16,
                                    static_cast<std::uint16_t>(chestColumn + dx),
                                    static_cast<std::uint16_t>(chestRow + dy)});
                        break;
                    }
                }
            }
        }
        return true;
    }
    return false;
}

bool GameData::PlaceMoonRoomStructures() noexcept {
    // 1010:6344..63df rolls column in 1..36, row in 1..32, excludes
    // the player (ce4e), and requires all nine cells empty (c1e6, force=0).
    // Stage all three placements to avoid declaring a partial event complete.
    auto staged = level_;
    std::array<POINT, 3> placements{};
    const auto available = [&](int column, int row) {
        for (int x = column; x < column + 3; ++x)
            for (int y = row; y < row + 3; ++y)
                if (staged[x][y] != 0 || (x == playerColumn_ && y == playerRow_)) return false;
        return true;
    };
    constexpr std::uint16_t tiles[3][3] = {
        {8, 9, 10}, {11, 0x18, 12}, {13, 14, 15}};
    for (auto& placement : placements) {
        bool found = false;
        for (int attempt = 0; attempt < 4096; ++attempt) {
            const int column = NextRandom(36);
            const int row = NextRandom(32);
            if (!available(column, row)) continue;
            placement = POINT{column, row};
            found = true;
            break;
        }
        // The original retries forever on a packed board. Keep the native
        // UI responsive: exhaustive fallback, or leave the event pending.
        if (!found) {
            for (int column = 1; column <= 36 && !found; ++column)
                for (int row = 1; row <= 32 && !found; ++row)
                    if (available(column, row)) {
                        placement = POINT{column, row};
                        found = true;
                    }
        }
        if (!found) return false;
        for (int x = 0; x < 3; ++x)
            for (int y = 0; y < 3; ++y)
                staged[placement.x + x][placement.y + y] = tiles[y][x];
    }
    level_ = staged;
    for (const auto& placement : placements)
        for (int x = 0; x < 3; ++x)
            for (int y = 0; y < 3; ++y)
                overlays_.push_back({0x16, static_cast<std::uint16_t>(placement.x + x),
                    static_cast<std::uint16_t>(placement.y + y)});
    return true;
}

bool GameData::PlaceRandomGuardedStructure(bool showConstructionEffects) noexcept {
    // FUN_1010_d06a uses FUN_1008_ce4e followed by FUN_1008_c1e6 with
    // independent 36-column/32-row rolls, retrying until a 3x3 area is free.
    for (int attempt = 0; attempt < 4096; ++attempt) {
        const int column = NextRandom(36);
        const int row = NextRandom(32);
        if (column + 2 >= static_cast<int>(kBoardColumns)
            || row + 2 >= static_cast<int>(kBoardRows)) {
            continue;
        }
        bool available = true;
        for (int objectColumn = column;
             available && objectColumn <= column + 2; ++objectColumn) {
            for (int objectRow = row; objectRow <= row + 2; ++objectRow) {
                if (level_[objectColumn][objectRow] != 0
                    || (objectColumn == static_cast<int>(playerColumn_)
                        && objectRow == static_cast<int>(playerRow_))) {
                    available = false;
                    break;
                }
            }
        }
        if (!available) continue;
        constexpr std::uint16_t objectTiles[3][3] = {
            {8, 9, 10}, {11, 0x18, 12}, {13, 14, 15}};
        for (int offsetRow = 0; offsetRow < 3; ++offsetRow) {
            for (int offsetColumn = 0; offsetColumn < 3; ++offsetColumn) {
                level_[column + offsetColumn][row + offsetRow] =
                    objectTiles[offsetRow][offsetColumn];
                if (showConstructionEffects) {
                    overlays_.push_back({0x16,
                        static_cast<std::uint16_t>(column + offsetColumn),
                        static_cast<std::uint16_t>(row + offsetRow)});
                }
            }
        }
        if (showConstructionEffects) QueueSound(GameplaySound::Construction);
        return true;
    }
    return false;
}

void GameData::ActivateLevelSixteenRegion(std::size_t left, std::size_t top,
                                          std::size_t right, std::size_t bottom) noexcept {
    // FUN_1010_3ece turns the dormant alien 0x5d/0x5e terrain back into the
    // ordinary wall/empty pair, then runs FUN_1008_c662 only over that region.
    for (std::size_t column = left; column <= right; ++column) {
        for (std::size_t row = top; row <= bottom; ++row) {
            if (level_[column][row] == 0x5e) level_[column][row] = 0;
            else if (level_[column][row] == 0x5d) level_[column][row] = 3;
        }
    }
    const auto isWall = [this](int column, int row) {
        return column >= 0 && row >= 0 && column < static_cast<int>(kBoardColumns)
            && row < static_cast<int>(kBoardRows)
            && (level_[static_cast<std::size_t>(column)][static_cast<std::size_t>(row)] == 3
                || (level_[static_cast<std::size_t>(column)][static_cast<std::size_t>(row)] >= 0x36
                    && level_[static_cast<std::size_t>(column)][static_cast<std::size_t>(row)] <= 0x40));
    };
    constexpr std::array<std::uint16_t, 11> wallTiles = {
        0x36, 0x37, 0x38, 0x39, 0x3b, 0x3a, 0x3c, 0x3d, 0x3e, 0x3f, 0x40};
    for (std::size_t column = left; column <= right; ++column) {
        for (std::size_t row = top; row <= bottom; ++row) {
            if (level_[column][row] != 3) continue;
            std::array<bool, 11> candidate{};
            candidate.fill(true);
            const auto disable = [&candidate](std::initializer_list<int> indices) {
                for (const int index : indices) candidate[static_cast<std::size_t>(index)] = false;
            };
            if (isWall(static_cast<int>(column) + 1, static_cast<int>(row))) disable({1, 3, 4, 7});
            else disable({2, 5, 6, 8, 9, 10});
            if (isWall(static_cast<int>(column) - 1, static_cast<int>(row))) disable({1, 2, 5, 6});
            else disable({3, 4, 7, 8, 9, 10});
            if (isWall(static_cast<int>(column), static_cast<int>(row) + 1)) disable({0, 4, 5, 8});
            else disable({2, 3, 6, 7, 9, 10});
            if (isWall(static_cast<int>(column), static_cast<int>(row) - 1)) disable({0, 2, 3, 9});
            else disable({4, 5, 6, 7, 8, 10});
            const auto selected = std::find(candidate.begin(), candidate.end(), true);
            if (selected != candidate.end()) {
                level_[column][row] = wallTiles[static_cast<std::size_t>(
                    std::distance(candidate.begin(), selected))];
            }
        }
    }
}

void GameData::DetectBoardTransition() noexcept {
    const std::uint16_t tile = level_[playerColumn_][playerRow_];
    // ba30 dispatches ALL blue drains through +18 (b882), independently
    // of the class-specific +14 down-drain callback. Only 1058 overrides it.
    if (tile >= 0x23 && tile <= 0x26) {
        if (levelNumber_ == 5) {
            if (tile == 0x23) pendingTransition_ = 6;
            else if (tile == 0x24) pendingTransition_ = 7;
        } else if (!rootBoard_) {
            pendingTransition_ = -1;
        }
        return;
    }
    if (levelNumber_ == 3) {
        switch (tile) {
        case 0x27: pendingTransition_ = 6; break;
        case 0x28: pendingTransition_ = 7; break;
        case 0x29: pendingTransition_ = 2; break;
        case 0x2a: pendingTransition_ = 11; break;
        default: break;
        }
    } else if (levelNumber_ == 6) {
        if (tile == 0x27) {
            pendingTransition_ = 5;
        } else if (tile == 0x28) {
            pendingTransition_ = 4;
        }
    } else if (levelNumber_ == 7) {
        if (tile == 0x27) {
            pendingTransition_ = 5;
        } else if (tile == 0x28) {
            // 1010:6e3c constructs the Power Plant via 1008:e07a.
            pendingTransition_ = 1;
        }
    } else if (levelNumber_ == 5) {
        if (tile == 0x27) {
            // 1010:582e constructs the c80e board.
            pendingTransition_ = 8;
        }
    } else if (levelNumber_ == 4 && tile == 0x27) {
        // FUN_1010_5062 lazily constructs FUN_1010_33b4 for selector-1050's exit.
        pendingTransition_ = 10;
    } else if (levelNumber_ == 8 && tile == 0x27) {
        // FUN_1010_c8cc lazily constructs the selector-cb9c child.
        pendingTransition_ = 11;
    } else if (levelNumber_ == 11) {
        if (tile == 0x27) {
            // FUN_1010_ce62 caches one randomly selected child for this exit.
            // Its rare first branch creates c80e; otherwise FUN_1008_2b54
            // allocates final-arena/maze children once globally, or another
            // cb9c board is created recursively.
            std::uint8_t targetCode = levelElevenRuntimeFlags_ >> 4u;
            if (targetCode == 0) {
                int target = 0;
                if (NextRandom(8) == 8) {
                    target = 8;
                    targetCode = 1;
                } else if (NextRandom(3) == 2) {
                    const std::uint8_t usedFactories = genericRuntimeFlags_ & 0x30u;
                    if (usedFactories != 0x30u) {
                        int factory = 0;
                        do {
                            factory = NextRandom(2) - 1;
                        } while ((genericRuntimeFlags_ & (0x10u << factory)) != 0);
                        genericRuntimeFlags_ |= static_cast<std::uint8_t>(0x10u << factory);
                        target = factory == 0 ? 12 : 13;
                        targetCode = static_cast<std::uint8_t>(factory == 0 ? 3 : 4);
                    }
                } else {
                    target = 11;
                    targetCode = 2;
                }
                // ce62 falls back to another recursive board when 2b54 finds
                // that both one-time factories have already been selected.
                if (target == 0) { target = 11; targetCode = 2; }
                levelElevenRuntimeFlags_ = static_cast<std::uint8_t>(
                    (levelElevenRuntimeFlags_ & 0x0fu) | (targetCode << 4u));
                pendingTransition_ = target;
            } else {
                constexpr int targets[] = {0, 8, 11, 12, 13};
                pendingTransition_ = targets[targetCode];
            }
        }
    } else if (levelNumber_ == 12 && tile == 0x27) {
        // FUN_1010_d5d4 creates a selector-cb9c child from the final arena.
        pendingTransition_ = 11;
    } else if (levelNumber_ >= 13 && levelNumber_ <= 15) {
        if (tile == 0x27) {
            pendingTransition_ = static_cast<int>(levelNumber_ + 1);
        } else if (tile == 0x28 && levelNumber_ == 13) {
            pendingTransition_ = 11;
        }
    } else if (levelNumber_ >= 17 && levelNumber_ <= 25) {
        if (tile == 0x27) {
            const int depth = static_cast<int>(levelNumber_ - 16);
            if (depth < 5) {
                pendingTransition_ = static_cast<int>(levelNumber_ + 1);
            } else if (depth <= 8) {
                // The original checks the cached child before drawing this
                // destination roll. ApplicationState owns that cache.
                pendingTransition_ = kDeferredProceduralExit;
            } else {
                pendingTransition_ = 3;
            }
        } else if (tile >= 0x28 && tile <= 0x2a) {
            pendingTransition_ = -1;
        }
    }
}

std::size_t GameData::LevelOneTargetAt(int column, int row) const noexcept {
    for (std::size_t id = 0; id < 2; ++id)
        if (!levelOneTargetDefeated_[id]
            && levelOneTargetColumns_[id] == column && levelOneTargetRows_[id] == row) return id;
    return 2;
}

void GameData::ReconcileLevelOneTargets() noexcept {
    if (levelNumber_ != 1) return;
    const auto isTarget = [this](int x, int y) {
        const auto tile = level_[x][y];
        return tile == 0x87 || tile == 0x90 || tile == 0x99 || tile == 0xa2;
    };
    std::array<bool, 2> retained{};
    for (std::size_t id = 0; id < 2; ++id) {
        retained[id] = levelOneTargetDefeated_[id]
            || isTarget(levelOneTargetColumns_[id], levelOneTargetRows_[id]);
        const auto tile = level_[levelOneTargetColumns_[id]][levelOneTargetRows_[id]];
        if (tile == 0x99 || tile == 0xa2) levelOneTargetDefeated_[id] = 1;
    }
    // Only migration/legacy edited boards need discovery. Normal movement
    // updates coordinates in place, preserving identity even after crossing.
    for (int y = 0; y <= 33; ++y) for (int x = 0; x <= 37; ++x) {
        if (!isTarget(x, y)) continue;
        const auto existing = LevelOneTargetAt(x, y);
        if (existing != 2 && retained[existing]) continue;
        std::size_t id = y < 18 ? 0 : 1;
        if (retained[id]) id = 1 - id;
        if (retained[id]) continue;
        levelOneTargetColumns_[id] = static_cast<std::uint16_t>(x);
        levelOneTargetRows_[id] = static_cast<std::uint16_t>(y);
        if (level_[x][y] == 0x99 || level_[x][y] == 0xa2) levelOneTargetDefeated_[id] = 1;
        retained[id] = true;
    }
}

void GameData::AdvanceBoardRuntime(bool movementAction, int deltaColumn, int deltaRow,
                                   bool preActorPhase) noexcept {
    AdvanceBoardRuntimeCore(movementAction, deltaColumn, deltaRow, preActorPhase);
    // Every derived +0x1c callback ends in common ba30: its turn increment
    // precedes the aacc mine pass, and room-specific gates see the pre-blast
    // board until the next callback.
    if (!preActorPhase) ResolveTriggeredMines();
}

void GameData::AdvanceBoardRuntimeCore(bool movementAction, int deltaColumn,
                                       int deltaRow, bool preActorPhase) noexcept {
    if (levelNumber_ == 1) {
        ReconcileLevelOneTargets();
        if (!preActorPhase) {
            // The unlisted routine beginning at 1008:e636 is selector 1038's
            // post-actor callback. The two 0x90..0x98 blocks stay dormant until
            // the Energy Sphere is gone. On five-turn boundaries the upper
            // target gets its one-in-two wake-up roll; the lower one can do so
            // only twelve turns after the upper target first woke.
            if (!levelOneSecurityGateOpened_ && level_[23][29] != 0x61) {
                // Security-door breach releases the right-half defenders and
                // raises spawn pressure independently of corner activation.
                for (BoardActor& actor : actors_) {
                    if (actor.column >= 20 && actor.column <= 39
                        && actor.row <= 35 && actor.behavior >= 1 && actor.behavior <= 3) {
                        actor.behavior = 0;
                    }
                }
                levelOneSecurityGateOpened_ = true;
                levelOneSpawnChance_ = static_cast<std::uint16_t>(difficulty_ + 9);
                levelOneMaximumGroup_ = static_cast<std::uint16_t>(std::max(difficulty_ + 2, 4));
            }
            if (!levelOneCornerPortalsActivated_ && level_[21][13] != 0x5c) {
                // cf50 forcibly reconstructs each complete 3x3 active portal,
                // including damaged corners, and emits construction effects.
                constexpr std::uint16_t portal[3][3] = {
                    {0x10, 0x11, 0x12}, {0x13, 0x1d, 0x14}, {0x15, 0x16, 0x17}};
                for (const auto [originColumn, originRow] :
                     {std::pair{2, 2}, std::pair{2, 22},
                      std::pair{22, 22}, std::pair{22, 2}}) {
                    for (int offsetColumn = 0; offsetColumn < 3; ++offsetColumn) {
                        for (int offsetRow = 0; offsetRow < 3; ++offsetRow) {
                            const int column = originColumn + offsetColumn;
                            const int row = originRow + offsetRow;
                            level_[column][row] = portal[offsetRow][offsetColumn];
                            overlays_.push_back({0x16,
                                static_cast<std::uint16_t>(column),
                                static_cast<std::uint16_t>(row)});
                        }
                    }
                }
                QueueSound(GameplaySound::Construction);
                levelOneCornerPortalsActivated_ = true;
                levelOneMaximumGroup_ = static_cast<std::uint16_t>(difficulty_ + 5);
            }

            if (playerColumn_ == 1 && playerRow_ == 19 && !levelOneArenaEntered_) {
                // The arena entrance branch paints a square containment ring,
                // freezes actors already inside it, and changes the structure
                // spawn parameters used on following turns.
                for (int column = 10; column <= 15; ++column) {
                    level_[column][8] = 0x22;
                    level_[column][17] = 0x22;
                    overlays_.push_back({0x16, static_cast<std::uint16_t>(column), 8});
                    overlays_.push_back({0x16, static_cast<std::uint16_t>(column), 17});
                }
                for (int row = 10; row <= 15; ++row) {
                    level_[8][row] = 0x22;
                    level_[17][row] = 0x22;
                    overlays_.push_back({0x16, 8, static_cast<std::uint16_t>(row)});
                    overlays_.push_back({0x16, 17, static_cast<std::uint16_t>(row)});
                }
                for (BoardActor& actor : actors_) {
                    if (actor.column >= 8 && actor.column <= 17
                        && actor.row >= 8 && actor.row <= 17
                        && actor.behavior >= 1 && actor.behavior <= 3) {
                        actor.behavior = 0;
                    }
                }
                levelOneArenaEntered_ = true;
                levelOneSpawnChance_ = static_cast<std::uint16_t>(difficulty_ * 2 + 3);
                levelOneMaximumGroup_ = 1;
                QueueSound(GameplaySound::Construction);
            }

            const bool energySphereDestroyed = energySphereDestroyed_;
            const bool playerOutsideArena = playerColumn_ < 8 || playerColumn_ > 17
                || playerRow_ < 8 || playerRow_ > 17;
            if (levelOneArenaEntered_ && !energySphereDestroyed && playerOutsideArena
                && turnCounter_ % 20 == 0) {
                // Until the Sphere falls, e636 repairs every missing segment
                // of the containment ring on its recovered twenty-turn cadence.
                for (int column = 10; column <= 15; ++column) {
                    if (level_[column][8] == 0) {
                        level_[column][8] = 0x22;
                        overlays_.push_back({0x16, static_cast<std::uint16_t>(column), 8});
                    }
                    if (level_[column][17] == 0) {
                        level_[column][17] = 0x22;
                        overlays_.push_back({0x16, static_cast<std::uint16_t>(column), 17});
                    }
                }
                for (int row = 10; row <= 15; ++row) {
                    if (level_[8][row] == 0) {
                        level_[8][row] = 0x22;
                        overlays_.push_back({0x16, 8, static_cast<std::uint16_t>(row)});
                    }
                    if (level_[17][row] == 0) {
                        level_[17][row] = 0x22;
                        overlays_.push_back({0x16, 17, static_cast<std::uint16_t>(row)});
                    }
                }
            }
            if (levelOneFirstTargetActivationTurn_ == 0) {
                const auto firstTile = level_[levelOneTargetColumns_[0]][levelOneTargetRows_[0]];
                const bool upperTargetWasActivated = firstTile == 0x87 || firstTile == 0x99;
                if (upperTargetWasActivated) {
                    levelOneFirstTargetActivationTurn_ = turnCounter_ > 12
                        ? turnCounter_ - 12 : std::max<std::uint32_t>(turnCounter_, 1);
                }
            }
            if (energySphereDestroyed && turnCounter_ % 5 == 0) {
                for (int column = 0; column + 2 < static_cast<int>(kBoardColumns); ++column) {
                    for (int row = 0; row + 2 < static_cast<int>(kBoardRows); ++row) {
                        if (level_[column][row] != 0x90) continue;
                        const std::size_t targetIndex = LevelOneTargetAt(column, row);
                        if (targetIndex == 2) continue;
                        // e9ec draws before testing the lower target's delay.
                        if (NextRandom(2) != 2) continue;
                        if (targetIndex == 1
                            && (levelOneFirstTargetActivationTurn_ == 0
                                || turnCounter_ - levelOneFirstTargetActivationTurn_ < 12)) {
                            continue;
                        }
                        if (targetIndex == 0) {
                            levelOneFirstTargetActivationTurn_ = turnCounter_;
                        }
                        for (int targetColumn = column; targetColumn < column + 3; ++targetColumn) {
                            for (int targetRow = row; targetRow < row + 3; ++targetRow) {
                                level_[targetColumn][targetRow] = static_cast<std::uint16_t>(
                                    level_[targetColumn][targetRow] - 9);
                                overlays_.push_back({0x16,
                                    static_cast<std::uint16_t>(targetColumn),
                                    static_cast<std::uint16_t>(targetRow)});
                            }
                        }
                        QueueSound(GameplaySound::WakeUp);
                    }
                }
            }

            if (!levelOneArenaEntered_) {
                // eae1 follows the lever and wake-up branches; the lever's
                // activation turn must not also turn the circulating actors.
                for (BoardActor& actor : actors_) {
                    if (actor.column == 10 && actor.row == 10) {
                        actor.deltaColumn = 1; actor.deltaRow = 0;
                    } else if (actor.column == 15 && actor.row == 10) {
                        actor.deltaColumn = 0; actor.deltaRow = 1;
                    } else if (actor.column == 15 && actor.row == 15) {
                        actor.deltaColumn = -1; actor.deltaRow = 0;
                    } else if (actor.column == 10 && actor.row == 15) {
                        actor.deltaColumn = 0; actor.deltaRow = -1;
                    }
                }
            }

            // Once either tracked 3x3 target has taken ten
            // hits it clears that target's active word, awards 50,000 points,
            // advances all nine target tiles by 0x12, and paints an explosion
            // on every square. If neither of its two active words remains, it
            // sets the application victory word at offset 0x100.
            for (int column = 0; column + 2 < static_cast<int>(kBoardColumns); ++column) {
                for (int row = 0; row + 2 < static_cast<int>(kBoardRows); ++row) {
                    if (level_[column][row] != 0x87 && level_[column][row] != 0x90) {
                        continue;
                    }
                    const std::size_t targetIndex = LevelOneTargetAt(column, row);
                    if (targetIndex == 2) continue;
                    if (levelOneTargetHits_[targetIndex] < 10) {
                        continue;
                    }
                    score_ += 50000;
                    levelOneTargetDefeated_[targetIndex] = 1;
                    for (int targetColumn = column; targetColumn < column + 3; ++targetColumn) {
                        for (int targetRow = row; targetRow < row + 3; ++targetRow) {
                            level_[targetColumn][targetRow] = static_cast<std::uint16_t>(
                                level_[targetColumn][targetRow] + 0x12);
                            overlays_.push_back({0x14,
                                static_cast<std::uint16_t>(targetColumn),
                                static_cast<std::uint16_t>(targetRow)});
                        }
                    }
                    QueueSound(GameplaySound::ExplosionLarge);
                }
            }
            if (levelOneTargetDefeated_[0] && levelOneTargetDefeated_[1]) {
                victoryTriggered_ = true;
            }
            return;
        }
        // FUN_1010_00ec operates only awakened selector-1038 targets. Dormant
        // 0x90..0x98 blocks remain fixed until FUN_1008_e636 converts them.
        std::vector<std::pair<int, int>> targets;
        std::vector<std::size_t> targetIds;
        for (std::size_t id = 0; id < 2; ++id) {
            const auto x = levelOneTargetColumns_[id], y = levelOneTargetRows_[id];
            if (!levelOneTargetDefeated_[id] && (level_[x][y] == 0x87 || level_[x][y] == 0x90)) {
                targets.emplace_back(x, y);
                targetIds.push_back(id);
            }
        }

        const auto targetDistance = [](int column, int row, int targetColumn, int targetRow) {
            // FUN_1010_0a12/0a7e compare against the target's center by
            // subtracting one before taking the absolute value.
            return std::max(std::abs(column - targetColumn - 1),
                            std::abs(row - targetRow - 1)) - 1;
        };
        const auto addActor = [this](std::uint16_t type, int column, int row) {
            if (actors_.size() < 300) {
                actors_.push_back({type, static_cast<std::uint16_t>(column),
                                   static_cast<std::uint16_t>(row), 0, 0, 0});
            }
        };
        for (std::size_t targetIndex = 0; targetIndex < targets.size(); ++targetIndex) {
            int targetColumn = targets[targetIndex].first;
            int targetRow = targets[targetIndex].second;
            // Dormant targets do not move, but still occupy their 3x3 area.
            if (level_[targetColumn][targetRow] != 0x87) continue;
            const int playerDistance = targetDistance(static_cast<int>(playerColumn_),
                static_cast<int>(playerRow_), targetColumn, targetRow);
            if (playerDistance == 1) {
                if (NextRandom(difficulty_ + 1) > 1) addActor(2, targetColumn, targetRow);
                if (NextRandom(difficulty_ + 1) > 1) addActor(2, targetColumn + 2, targetRow);
                if (NextRandom(difficulty_ + 1) > 1) addActor(2, targetColumn, targetRow + 2);
                if (NextRandom(difficulty_ + 1) > 1) addActor(2, targetColumn + 2, targetRow + 2);
                continue;
            }

            int nearestMissile = 1000;
            for (const Projectile& projectile : projectiles_) {
                if (projectile.type == 2) {
                    nearestMissile = std::min(nearestMissile,
                        targetDistance(projectile.column, projectile.row, targetColumn, targetRow));
                }
            }
            if (NextRandom(50) == 50
                || (nearestMissile < 2 && NextRandom(difficulty_ + 1) > 1)) {
                for (int row = 0; row < 3; ++row) {
                    for (int column = 0; column < 3; ++column) {
                        if (column == 1 && row == 1) continue;
                        const bool corner = column != 1 && row != 1;
                        const std::uint16_t type = NextRandom(2) == 1
                            ? static_cast<std::uint16_t>(corner ? 2 : 1) : 0;
                        const int quantity = NextRandom(1);
                        for (int actor = 0; actor < quantity; ++actor)
                            addActor(type, targetColumn + column, targetRow + row);
                    }
                }
                continue;
            }
            if (NextRandom(3) != 1) continue;

            for (int column = targetColumn; column < targetColumn + 3; ++column) {
                for (int row = targetRow; row < targetRow + 3; ++row) {
                    level_[column][row] = 0;
                }
            }
            int targetDeltaColumn = static_cast<int>(playerColumn_) > targetColumn + 1 ? 1
                : (static_cast<int>(playerColumn_) < targetColumn + 1 ? -1 : 0);
            int targetDeltaRow = static_cast<int>(playerRow_) > targetRow + 1 ? 1
                : (static_cast<int>(playerRow_) < targetRow + 1 ? -1 : 0);
            const auto movingId = targetIds[targetIndex];
            const auto overlapsOtherTarget = [this, movingId](int column, int row) {
                for (std::size_t other = 0; other < levelOneTargetColumns_.size(); ++other) {
                    // 1010:09c0 retains the other boss's last coordinates even
                    // after defeat; its wreckage still blocks this 3x3 body.
                    if (other != movingId
                        && std::abs(static_cast<int>(levelOneTargetColumns_[other]) - column) < 3
                        && std::abs(static_cast<int>(levelOneTargetRows_[other]) - row) < 3)
                        return true;
                }
                return false;
            };
            if (overlapsOtherTarget(targetColumn + targetDeltaColumn,
                                    targetRow + targetDeltaRow)) {
                if (!overlapsOtherTarget(targetColumn, targetRow + targetDeltaRow)) {
                    targetDeltaColumn = 0;
                } else if (!overlapsOtherTarget(targetColumn + targetDeltaColumn, targetRow)) {
                    targetDeltaRow = 0;
                } else {
                    targetDeltaColumn = targetDeltaRow = 0;
                }
            }
            if (targetColumn + targetDeltaColumn < 0
                || targetColumn + targetDeltaColumn > 37) targetDeltaColumn = 0;
            if (targetRow + targetDeltaRow < 0
                || targetRow + targetDeltaRow > 33) targetDeltaRow = 0;
            targetColumn += targetDeltaColumn;
            targetRow += targetDeltaRow;
            targets[targetIndex] = {targetColumn, targetRow};
            const auto bossId = targetIds[targetIndex];
            levelOneTargetColumns_[bossId] = static_cast<std::uint16_t>(targetColumn);
            levelOneTargetRows_[bossId] = static_cast<std::uint16_t>(targetRow);

            for (int column = targetColumn; column < targetColumn + 3; ++column) {
                for (int row = targetRow; row < targetRow + 3; ++row) {
                    const std::uint16_t tile = level_[column][row];
                    if (tile < tiles_.size()) {
                        const std::uint16_t flags = tiles_[tile].flags;
                        if ((flags & 2u) != 0 && (flags & 4u) == 0) {
                            DamageBoardSquare(column, row, 3);
                        }
                    }
                    level_[column][row] = static_cast<std::uint16_t>(
                        0x87 + (row - targetRow) * 3 + column - targetColumn);
                }
            }

            for (auto projectile = projectiles_.begin(); projectile != projectiles_.end();) {
                const bool inside = projectile->column >= targetColumn
                    && projectile->column <= targetColumn + 2
                    && projectile->row >= targetRow && projectile->row <= targetRow + 2;
                const bool adjacentCharge = !inside && projectile->type == 3
                    && projectile->column >= targetColumn - 1
                    && projectile->column <= targetColumn + 3
                    && projectile->row >= targetRow - 1
                    && projectile->row <= targetRow + 3
                    && NextRandom(difficulty_ * 2) > 1;
                if (!inside && !adjacentCharge) {
                    ++projectile;
                    continue;
                }
                if (projectile->type == 2 && inside) {
                    overlays_.push_back({0x14,
                        static_cast<std::uint16_t>(projectile->column),
                        static_cast<std::uint16_t>(projectile->row)});
                    const std::size_t hitTarget = bossId;
                    if (levelOneTargetHits_[hitTarget] < 9) score_ += 2500;
                    ++levelOneTargetHits_[hitTarget];
                    QueueSound(GameplaySound::BoardBreak);
                } else if (adjacentCharge) {
                    overlays_.push_back({0x14,
                        static_cast<std::uint16_t>(projectile->column),
                        static_cast<std::uint16_t>(projectile->row)});
                }
                projectile = projectiles_.erase(projectile);
            }
        }
        ++turnCounter_;
        return;
    }
    if (preActorPhase) return;
    if (levelNumber_ == 2) {
        MaintainCasinoStructures();
        // 1010:1180 advances the existing machine cycle before the player's
        // interaction; a winning hand can invoke 28ac again on this turn.
        AdvanceCasinoRetaliation(0);
        // FUN_1010_1180 operates the casino only after actual movement. During
        // the dealt phase, the five controls at row 23 toggle held cards.
        if (movementAction && (deltaColumn != 0 || deltaRow != 0)
            && casinoStatus_ == 2 && playerRow_ == 23) {
            for (std::size_t card = 0; card < casinoHeld_.size(); ++card) {
                if (playerColumn_ != (card + 6) * 3) continue;
                casinoHeld_[card] = casinoHeld_[card] == 0 ? 1 : 0;
                pendingMessages_.emplace_back(casinoHeld_[card] != 0 ? L"Card is Held" : L"Card is Unheld");
                break;
            }
        }

        // Pulling the active lever at (26,26) creates a ten-card, no-duplicate
        // deck through FUN_1010_1cbc and deals its first five cards. Card IDs
        // are zero based; rank and suit use the original one-based forms.
        if (movementAction && (deltaColumn != 0 || deltaRow != 0)
            && playerColumn_ == 26 && playerRow_ == 26 && casinoStatus_ == 1) {
            std::array<bool, 52> usedCards{};
            for (std::uint16_t& card : casinoDeck_) {
                do {
                    card = static_cast<std::uint16_t>(NextRandom(52) - 1);
                } while (usedCards[card]);
                usedCards[card] = true;
            }
            casinoDeckIndex_ = 0;
            casinoHeld_.fill(0);
            for (std::size_t card = 0; card < casinoRanks_.size(); ++card) {
                const std::uint16_t encoded = casinoDeck_[casinoDeckIndex_++];
                casinoRanks_[card] = static_cast<std::uint16_t>(encoded % 13 + 1);
                casinoSuits_[card] = static_cast<std::uint16_t>(encoded / 13 + 1);
                overlays_.push_back({0x1e, static_cast<std::uint16_t>(card * 3 + 17), 17, 1});
                overlays_.push_back({0x2b, static_cast<std::uint16_t>((card + 6) * 3), 23});
            }
            casinoStatus_ = 2;
            SetCasinoAnnouncement(1);
            pendingMessages_.emplace_back(L"Your cards, sir...");
            pendingMessages_.emplace_back(L"Move onto the red boxes to hold cards. "
                              L"Pull the lever when you are ready to draw.");
            QueueSound(GameplaySound::CasinoDraw);
        } else if (movementAction && (deltaColumn != 0 || deltaRow != 0)
            && playerColumn_ == 26 && playerRow_ == 26 && casinoStatus_ == 2) {
            for (std::size_t card = 0; card < casinoRanks_.size(); ++card) {
                if (casinoHeld_[card] != 0) continue;
                const std::uint16_t encoded = casinoDeck_[casinoDeckIndex_++];
                casinoRanks_[card] = static_cast<std::uint16_t>(encoded % 13 + 1);
                casinoSuits_[card] = static_cast<std::uint16_t>(encoded / 13 + 1);
                overlays_.push_back({0x1e, static_cast<std::uint16_t>(card * 3 + 17), 17, 1});
            }
            casinoHeld_.fill(0);

            std::array<int, 14> rankCounts{};
            std::array<int, 5> suitCounts{};
            int wilds = 0;
            for (std::size_t card = 0; card < casinoRanks_.size(); ++card) {
                if (casinoWildRank_ != 0 && casinoRanks_[card] == casinoWildRank_) {
                    ++wilds;
                } else {
                    ++rankCounts[casinoRanks_[card]];
                    ++suitCounts[casinoSuits_[card]];
                }
            }
            // 20c2 treats Ace as both the first and last straight endpoint.
            const auto isStraight = [&rankCounts, wilds]() {
                std::array<bool, 14> present{};
                for (int rank = 1; rank <= 13; ++rank)
                    present[rank - 1] = rankCounts[rank] != 0;
                present[13] = present[0];
                const auto check = [&present, wilds](int first, int last) {
                    int missing = 0;
                    for (int index = first + 1; index <= first + 4; ++index)
                        missing += index > last || !present[index];
                    return missing <= wilds;
                };
                int first = 0;
                while (first < 13 && !present[first]) ++first;
                if (first == 13) return false;
                if (check(first, 12)) return true;
                if (first != 0) return false;
                first = 1;
                while (first < 14 && !present[first]) ++first;
                return first < 14 && check(first, 13);
            };
            const bool ordinaryTable = casinoWildRank_ == 0;
            std::wstring handName;
            int multiplier = 0;
            const bool flush = *std::max_element(suitCounts.begin(), suitCounts.end()) + wilds >= 5;
            const bool straight = isStraight();
            int lowestNonAce = 255;
            for (int rank = 2; rank <= 13; ++rank)
                if (rankCounts[rank] != 0) lowestNonAce = std::min(lowestNonAce, rank);
            const bool royalFlush = straight && flush && lowestNonAce >= 10;
            enum class Group { None, Pair, TwoPair, Three, Full, Four, Five };
            Group group = Group::None;
            bool pair = false, triple = false, highPair = false;
            for (int rank = 1; rank <= 13; ++rank) {
                if (rank == casinoWildRank_) continue;
                const int count = rankCounts[rank];
                if (count + wilds == 5) { group = Group::Five; break; }
                if (count + wilds == 4) { group = Group::Four; break; }
                if (count == 3) {
                    if (pair) { group = Group::Full; break; }
                    triple = true;
                } else if (count == 2) {
                    if (triple || (wilds > 0 && pair)) { group = Group::Full; break; }
                    if (wilds == 0 && pair) { group = Group::TwoPair; break; }
                    highPair = highPair || rank == 1 || rank >= 11;
                    pair = true;
                }
            }
            if (group == Group::None)
                group = triple || (pair && wilds == 1) || wilds == 2
                    ? Group::Three : highPair ? Group::Pair : Group::None;
            if (royalFlush) { handName = L"Royal Flush"; multiplier = ordinaryTable ? 200 : 25; }
            else if (group == Group::Five) { handName = L"5-of-a-kind"; multiplier = 25; }
            else if (straight && flush) { handName = L"Straight Flush"; multiplier = ordinaryTable ? 100 : 20; }
            else if (group == Group::Four) { handName = L"4-of-a-kind"; multiplier = ordinaryTable ? 40 : 12; }
            else if (group == Group::Full) { handName = L"Full House"; multiplier = ordinaryTable ? 12 : 6; }
            else if (flush) {
                handName = L"Flush"; multiplier = ordinaryTable ? 10 : 5;
            } else if (straight) { handName = L"Straight"; multiplier = ordinaryTable ? 8 : 4; }
            else if (group == Group::Three) { handName = L"3-of-a-kind"; multiplier = ordinaryTable ? 5 : 2; }
            else if (group == Group::TwoPair) { handName = L"2-Pair"; multiplier = ordinaryTable ? 2 : 1; }
            else if (group == Group::Pair) { handName = L"Pair"; multiplier = ordinaryTable ? 1 : 0; }

            overlays_.erase(std::remove_if(overlays_.begin(), overlays_.end(),
                [](const BoardOverlay& overlay) { return overlay.type == 0x2b; }), overlays_.end());
            for (BoardOverlay& overlay : overlays_) {
                if (overlay.column == 26 && overlay.row == 26 && overlay.type == 0x2d) {
                    overlay.type = 0x2c;
                    break;
                }
            }
            if (multiplier > 0) {
                const std::uint16_t winnings = static_cast<std::uint16_t>(casinoBetCount_ * multiplier);
                AddInventoryItem(casinoBetItemType_, winnings);
                pendingMessages_.emplace_back(L"*** WINNER ***\n" + handName + L"!\nReturns "
                    + std::to_wstring(winnings) + L" on a bet of "
                    + std::to_wstring(casinoBetCount_) + L".");
                QueueSound(GameplaySound::CasinoWin);
                SetCasinoAnnouncement(2);

                // FUN_1010_1180 calls FUN_1010_28ac only for returns above
                // one-to-one. Its mode begins at one, rises with a large bet,
                // an eight-times hand, and the elapsed board-turn thresholds,
                // then caps at three.
                if (multiplier > 1) {
                    int retaliationMode = casinoBetCount_ > 8 ? 2 : 1;
                    if (multiplier >= 8) ++retaliationMode;
                    if (turnCounter_ > 500) ++retaliationMode;
                    if (turnCounter_ > 1500) ++retaliationMode;
                    AdvanceCasinoRetaliation(std::min(retaliationMode, 3));
                    if (levelTwoSequenceMode_ < 3) {
                        pendingMessages_.emplace_back(NextRandom(2) == 1
                            ? L"Here come the pit bosses!"
                            : L"You hear angry voices.");
                    } else {
                        pendingMessages_.emplace_back(NextRandom(2) == 1
                            ? L"Here come the pit bosses and the IRS!"
                            : L"Here comes the Mob!");
                    }
                }

                // A twelve-times or better return advances the casino's exact
                // three-step bonus progression at object offset +e4.
                if (multiplier >= 12) {
                    if (casinoBonusStage_ == 0) {
                        level_[13][19] = 0x5c;
                        overlays_.push_back({0x16, 13, 19});
                        pendingMessages_.emplace_back(L"You won a bonus prize!\n(Left of table.)");
                        casinoBonusStage_ = 1;
                    } else if (casinoBonusStage_ == 1) {
                        level_[35][19] = 0x5c;
                        overlays_.push_back({0x16, 35, 19});
                        pendingMessages_.emplace_back(L"You won a bonus prize!\n(Right of table.)");
                        casinoBonusStage_ = 2;
                    } else if (casinoBonusStage_ == 2) {
                        casinoBonusStage_ = 3;
                        casinoStatus_ = 4;
                        SetCasinoAnnouncement(3);
                        pendingMessages_.emplace_back(L"BONUS! You have gained access to the treasure room.");
                        pendingMessages_.emplace_back(L"You're too good.\nCasino is closed...");
                        for (int column : {7, 8, 9, 32, 33, 34}) {
                            for (int row = 2; row <= 4; ++row) {
                                level_[column][row] = 0;
                                overlays_.push_back({0x14, static_cast<std::uint16_t>(column),
                                                     static_cast<std::uint16_t>(row)});
                            }
                        }
                    }
                }
            } else {
                pendingMessages_.emplace_back(L"Game Over. Play Again.");
                QueueSound(GameplaySound::CasinoLose);
                SetCasinoAnnouncement(4);
            }
            if (casinoStatus_ != 4) {
                casinoStatus_ = 3;
                casinoResultTicks_ = 0;
            }
        }

        ++turnCounter_;
        return;
    }
    if (levelNumber_ == 12) {
        RefreshFinalArenaSpawnParameters();
        ++turnCounter_;
        return;
    }
    if (levelNumber_ == 3) {
        // FUN_1010_4692 replenishes a destroyed 0x18 structure every
        // (6-difficulty)*10 turns.
        const auto interval = static_cast<std::uint32_t>((6 - difficulty_) * 10);
        if (turnCounter_ % interval == 0u) {
            PlaceLevelThreeStructure(true);
        }
        // 1010:4692 updates +0x4a after the common actor turn. The lower
        // sixth-turn value must not alter the spawn roll already made above.
        if (turnCounter_ == 6)
            levelThreeSpawnChance_ = static_cast<std::uint16_t>(difficulty_ + 4);
        // Four FUN_1008_cdbe calls turn the vector-driven perimeter actors at
        // the corners. They form a clockwise loop instead of becoming hunters
        // when their original straight-line vector reaches the next wall.
        for (BoardActor& actor : actors_) {
            if (actor.column == 28 && actor.row == 24) {
                actor.deltaColumn = 1;
                actor.deltaRow = 0;
            } else if (actor.column == 39 && actor.row == 24) {
                actor.deltaColumn = 0;
                actor.deltaRow = 1;
            } else if (actor.column == 39 && actor.row == 35) {
                actor.deltaColumn = -1;
                actor.deltaRow = 0;
            } else if (actor.column == 28 && actor.row == 35) {
                actor.deltaColumn = 0;
                actor.deltaRow = -1;
            }
        }
        // Standing on the lever at (38,19) while the lower mechanism is
        // closed executes the recovered one-shot machinery branch. It opens
        // the chamber, redraws its eight exposed edge cells, activates the
        // lower structure and releases three difficulty-scaled actor groups.
        if (playerColumn_ == 38 && playerRow_ == 19 && level_[30][28] == 0x38) {
            for (std::size_t column = 30; column <= 31; ++column) {
                for (std::size_t row = 28; row <= 31; ++row) {
                    level_[column][row] = 0;
                }
            }
            level_[31][28] = 0x3b;
            level_[30][28] = 0x36;
            level_[29][28] = 0x3a;
            level_[29][27] = 0x37;
            level_[31][31] = 0x39;
            level_[30][31] = 0x36;
            level_[29][31] = 0x38;
            level_[29][32] = 0x37;
            for (const auto [column, row] : {
                     std::pair{31, 28}, std::pair{30, 28}, std::pair{29, 28},
                     std::pair{29, 27}, std::pair{31, 31}, std::pair{30, 31},
                     std::pair{29, 31}, std::pair{29, 32}}) {
                overlays_.push_back({0x16, static_cast<std::uint16_t>(column),
                                     static_cast<std::uint16_t>(row)});
            }
            for (std::size_t column = 30; column <= 31; ++column) {
                for (std::size_t row = 29; row <= 30; ++row) {
                    overlays_.push_back({0x14, static_cast<std::uint16_t>(column),
                                         static_cast<std::uint16_t>(row)});
                }
            }
            for (std::size_t column = 30; column <= 34; ++column) {
                for (std::size_t row = 19; row <= 23; ++row) {
                    if (level_[column][row] == 0x5f) {
                        level_[column][row] = 0;
                        overlays_.push_back({0x14, static_cast<std::uint16_t>(column),
                                             static_cast<std::uint16_t>(row)});
                    }
                }
            }
            level_[32][21] = 0x18;
            const auto addActors = [this](std::uint16_t type, int count) {
                for (int index = 0; index < count && actors_.size() < 300; ++index) {
                    actors_.push_back({type, 32, 21, 0, 0, 0});
                }
            };
            addActors(0, NextRandom(10));
            addActors(1, NextRandom(4));
            addActors(2, NextRandom(difficulty_ * 2));
            QueueSound(GameplaySound::BoardBreak);
        }
        ++turnCounter_;
        return;
    }
    if (levelNumber_ == 6) {
        // FUN_1010_6308 is selector 1060's per-turn state machine. Its four
        // object flags occupy words 0x42..0x48 in the original class.
        const auto addActors = [this](std::uint16_t type, int column, int row, int count) {
            for (int index = 0; index < count && actors_.size() < 300; ++index) {
                actors_.push_back({type, static_cast<std::uint16_t>(column),
                                   static_cast<std::uint16_t>(row), 0, 0, 0});
            }
        };
        if (!levelSixStructuresSpawned_ && turnCounter_ > 1000 && NextRandom(100) == 1) {
            levelSixStructuresSpawned_ = PlaceMoonRoomStructures();
        }
        if (!levelSixUpperTransporterTriggered_ && level_[18][11] != 0x7f) {
            addActors(2, 19, 9, NextRandom(5));
            addActors(2, 20, 10, NextRandom(5));
            addActors(2, 20, 11, NextRandom(5));
            levelSixUpperTransporterTriggered_ = true;
        }
        if (!levelSixLowerTransporterTriggered_ && level_[20][22] != 0x7f) {
            addActors(2, 22, 22, NextRandom(5));
            addActors(2, 21, 24, NextRandom(5));
            levelSixLowerTransporterTriggered_ = true;
        }
        if (!levelSixCenterOpened_ && level_[10][17] != 0x59) {
            level_[18][15] = 0x1b;
            level_[18][20] = 0x1b;
            levelSixCenterOpened_ = true;
        }
        const auto breakSeal = [this](int row) {
            if (level_[32][row] != 0x37) return;
            level_[32][row] = 0x5c;
            overlays_.push_back({0x16, 32, static_cast<std::uint16_t>(row)});
            QueueSound(GameplaySound::Construction);
        };
        if (playerColumn_ == 25 && playerRow_ == 24) breakSeal(23);
        if (playerColumn_ == 25 && playerRow_ == 9) breakSeal(10);

        const auto releasePair = [this, &addActors](int row) {
            // 1010:65e9/673b: intact walls (0x37) and switch-opened
            // seals (0x5c) both suppress the trap. Only destroying the seal
            // releases it. The lower chamber tests its bottom-left wall.
            const int sentinelRow = row == 10 ? row - 1 : row + 1;
            if (level_[32][row] == 0x5c || level_[32][row] == 0x37
                || level_[31][sentinelRow] != 0x36) return;
            for (const int actorRow : {row - 1, row + 1}) {
                for (const int column : {31, 32}) {
                    level_[column][actorRow] = 0;
                    addActors(3, column, actorRow, 1);
                    overlays_.push_back({0x14, static_cast<std::uint16_t>(column),
                                         static_cast<std::uint16_t>(actorRow)});
                }
            }
            QueueSound(GameplaySound::ExplosionSmall);
        };
        releasePair(10);
        releasePair(23);
        ++turnCounter_;
        return;
    }
    if (levelNumber_ == 7) {
        constexpr std::array<std::pair<int, int>, 4> structureCenters = {
            std::pair{9, 8}, std::pair{9, 30}, std::pair{31, 8}, std::pair{31, 30}};
        const auto reduceSpawnDivisor = [this]() {
            switch (levelSevenSpawnDivisor_) {
            case 25: levelSevenSpawnDivisor_ = 10; break;
            case 10: levelSevenSpawnDivisor_ = 2; break;
            case 2: levelSevenSpawnDivisor_ = 1; break;
            case 1: levelSevenSpawnDivisor_ = 0; break;
            default: break;
            }
        };
        for (std::size_t index = 0; index < structureCenters.size(); ++index) {
            const auto [column, row] = structureCenters[index];
            if (levelSevenStructureActive_[index] && level_[column][row] != 0x1f) {
                reduceSpawnDivisor();
                levelSevenStructureActive_[index] = false;
            }
        }

        if (levelSevenSpawnDivisor_ != 0 &&
            NextRandom(levelSevenSpawnDivisor_) == levelSevenSpawnDivisor_) {
            for (std::size_t index = 0; index < structureCenters.size(); ++index) {
                if (!levelSevenStructureActive_[index]) {
                    continue;
                }
                const auto [column, row] = structureCenters[index];
                const auto type = static_cast<std::uint16_t>(NextRandom(4) < 2);
                if (actors_.size() < 300)
                    actors_.push_back({type, static_cast<std::uint16_t>(column),
                                       static_cast<std::uint16_t>(row), 0, 0, 0});
            }
        }

        if (levelSevenSpawnDivisor_ == 0 && !levelSevenEndPhase_) {
            int attemptsRemaining = 200;
            for (int object = 0; object < 12 && attemptsRemaining >= 0; ++object) {
                bool placed = false;
                while (!placed && attemptsRemaining >= 0) {
                    const int column = NextRandom(36);
                    const int row = NextRandom(32);
                    if (static_cast<int>(playerColumn_) >= column &&
                        static_cast<int>(playerColumn_) <= column + 2 &&
                        static_cast<int>(playerRow_) >= row &&
                        static_cast<int>(playerRow_) <= row + 2) {
                        continue;
                    }
                    --attemptsRemaining;
                    if (column + 2 >= static_cast<int>(kBoardColumns) ||
                        row + 2 >= static_cast<int>(kBoardRows)) {
                        continue;
                    }
                    placed = true;
                    for (int objectRow = row; objectRow <= row + 2; ++objectRow) {
                        for (int objectColumn = column; objectColumn <= column + 2; ++objectColumn) {
                            placed = placed && level_[objectColumn][objectRow] == 0;
                        }
                    }
                    if (!placed) {
                        continue;
                    }
                    constexpr std::uint16_t objectTiles[3][3] = {
                        {8, 9, 10}, {11, 0x18, 12}, {13, 14, 15}};
                    for (int offsetRow = 0; offsetRow < 3; ++offsetRow) {
                        for (int offsetColumn = 0; offsetColumn < 3; ++offsetColumn) {
                            level_[column + offsetColumn][row + offsetRow] =
                                objectTiles[offsetRow][offsetColumn];
                            overlays_.push_back(
                                {0x16, static_cast<std::uint16_t>(column + offsetColumn),
                                 static_cast<std::uint16_t>(row + offsetRow)});
                        }
                    }
                }
            }
            levelSevenEndPhase_ = true;
        }
        ++turnCounter_;
        return;
    }

    if (levelNumber_ == 5) {
        // FUN_1010_5a46 toggles two machinery chambers only after actual movement.
        // Weapon turns and keypad-5 waits do not operate the floor switches.
        if (movementAction && (deltaColumn != 0 || deltaRow != 0)
            && playerColumn_ == 37 && playerRow_ == 2) {
            if (!levelFiveFirstSwitchActive_) {
                for (std::size_t column = 31; column <= 33; ++column) {
                    level_[column][3] = 0x36;
                    level_[column][4] = 0;
                    level_[column][5] = 0x36;
                }
                level_[32][3] = 0x3e;
                level_[32][5] = 0x3f;
            } else {
                // 1010:5b5a clears the rectangle (ce88), builds its framed
                // perimeter (cafa), then restores the two doorway markers.
                constexpr std::uint16_t restored[3][3] = {
                    {0x38, 0x3e, 0x39}, {0x37, 0, 0x37}, {0x3a, 0x3f, 0x3b}};
                for (std::size_t row = 3; row <= 5; ++row) {
                    for (std::size_t column = 31; column <= 33; ++column) {
                        level_[column][row] = restored[row - 3][column - 31];
                    }
                }
            }
            levelFiveFirstSwitchActive_ = !levelFiveFirstSwitchActive_;
            for (std::size_t column = 31; column <= 33; ++column) {
                for (std::size_t row = 3; row <= 5; ++row) {
                    overlays_.push_back({0x14, static_cast<std::uint16_t>(column),
                                         static_cast<std::uint16_t>(row)});
                }
            }
            QueueSound(GameplaySound::Construction);
        }
        if (movementAction && (deltaColumn != 0 || deltaRow != 0)
            && playerColumn_ == 37 && playerRow_ == 3) {
            if (!levelFiveSecondSwitchActive_) {
                for (std::size_t row = 6; row <= 8; ++row) {
                    level_[34][row] = 0x37;
                    level_[35][row] = 0;
                    level_[36][row] = 0x37;
                }
                level_[34][7] = 0x3d;
                level_[36][7] = 0x3c;
            } else {
                constexpr std::uint16_t restored[3][3] = {
                    {0x38, 0x36, 0x39}, {0x3d, 0, 0x3c}, {0x3a, 0x36, 0x3b}};
                for (std::size_t row = 6; row <= 8; ++row) {
                    for (std::size_t column = 34; column <= 36; ++column) {
                        level_[column][row] = restored[row - 6][column - 34];
                    }
                }
            }
            levelFiveSecondSwitchActive_ = !levelFiveSecondSwitchActive_;
            for (std::size_t column = 34; column <= 36; ++column) {
                for (std::size_t row = 6; row <= 8; ++row) {
                    overlays_.push_back({0x16, static_cast<std::uint16_t>(column),
                                         static_cast<std::uint16_t>(row)});
                }
            }
            QueueSound(GameplaySound::Construction);
        }

        // The constructor retains one random horizontal pair of ordinary wall
        // squares. FUN_1010_5a46 watches the pair as it is breached, then
        // activates the recovered 17-cell mechanism when the player reaches it.
        if (movementAction && (deltaColumn != 0 || deltaRow != 0)
            && levelFiveFirstProbeBroken_ && !levelFiveProbeTransformed_
            && playerColumn_ == levelFiveProbeColumn_
            && playerRow_ == levelFiveProbeRow_) {
            constexpr std::array<std::array<std::uint16_t, 3>, 17> mechanism = {{
                {{28, 10, 59}}, {{28, 9, 56}}, {{29, 9, 59}}, {{29, 10, 0}},
                {{29, 11, 56}}, {{30, 11, 59}}, {{30, 10, 56}}, {{27, 11, 59}},
                {{27, 12, 0}}, {{28, 12, 56}}, {{30, 8, 59}}, {{30, 7, 56}},
                {{31, 7, 59}}, {{31, 8, 0}}, {{32, 8, 56}}, {{32, 9, 59}},
                {{31, 9, 56}}
            }};
            for (const auto& entry : mechanism) {
                level_[entry[0]][entry[1]] = entry[2];
                overlays_.push_back({0x16, entry[0], entry[1]});
            }
            levelFiveProbeTransformed_ = true;
            QueueSound(GameplaySound::Construction);
        }
        if (!levelFiveFirstProbeBroken_
            && level_[levelFiveProbeColumn_][levelFiveProbeRow_] != 3) {
            overlays_.push_back({0x2d, levelFiveProbeColumn_, levelFiveProbeRow_});
            levelFiveFirstProbeBroken_ = true;
        }
        if (!levelFiveSecondProbeBroken_
            && level_[levelFiveProbeColumn_ + 1][levelFiveProbeRow_] != 3) {
            level_[levelFiveProbeColumn_ + 1][levelFiveProbeRow_] = 0x5d;
            levelFiveSecondProbeBroken_ = true;
        }
        ++turnCounter_;
        return;
    }

    if (levelNumber_ == 8) {
        // FUN_1010_c9f8 waits for any member of the 4x3 transporter grid
        // to be breached, then raises an alarm and attempts twelve guarded
        // structures using one shared 200-attempt budget.
        if (!levelEightAlarmTriggered_) {
            bool allTransportersIntact = true;
            for (int column = 12; column <= 27; column += 5) {
                for (int row = 12; row <= 22; row += 5) {
                    allTransportersIntact = allTransportersIntact
                        && level_[column][row] == 0x7f;
                }
            }
            if (!allTransportersIntact) {
                pendingMessages_.emplace_back(L"An alarm sounds! You hear explosions.");
                int attemptsRemaining = 200;
                int placedObjects = 0;
                // ce4e rejects player overlap before the shared attempt
                // budget is charged. Keep a separate safety ceiling so a
                // pathological occupied board cannot hang the Win64 UI.
                int draws = 0;
                while (placedObjects < 12 && attemptsRemaining >= 0 && draws++ < 100000) {
                    const int column = NextRandom(36);
                    const int row = NextRandom(32);
                    if (playerColumn_ >= column && playerColumn_ <= column + 2
                        && playerRow_ >= row && playerRow_ <= row + 2)
                        continue;
                    --attemptsRemaining;
                    if (column < 0 || row < 0 || column + 2 >= static_cast<int>(kBoardColumns)
                        || row + 2 >= static_cast<int>(kBoardRows)) {
                        continue;
                    }
                    bool available = true;
                    for (int objectColumn = column;
                         available && objectColumn <= column + 2; ++objectColumn) {
                        for (int objectRow = row; objectRow <= row + 2; ++objectRow) {
                            const std::uint16_t tile = level_[objectColumn][objectRow];
                            if (tile != 0) {
                                available = false;
                                break;
                            }
                        }
                    }
                    if (!available) continue;
                    constexpr std::uint16_t objectTiles[3][3] = {
                        {8, 9, 10}, {11, 0x18, 12}, {13, 14, 15}};
                    for (int offsetRow = 0; offsetRow < 3; ++offsetRow) {
                        for (int offsetColumn = 0; offsetColumn < 3; ++offsetColumn) {
                            level_[column + offsetColumn][row + offsetRow] =
                                objectTiles[offsetRow][offsetColumn];
                            overlays_.push_back({0x16,
                                static_cast<std::uint16_t>(column + offsetColumn),
                                static_cast<std::uint16_t>(row + offsetRow)});
                        }
                    }
                    QueueSound(GameplaySound::Construction);
                    ++placedObjects;
                }
                levelEightAlarmTriggered_ = true;
            }
        }
        ++turnCounter_;
        return;
    }

    if (levelNumber_ == 9) {
        // FUN_1010_d968 schedules three fixed actor waves for selector 1078.
        const auto addWave = [this](std::uint16_t type, int column, int row, int count) {
            for (int index = 0; index < count && actors_.size() < 300; ++index) {
                actors_.push_back({type, static_cast<std::uint16_t>(column),
                                   static_cast<std::uint16_t>(row), 0, 0, 0});
            }
        };
        if (turnCounter_ == 5) {
            addWave(0, 32, 33, NextRandom(8) + 8);
        } else if (turnCounter_ == 28) {
            addWave(0, 6, 6, NextRandom(5) + 8);
            addWave(2, 6, 6, NextRandom(5) + 8);
            addWave(0, 6, 6, NextRandom(5) + 8);
            addWave(2, 6, 6, NextRandom(5) + 8);
        } else if (turnCounter_ == 181) {
            addWave(0, 7, 32, NextRandom(3) + 3);
            addWave(2, 7, 32, NextRandom(3) + 3);
            addWave(1, 7, 32, NextRandom(3) + 3);
            addWave(3, 7, 32, NextRandom(3) + 3);
        }
        ++turnCounter_;
        return;
    }

    if (levelNumber_ == 10) {
        // Selector-33b4's FUN_1010_37a2 callback first tests the vessel against
        // the exact 2x2 0x83..0x86 device. It clears those four squares, grants
        // one Giant Emerald (inventory class 20), and reports the recovered text.
        if (!levelTenEmeraldClaimed_
            && playerColumn_ >= 10 && playerColumn_ <= 11
            && playerRow_ >= 20 && playerRow_ <= 21) {
            for (std::size_t column = 10; column <= 11; ++column) {
                for (std::size_t row = 20; row <= 21; ++row) {
                    level_[column][row] = 0;
                }
            }
            levelTenEmeraldClaimed_ = true;
            AddInventoryItem(20, 1);
            pendingMessages_.emplace_back(L"You pick up the Giant Emerald!");
        }

        // The same callback watches four fortress sentinels. Breaching any one
        // removes every tile-0x21 seal and changes tile 0x20 to 0x22 only in
        // the recovered columns 9..12 / rows 19..22 inner barrier.
        if (!levelTenBarrierOpened_
            && (level_[5][15] == 0 || level_[16][15] == 0
                || level_[16][26] == 0 || level_[5][26] == 0)) {
            levelTenBarrierOpened_ = true;
            for (BoardColumn& column : level_) {
                for (std::uint16_t& tile : column) {
                    if (tile == 0x21) tile = 0x20;
                }
            }
            for (std::size_t column = 9; column <= 12; ++column) {
                for (std::size_t row = 19; row <= 22; ++row) {
                    if (level_[column][row] == 0x20) level_[column][row] = 0x22;
                }
            }
        }
        ++turnCounter_;
        return;
    }

    if (levelNumber_ == 16) {
        // FUN_1010_3d50 first lets the vessel erase dormant empty terrain on
        // contact, then opens three alien-maze quadrants as their sentinel
        // structure centers disappear.
        if (level_[playerColumn_][playerRow_] == 0x5e) {
            level_[playerColumn_][playerRow_] = 0;
        }
        if (levelSixteenChambersActive_[2] && level_[7][29] == 0) {
            levelSixteenChambersActive_[2] = false;
            ActivateLevelSixteenRegion(0, 22, 13, 35);
        }
        if (levelSixteenChambersActive_[1] && level_[33][29] == 0) {
            levelSixteenChambersActive_[1] = false;
            ActivateLevelSixteenRegion(26, 22, 39, 35);
        }
        if (levelSixteenChambersActive_[0] && level_[33][7] == 0) {
            levelSixteenChambersActive_[0] = false;
            ActivateLevelSixteenRegion(26, 0, 39, 13);
        }
        if (!levelSixteenCoreActivated_
            && std::none_of(levelSixteenChambersActive_.begin(),
                            levelSixteenChambersActive_.end(),
                            [](bool active) { return active; })) {
            ActivateLevelSixteenRegion(17, 15, 23, 21);
            overlays_.push_back({4, 20, 18});
            level_[20][16] = 0x5c;
            const int count = NextRandom(3) + 3;
            for (int index = 0; index < count && actors_.size() < 300; ++index) {
                actors_.push_back({2, 20, 18, 0, 0, 0});
            }
            levelSixteenCoreActivated_ = true;
        }
        ++turnCounter_;
        return;
    }

    if (levelNumber_ == 11) {
        // FUN_1010_d06a escalates the recursive selector-1048 branch
        // through four independently persisted turn thresholds.
        if (turnCounter_ > 150 && (levelElevenRuntimeFlags_ & 0x01u) == 0) {
            levelElevenRuntimeFlags_ |= 0x01u;
        }
        if (turnCounter_ > 300 && (levelElevenRuntimeFlags_ & 0x02u) == 0) {
            levelElevenRuntimeFlags_ |= 0x02u;
        }
        if (turnCounter_ > 400u + boardDepth_ * 30u && (levelElevenRuntimeFlags_ & 0x04u) == 0) {
            PlaceRandomGuardedStructure(true);
            levelElevenRuntimeFlags_ |= 0x04u;
        }
        if (turnCounter_ > (boardDepth_ + 13u) * 50u && (levelElevenRuntimeFlags_ & 0x08u) == 0) {
            for (int object = 0; object < boardDepth_ + 5; ++object) {
                PlaceRandomGuardedStructure(true);
            }
            levelElevenRuntimeFlags_ |= 0x08u;
        }
        ++turnCounter_;
        return;
    }

    if (levelNumber_ >= 13 && levelNumber_ <= 15) {
        // FUN_1010_8f56 visits the 3x3 neighborhood around the player and
        // advances ordinary maze wall forms 0x2b..0x35 into 0x36..0x40.
        // This runs before the inherited square/transition callback.
        const int centerColumn = static_cast<int>(playerColumn_);
        const int centerRow = static_cast<int>(playerRow_);
        for (int column = centerColumn - 1; column <= centerColumn + 1; ++column) {
            for (int row = centerRow - 1; row <= centerRow + 1; ++row) {
                if (column < 0 || row < 0 || column >= static_cast<int>(kBoardColumns)
                    || row >= static_cast<int>(kBoardRows)) {
                    continue;
                }
                std::uint16_t& tile = level_[static_cast<std::size_t>(column)]
                                            [static_cast<std::size_t>(row)];
                if (tile >= 0x2b && tile <= 0x35) {
                    tile = static_cast<std::uint16_t>(tile + 0x0b);
                }
            }
        }
        ++turnCounter_;
        return;
    }

    if (levelNumber_ >= 17 && levelNumber_ <= 25) {
        // FUN_1008_dd76 escalates each procedural depth through four persisted
        // thresholds. The structure-spawn and terrain-break rates are consumed
        // by FUN_1008_81dc and FUN_1008_8310 on the following turn.
        const int depth = static_cast<int>(levelNumber_ - 16);
        if (turnCounter_ == 6) {
            genericSpawnChance_ = std::min(difficulty_ + 5, depth + difficulty_ - 1);
        }
        if (turnCounter_ > 50 && (genericRuntimeFlags_ & 0x01u) == 0) {
            genericBreakDivisor_ -= 5;
            genericSpawnChance_ += 2;
            genericRuntimeFlags_ |= 0x01u;
        }
        if (turnCounter_ > 150 && (genericRuntimeFlags_ & 0x02u) == 0) {
            genericBreakDivisor_ /= 2;
            genericSpawnChance_ *= 2;
            genericRuntimeFlags_ |= 0x02u;
        }
        if (turnCounter_ > static_cast<std::uint32_t>((depth + 10) * 30)
            && (genericRuntimeFlags_ & 0x04u) == 0) {
            PlaceRandomGuardedStructure(true);
            genericBreakDivisor_ = 10;
            genericSpawnChance_ = 25;
            genericRuntimeFlags_ |= 0x04u;
        }
        if (turnCounter_ > static_cast<std::uint32_t>((depth + 9) * 50)
            && (genericRuntimeFlags_ & 0x08u) == 0) {
            for (int object = 0; object < depth + 5; ++object) {
                PlaceRandomGuardedStructure(true);
            }
            genericBreakDivisor_ = 5;
            genericSpawnChance_ = 40;
            genericRuntimeFlags_ |= 0x08u;
        }
        ++turnCounter_;
        return;
    }

    if (levelNumber_ != 4) {
        ++turnCounter_;
        return;
    }

    const auto seedBrokenTransporter = [this]() {
        for (int attempt = 0; attempt < 7; ++attempt) {
            const int column = NextRandom(24) + 10;
            const int row = NextRandom(7) + column - 4;
            if (column < 0 || row < 0 || column >= static_cast<int>(kBoardColumns) ||
                row >= static_cast<int>(kBoardRows) ||
                (column == static_cast<int>(playerColumn_) &&
                 row == static_cast<int>(playerRow_))) {
                continue;
            }
            level_[static_cast<std::size_t>(column)][static_cast<std::size_t>(row)] = 0x22;
            overlays_.push_back({0x16, static_cast<std::uint16_t>(column),
                                 static_cast<std::uint16_t>(row)});
        }
    };

    // FUN_1010_518c watches the top-left tile of each FUN_1008_c452 transporter.
    if (!levelFourNortheastTriggered_ && level_[33][5] != 0x7f) {
        seedBrokenTransporter();
        levelFourNortheastTriggered_ = true;
    }
    if (!levelFourSouthwestTriggered_ && level_[5][29] != 0x7f) {
        seedBrokenTransporter();
        levelFourSouthwestTriggered_ = true;
    }

    // FUN_1010_4f76 seeds a 24-turn counter at offsets 0x54/0x56.
    // FUN_1010_518c performs four independent difficulty-scaled rolls per
    // turn and creates the fixed vector-driven type-4 hazards on a success.
    if (levelFourCountdown_ != 0) {
        constexpr std::array<std::pair<int, int>, 4> spawnPoints = {
            std::pair<int, int>{0x1c, 0x20},
            {0x1f, 0x21}, {0x25, 0x1e}, {0x23, 0x1b}};
        for (const auto [column, row] : spawnPoints) {
            if (NextRandom(difficulty_ * 2) > 1 && actors_.size() < 300) {
                actors_.push_back({4, static_cast<std::uint16_t>(column),
                                   static_cast<std::uint16_t>(row), -1, -1, 3});
            }
        }
        --levelFourCountdown_;
        if (levelFourCountdown_ == 0) {
            for (BoardActor& actor : actors_) {
                actor.behavior = 0;
            }
            for (BoardColumn& column : level_) {
                std::replace(column.begin(), column.end(),
                             static_cast<std::uint16_t>(0x20),
                             static_cast<std::uint16_t>(0x21));
            }
        }
    }

    if (!levelFourFloodTriggered_ && turnCounter_ > 800) {
        for (int attempt = 0; attempt < 24; ++attempt) {
            const int column = NextRandom(24) + 10;
            const int row = NextRandom(7) + column - 4;
            if (column < 0 || row < 0 || column >= static_cast<int>(kBoardColumns) ||
                row >= static_cast<int>(kBoardRows) ||
                (column == static_cast<int>(playerColumn_) &&
                 row == static_cast<int>(playerRow_))) {
                continue;
            }
            level_[static_cast<std::size_t>(column)][static_cast<std::size_t>(row)] = 0x22;
            overlays_.push_back({0x16, static_cast<std::uint16_t>(column),
                                 static_cast<std::uint16_t>(row)});
        }
        // 1010:518c paints removal effects backward, excluding actor zero.
        for (std::size_t index = actors_.size(); index-- > 1;)
            overlays_.push_back({0x16, actors_[index].column, actors_[index].row});
        actors_.clear();
        levelFourFloodTriggered_ = true;
    }
    ++turnCounter_;
}

std::wstring_view GameData::SignpostText() const noexcept {
    switch (levelNumber_) {
    case 1:
        return L"Power Plant.\nEnter here. (Proper I.D. required)";
    case 2:
        return playerColumn_ == 12 && playerRow_ == 23
            ? L"Read the scrolling table display for playing instructions."
            : L"Welcome to Demon-Dust Poker Casino.";
    case 3:
        if (playerColumn_ == 0 && playerRow_ == 15) return L"The world of Xoids...";
        if (playerColumn_ == 32 && playerRow_ == 8)
            return L"Demon-Dust Casino.\n\nStraight ahead and down.";
        if (playerColumn_ == 32 && playerRow_ == 28)
            return L"Entrance to Gorloth's private quarters.";
        if (playerColumn_ == 4 && playerRow_ == 28)
            return L"The Abyss.\nDeath awaits below.";
        return L"SEWER CITY\nUnauthorized personnel will be destroyed.";
    case 4: return L"The Gauntlet.";
    case 5: return L"The Breakout Board.";
    case 6:
        return playerColumn_ == 8 && playerRow_ == 19
            ? L"Moon Room.\n\n(Main Entrance)"
            : L"Xoid Breeding Grounds.";
    case 7: return L"The Statue Room";
    case 9: return {};
    case 10: return L"The Xoid Fortress.";
    case 11: return L"You have entered the Abyss.\nBeware.";
    case 12: return L"Caution:\nMain Portal Room";
    case 13: case 14: case 15: return L"The Labyrinth.";
    case 16: return L"Things may not be as they appear...";
    default:
        if (levelNumber_ >= 17 && levelNumber_ <= 25)
            return L"Welcome to your shower drain.\n\nHey, it's tiled!";
        return {};
    }
}

void GameData::AdvanceRepellent() noexcept {
    if (repellentEnergy_ <= 0) {
        return;
    }
    // Selector 1038 overrides vtable +0x44 with FUN_1010_0bd2, draining ten
    // units per accepted turn. All other boards inherit FUN_1008_c55c's two.
    repellentEnergy_ -= levelNumber_ == 1 ? 10 : 2;
    if (repellentEnergy_ < 0) {
        repellentEnergy_ = 0;
    }
}

void GameData::AdvanceProjectiles() noexcept {
    // 77d0 iterates the live array: fragments appended after an impact are
    // reached later in this same pass, not deferred to the next world turn.
    for (std::size_t projectileIndex = 0; projectileIndex < projectiles_.size();) {
        Projectile* projectile = &projectiles_[projectileIndex];
        std::vector<Projectile> spawnedProjectiles;
        if (projectile->deltaColumn == 0 && projectile->deltaRow == 0) {
            ++projectileIndex;
            continue;
        }
        projectile->column += projectile->deltaColumn;
        projectile->row += projectile->deltaRow;
        if (projectile->column < 0 || projectile->row < 0 ||
            projectile->column >= static_cast<int>(kBoardColumns) ||
            projectile->row >= static_cast<int>(kBoardRows)) {
            projectiles_.erase(projectiles_.begin()
                + static_cast<std::ptrdiff_t>(projectileIndex));
            continue;
        }
        bool removeProjectile = false;
        for (std::size_t actorIndex = 0; actorIndex < actors_.size();) {
            const BoardActor actor = actors_[actorIndex];
            const int horizontal = std::abs(static_cast<int>(actor.column) - projectile->column);
            const int vertical = std::abs(static_cast<int>(actor.row) - projectile->row);
            const bool directHit = horizontal == 0 && vertical == 0;
            // 1008:790c-79f7: fireballs scan the complete surrounding 3x3 area.
            const bool fireballHit = projectile->type == 4 && horizontal <= 1 && vertical <= 1
                && (directHit || actor.type == 0 || actor.type == 1 || actor.type == 4
                    || NextRandom(horizontal + vertical + 1) == 1);
            if (directHit || fireballHit) {
                const bool affected = projectile->type == 1 || projectile->type == 2 ||
                    (projectile->type == 0 &&
                     (actor.type == 0 || actor.type == 1 || actor.type == 4)) ||
                    fireballHit;
                if (affected) {
                    const std::uint16_t actorType = actor.type;
                    const std::uint16_t actorColumn = actor.column;
                    const std::uint16_t actorRow = actor.row;
                    AwardActorScore(actorType);
                    actors_.erase(actors_.begin() + static_cast<std::ptrdiff_t>(actorIndex));
                    if (projectile->type == 0 && (actorType == 0 || actorType == 1)) {
                        overlays_.push_back({0x16, actorColumn, actorRow});
                        QueueSound(GameplaySound::Pop);
                        if (level_[actorColumn][actorRow] == 0)
                            level_[actorColumn][actorRow] = actorType == 1 ? 4
                                : (ActorSprite(0, animationFrame_) < 4 ? 5 : 6);
                    } else if (projectile->type == 0 && actorType == 4) {
                        overlays_.push_back({0x17, actorColumn, actorRow});
                        QueueSound(GameplaySound::DeadXoid);
                    } else if (projectile->type == 1 || projectile->type == 2
                               || projectile->type == 4) {
                        overlays_.push_back({0x14, actorColumn, actorRow});
                        // FUN_1008_77d0 gives the persistent type-4 fireball
                        // its dedicated crackle sample; ordinary projectile
                        // classes retain the small explosion at table entry 12.
                        QueueSound(projectile->type == 4
                            ? GameplaySound::Crackle : GameplaySound::ExplosionSmall);
                        if (actorType == 2 && NextRandom(10) == 2) {
                            overlays_.push_back({1, actorColumn, actorRow});
                        }
                        if (actorType == 3) {
                            const int count = NextRandom(7);
                            for (int index = 0; index < count && actors_.size() < 300; ++index) {
                                actors_.push_back({2, actorColumn, actorRow, 0, 0, 0});
                            }
                        }
                    }
                    removeProjectile = projectile->type != 4;
                    if (projectile->type == 2) {
                        for (int column = -1; column <= 1; ++column) {
                            for (int row = -1; row <= 1; ++row) {
                                if (column != 0 || row != 0) {
                                    spawnedProjectiles.push_back(
                                        {1, projectile->column, projectile->row, column, row});
                                }
                            }
                        }
                    }
                    if (projectile->type == 4) continue;
                } else if (projectile->type == 0 && (actor.type == 2 || actor.type == 3)) {
                    // 1008:7a1d -> 7ba6: immune enemies still absorb the bubble.
                    removeProjectile = true;
                }
                if (removeProjectile) break;
            }
            ++actorIndex;
        }
        const std::uint16_t tile = level_[projectile->column][projectile->row];
        const bool passable = tile < tiles_.size() && (tiles_[tile].flags & 1u) != 0;
        if (!removeProjectile && !passable && projectile->type != 3) {
            if ((projectile->type == 1 || projectile->type == 2)
                && tile < tiles_.size() && (tiles_[tile].flags & 4u) != 0) {
                // 1008:77d0 still displays the impact on indestructible rock.
                overlays_.push_back({0x14,
                    static_cast<std::uint16_t>(projectile->column),
                    static_cast<std::uint16_t>(projectile->row)});
                QueueSound(GameplaySound::ExplosionSmall);
            }
            if (projectile->type == 1) {
                DamageBoardSquare(projectile->column, projectile->row, 1, &spawnedProjectiles);
            } else if (projectile->type == 2) {
                DamageBoardSquare(projectile->column, projectile->row, 2, &spawnedProjectiles);
            }
            removeProjectile = true;
        }
        if (removeProjectile) {
            projectiles_.erase(projectiles_.begin()
                + static_cast<std::ptrdiff_t>(projectileIndex));
        } else {
            ++projectileIndex;
        }
        for (const Projectile& fragment : spawnedProjectiles) {
            if (projectiles_.size() >= 150) break;
            projectiles_.push_back(fragment);
        }
    }
}

bool GameData::AffectRadioBlastSquare(int column, int row) noexcept {
    // Exact FUN_1008_a980 square callback. It paints every accepted blast
    // cell, dispatches strength three against solid terrain, and removes all
    // actors there through the normal score/drop path.
    if (column < 0 || row < 0 || column >= static_cast<int>(kBoardColumns)
        || row >= static_cast<int>(kBoardRows)) {
        return false;
    }
    const std::uint16_t tile = level_[column][row];
    if (tile >= tiles_.size()) return false;
    const std::uint16_t flags = tiles_[tile].flags;
    if ((flags & 2u) != 0 && (flags & 4u) != 0) return false;

    overlays_.push_back({0x14, static_cast<std::uint16_t>(column),
                         static_cast<std::uint16_t>(row)});
    DamageBoardSquare(column, row, 3);
    for (auto actor = actors_.begin(); actor != actors_.end();) {
        if (actor->column != column || actor->row != row) {
            ++actor;
            continue;
        }
        const std::uint16_t actorType = actor->type;
        AwardActorScore(actorType);
        if (actorType == 2 && NextRandom(10) == 2) {
            overlays_.push_back({1, static_cast<std::uint16_t>(column),
                                 static_cast<std::uint16_t>(row)});
        }
        actor = actors_.erase(actor);
    }
    return true;
}

void GameData::ApplyRadioExplosion(int centerColumn, int centerRow) noexcept {
    AffectRadioBlastSquare(centerColumn, centerRow);
    for (const auto [deltaColumn, deltaRow] :
         {std::pair{-1, -1}, std::pair{1, -1}, std::pair{-1, 1}, std::pair{1, 1}}) {
        if (AffectRadioBlastSquare(centerColumn + deltaColumn, centerRow + deltaRow)) {
            AffectRadioBlastSquare(centerColumn + deltaColumn * 2, centerRow + deltaRow);
            AffectRadioBlastSquare(centerColumn + deltaColumn, centerRow + deltaRow * 2);
        }
    }
    for (const auto [deltaColumn, deltaRow] :
         {std::pair{0, -1}, std::pair{-1, 0}, std::pair{1, 0}, std::pair{0, 1}}) {
        if (AffectRadioBlastSquare(centerColumn + deltaColumn, centerRow + deltaRow)) {
            AffectRadioBlastSquare(centerColumn + deltaColumn * 2,
                                   centerRow + deltaRow * 2);
        }
    }
}

bool GameData::DetonateCharges() noexcept {
    bool detonated = false;
    for (auto projectile = projectiles_.begin(); projectile != projectiles_.end();) {
        if (projectile->type != 3) {
            ++projectile;
            continue;
        }
        detonated = true;
        const int centerColumn = projectile->column;
        const int centerRow = projectile->row;
        ApplyRadioExplosion(centerColumn, centerRow);
        projectile = projectiles_.erase(projectile);
    }
    return detonated;
}

void GameData::ResolveActorCollision() noexcept {
    for (auto actor = actors_.begin(); actor != actors_.end();) {
        if (actor->column == playerColumn_ && actor->row == playerRow_) {
            // FUN_1008_2920: types 0, 1, 2, and 4 cost one health; type 3 costs two.
            // Unknown record types still make contact but do not alter health.
            int damage = 0;
            if (actor->type <= 2 || actor->type == 4) damage = 1;
            else if (actor->type == 3) damage = 2;
            health_ -= damage;
            if (health_ < 0) {
                health_ = 0;
            }
            // FUN_1008_2920 always queues the ordinary hit first. Only a
            // playable root board (not an attract-mode or child board) adds
            // the higher-priority death event when health reaches zero.
            QueueSound(GameplaySound::PlayerHit);
            if (health_ == 0 && terminalDeathEnabled_) {
                fatalHitLatched_ = true;
                QueueSound(GameplaySound::PlayerDies);
            }
            overlays_.push_back({0x18, actor->column, actor->row});
            actor = actors_.erase(actor);
        } else {
            ++actor;
        }
    }
}

int GameData::ActorBreakDivisor() const noexcept {
    if (levelNumber_ == 1) return 5;
    if (levelNumber_ == 3) return turnCounter_ % 200u >= 180u ? 1 : 25;
    if (levelNumber_ == 5) return 85 - 10 * difficulty_;
    if (levelNumber_ == 6) return levelSixStructuresSpawned_ ? 5 : 80;
    if (levelNumber_ == 7) return levelSevenSpawnDivisor_ == 0 ? 5 : 15;
    if (levelNumber_ == 8) return levelEightAlarmTriggered_ ? 4 : 25;
    if (levelNumber_ == 9) return 75;
    if (levelNumber_ == 11) {
        // 1010:ce3f / d06a: apply the persisted escalation phases in order.
        int divisor = std::max(10, 100 - boardDepth_ - 15 * difficulty_);
        if (levelElevenRuntimeFlags_ & 1u) divisor -= 5;
        if (levelElevenRuntimeFlags_ & 2u) divisor /= 2;
        if (levelElevenRuntimeFlags_ & 4u) divisor = 10;
        if (levelElevenRuntimeFlags_ & 8u) divisor = 5;
        return divisor;
    }
    if (levelNumber_ >= 13 && levelNumber_ <= 16) return (6 - difficulty_) * 10;
    if (levelNumber_ == 12) return 100;
    if (levelNumber_ >= 17 && levelNumber_ <= 25) return genericBreakDivisor_;
    return 20;
}

void GameData::AdvanceActors() noexcept {
    // Reconstructed core of FUN_1008_8310. Behavior 1 is stationary, behavior 3
    // follows the stored vector until blocked, and behavior 0 hunts the player.
    // FUN_1008_adee also gathers the three native lure-square families used by
    // FUN_1008_93a4 when scoring hunter destinations.
    std::vector<std::pair<int, int>> attractors;
    for (int column = 0; column < static_cast<int>(kBoardColumns); ++column) {
        for (int row = 0; row < static_cast<int>(kBoardRows); ++row) {
            const std::uint16_t tile = level_[column][row];
            if (tile == 7 || tile == 0x74 || tile == 0x7c) {
                attractors.emplace_back(column, row);
            }
        }
    }
    const auto hunterDistance = [this, &attractors](int column, int row) {
        const int playerHorizontal = std::abs(static_cast<int>(playerColumn_) - column);
        const int playerVertical = std::abs(static_cast<int>(playerRow_) - row);
        int distance = std::max(playerHorizontal, playerVertical);
        if (repellentEnergy_ > 0) distance = std::abs(5 - distance);
        for (const auto& [targetColumn, targetRow] : attractors) {
            distance = std::min(distance,
                std::max(std::abs(targetColumn - column), std::abs(targetRow - row)));
        }
        return distance;
    };
    for (std::size_t index = 0; index < actors_.size();) {
        BoardActor& actor = actors_[index];
        if (actor.behavior == 1) {
            if (!ResolveActorTurnCollision(index)) ++index;
            continue;
        }
        const auto canEnter = [this, index](int column, int row) {
            if (column < 0 || row < 0 || column >= static_cast<int>(kBoardColumns) ||
                row >= static_cast<int>(kBoardRows)) {
                return false;
            }
            const std::uint16_t tile = level_[static_cast<std::size_t>(column)]
                                              [static_cast<std::size_t>(row)];
            if (tile >= tiles_.size()) {
                return false;
            }
            const auto actorType = actors_[index].type;
            const bool occupiedByActor = std::any_of(actors_.begin(), actors_.end(),
                [=](const BoardActor& other) {
                    return other.column == column && other.row == row;
                });
            const bool occupiedByProjectile = std::any_of(
                projectiles_.begin(), projectiles_.end(),
                [=](const Projectile& shot) {
                    return shot.column == column && shot.row == row;
                });
            // 1008:91ec has an exceptional empty-cell path. Types 0/1/4
            // ignore projectiles there; types 2/3 make a one-in-two dodge
            // roll only when a projectile occupies that empty cell.
            if (tile == 0 && !occupiedByActor) {
                if (actorType == 0 || actorType == 1 || actorType == 4) return true;
                if (actorType == 2 || actorType == 3)
                    return !occupiedByProjectile || NextRandom(2) != 2;
            }
            // Other actor-passable terrain still excludes projectile-occupied
            // cells for types 2/3. The machinery range includes tile eight.
            const bool actorPassable = (tiles_[tile].flags & 1u) != 0
                || (tile >= 8 && tile < 0x18)
                || (tile > 0x86 && tile < 0xa2);
            return actorPassable && !occupiedByActor
                && ((actorType != 2 && actorType != 3) || !occupiedByProjectile);
        };
        if (actor.behavior == 2) {
            // FUN_1008_8310 gives random actors eight attempts at a neighboring square.
            for (int attempt = 0; attempt < 8; ++attempt) {
                const int deltaColumn = NextRandom(3) - 2;
                const int deltaRow = NextRandom(3) - 2;
                const int column = static_cast<int>(actor.column) + deltaColumn;
                const int row = static_cast<int>(actor.row) + deltaRow;
                if (canEnter(column, row)) {
                    actor.column = static_cast<std::uint16_t>(column);
                    actor.row = static_cast<std::uint16_t>(row);
                    break;
                }
            }
            if (!ResolveActorTurnCollision(index)) ++index;
            continue;
        }
        int bestColumn = actor.column;
        int bestRow = actor.row;
        int bestDistance = 0x7fff;
        int firstDeltaColumn = -1;
        int lastDeltaColumn = 1;
        int firstDeltaRow = -1;
        int lastDeltaRow = 1;
        bool brokeTerrain = false;
        if (actor.behavior == 3) {
            firstDeltaColumn = lastDeltaColumn = actor.deltaColumn;
            firstDeltaRow = lastDeltaRow = actor.deltaRow;
        }
        // The native candidate loop is column-major (-1..1), then row-major.
        for (int deltaColumn = firstDeltaColumn; deltaColumn <= lastDeltaColumn; ++deltaColumn) {
            for (int deltaRow = firstDeltaRow; deltaRow <= lastDeltaRow; ++deltaRow) {
                const int column = static_cast<int>(actor.column) + deltaColumn;
                const int row = static_cast<int>(actor.row) + deltaRow;
                if (actor.behavior == 0 && (actor.type == 1 || actor.type == 3)
                    && column >= 0 && row >= 0
                    && column < static_cast<int>(kBoardColumns)
                    && row < static_cast<int>(kBoardRows)) {
                    const std::uint16_t obstacle = level_[column][row];
                    const int baseBreakDivisor = ActorBreakDivisor();
                    const int breakDivisor = obstacle == 7
                        ? baseBreakDivisor * 2 : baseBreakDivisor;
                    if (((obstacle >= 3 && obstacle <= 6) || obstacle == 7)
                        && NextRandom(breakDivisor) == 1) {
                        level_[column][row] = 0;
                        overlays_.push_back({0x14, static_cast<std::uint16_t>(column),
                                             static_cast<std::uint16_t>(row)});
                        QueueSound(GameplaySound::BoardBreak);
                        brokeTerrain = true;
                    }
                }
                if (!canEnter(column, row)) {
                    continue;
                }
                if (actor.behavior == 3) {
                    bestColumn = column;
                    bestRow = row;
                    bestDistance = 0;
                    continue;
                }
                const int distance = hunterDistance(column, row);
                if (distance < bestDistance
                    || (distance == bestDistance && NextRandom(100) > 70)) {
                    bestDistance = distance;
                    bestColumn = column;
                    bestRow = row;
                }
            }
        }
        if (actor.behavior == 3 && bestDistance == 0x7fff) {
            // FUN_1008_8310 changes a blocked vector-driven actor to hunter mode.
            actor.behavior = 0;
            ++index;
            continue;
        }
        if (brokeTerrain) { ++index; continue; }
        actor.column = static_cast<std::uint16_t>(bestColumn);
        actor.row = static_cast<std::uint16_t>(bestRow);
        if (!ResolveActorTurnCollision(index)) ++index;
    }

}

bool GameData::ResolveActorTurnCollision(std::size_t actorIndex) noexcept {
    // 1008:864d: player contact precedes projectile contact for this actor.
    const BoardActor current = actors_[actorIndex];
    if (current.column == playerColumn_ && current.row == playerRow_) {
        const int damage = current.type == 3 ? 2 : (current.type <= 4 ? 1 : 0);
        health_ = std::max(0, health_ - damage);
        QueueSound(GameplaySound::PlayerHit);
        if (health_ == 0 && terminalDeathEnabled_) {
            fatalHitLatched_ = true;
            QueueSound(GameplaySound::PlayerDies);
        }
        overlays_.push_back({0x18, current.column, current.row});
        actors_.erase(actors_.begin() + static_cast<std::ptrdiff_t>(actorIndex));
        return true;
    }
    std::vector<BoardActor> spawnedActors;
    std::vector<Projectile> spawnedProjectiles;
    bool actorRemoved = false;
    for (auto projectile = projectiles_.begin(); projectile != projectiles_.end();) {
        BoardActor& actor = actors_[actorIndex];
        if (projectile->column != actor.column || projectile->row != actor.row) {
            ++projectile;
            continue;
        }

        const std::uint16_t projectileType = projectile->type;
        const std::uint16_t actorType = actor.type;
        const std::uint16_t column = actor.column;
        const std::uint16_t row = actor.row;
        bool removeProjectile = false;
        bool removeActor = false;
        bool stopActorProjectiles = false;
        if (projectileType == 0) {
            removeProjectile = true;
            if (actorType == 0 || actorType == 1) {
                overlays_.push_back({0x16, column, row});
                QueueSound(GameplaySound::Pop);
                if (level_[column][row] == 0) {
                    level_[column][row] = actorType == 1 ? 4
                        : (ActorSprite(0, animationFrame_) < 4 ? 5 : 6);
                }
                removeActor = true;
            } else if (actorType == 4) {
                overlays_.push_back({0x17, column, row});
                QueueSound(GameplaySound::DeadXoid);
                removeActor = true;
            }
        } else if (projectileType == 1) {
            removeProjectile = true;
            if (projectile->deltaColumn == 0 && projectile->deltaRow == 0) {
                overlays_.push_back({0x24, column, row});
                stopActorProjectiles = true;
            } else {
                removeActor = true;
            }
        } else if (projectileType == 2) {
            removeProjectile = true;
            removeActor = true;
            for (int deltaColumn = -1; deltaColumn <= 1; ++deltaColumn) {
                for (int deltaRow = -1; deltaRow <= 1; ++deltaRow) {
                    if (deltaColumn != 0 || deltaRow != 0) {
                        spawnedProjectiles.push_back(
                            {1, column, row, deltaColumn, deltaRow});
                    }
                }
            }
        } else if (projectileType == 4) {
            removeActor = true;
        }

        if (removeActor && projectileType != 0) {
            overlays_.push_back({0x14, column, row});
            // FUN_1008_8310 repeats FUN_1008_77d0's type-4-specific
            // sound choice in its post-actor collision phase.
            QueueSound(projectileType == 4
                ? GameplaySound::Crackle : GameplaySound::ExplosionSmall);
            if (actorType == 2 && NextRandom(10) == 2) {
                overlays_.push_back({1, column, row});
            }
            if (actorType == 3) {
                const int count = NextRandom(7);
                for (int index = 0; index < count && actors_.size() + spawnedActors.size() < 300; ++index) {
                    spawnedActors.push_back({2, column, row, 0, 0, 0});
                }
            }
        }
        if (removeActor) {
            AwardActorScore(actorType);
            actors_.erase(actors_.begin() + static_cast<std::ptrdiff_t>(actorIndex));
            actorRemoved = true;
        }
        if (removeProjectile) {
            projectile = projectiles_.erase(projectile);
        } else {
            ++projectile;
        }
        if (actorRemoved || stopActorProjectiles) break;
    }
    for (const BoardActor& actor : spawnedActors) {
        if (actors_.size() >= 300) break;
        actors_.push_back(actor);
    }
    for (const Projectile& projectile : spawnedProjectiles) {
        if (projectiles_.size() >= 150) break;
        projectiles_.push_back(projectile);
    }
    return actorRemoved;
}

std::uint16_t GameData::SelectProceduralStructureType(int depth) noexcept {
    // FUN_1008_d4e0: only successful placement attempts at depth >= 5
    // consume this roll, before either coordinate is drawn.
    return depth >= 5 && NextRandom(20) == 20 ? 0x1a : 0x18;
}

int GameData::NextRandom(int upperBound) noexcept {
    // Exact translation of FUN_1000_1f6e and FUN_1008_3c88. This is the
    // Microsoft C runtime LCG used by the Win16 executable.
    randomState_ = randomState_ * 0x343fdu + 0x269ec3u;
    const std::uint32_t value = (randomState_ >> 16u) & 0x7fffu;
    return static_cast<int>(value % static_cast<std::uint32_t>(upperBound)) + 1;
}

void GameData::DrawLevel(HDC destination, const BitmapSurface& atlas) const {
    for (std::size_t column = 0; column < kBoardColumns; ++column) {
        for (std::size_t row = 0; row < kBoardRows; ++row) {
            const TileDescriptor& tile = tiles_[level_[column][row]];
            const auto tileIndex = level_[column][row];
            const auto& visual = terrainAnimation_[column][row];
            const bool cached = (tileIndex >= 0x18 && tileIndex <= 0x22)
                || tileIndex == 0xaf || (tileIndex >= 0x83 && tileIndex <= 0x86)
                || (tileIndex >= 0xa2 && tileIndex <= 0xaa);
            const auto spriteIndex = cached
                ? (visual.tile == tileIndex ? visual.sprite : tile.sprite)
                : TerrainSprite(tileIndex, tile.sprite, animationFrame_);
            if (spriteIndex >= sprites_.size()) {
                continue;
            }
            const SpriteRecord& sprite = sprites_[spriteIndex];
            atlas.DrawRegion(destination,
                             static_cast<int>(column) * kCellSize,
                             static_cast<int>(row) * kCellSize,
                             static_cast<int>(sprite.sourceColumn) * kCellSize,
                             static_cast<int>(sprite.sourceRow) * kCellSize,
                             static_cast<int>(sprite.widthInCells) * kCellSize,
                             static_cast<int>(sprite.heightInCells) * kCellSize);
        }
    }
    const auto drawOverlays = [this, destination, &atlas](bool transientPass) {
    for (auto node = overlays_.rbegin(); node != overlays_.rend(); ++node) {
        const BoardOverlay& overlay = *node;
        // 1008:b09e/9e68 only expose ordinary overlays on passable
        // terrain. In particular a pickup under crud must remain hidden.
        // 8be6's transient effects are a separate pass and may cover walls.
        const bool transient = overlay.type >= 0x14 && overlay.type <= 0x1f;
        if (transient != transientPass) continue;
        if (overlay.column >= kBoardColumns || overlay.row >= kBoardRows) continue;
        if (!transient && (tiles_[level_[overlay.column][overlay.row]].flags & 1u) == 0)
            continue;
        // Selector 1040's card objects use FUN_1010_23ce rather than their
        // ordinary overlay sprite. The recovered 3x4 card grid is drawn below.
        if (levelNumber_ == 2 && overlay.type == 0x1e && overlay.row == 17
            && overlay.column >= 17 && overlay.column <= 29
            && (overlay.column - 17) % 3 == 0) {
            continue;
        }
        if (overlay.type >= overlaySprites_.size()) {
            continue;
        }
        std::int16_t spriteIndex = overlaySprites_[overlay.type];
        // 9e68 randomized updates are cached on timer ticks, not repaints.
        switch (overlay.type) {
        case 1:
            if (overlay.visualSprite != 0xffff) spriteIndex = overlay.visualSprite;
            break;
        case 2: case 3: case 5: case 6: case 7: case 8: case 9:
        case 10: case 0xb: case 0xc: case 0xd: case 0xf: case 0x10: case 0x11:
            spriteIndex = static_cast<std::int16_t>(0x57 + animationFrame_ % 4);
            break;
        case 0x2b: spriteIndex = animationFrame_ % 5 < 3 ? 0xfb : 0; break;
        case 0x2d: {
            constexpr std::int16_t frames[] = {0xfc, 0xfd, 0xfe, 0xff, 0xfe, 0xfd, 0xfc, 0xfc, 0xfc, 0xfc};
            spriteIndex = frames[animationFrame_ % 10];
            break;
        }
        default: break;
        }
        if (IsTransientOverlay(overlay)) {
            const std::size_t frame = overlay.animationFrame == 0 ? 0 : overlay.animationFrame - 1;
            switch (overlay.type) {
            case 0x14: {
                constexpr std::array<std::int16_t, 5> frames = {0x60, 0x61, 0x62, 0x63, 0};
                spriteIndex = frames[frame];
                break;
            }
            case 0x15: spriteIndex = -1; break;
            case 0x16: spriteIndex = static_cast<std::int16_t>(0x5b + frame); break;
            case 0x17: spriteIndex = static_cast<std::int16_t>(0x64 + frame); break;
            case 0x18: spriteIndex = static_cast<std::int16_t>(0x3a + frame); break;
            case 0x19: spriteIndex = static_cast<std::int16_t>(0x4d + frame * 2); break;
            case 0x1a: spriteIndex = static_cast<std::int16_t>(0x4e + frame * 2); break;
            case 0x1b: spriteIndex = 0x4b; break;
            case 0x1c: spriteIndex = 0x4c; break;
            case 0x1d: spriteIndex = static_cast<std::int16_t>(0xdd + frame); break;
            case 0x1f: spriteIndex = static_cast<std::int16_t>(0x1ce + frame); break;
            default: break;
            }
        }
        if (spriteIndex < 0 || static_cast<std::size_t>(spriteIndex) >= sprites_.size()) {
            continue;
        }
        const SpriteRecord& sprite = sprites_[static_cast<std::size_t>(spriteIndex)];
        atlas.DrawRegion(destination,
                         static_cast<int>(overlay.column) * kCellSize,
                         static_cast<int>(overlay.row) * kCellSize,
                         static_cast<int>(sprite.sourceColumn) * kCellSize,
                         static_cast<int>(sprite.sourceRow) * kCellSize,
                         static_cast<int>(sprite.widthInCells) * kCellSize,
                         static_cast<int>(sprite.heightInCells) * kCellSize);
    }
    };
    drawOverlays(false);
    if (levelNumber_ == 2) {
        const auto drawCasinoCell = [this, destination, &atlas](int column, int row,
                                                                 std::uint16_t spriteIndex) {
            if (spriteIndex >= sprites_.size()) return;
            const SpriteRecord& sprite = sprites_[spriteIndex];
            atlas.DrawRegion(destination, column * kCellSize, row * kCellSize,
                             static_cast<int>(sprite.sourceColumn) * kCellSize,
                             static_cast<int>(sprite.sourceRow) * kCellSize,
                             static_cast<int>(sprite.widthInCells) * kCellSize,
                             static_cast<int>(sprite.heightInCells) * kCellSize);
        };
        for (std::size_t card = 0; card < casinoRanks_.size(); ++card) {
            const int left = static_cast<int>(card) * 3 + 17;
            const auto flip = std::find_if(overlays_.rbegin(), overlays_.rend(),
                [left](const BoardOverlay& overlay) {
                    return overlay.type == 0x1e && overlay.column == left && overlay.row == 17
                        && overlay.animationFrame != 0;
                });
            // 1010:2cac clears the slot at phase one and paints a card back
            // at phase three. The final phase exposes 23ce's face drawing.
            if (flip != overlays_.rend() && flip->animationFrame <= 2) {
                for (int row = 17; row <= 20; ++row)
                    for (int column = left; column < left + 3; ++column)
                        drawCasinoCell(column, row, 0xf0);
                continue;
            }

            if (casinoStatus_ < 2 || (flip != overlays_.rend() && flip->animationFrame <= 4)) {
                for (int offset = 0; offset < 3; ++offset) {
                    drawCasinoCell(left + offset, 17,
                                   static_cast<std::uint16_t>(0x12a + offset));
                    drawCasinoCell(left + offset, 20,
                                   static_cast<std::uint16_t>(0x130 + offset));
                }
                for (int row = 18; row <= 19; ++row) {
                    drawCasinoCell(left, row, 0x12d);
                    drawCasinoCell(left + 1, row, 0x12e);
                    drawCasinoCell(left + 2, row, 0x12f);
                }
                continue;
            }

            const std::uint16_t heldOffset = casinoHeld_[card] != 0 ? 8 : 0;
            for (int offset = 0; offset < 3; ++offset) {
                drawCasinoCell(left + offset, 17,
                               static_cast<std::uint16_t>(0x11a + heldOffset + offset));
                drawCasinoCell(left + offset, 20,
                               static_cast<std::uint16_t>(0x11f + heldOffset + offset));
            }
            for (int row = 18; row <= 19; ++row) {
                drawCasinoCell(left, row, static_cast<std::uint16_t>(0x11d + heldOffset));
                drawCasinoCell(left + 2, row, static_cast<std::uint16_t>(0x11e + heldOffset));
            }
            const std::uint16_t redSuitOffset =
                casinoSuits_[card] == 2 || casinoSuits_[card] == 3 ? 13 : 0;
            drawCasinoCell(left + 1, 18,
                           static_cast<std::uint16_t>(0xff + casinoRanks_[card] + redSuitOffset));
            drawCasinoCell(left + 1, 19,
                           static_cast<std::uint16_t>(0xeb + casinoSuits_[card]));
        }

        // FUN_1010_23ce's compact label identifies the active wild table.
        if (casinoWildRank_ == 0) {
            for (int column = 22; column <= 26; ++column) drawCasinoCell(column, 16, 0xf0);
        } else if (casinoWildRank_ == 2 || casinoWildRank_ == 7) {
            drawCasinoCell(22, 16, casinoWildRank_ == 2 ? 0x134 : 0x136);
            drawCasinoCell(23, 16, 0x135);
            for (int column = 24; column <= 26; ++column)
                drawCasinoCell(column, 16, static_cast<std::uint16_t>(column + 0x11f));
        }
    }
    for (const BoardActor& actor : actors_) {
        const std::uint16_t spriteIndex = actor.visualType == actor.type
            && actor.visualSprite != 0xffff ? actor.visualSprite : ActorSprite(actor.type, animationFrame_);
        if (spriteIndex >= sprites_.size()) {
            continue;
        }
        const SpriteRecord& sprite = sprites_[spriteIndex];
        atlas.DrawRegion(destination,
                         static_cast<int>(actor.column) * kCellSize,
                         static_cast<int>(actor.row) * kCellSize,
                         static_cast<int>(sprite.sourceColumn) * kCellSize,
                         static_cast<int>(sprite.sourceRow) * kCellSize,
                         static_cast<int>(sprite.widthInCells) * kCellSize,
                         static_cast<int>(sprite.heightInCells) * kCellSize);
    }
    for (const Projectile& projectile : projectiles_) {
        std::uint16_t spriteIndex = 0;
        switch (projectile.type) {
        case 0: spriteIndex = 0x12; break;
        case 1:
            spriteIndex = projectile.deltaColumn == 0 && projectile.deltaRow == 0 ? 0x14 : 0x13;
            break;
        case 2: {
            constexpr int directionSprites[8][3] = {
                {0, -1, 27}, {-1, 0, 25}, {1, 0, 22}, {0, 1, 28},
                {1, 1, 21}, {1, -1, 23}, {-1, -1, 26}, {-1, 1, 24}};
            spriteIndex = 0x13;
            for (const auto& direction : directionSprites) {
                if (direction[0] == projectile.deltaColumn && direction[1] == projectile.deltaRow) {
                    spriteIndex = static_cast<std::uint16_t>(direction[2]);
                    break;
                }
            }
            break;
        }
        // 1008:9d9c animates these only when no actor occupies the cell.
        case 3: case 4: {
            const bool actorPresent = std::any_of(actors_.begin(), actors_.end(),
                [&projectile](const BoardActor& actor) {
                    return actor.column == projectile.column && actor.row == projectile.row;
                });
            if (actorPresent) continue;
            spriteIndex = static_cast<std::uint16_t>((projectile.type == 3 ? 0x1d : 0x21)
                + animationFrame_ % 4);
            break;
        }
        default: continue;
        }
        if (spriteIndex >= sprites_.size()) {
            continue;
        }
        const SpriteRecord& sprite = sprites_[spriteIndex];
        atlas.DrawRegion(destination, projectile.column * kCellSize, projectile.row * kCellSize,
                         static_cast<int>(sprite.sourceColumn) * kCellSize,
                         static_cast<int>(sprite.sourceRow) * kCellSize,
                         static_cast<int>(sprite.widthInCells) * kCellSize,
                         static_cast<int>(sprite.heightInCells) * kCellSize);
    }
    // 1008:8b2c/8be6 is a late, separate effect pass. Ordinary pickups stay
    // below actors, but impacts and hit flashes cover surviving entities.
    drawOverlays(true);
    if (levelNumber_ == 2) {
        // 1010:26fe: opaque black ticker at (216,156), thirteen characters.
        const int saved = SaveDC(destination);
        HFONT font = CreateFontW(12, 6, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY,
            FIXED_PITCH | FF_MODERN, L"Courier New");
        SelectObject(destination, font);
        SetBkMode(destination, OPAQUE);
        SetBkColor(destination, RGB(0, 0, 0));
        SetTextColor(destination, casinoAnnouncement_ == 2 ? RGB(255, 0, 0) :
            casinoAnnouncement_ == 3 ? RGB(0, 0, 255) : RGB(255, 255, 0));
        const auto window = CasinoAnnouncementWindow();
        const RECT bounds{216, 156, 294, 168};
        ExtTextOutW(destination, 216, 156, ETO_OPAQUE | ETO_CLIPPED, &bounds,
            window.c_str(), static_cast<UINT>(window.size()), nullptr);
        RestoreDC(destination, saved);
        DeleteObject(font);
    }
    // The original 8f32 skips the player's square and 9680 ends the entity
    // animation pass with 974c (player). Draw last in our full-frame renderer
    // so projectiles, actors and board-specific graphics cannot hide it.
    // The separate 8be6 hit pass replaces the player with sprites 3a..3e.
    // Preserve that exception without elevating ordinary projectiles/effects.
    std::size_t playerSpriteIndex = PlayerSpriteIndex(
        (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0);
    for (auto effect = overlays_.rbegin(); effect != overlays_.rend(); ++effect) {
        if (effect->type == 0x18 && effect->column == playerColumn_
            && effect->row == playerRow_) {
            const unsigned frame = effect->animationFrame == 0 ? 0 : effect->animationFrame - 1;
            playerSpriteIndex = 0x3a + frame;
            break;
        }
    }
    if (playerSpriteIndex < sprites_.size()) {
        const SpriteRecord& sprite = sprites_[playerSpriteIndex];
        atlas.DrawRegion(destination,
                         static_cast<int>(playerColumn_) * kCellSize,
                         static_cast<int>(playerRow_) * kCellSize,
                         static_cast<int>(sprite.sourceColumn) * kCellSize,
                         static_cast<int>(sprite.sourceRow) * kCellSize,
                         static_cast<int>(sprite.widthInCells) * kCellSize,
                         static_cast<int>(sprite.heightInCells) * kCellSize);
    }
}

} // namespace drain_storm
