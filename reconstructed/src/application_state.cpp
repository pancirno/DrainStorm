#include "application_state.h"
#include "visual_animation.h"

#include <mmsystem.h>

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace drain_storm {
namespace {

constexpr char kGraphSaveSignature[] = {'D', 'A', 'G', '6', '4'};
constexpr std::uint16_t kGraphSaveVersion = 3;
constexpr std::uint32_t kMaximumBoardHistory = 100;
constexpr std::uint32_t kMaximumSharedBoards = 4;
constexpr std::uint32_t kMaximumCachedBoards = 256;
constexpr char kKeyBindingSignature[] = {'D', 'S', 'K', '6', '4'};
constexpr std::uint16_t kKeyBindingVersion = 1;
constexpr ApplicationState::KeyBindings kDefaultKeyBindings = {
    VK_UP, VK_DOWN, VK_RIGHT, VK_LEFT, VK_PRIOR, VK_NEXT, VK_HOME, VK_END, VK_CLEAR,
    'A', 'Z', 'F', 'P', 'M', 'Q', 'S', 'T', 'U', 'D'};

struct DemoCommand {
    UINT key{};
    bool shift{};
};

std::string_view DemoScript(std::size_t level) noexcept {
    // Extracted from selector-1080 tables 1588, 1638, 186a, and 207a. Lowercase
    // tokens carry the table's shift flag; '5' is VK_CLEAR (wait).
    switch (level) {
    case 1: return "RRRZZZZZl55555555555RRRUUUUZZZZZ555u55555";
    case 2: return "5RRRRREEDDDDDDDDDDDDDEZZZnlhRRRRRRRRRQQQ55AA555hhhhhhhhhh555555555555URR555555UUQ555NLH555DEE555555555555555555UQRREEEr5r55r5r5555555555555";
    case 10: return "RRRR5555555hhllnn5555";
    case 9: return "UUUUUUU5ZZ5d5dZZ55dUUUUUUURZZZZZZZ5555dLLd555AAAAAAUlUlUlUlAADlDlDlDlEEER5Z5l5ZZZhnEEEE555Z555555h555UHUHUHLLLLLLLLLLLLLLLLLLLLNNAAAAAAAhh5555555555ZZZZhhh55555555NNNDEE5AAAA5rr5555555RRRRRRRRRZZZZZZZZ55r5555RRULNNNNNNNLLLLLLLLLUUUUUUUUUUUU555555555555";
    default: return {};
    }
}

DemoCommand DecodeDemoCommand(char token) noexcept {
    const bool shift = token >= 'a' && token <= 'z';
    if (shift) token = static_cast<char>(token - 'a' + 'A');
    UINT key = 0;
    switch (token) {
    case '5': key = VK_CLEAR; break;
    case 'Q': key = VK_PRIOR; break;
    case 'E': key = VK_NEXT; break;
    case 'N': key = VK_END; break;
    case 'H': key = VK_HOME; break;
    case 'L': key = VK_LEFT; break;
    case 'U': key = VK_UP; break;
    case 'R': key = VK_RIGHT; break;
    case 'D': key = VK_DOWN; break;
    case 'A': key = 'A'; break;
    case 'Z': key = 'Z'; break;
    default: break;
    }
    return {key, shift};
}

bool IsSharedBoard(std::size_t level) noexcept {
    return level == 3 || level == 5 || level == 6 || level == 7;
}

template <typename T>
bool WriteGraphValue(std::ostream& stream, const T& value) {
    stream.write(reinterpret_cast<const char*>(&value), sizeof(value));
    return stream.good();
}

template <typename T>
bool ReadGraphValue(std::istream& stream, T& value) {
    stream.read(reinterpret_cast<char*>(&value), sizeof(value));
    return stream.good();
}

} // namespace

// Reconstructed from FUN_1008_a5fa. The original iterates 40 columns by 36 rows and sets bit 0.
void ApplicationState::MarkEntireBoardDirty() noexcept {
    for (BoardColumn& column : boardFlags_) {
        for (std::uint16_t& flags : column) {
            flags |= kBoardCellDirty;
        }
    }
}

void ApplicationState::AdvanceAnimation() noexcept {
    if (IsDrainAnimating()) return;
    gameData_.AdvanceAnimation();
    if (!demoMode_) {
        if (gameData_.fatalHitLatched_ && !IsFatalHitAnimating()) HandleEndState();
        return;
    }
    const std::string_view script = DemoScript(gameData_.LevelNumber());
    if (demoStep_ >= script.size()) {
        RestartDemo();
        return;
    }
    const DemoCommand command = DecodeDemoCommand(script[demoStep_]);
    const int delay = demoStep_ == 0 ? 10 : (command.shift ? 2 : 1);
    if (++demoTick_ < delay) {
        return;
    }
    demoTick_ = 0;
    if (++demoStep_ >= script.size()) {
        // 1010:dafe requests the next showcase while the final command is
        // being fetched; the last cached command is never dispatched.
        RestartDemo();
        return;
    }

    int deltaColumn = 0;
    int deltaRow = 0;
    if (command.key == VK_UP) deltaRow = -1;
    else if (command.key == VK_DOWN) deltaRow = 1;
    else if (command.key == VK_RIGHT) deltaColumn = 1;
    else if (command.key == VK_LEFT) deltaColumn = -1;
    else if (command.key == VK_PRIOR) { deltaColumn = 1; deltaRow = -1; }
    else if (command.key == VK_NEXT) { deltaColumn = 1; deltaRow = 1; }
    else if (command.key == VK_HOME) { deltaColumn = -1; deltaRow = -1; }
    else if (command.key == VK_END) { deltaColumn = -1; deltaRow = 1; }
    if (command.key == 'A') {
        gameData_.SelectPreviousItem();
    } else if (command.key == 'Z') {
        gameData_.SelectNextItem();
    } else if (command.key == VK_CLEAR || deltaColumn != 0 || deltaRow != 0) {
        if (command.shift) gameData_.UseSelectedItem(deltaColumn, deltaRow);
        else gameData_.MovePlayer(deltaColumn, deltaRow);
        PlayPendingGameplaySound();
    }
}

bool ApplicationState::MovePlayer(int deltaColumn, int deltaRow) noexcept {
    if (demoMode_ || gameData_.fatalHitLatched_) return false;
    const bool moved = gameData_.MovePlayer(deltaColumn, deltaRow);
    PlayPendingGameplaySound();
    if (moved) {
        HandleBoardTransition();
        HandleEndState();
    }
    return moved;
}

bool ApplicationState::MovePlayerToward(int column, int row) noexcept {
    if (demoMode_ || gameData_.fatalHitLatched_) return false;
    const bool moved = gameData_.MovePlayerToward(column, row);
    PlayPendingGameplaySound();
    if (moved) {
        HandleBoardTransition();
        HandleEndState();
    }
    return moved;
}

bool ApplicationState::UseSelectedItemToward(int column, int row) noexcept {
    if (demoMode_ || gameData_.fatalHitLatched_) return false;
    const bool used = gameData_.UseSelectedItemToward(column, row);
    PlayPendingGameplaySound();
    if (used) {
        HandleBoardTransition();
        HandleEndState();
    }
    return used;
}

bool ApplicationState::ActivateTransporter(bool upward) noexcept {
    if (demoMode_ || gameData_.fatalHitLatched_) return false;
    if (!gameData_.ActivateTransporter(upward)) {
        return false;
    }
    HandleBoardTransition();
    return true;
}

bool ApplicationState::UseSelectedItem(int deltaColumn, int deltaRow) noexcept {
    if (demoMode_ || gameData_.fatalHitLatched_) return false;
    const bool used = gameData_.UseSelectedItem(deltaColumn, deltaRow);
    PlayPendingGameplaySound();
    if (used) {
        HandleBoardTransition();
        HandleEndState();
    }
    return used;
}

std::wstring ApplicationState::TakeGameplayMessage() {
    std::vector<std::wstring> messages = gameData_.TakeMessages();
    if (messages.empty()) return {};
    for (std::wstring& message : messages) RecordGameplayMessage(std::move(message));
    // A batch can evict its own oldest messages. Select the first retained
    // message from this batch, never subtract beyond the bounded history.
    messageHistoryCursor_ = messageHistory_.size() -
        std::min(messages.size(), messageHistory_.size());
    messageReviewBoundary_ = messageHistoryCursor_;
    return messageHistory_[messageHistoryCursor_];
}

void ApplicationState::PostGameplayNotice(std::wstring message) {
    RecordGameplayMessage(std::move(message));
}

void ApplicationState::RecordGameplayMessage(std::wstring message) {
    // FUN_1008_55e0 owns twenty 100-byte message buffers. A twenty-first
    // message discards the oldest entry before appending a 99-character copy.
    constexpr std::size_t kHistoryCapacity = 20;
    constexpr std::size_t kMessageCharacters = 99;
    if (message.size() > kMessageCharacters) message.resize(kMessageCharacters);
    if (messageHistory_.size() == kHistoryCapacity) messageHistory_.erase(messageHistory_.begin());
    messageHistory_.push_back(std::move(message));
    // FUN_1008_554e selects the just-appended entry, not the past-end slot.
    messageHistoryCursor_ = messageHistory_.size() - 1;
    messageReviewBoundary_ = messageHistoryCursor_;
}

std::wstring ApplicationState::TakeLifecycleMessage() {
    std::wstring message = std::move(pendingLifecycleMessage_);
    pendingLifecycleMessage_.clear();
    if (message.empty()) message = gameData_.TakeDialogMessage();
    return message;
}

std::wstring ApplicationState::CurrentGameplayMessage() const {
    if (messageHistoryCursor_ >= messageHistory_.size()) return {};
    return messageHistory_[messageHistoryCursor_];
}

bool ApplicationState::HasMoreGameplayMessages() const noexcept {
    return messageHistoryCursor_ < messageHistory_.size()
        && messageHistory_.size() - messageHistoryCursor_ > 1;
}

int ApplicationState::Health() const noexcept {
    return gameData_.Health();
}

std::uint32_t ApplicationState::Score() const noexcept {
    return gameData_.Score();
}

std::size_t ApplicationState::CurrentLevel() const noexcept {
    return gameData_.LevelNumber();
}

int ApplicationState::CurrentDifficulty() const noexcept {
    return gameData_.Difficulty();
}

int ApplicationState::PlayerColumn() const noexcept {
    return static_cast<int>(gameData_.PlayerColumn());
}

int ApplicationState::PlayerRow() const noexcept {
    return static_cast<int>(gameData_.PlayerRow());
}

std::uint16_t ApplicationState::CurrentTileAt(int column, int row) const {
    if (column < 0 || row < 0 || column >= static_cast<int>(kBoardColumns)
        || row >= static_cast<int>(kBoardRows)) {
        throw std::out_of_range("board coordinate");
    }
    return gameData_.TileAt(static_cast<std::size_t>(column), static_cast<std::size_t>(row));
}

std::wstring ApplicationState::IdentifySquare(int column, int row) {
    // FUN_1008_60f4 shares FUN_1008_6f46's board-owner gate. The attract
    // showcase carries that sentinel, so right-click identification is inert
    // there just like movement, item use, and the configurable commands.
    if (demoMode_ || column < 0 || row < 0 || column >= static_cast<int>(kBoardColumns)
        || row >= static_cast<int>(kBoardRows)) {
        return {};
    }
    std::wstring description = gameData_.IdentifySquare(static_cast<std::size_t>(column),
                                                        static_cast<std::size_t>(row));
    if (!description.empty()) RecordGameplayMessage(description);
    return description;
}

std::wstring ApplicationState::NavigateMessageHistory(bool previous) {
    if (messageHistory_.empty()) return {};
    if (previous) {
        if (messageHistoryCursor_ > 0) --messageHistoryCursor_;
    } else if (messageHistoryCursor_ < messageHistory_.size()) {
        ++messageHistoryCursor_;
        messageReviewBoundary_ = std::max(messageReviewBoundary_, messageHistoryCursor_);
    }
    return CurrentGameplayMessage();
}

std::wstring ApplicationState::HighScoresText() const {
    if (highScoreReadError_) return L"Unable to read the high-score table.";
    return highScores_.Format();
}

bool ApplicationState::HasPendingHighScore() const noexcept {
    return pendingHighScoreAvailable_;
}

bool ApplicationState::SubmitPendingHighScore(const std::wstring& name) {
    if (!pendingHighScoreAvailable_) {
        return false;
    }
    std::string narrowName;
    narrowName.reserve(name.size());
    for (const wchar_t character : name) {
        narrowName.push_back(character >= 0x20 && character <= 0x7e
            ? static_cast<char>(character) : '?');
    }
    const bool inserted = highScores_.Submit(
        std::move(narrowName), pendingHighScore_, pendingHighScoreSkill_,
        pendingHighScoreHeroBonus_, pendingHighScoreSphereBonus_);
    pendingHighScore_ = 0;
    pendingHighScoreSkill_ = 0;
    pendingHighScoreAvailable_ = false;
    pendingHighScoreHeroBonus_ = false;
    pendingHighScoreSphereBonus_ = false;
    const bool saved = inserted && highScores_.Save(highScorePath_);
    if (saved) highScoreReadError_ = false;
    return saved;
}

bool ApplicationState::SelectPreviousItem() noexcept {
    return !demoMode_ && gameData_.SelectPreviousItem();
}

bool ApplicationState::SelectNextItem() noexcept {
    return !demoMode_ && gameData_.SelectNextItem();
}

bool ApplicationState::SelectVisibleArsenalItem(int clientY) noexcept {
    return !demoMode_ && gameData_.SelectVisibleArsenalItem(clientY);
}

bool ApplicationState::ScrollVisibleArsenal(bool upward) noexcept {
    return !demoMode_ && gameData_.ScrollVisibleArsenal(upward);
}

bool ApplicationState::RecallDiscoveredLaw() noexcept {
    if (demoMode_) return false;
    if (!gameData_.RecallDiscoveredLaw()) return false;
    // FUN_1008_67e6 appends the recalled selector-1020 record through
    // FUN_1008_55e0 and immediately selects it through FUN_1008_554e.
    TakeGameplayMessage();
    return true;
}

bool ApplicationState::ToggleSound() noexcept {
    soundEnabled_ = !soundEnabled_;
    if (!soundEnabled_) {
        PlaySoundW(nullptr, nullptr, 0);
    }
    return soundEnabled_;
}

UINT ApplicationState::KeyBinding(std::size_t index) const noexcept {
    return index < keyBindings_.size() ? keyBindings_[index] : 0;
}

bool ApplicationState::SaveKeyBindings() const {
    std::ofstream stream(keyBindingPath_, std::ios::binary | std::ios::trunc);
    if (!stream) return false;
    stream.write(kKeyBindingSignature, sizeof(kKeyBindingSignature));
    stream.write(reinterpret_cast<const char*>(&kKeyBindingVersion), sizeof(kKeyBindingVersion));
    stream.write(reinterpret_cast<const char*>(keyBindings_.data()),
                 static_cast<std::streamsize>(sizeof(keyBindings_)));
    return stream.good();
}

bool ApplicationState::LoadKeyBindings() {
    keyBindings_ = kDefaultKeyBindings;
    std::ifstream stream(keyBindingPath_, std::ios::binary);
    if (!stream) {
        std::error_code error;
        return !std::filesystem::exists(keyBindingPath_, error) && !error;
    }
    char signature[sizeof(kKeyBindingSignature)]{};
    std::uint16_t version{};
    KeyBindings bindings{};
    stream.read(signature, sizeof(signature));
    stream.read(reinterpret_cast<char*>(&version), sizeof(version));
    stream.read(reinterpret_cast<char*>(bindings.data()),
                static_cast<std::streamsize>(sizeof(bindings)));
    if (!stream || !std::equal(std::begin(signature), std::end(signature),
                               std::begin(kKeyBindingSignature))
        || version != kKeyBindingVersion
        || std::any_of(bindings.begin(), bindings.end(), [](UINT key) {
               return key == 0 || key > 0xff;
           })) {
        return false;
    }
    keyBindings_ = bindings;
    return true;
}

bool ApplicationState::SetKeyBindings(const KeyBindings& bindings) {
    if (std::any_of(bindings.begin(), bindings.end(), [](UINT key) {
            return key == 0 || key > 0xff;
        })) return false;
    const KeyBindings previous = keyBindings_;
    keyBindings_ = bindings;
    if (!SaveKeyBindings()) {
        keyBindings_ = previous;
        return false;
    }
    return true;
}

bool ApplicationState::ResetKeyBindings() {
    return SetKeyBindings(kDefaultKeyBindings);
}

bool ApplicationState::IsBoardCellDirty(std::size_t column, std::size_t row) const {
    if (column >= kBoardColumns || row >= kBoardRows) {
        throw std::out_of_range("board coordinate");
    }
    return (boardFlags_[column][row] & kBoardCellDirty) != 0;
}

bool ApplicationState::LoadOriginalResources(const std::filesystem::path& executablePath) {
    resourceDirectory_ = executablePath.parent_path().parent_path()
        / L"assets" / L"original";
    highScorePath_ = executablePath.parent_path() / L"DRAIN64.SCO";
    keyBindingPath_ = executablePath.parent_path() / L"DRAIN64.KEY";
    // The original keeps this table beside the executable in its 296-byte BB.BIN sidecar.
    // A malformed native table is non-fatal, matching the original error path.
    highScoreReadError_ = !highScores_.Load(highScorePath_);
    pendingLifecycleMessage_.clear();
    if (highScoreReadError_) {
        highScores_ = HighScoreTable{};
        pendingLifecycleMessage_ = L"Unable to read the high-score table. The existing file has not been changed.";
    }
    if (!LoadKeyBindings()) {
        keyBindings_ = kDefaultKeyBindings;
        if (!pendingLifecycleMessage_.empty()) pendingLifecycleMessage_ += L"\n\n";
        pendingLifecycleMessage_ += L"Unable to read keyboard commands. Default commands will be used; the existing file has not been changed.";
    }
    boardHistory_.clear();
    sharedBoards_.clear();
    currentBoardPath_.clear();
    cachedBoards_.clear();
    messageHistory_.clear();
    messageReviewBoundary_ = 0;
    messageHistoryCursor_ = 0;
    demoSeed_ = static_cast<std::uint32_t>(GetTickCount());
    if (demoSeed_ == 0) demoSeed_ = 1;
    const bool loaded = boardAtlas_.Load(resourceDirectory_ / L"bitmap_BOARD_BITMAPS.bmp")
        && gameData_.LoadDemoGame(resourceDirectory_ / L"game-data", 1, demoSeed_);
    gameData_.SetRootBoard(true);
    gameData_.SetTerminalDeathEnabled(false);
    demoMode_ = loaded;
    drainAnimationBoard_.reset();
    demoStep_ = 0;
    demoTick_ = 0;
    return loaded;
}

bool ApplicationState::NewGame(int difficulty) {
    GameData freshGame;
    demoSeed_ = demoSeed_ * 0x343fdu + 0x269ec3u
        + static_cast<std::uint32_t>(GetTickCount());
    if (demoSeed_ == 0) demoSeed_ = 1;
    if (!freshGame.LoadNewGame(resourceDirectory_ / L"game-data", difficulty, demoSeed_)) {
        return false;
    }
    gameData_ = std::move(freshGame);
    gameData_.SetRootBoard(true);
    gameData_.SetTerminalDeathEnabled(true);
    demoMode_ = false;
    drainAnimationBoard_.reset();
    demoStep_ = 0;
    demoTick_ = 0;
    boardHistory_.clear();
    sharedBoards_.clear();
    currentBoardPath_.clear();
    cachedBoards_.clear();
    messageHistory_.clear();
    messageReviewBoundary_ = 0;
    messageHistoryCursor_ = 0;
    MarkEntireBoardDirty();
    return true;
}

bool ApplicationState::QuitCurrentGame(bool submitScore) {
    // FUN_1008_1efe submits only the IDNO (quit without saving) path. IDYES
    // attempts a save first and then resets without adding a score.
    const std::uint32_t finalScore = gameData_.Score();
    const auto finalSkill = static_cast<std::uint16_t>(gameData_.Difficulty());
    const bool sphereBonus = gameData_.EnergySphereDestroyed();
    if (!RestartDemo()) return false;
    if (submitScore && highScores_.Qualifies(finalScore, false, sphereBonus)) {
        pendingHighScore_ = finalScore;
        pendingHighScoreSkill_ = finalSkill;
        pendingHighScoreAvailable_ = true;
        pendingHighScoreHeroBonus_ = false;
        pendingHighScoreSphereBonus_ = sphereBonus;
    }
    return true;
}

bool ApplicationState::IsFatalHitAnimating() const noexcept {
    return gameData_.fatalHitLatched_ && std::any_of(
        gameData_.overlays_.begin(), gameData_.overlays_.end(),
        [this](const BoardOverlay& overlay) {
            return overlay.type == 0x18 && overlay.column == gameData_.playerColumn_
                && overlay.row == gameData_.playerRow_;
        });
}

void ApplicationState::HandleEndState() {
    // FUN_1008_1b22 checks the application victory word before its separate
    // root-only death gate. The Power Plant may therefore complete the game
    // even when it was reached through a retained child-board path.
    if (gameData_.VictoryTriggered()) {
        const std::uint32_t finalScore = gameData_.Score();
        const std::uint16_t finalSkill = static_cast<std::uint16_t>(gameData_.Difficulty());
        const bool sphereBonus = gameData_.EnergySphereDestroyed();
        if (highScores_.Qualifies(finalScore, true, sphereBonus)) {
            pendingHighScore_ = finalScore;
            pendingHighScoreSkill_ = finalSkill;
            pendingHighScoreAvailable_ = true;
            pendingHighScoreHeroBonus_ = true;
            pendingHighScoreSphereBonus_ = sphereBonus;
        }
        if (RestartDemo()) {
            pendingLifecycleMessage_ =
                L"YOU ARE A HERO!\n\n"
                L"You have solved the\nultimate challenge.\n\n"
                L"The Evil in the sewer has been destroyed!\n\n"
                L"Go now, and live out your days\nin blissful retirement.\n";
        }
        return;
    }
    // FUN_1008_1b22 gates game over on application offset f4 and board offset
    // 0c. The latter distinguishes scripted attract boards, not retained
    // gameplay ancestry; death is terminal on every playable descendant.
    if ((gameData_.Health() != 0 && !gameData_.fatalHitLatched_) || demoMode_) {
        return;
    }
    if (IsFatalHitAnimating()) return;
    const std::uint32_t finalScore = gameData_.Score();
    // FUN_1018_0252 stores application offset 0xe4 at record offset 0x12.
    // FUN_1008_0630 fills that word from the difficulty dialog; it is Skill,
    // not the active board selector.
    const std::uint16_t finalSkill = static_cast<std::uint16_t>(gameData_.Difficulty());
    const std::int32_t signedFinalScore = static_cast<std::int32_t>(finalScore);
    std::wstring message;
    // FUN_1008_1b22 compares the score's high word as signed before its
    // unsigned low-word threshold, which is exactly a signed 32-bit compare.
    if (signedFinalScore < 1000) {
        message = L"Game Over.\n\n"
                  L"You did not fare well.\n"
                  L"And, you made a nice wreck out of the Effer-vessel.\n\n"
                  L"But heck, it's a game...\n"
                  L"Try again.";
    } else if (signedFinalScore < 20000) {
        message = L"Game Over.\n\n"
                  L"You showed some skill.\n"
                  L"However, Gorloth isn't overly concerned...\n"
                  L"Keep trying.";
    } else {
        message = L"Game Over.\n\n"
                  L"You have fought well.\n"
                  L"Gorloth has been warned that a skillful\n"
                  L"and dangerous warrior now pilots the Effer-vessel.";
    }
    const bool sphereBonus = gameData_.EnergySphereDestroyed();
    if (highScores_.Qualifies(finalScore, false, sphereBonus)) {
        pendingHighScore_ = finalScore;
        pendingHighScoreSkill_ = finalSkill;
        pendingHighScoreAvailable_ = true;
        pendingHighScoreHeroBonus_ = false;
        pendingHighScoreSphereBonus_ = sphereBonus;
    }
    if (RestartDemo()) {
        pendingLifecycleMessage_ = std::move(message);
    }
}

bool ApplicationState::SaveGame(const std::filesystem::path& path) const {
    if (demoMode_) return false;
    std::ofstream stream(path, std::ios::binary | std::ios::trunc);
    if (!stream) {
        return false;
    }
    stream.write(kGraphSaveSignature, sizeof(kGraphSaveSignature));
    const auto historyCount = static_cast<std::uint32_t>(boardHistory_.size());
    const auto sharedCount = static_cast<std::uint32_t>(sharedBoards_.size());
    const auto cachedCount = static_cast<std::uint32_t>(cachedBoards_.size());
    if (!WriteGraphValue(stream, kGraphSaveVersion)
        || !WriteGraphValue(stream, historyCount)
        || !WriteGraphValue(stream, sharedCount)
        || !WriteGraphValue(stream, cachedCount)) {
        return false;
    }
    for (const BoardHistoryEntry& entry : boardHistory_) {
        if (!WriteGraphValue(stream, entry.returnTile) || !entry.board.WriteGame(stream)) {
            return false;
        }
    }
    for (const auto& [level, board] : sharedBoards_) {
        const auto storedLevel = static_cast<std::uint16_t>(level);
        if (!WriteGraphValue(stream, storedLevel) || !board.WriteGame(stream)) {
            return false;
        }
    }
    for (const CachedBoardEntry& entry : cachedBoards_) {
        const auto pathLength = static_cast<std::uint32_t>(entry.path.size());
        if (!WriteGraphValue(stream, pathLength)) return false;
        for (const std::uint16_t edge : entry.path) {
            if (!WriteGraphValue(stream, edge)) return false;
        }
        if (!entry.board.WriteGame(stream)) return false;
    }
    return gameData_.WriteGame(stream);
}

bool ApplicationState::LoadGame(const std::filesystem::path& path) {
    std::ifstream stream(path, std::ios::binary);
    char signature[sizeof(kGraphSaveSignature)]{};
    stream.read(signature, sizeof(signature));
    if (!stream) {
        return false;
    }
    if (!std::equal(std::begin(signature), std::end(signature),
                    std::begin(kGraphSaveSignature))) {
        stream.close();
        if (!gameData_.LoadGame(path)) {
            return false;
        }
        gameData_.SetRootBoard(true);
        gameData_.SetTerminalDeathEnabled(true);
        boardHistory_.clear();
        sharedBoards_.clear();
        currentBoardPath_.clear();
        cachedBoards_.clear();
        messageHistory_.clear();
        messageReviewBoundary_ = 0;
        messageHistoryCursor_ = 0;
        demoMode_ = false;
        drainAnimationBoard_.reset();
        demoStep_ = 0;
        demoTick_ = 0;
        MarkEntireBoardDirty();
        return true;
    }

    std::uint16_t version{};
    std::uint32_t historyCount{};
    if (!ReadGraphValue(stream, version) || (version < 1 || version > kGraphSaveVersion)
        || !ReadGraphValue(stream, historyCount) || historyCount > kMaximumBoardHistory) {
        return false;
    }
    std::uint32_t sharedCount{};
    if (version >= 2 && (!ReadGraphValue(stream, sharedCount)
        || sharedCount > kMaximumSharedBoards)) {
        return false;
    }
    std::uint32_t cachedCount{};
    if (version >= 3 && (!ReadGraphValue(stream, cachedCount)
        || cachedCount > kMaximumCachedBoards)) {
        return false;
    }
    std::vector<BoardHistoryEntry> restoredHistory;
    restoredHistory.reserve(historyCount);
    for (std::uint32_t index = 0; index < historyCount; ++index) {
        std::uint16_t returnTile{};
        GameData board = gameData_;
        if (!ReadGraphValue(stream, returnTile) || returnTile < 0x23 || returnTile > 0x2a
            || !board.ReadGame(stream)) {
            return false;
        }
        restoredHistory.push_back({std::move(board), returnTile});
    }
    std::unordered_map<std::size_t, GameData> restoredShared;
    for (std::uint32_t index = 0; index < sharedCount; ++index) {
        std::uint16_t level{};
        GameData board = gameData_;
        if (!ReadGraphValue(stream, level) || !IsSharedBoard(level)
            || restoredShared.contains(level) || !board.ReadGame(stream)
            || board.LevelNumber() != level) {
            return false;
        }
        restoredShared.emplace(level, std::move(board));
    }
    std::vector<CachedBoardEntry> restoredCached;
    restoredCached.reserve(cachedCount);
    for (std::uint32_t index = 0; index < cachedCount; ++index) {
        std::uint32_t pathLength{};
        if (!ReadGraphValue(stream, pathLength) || pathLength == 0
            || pathLength > kMaximumBoardHistory) return false;
        std::vector<std::uint16_t> boardPath(pathLength);
        for (std::uint16_t& edge : boardPath) {
            if (!ReadGraphValue(stream, edge) || edge < 0x23 || edge > 0x2a) return false;
        }
        if (std::any_of(restoredCached.begin(), restoredCached.end(),
                        [&](const CachedBoardEntry& entry) { return entry.path == boardPath; })) {
            return false;
        }
        GameData board = gameData_;
        if (!board.ReadGame(stream)) return false;
        restoredCached.push_back({std::move(boardPath), std::move(board)});
    }
    GameData restoredCurrent = gameData_;
    if (!restoredCurrent.ReadGame(stream)) {
        return false;
    }
    gameData_ = std::move(restoredCurrent);
    boardHistory_ = std::move(restoredHistory);
    sharedBoards_ = std::move(restoredShared);
    cachedBoards_ = std::move(restoredCached);
    // Pre-v23 board payloads omitted depth. Recover it from retained ancestry
    // where available; standalone legacy boards use their inferred root depth.
    const auto rootDepth = boardHistory_.empty() ? gameData_.boardDepth_
        : boardHistory_.front().board.boardDepth_;
    const auto restoreDepth = [rootDepth](GameData& board, std::size_t distance) {
        if (!board.depthFromSave_) board.boardDepth_ = static_cast<std::uint16_t>(
            std::min<std::size_t>(0x7fff, rootDepth + distance));
    };
    restoreDepth(gameData_, boardHistory_.size());
    for (std::size_t index = 0; index < boardHistory_.size(); ++index)
        restoreDepth(boardHistory_[index].board, index);
    for (auto& entry : cachedBoards_) restoreDepth(entry.board, entry.path.size());
    for (auto& [level, board] : sharedBoards_) {
        if (board.depthFromSave_) continue;
        if (gameData_.LevelNumber() == level) board.boardDepth_ = gameData_.boardDepth_;
        else for (const auto& entry : boardHistory_)
            if (entry.board.LevelNumber() == level) board.boardDepth_ = entry.board.boardDepth_;
    }
    gameData_.SetRootBoard(boardHistory_.empty());
    gameData_.SetTerminalDeathEnabled(true);
    for (std::size_t index = 0; index < boardHistory_.size(); ++index) {
        boardHistory_[index].board.SetRootBoard(index == 0);
        boardHistory_[index].board.SetTerminalDeathEnabled(true);
    }
    for (auto& [level, board] : sharedBoards_) {
        static_cast<void>(level);
        board.SetRootBoard(false);
        board.SetTerminalDeathEnabled(true);
    }
    for (CachedBoardEntry& entry : cachedBoards_) {
        entry.board.SetRootBoard(false);
        entry.board.SetTerminalDeathEnabled(true);
    }
    currentBoardPath_.clear();
    currentBoardPath_.reserve(boardHistory_.size());
    for (const BoardHistoryEntry& entry : boardHistory_) {
        currentBoardPath_.push_back(entry.returnTile);
    }
    messageHistory_.clear();
    messageReviewBoundary_ = 0;
    messageHistoryCursor_ = 0;
    demoMode_ = false;
    drainAnimationBoard_.reset();
    demoStep_ = 0;
    demoTick_ = 0;
    MarkEntireBoardDirty();
    return true;
}

bool ApplicationState::RestartDemo() noexcept {
    drainAnimationBoard_.reset();
    try {
        demoSeed_ = demoSeed_ * 0x343fdu + 0x269ec3u;
        if (demoSeed_ == 0) demoSeed_ = 1;
        GameData nextDemo;
        if (!nextDemo.LoadDemoGame(resourceDirectory_ / L"game-data", 1, demoSeed_)) {
            return false;
        }
        gameData_ = std::move(nextDemo);
        gameData_.SetRootBoard(true);
        gameData_.SetTerminalDeathEnabled(false);
        demoMode_ = true;
        boardHistory_.clear();
        sharedBoards_.clear();
        currentBoardPath_.clear();
        cachedBoards_.clear();
        demoStep_ = 0;
        demoTick_ = 0;
        MarkEntireBoardDirty();
        return true;
    } catch (...) {
        return false;
    }
}

bool ApplicationState::PrepareHubBranches() {
    // 1010:4382 creates every hub connection on the first departure, in this
    // order. Keep construction atomic so a failed resource read leaves the
    // current graph and global random stream untouched.
    auto preparedShared = sharedBoards_;
    auto preparedCached = cachedBoards_;
    std::uint32_t nextRandomState = gameData_.randomState_;
    constexpr std::pair<std::size_t, std::uint16_t> branches[] = {
        {6u, static_cast<std::uint16_t>(0x27)},
        {7u, static_cast<std::uint16_t>(0x28)},
        {2u, static_cast<std::uint16_t>(0x29)},
        {11u, static_cast<std::uint16_t>(0x2a)}};
    for (const auto [level, exitTile] : branches) {
        auto path = currentBoardPath_;
        path.push_back(exitTile);
        const bool present = IsSharedBoard(level) ? preparedShared.contains(level)
            : std::any_of(preparedCached.begin(), preparedCached.end(),
                [&](const CachedBoardEntry& entry) { return entry.path == path; });
        if (present) continue;
        GameData board;
        if (!board.Load(resourceDirectory_ / L"game-data", level, nextRandomState,
                        gameData_.Difficulty(), std::min<int>(0x7fff, gameData_.boardDepth_ + 1),
                        level == 11)) {
            return false;
        }
        nextRandomState = board.randomState_;
        if (IsSharedBoard(level)) preparedShared.insert_or_assign(level, std::move(board));
        else preparedCached.push_back({std::move(path), std::move(board)});
    }
    if (!preparedShared.contains(5)) {
        GameData board;
        if (!board.Load(resourceDirectory_ / L"game-data", 5, nextRandomState,
                        gameData_.Difficulty(), std::min<int>(0x7fff, gameData_.boardDepth_ + 2))) {
            return false;
        }
        nextRandomState = board.randomState_;
        preparedShared.insert_or_assign(5, std::move(board));
    }
    sharedBoards_ = std::move(preparedShared);
    cachedBoards_ = std::move(preparedCached);
    gameData_.randomState_ = nextRandomState;
    return true;
}

void ApplicationState::HandleBoardTransition() {
    const int request = gameData_.TakeTransitionRequest();
    if (request == 0) {
        return;
    }
    if (request > 0 && gameData_.LevelNumber() == 3 && !PrepareHubBranches()) {
        return;
    }
    // 1008:b78a shows five 50ms frames on the source drain before the
    // destination appears. Keep this visual snapshot out of saved board state.
    const auto drain = gameData_.TileAt(gameData_.PlayerColumn(), gameData_.PlayerRow());
    if (!demoMode_ && drain >= 0x23 && drain <= 0x2a) {
        drainAnimationBoard_ = gameData_;
        drainAnimationFrame_ = 0;
    }
    const auto cachePath = [this](std::vector<std::uint16_t> path) {
        // The shared 1058 board and its descendants have one identity even
        // when reached through the hub's other arm. Keep traversal history
        // separate from the canonical child-cache key.
        for (std::size_t index = 2; index <= boardHistory_.size(); ++index) {
            const auto level = index == boardHistory_.size() ? gameData_.LevelNumber()
                : boardHistory_[index].board.LevelNumber();
            if (level == 5 && boardHistory_[index - 2].board.LevelNumber() == 3
                && path.size() >= index && path[index - 1] == 0x27
                && (path[index - 2] == 0x27 || path[index - 2] == 0x28)) {
                path[index - 2] = 0x27;
                path[index - 1] = 0x27;
            }
        }
        return path;
    };
    if (IsSharedBoard(gameData_.LevelNumber())) {
        sharedBoards_.insert_or_assign(gameData_.LevelNumber(), gameData_);
    }
    if (request < 0) {
        if (boardHistory_.empty()) {
            return;
        }
        auto retainedHistory = boardHistory_;
        auto retainedPath = currentBoardPath_;
        // Earlier builds pushed 1058 onto the stack when returning to its
        // parents. Repair those recoverable old-save paths using the hub.
        if (gameData_.LevelNumber() == 6 || gameData_.LevelNumber() == 7) {
            for (std::size_t index = retainedHistory.size(); index-- > 0;) {
                if (retainedHistory[index].board.LevelNumber() != 3) continue;
                retainedHistory.resize(index + 1);
                retainedHistory.back().returnTile = gameData_.LevelNumber() == 6 ? 0x27 : 0x28;
                retainedPath.resize(index + 1);
                retainedPath.back() = retainedHistory.back().returnTile;
                break;
            }
        }
        // Do not move out of live history until the destination is validated.
        BoardHistoryEntry parent = retainedHistory.back();
        if (const auto latest = sharedBoards_.find(parent.board.LevelNumber());
            latest != sharedBoards_.end()) parent.board = latest->second;
        parent.board.AdoptPlayerStateFrom(gameData_);
        if (!parent.board.PositionPlayerAtTile(parent.returnTile)) {
            return;
        }
        if (!currentBoardPath_.empty() && !IsSharedBoard(gameData_.LevelNumber())) {
            const auto key = cachePath(currentBoardPath_);
            const auto existing = std::find_if(
                cachedBoards_.begin(), cachedBoards_.end(),
                [&](const CachedBoardEntry& entry) { return cachePath(entry.path) == key; });
            if (existing != cachedBoards_.end()) {
                existing->board = gameData_;
            }
            else if (cachedBoards_.size() < kMaximumCachedBoards) {
                cachedBoards_.push_back({key, gameData_});
            }
        }
        retainedHistory.pop_back();
        if (!retainedPath.empty()) retainedPath.pop_back();
        boardHistory_ = std::move(retainedHistory);
        currentBoardPath_ = std::move(retainedPath);
        gameData_ = std::move(parent.board);
        gameData_.SetRootBoard(boardHistory_.empty());
        gameData_.SetTerminalDeathEnabled(true);
        MarkEntireBoardDirty();
        return;
    }

    int resolvedRequest = request;
    if (request == GameData::kDeferredProceduralExit) {
        auto path = currentBoardPath_;
        path.push_back(gameData_.TileAt(gameData_.PlayerColumn(), gameData_.PlayerRow()));
        const auto existing = std::find_if(cachedBoards_.begin(), cachedBoards_.end(),
            [&](const CachedBoardEntry& entry) { return cachePath(entry.path) == cachePath(path); });
        if (existing != cachedBoards_.end()) {
            resolvedRequest = static_cast<int>(existing->board.LevelNumber());
        } else {
            const int depth = static_cast<int>(gameData_.LevelNumber() - 16);
            resolvedRequest = gameData_.NextRandom(8) >= depth
                ? static_cast<int>(gameData_.LevelNumber() + 1) : 3;
        }
    }
    const std::size_t requestedLevel = static_cast<std::size_t>(resolvedRequest);
    GameData child;
    bool constructedFreshChild = false;
    std::uint16_t entryTile = gameData_.LevelNumber() == 7 && requestedLevel == 5 ? 0x24 : 0x23;
    auto nextHistory = boardHistory_;
    bool retainSource = true;
    const std::uint16_t returnTile = gameData_.TileAt(
        gameData_.PlayerColumn(), gameData_.PlayerRow());
    std::vector<std::uint16_t> childPath = currentBoardPath_;
    childPath.push_back(returnTile);
    // 1058 has two real parents, not a traversal-stack backlink: 596a
    // selects 1060/1068 and lands on their 0x27. Rebuild that branch's
    // ancestry through the common hub even if it has never been visited.
    if (gameData_.LevelNumber() == 5 && (requestedLevel == 6 || requestedLevel == 7)) {
        entryTile = 0x27;
        retainSource = false;
        for (std::size_t index = nextHistory.size(); index-- > 0;) {
            if (nextHistory[index].board.LevelNumber() != 3) continue;
            nextHistory.resize(index + 1);
            nextHistory.back().returnTile = requestedLevel == 6 ? 0x27 : 0x28;
            childPath.resize(index);
            childPath.push_back(nextHistory.back().returnTile);
            break;
        }
        if (const auto existing = sharedBoards_.find(requestedLevel); existing != sharedBoards_.end()) {
            child = existing->second;
        } else if (!child.Load(resourceDirectory_ / L"game-data", requestedLevel, gameData_.randomState_,
                               gameData_.Difficulty(), std::max<int>(1, gameData_.boardDepth_ - 1))) {
            return;
        } else {
            constructedFreshChild = true;
        }
        goto child_ready;
    }
    if (IsSharedBoard(requestedLevel)) {
        for (std::size_t index = boardHistory_.size(); index-- > 0;) {
            if (boardHistory_[index].board.LevelNumber() != requestedLevel) {
                continue;
            }
            entryTile = boardHistory_[index].returnTile;
            child = sharedBoards_.contains(requestedLevel)
                ? sharedBoards_.at(requestedLevel) : boardHistory_[index].board;
            nextHistory.resize(index);
            childPath.resize(index);
            retainSource = false;
            goto child_ready;
        }
        if (const auto existing = sharedBoards_.find(requestedLevel);
            existing != sharedBoards_.end()) {
            child = existing->second;
            goto child_ready;
        }
    } else if (const auto existing = std::find_if(
                   cachedBoards_.begin(), cachedBoards_.end(),
                   [&](const CachedBoardEntry& entry) { return cachePath(entry.path) == cachePath(childPath); });
               existing != cachedBoards_.end()) {
        child = existing->board;
        goto child_ready;
    }
    if (!child.Load(resourceDirectory_ / L"game-data", requestedLevel, gameData_.randomState_,
                    gameData_.Difficulty(), std::min<int>(0x7fff, gameData_.boardDepth_ + 1))) {
        return;
    }
    constructedFreshChild = true;
    if (gameData_.LevelNumber() == 11 && requestedLevel == 8) {
        // ce62 writes the otherwise absent forward drain only for this rare
        // caller-specific branch of c80e.
        child.level_[20][5] = 0x27;
    }
child_ready:
    child.AdoptPlayerStateFrom(gameData_, constructedFreshChild);
    if (!child.PositionPlayerAtTile(entryTile)) {
        return;
    }
    if (retainSource) nextHistory.push_back({gameData_, returnTile});
    boardHistory_ = std::move(nextHistory);
    child.SetRootBoard(boardHistory_.empty());
    child.SetTerminalDeathEnabled(true);
    gameData_ = std::move(child);
    currentBoardPath_ = std::move(childPath);
    MarkEntireBoardDirty();
}

void ApplicationState::PlayPendingGameplaySound() noexcept {
    const GameplaySound event = gameData_.TakeSoundEvent();
    if (!soundEnabled_) {
        return;
    }
    // FUN_1008_2eba's complete table, in the same order consumed by
    // FUN_1008_31d4's priority comparison.
    constexpr const wchar_t* soundFiles[] = {
        L"wave_SND1_THUK.wav", L"wave_SND3_THUKS.wav",
        L"wave_SNDBOMB_SHOT.wav", L"wave_SNDBOMB_SHOTX8.wav",
        L"wave_SNDMISSILE_LAUNCH.wav", L"wave_SNDMISSILE_LAUNCHX8.wav",
        L"wave_SNDDEAD_XOID.wav", L"wave_SNDDEAD_XOID2.wav",
        L"wave_SNDDEAD_XOID3.wav", L"wave_SNDPOP.wav",
        L"wave_SNDDOUBLE_POP.wav", L"wave_SNDTRIPLE_POP.wav",
        L"wave_SNDEXPLODE1.wav", L"wave_SNDCRACKLE.wav",
        L"wave_SNDCASINOBET.wav", L"wave_SNDCASINOWIN.wav",
        L"wave_SNDCASINOLOSE.wav", L"wave_SNDCASINODRAWLEV.wav",
        L"wave_SNDDOOROPEN.wav", L"wave_SNDSPEC_DOOROPEN.wav",
        L"wave_SNDTREASURE1.wav", L"wave_SNDTREASURE2.wav",
        L"wave_SNDDOOWUK.wav", L"wave_SNDRADIO_BEEP.wav",
        L"wave_SNDEXPLODE2.wav", L"wave_SNDEXPLODE3.wav",
        L"wave_SNDYEOW.wav", L"wave_SNDWAKEUP.wav",
        L"wave_SNDPLAYER_HIT.wav", L"wave_SNDPLAYER_HIT_TWICE.wav",
        L"wave_SNDPLAYER_DIES.wav"};
    const int soundIndex = static_cast<int>(event);
    const wchar_t* fileName = soundIndex >= 0
        && soundIndex < static_cast<int>(std::size(soundFiles))
        ? soundFiles[soundIndex] : nullptr;
    if (fileName != nullptr) {
        const std::filesystem::path path = resourceDirectory_ / fileName;
        PlaySoundW(path.c_str(), nullptr, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);
    }
}

void ApplicationState::DrawBoard(HDC destination, int pressedDemoButton) const {
    if (drainAnimationBoard_) {
        const auto& source = *drainAnimationBoard_;
        source.DrawLevel(destination, boardAtlas_);
        const auto tile = source.TileAt(source.PlayerColumn(), source.PlayerRow());
        const auto& sprite = source.sprites_[(tile <= 0x26 ? 0x35 : 0x30) + drainAnimationFrame_];
        boardAtlas_.DrawRegion(destination, static_cast<int>(source.PlayerColumn()) * 12,
            static_cast<int>(source.PlayerRow()) * 12, sprite.sourceColumn * 12,
            sprite.sourceRow * 12, sprite.widthInCells * 12, sprite.heightInCells * 12);
        return;
    }
    gameData_.DrawLevel(destination, boardAtlas_);
    if (demoMode_) DrawDemoPanel(destination, pressedDemoButton);
}

bool ApplicationState::AdvanceDrainAnimation() noexcept {
    if (!drainAnimationBoard_) return false;
    if (++drainAnimationFrame_ >= 5) drainAnimationBoard_.reset();
    return true;
}

RECT ApplicationState::DemoPanelBounds() const noexcept {
    // 1008:198e moves the 240x144 panel in board-client pixel coordinates.
    const auto level = gameData_.LevelNumber();
    const int x = level == 5 || level == 10 ? 12 : level == 6 ? 228 : level == 16 ? 24 : 144;
    const int y = level == 5 || level == 6 || level == 10 ? 12 : 276;
    return {x, y, x + 240, y + 144};
}

int ApplicationState::DemoButtonAt(int x, int y) const noexcept {
    if (!demoMode_) return -1;
    const RECT panel = DemoPanelBounds();
    x -= panel.left; y -= panel.top;
    for (int button = 0; button < 3; ++button)
        if (x >= 24 + button * 84 && x < 48 + button * 84 && y >= 96 && y < 120)
            return button;
    return -1;
}

void ApplicationState::DrawDemoPanel(HDC dc, int pressedButton) const {
    // Missing Ghidra export: original paint handler 1010:78e0, with embossed
    // title helper 1008:3d50 and atlas-backed buttons at 1010:8216.
    const int saved = SaveDC(dc);
    const RECT panel = DemoPanelBounds();
    OffsetViewportOrgEx(dc, panel.left, panel.top, nullptr);
    IntersectClipRect(dc, 0, 0, 240, 144);
    RECT area{0, 0, 240, 144};
    FillRect(dc, &area, static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH)));
    const auto line = [&](int x1, int y1, int x2, int y2, COLORREF color, int width = 1) {
        HPEN pen = CreatePen(PS_SOLID, width, color);
        const auto old = SelectObject(dc, pen);
        MoveToEx(dc, x1, y1, nullptr); LineTo(dc, x2, y2);
        SelectObject(dc, old); DeleteObject(pen);
    };
    line(0, 144, 0, 0, RGB(255,255,255), 3);
    line(0, 0, 239, 0, RGB(255,255,255), 3);
    line(239, 0, 239, 143, RGB(64,64,64), 3);
    line(239, 143, 0, 143, RGB(64,64,64), 3);
    for (int offset = 0; offset < 3; ++offset) {
        const COLORREF color = offset == 0 ? RGB(255,255,255) : offset == 1 ? RGB(128,128,128) : RGB(0,0,0);
        const int left = 5 + offset, right = 232 + offset, top = 5 + offset, bottom = 84 + offset;
        line(left, top, right, top, color); line(right, top, right, bottom, color);
        line(right, bottom, left, bottom, color); line(left, bottom, left, top, color);
    }
    const auto font = [](int height, int weight) {
        return CreateFontW(-height, 0, 0, 0, weight, FALSE, FALSE, FALSE, ANSI_CHARSET,
            OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, L"Courier New");
    };
    HFONT large = font(36, FW_BOLD), medium = font(24, FW_BOLD), normal = font(12, FW_NORMAL);
    TEXTMETRICW largeMetrics{}, mediumMetrics{}, normalMetrics{};
    SelectObject(dc, large); GetTextMetricsW(dc, &largeMetrics);
    SelectObject(dc, medium); GetTextMetricsW(dc, &mediumMetrics);
    SelectObject(dc, normal); GetTextMetricsW(dc, &normalMetrics);
    SetBkMode(dc, TRANSPARENT);
    const auto text = [&](int x, int y, std::wstring_view value, COLORREF color) {
        SetTextColor(dc, color); TextOutW(dc, x, y, value.data(), static_cast<int>(value.size()));
    };
    const auto embossed = [&](int x, int y, std::wstring_view value) {
        text(x - 1, y - 1, value, RGB(255,255,255));
        text(x + 1, y + 1, value, RGB(0,0,0));
        text(x, y, value, RGB(255,0,0));
    };
    const int a = largeMetrics.tmAveCharWidth, b = mediumMetrics.tmAscent;
    const int e = mediumMetrics.tmAveCharWidth, n = normalMetrics.tmAveCharWidth * 5;
    text(2 * (2 * (e + 9) + a) + n, 3 * b / 2 + 5, L"TM", RGB(0,0,0));
    text(12, 68, L"© 1997 Dave Horner", RGB(0,0,255));
    text(12, 10, L"Version", RGB(0,0,0)); text(12, 22, L"1.02", RGB(0,0,0));
    SelectObject(dc, large);
    embossed(n + 36, b / 2, L"D"); embossed(n + a + 36, 3 * b / 2, L"S");
    SelectObject(dc, medium);
    embossed(n + a + 36, b / 2 + 5, L"RAIN");
    embossed(n + 2 * (a + 18), 3 * b / 2 + 5, L"TORM");
    SelectObject(dc, normal);
    const auto sprite = [&](int x, int y, std::size_t index) {
        if (index >= gameData_.sprites_.size()) return;
        const auto& record = gameData_.sprites_[index];
        boardAtlas_.DrawRegion(dc, x, y, record.sourceColumn * 12, record.sourceRow * 12,
            record.widthInCells * 12, record.heightInCells * 12);
    };
    const auto frame = gameData_.animationFrame_;
    const auto squid = frame % 10;
    constexpr unsigned bulldozer[] = {10, 11, 10, 12};
    const std::size_t icons[] = {gameData_.PlayerSpriteIndex(false),
        squid > 5 ? 11 - squid : squid + 1, bulldozer[frame % 4]};
    for (int button = 0; button < 3; ++button) {
        const int pressed = button == pressedButton ? 1 : 0;
        sprite(24 + button * 84, 96, 0x1b0 + pressed);
        sprite(30 + button * 84 + pressed, 102 + pressed, icons[button]);
    }
    text(12, 122, L"New Game", RGB(0,0,0)); text(12, 122, L"N", RGB(255,0,0));
    text(96, 122, L"Load Game", RGB(0,0,0)); text(96, 122, L"L", RGB(255,0,0));
    text(192, 122, L"Help", RGB(0,0,0));
    RestoreDC(dc, saved);
    DeleteObject(large); DeleteObject(medium); DeleteObject(normal);
}

void ApplicationState::DrawMessagePanel(HDC dc, int width) const {
    const int saved = SaveDC(dc);
    IntersectClipRect(dc, 0, 95, width, 193);
    SelectObject(dc, GetStockObject(DC_PEN));
    SetDCPenColor(dc, RGB(0, 0, 0));
    MoveToEx(dc, 0, 193, nullptr); LineTo(dc, width - 1, 193); LineTo(dc, width - 1, 95);
    SetDCPenColor(dc, RGB(128, 128, 128));
    MoveToEx(dc, 0, 193, nullptr); LineTo(dc, 0, 95); LineTo(dc, width - 1, 95);
    HFONT font = CreateFontW(14, 8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        FIXED_PITCH | FF_MODERN, L"Courier New");
    SelectObject(dc, font);
    SetBkMode(dc, TRANSPARENT);
    const bool reviewing = messageHistoryCursor_ < messageReviewBoundary_;
    SetTextColor(dc, reviewing ? RGB(96, 96, 96) : RGB(0, 0, 128));
    const auto message = CurrentGameplayMessage();
    // 520c uses an eight-pixel character grid and fourteen-pixel lines,
    // starts at (3,98), and stops after y=179. Preserve explicit newlines.
    int x = 3, y = 98;
    std::size_t cursor = 0;
    while (cursor < message.size() && y <= 179) {
        if (message[cursor] == L'\r') { ++cursor; continue; }
        if (message[cursor] == L'\n') { x = 3; y += 14; ++cursor; continue; }
        if (x == 3 && message[cursor] == L' ') { ++cursor; continue; }
        auto wordEnd = cursor;
        while (wordEnd < message.size() && message[wordEnd] != L' '
            && message[wordEnd] != L'\n' && message[wordEnd] != L'\r') ++wordEnd;
        if (x + static_cast<int>(wordEnd - cursor) * 8 >= width && x != 3) {
            x = 3; y += 14; continue;
        }
        if (x + 8 > width) { x = 3; y += 14; continue; }
        TextOutW(dc, x, y, &message[cursor], 1);
        ++cursor;
        x += 8;
    }
    if (HasMoreGameplayMessages()) {
        SetTextColor(dc, RGB(96, 96, 96));
        TextOutW(dc, 48, 178, L"[ ]ore", 6);
        SetTextColor(dc, reviewing ? RGB(0, 0, 0) : RGB(0, 0, 255));
        TextOutW(dc, 55, 179, L"M", 1);
    }
    RestoreDC(dc, saved);
    DeleteObject(font);
}

void ApplicationState::DrawControlSurroundings(HDC dc, const RECT& client, RECT arsenal, RECT board) {
    const int saved = SaveDC(dc);
    ExcludeClipRect(dc, arsenal.left, arsenal.top, arsenal.right, arsenal.bottom);
    ExcludeClipRect(dc, board.left, board.top, board.right, board.bottom);
    FillRect(dc, &client, static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH)));
    for (RECT border : {arsenal, board}) {
        InflateRect(&border, 2, 2);
        DrawEdge(dc, &border, EDGE_SUNKEN, BF_RECT);
    }
    RestoreDC(dc, saved);
}

std::wstring ApplicationState::HelpPageText(int page) const {
    switch (page) {
    case 1:
        return L"A Tactical Nightmare...\r\n\r\n"
            L"IN THE MURKY CHAMBERS beneath your shower drain, an army of gruesome invaders "
            L"is mobilizing. Their evil plan: to take over the shower, then the whole bathroom, "
            L"then the ENTIRE GALAXY.\r\n\r\n"
            L"Only you can stop them. Fortunately you have help in the form of a sophisticated "
            L"and powerful drain probe.\r\n\r\nClick \">\" button (at top) to read on...";
    case 2:
        return L"...continued\r\n\r\nThis mighty drain probe is named the Effer-vessel.\r\n\r\n"
            L"The Effer-vessel has a unique weapon: It creates giant, synthetic bubbles capable "
            L"of engulfing whole monsters. But beware, the enemy attacks relentlessly and in huge "
            L"numbers. Use these bubbles early and often.\r\n\r\n"
            L"You'll find other handy weapons and items along the way. These will aid you as you "
            L"fight your way to deeper and deeper levels.\r\n\r\nHere's a look:\r\n\r\nClick \">\" button to read on...";
    case 3:
        return L"Good Luck.\r\n\r\nOnce you've mastered the art of survival, you can turn your "
            L"attention to a dangerous mission:\r\n\r\nA dark and powerful force lurks deep below which "
            L"enslaves and controls the beasts of the drain. Your ultimate challenge is to find and "
            L"destroy this sinister creature and annihilate his ability to wage war...\r\n\r\n"
            L"One warning: do not go lightly armed into that final confrontation...";
    case 4:
        return L"Some Elements of the Game\r\n\r\n"
            L"SEWER SQUID. Countless in numbers; Relentless in pursuit.\r\n\r\n"
            L"BULLDOZER. Destroys crud, decoys, and YOU!\r\n\r\n"
            L"DRAINS. descend/ascend to next level.\r\n\r\n"
            L"PORTAL. Brings monsters into battle. Use bombs to destroy.\r\n\r\n"
            L"CRUD. (hard water buildup).\r\n\r\n"
            L"TREASURE. Replenish your arsenal with weapons and other items.\r\n\r\n"
            L"NOTE: RIGHT-CLICK ON ANY SQUARE TO IDENTIFY.";
    case 5:
        return L"Your Weapons\r\n\r\n"
            L"Bubble: Stops monsters in their tracks.\r\n"
            L"Bomb: Destroys most objects.\r\n"
            L"Missile: More powerful than bombs. Destroys Doors.\r\n"
            L"Mine: Destroys monsters who step on them.\r\n"
            L"Decoy: Attracts nearby monsters. Very useful.\r\n"
            L"Fireball: Kills all monsters in its path.\r\n"
            L"Burrow Bomb: Good for blasting through rockpiles.\r\n"
            L"Radio-Controlled Bomb: Destroys large area. Detonates on command.";
    case 6:
        return L"Keyboard Commands\r\n\r\nUse these simple keys...\r\n\r\n"
            L"1) Use number pad (arrow keys) to move.\r\n"
            L"2) Press [Shift]+arrow key to shoot in any direction.\r\n"
            L"3) Press [A] and [Z] to choose weapon from arsenal.\r\n\r\n"
            L"Note: [Num Lock] key must be off.";
    case 7:
        return L"More Commands\r\n\r\n"
            L"Start a [N]ew game\r\n[L]oad a previously saved game\r\n"
            L"[S]ave the current game\r\n[Q]uit the current game\r\n"
            L"[T]oggle sound\r\nRead next [M]essage if there is one\r\n"
            L"Scroll through [P]revious messages\r\nReread your [F]ortunes\r\n"
            L"Display Help and Information";
    case 8:
        return L"Laws of the Drain\r\n\r\nAn enchantment lies below your shower drain which "
            L"alters the laws of nature as follows:\r\n\r\n"
            L"* Each time you move one square, the monsters also move one square, likely in your "
            L"direction. There is no time limit between your moves.\r\n\r\n"
            L"* Bubbles, Bombs and other projectiles also move one square each time you move. "
            L"They follow a straight line of motion.\r\n\r\n"
            L"* Your weapons will not harm you. You can step over mines, and bombs will pass "
            L"right through you.\r\n\r\n"
            L"* You have an endless supply of Bubbles. Other weapons are limited to the amount "
            L"displayed in your arsenal.";
    case 9:
        return L"Tips\r\n\r\n"
            L"* Some weapons are not direction-specific, such as Bomb Bursts and Smart Bombs. "
            L"These can be used by pressing <Shift> with any arrow key.\r\n\r\n"
            L"* Press number pad key 5 to skip turn (i.e. monsters and projectiles move one "
            L"square, you don't).\r\n\r\n"
            L"* A red question-mark is a signpost which may contain helpful information.\r\n\r\n"
            L"* Right-click on any object in the playing field of the Drain to identify it.\r\n\r\n"
            L"* Use the <Shift>-number pad key combination for more than just shooting weapons. "
            L"You can open doors with keys, deploy decoys, read fortune cookies, and wager at the "
            L"casino (while on the betting square).";
    case 10:
        return L"High Scores\r\n\r\n" + HighScoresText();
    case 11:
        return L"Registration\r\n\r\nThis version of Drain Storm is $0 shareware. There's no cost!\r\n\r\n"
            L"I ask only that you register your copy if you fit either category below:\r\n\r\n"
            L"* You enjoy playing Drain Storm.\r\n"
            L"* You don't enjoy it. But you play because the drain monsters frustrate and anger "
            L"you, and killing them gives you satisfaction.\r\n\r\n"
            L"Registered users can be updated on future Drain Storm releases. It's easy (and free!).";
    default:
        return L"Coming Attractions\r\n\r\nDrain Storm, Deluxe Version is coming soon. It contains:\r\n"
            L"* New, never before seen challenge boards.\r\n"
            L"* Full-window, resizable weapon selector.\r\n"
            L"* Configurable keyboard commands. Great for laptops.\r\n"
            L"* Drain Storm Custom Editor.\r\n* Drain Storm II.";
    }
}

bool ApplicationState::DrawHelpPage(HDC dc, int page, const RECT& bounds,
                                    std::uint32_t animationFrame, std::wstring_view narrative) const {
    if (page < 1 || page > 12) return false;
    const auto defaultNarrative = narrative.empty() ? HelpPageText(page) : std::wstring{};
    if (narrative.empty()) narrative = defaultNarrative;
    if (bounds.right <= bounds.left || bounds.bottom <= bounds.top) return false;
    const auto playerIllustrationSprite = [this, animationFrame]() {
        // 1010:afd8 uses 1008:2d50, including facing, repellent, attack and
        // wreck branches. Paint a copy so help redraws cannot advance attack.
        GameData visual = gameData_;
        visual.animationFrame_ = animationFrame;
        return visual.PlayerSpriteIndex((GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0);
    };
    const int saved = SaveDC(dc);
    // Original page callbacks use a 444 by 348 logical drawing area.
    IntersectClipRect(dc, bounds.left, bounds.top, bounds.right, bounds.bottom);
    SetMapMode(dc, MM_ANISOTROPIC);
    SetWindowExtEx(dc, 444, 348, nullptr);
    SetViewportExtEx(dc, bounds.right - bounds.left, bounds.bottom - bounds.top, nullptr);
    SetViewportOrgEx(dc, bounds.left, bounds.top, nullptr);
    RECT canvas{0, 0, 444, 348};
    FillRect(dc, &canvas, static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH)));
    HFONT normal = CreateFontW(14, 8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        FIXED_PITCH | FF_MODERN, L"Courier New");
    HFONT heading = CreateFontW(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH, L"Arial");
    SetBkMode(dc, TRANSPARENT);
    const auto text = [&](int x, int y, int width, int height, std::wstring_view value,
                          COLORREF color = RGB(128, 0, 0), bool title = false,
                          bool embossed = false) {
        SelectObject(dc, title ? heading : normal);
        SetTextColor(dc, color);
        RECT area{x, y, x + width, y + height};
        if (title || embossed) {
            // 1008:3cc2/3d50: white upper-left, black lower-right, then ink.
            RECT highlight = area, shadow = area;
            OffsetRect(&highlight, -1, -1);
            OffsetRect(&shadow, 1, 1);
            SetTextColor(dc, RGB(255, 255, 255));
            DrawTextW(dc, value.data(), static_cast<int>(value.size()), &highlight,
                DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);
            SetTextColor(dc, RGB(0, 0, 0));
            DrawTextW(dc, value.data(), static_cast<int>(value.size()), &shadow,
                DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);
            SetTextColor(dc, color);
        }
        DrawTextW(dc, value.data(), static_cast<int>(value.size()), &area,
            DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);
    };
    const auto sprite = [&](int x, int y, std::size_t index) {
        if (index >= gameData_.sprites_.size()) return;
        const auto& record = gameData_.sprites_[index];
        boardAtlas_.DrawRegion(dc, x, y, record.sourceColumn * 12, record.sourceRow * 12,
            record.widthInCells * 12, record.heightInCells * 12);
    };
    if (page == 4) {
        // 1010:a16e: examples are composed from the gameplay atlas, not icons
        // guessed from the accompanying descriptions.
        text(12, 12, 420, 28, L"Some Elements of the Game", RGB(80, 80, 80), true);
        text(12, 48, 168, 112, L"SEWER SQUID. Countless in numbers; Relentless in pursuit.");
        const auto squidPhase = animationFrame % 10;
        sprite(192, 60, squidPhase > 5 ? 11 - squidPhase : squidPhase + 1);
        text(300, 48, 132, 84, L"BULLDOZER. Destroys crud, decoys, and YOU!", RGB(0, 0, 128));
        constexpr std::uint16_t bulldozer[] = {10, 11, 10, 12};
        sprite(408, 48, bulldozer[animationFrame % 4]);
        // 1010:afd8 animates the drains at cell (5,12)/(7,12).
        sprite(60, 144, animationFrame % 8 < 4 ? 0x2c + animationFrame % 4 : 0x2b);
        sprite(84, 144, animationFrame % 8 < 4 ? 0x42 + animationFrame % 4 : 0x41);
        text(12, 168, 168, 56, L"DRAINS. descend/ascend to next level.", RGB(0, 128, 0));
        constexpr std::uint16_t portal[] = {0x69, 0x6a, 0x6b, 0x6c, 0x6e, 0x74, 0x75, 0x76, 0x77};
        for (int row = 0; row < 3; ++row)
            for (int column = 0; column < 3; ++column) sprite(192 + column * 12, 132 + row * 12, portal[row * 3 + column]);
        sprite(204, 144, animationFrame % 5 == 1 ? 0x6d : 0x6e);
        text(240, 132, 192, 76, L"PORTAL. Brings monsters into battle. Use bombs to destroy.");
        for (const POINT cell : {POINT{28, 16}, POINT{29, 16}, POINT{29, 17}, POINT{30, 17}})
            sprite(cell.x * 12, cell.y * 12, 0x25);
        text(264, 216, 156, 42, L"CRUD. (hard water buildup).", RGB(0, 128, 0));
        for (int index = 0; index < 5; ++index) {
            std::uint16_t treasure = 0x46;
            for (unsigned tick = 0; tick <= animationFrame % 120; ++tick) {
                if (tick % 6 == 0) treasure = 0x46;
                else if ((tick % 6 == 4 || tick % 6 == 5)
                    && presentation::VisualChance(tick, index + 8, 3))
                    treasure = tick % 6 == 4 ? 0x47 : 0x48;
            }
            sprite(96 + index * 12, 300, index % 2 ? 0 : index == 2 ? 0x57 + animationFrame % 4 : treasure);
        }
        text(24, 240, 192, 56, L"TREASURE. Replenish your arsenal with weapons and other items.", RGB(0, 0, 128));
        text(240, 270, 180, 42, L"NOTE: RIGHT-CLICK ON ANY SQUARE TO IDENTIFY.", RGB(96, 96, 96));
        RECT note{228, 264, 432, 312};
        HBRUSH noteBrush = CreateSolidBrush(animationFrame % 8 < 2 ? RGB(0, 0, 255)
            : animationFrame % 8 >= 4 && animationFrame % 8 < 6 ? RGB(255, 0, 0) : RGB(0, 0, 0));
        FrameRect(dc, &note, noteBrush);
        DeleteObject(noteBrush);
    } else if (page == 5) {
        // 1010:b638: eight weapon rows, 36 pixels apart, at x=24/72.
        text(12, 12, 420, 28, L"Your Weapons", RGB(192, 192, 0), true);
        constexpr std::wstring_view names[] = {L"Bubble", L"Bomb", L"Missile", L"Mine", L"Decoy", L"Fireball", L"Burrow Bomb", L"Radio-Controlled Bomb"};
        constexpr std::wstring_view descriptions[] = {L"Stops monsters in their tracks.", L"Destroys most objects.", L"More powerful than bombs. Destroys Doors.", L"Destroys monsters who step on them.", L"Attracts nearby monsters. Very useful.", L"Kills all monsters in its path.", L"Good for blasting through rockpiles.", L"Destroys large area. Detonates on command."};
        constexpr std::size_t images[] = {0x12, 0x13, 0x16, 0x14, 0x154, 0x21, 0x13, 0x1d};
        for (int y = 52; y <= 304; y += 12)
            for (int column = 0; column < 3; ++column)
                sprite(12 + column * 12, y, 0);
        for (int index = 0; index < 8; ++index) {
            // 1010:afd8 cycles fireball and radio-bomb sprites independently
            // of player input. Rendering must not advance the gameplay RNG.
            sprite(24, 52 + index * 36, images[index]
                + ((index == 5 || index == 7) ? animationFrame % 4 : 0));
            text(72, 48 + index * 36, 360, 16, names[index], RGB(255, 0, 0));
            text(72, 60 + index * 36, 360, 28, descriptions[index]);
        }
    } else if (page == 6) {
        text(6, 6, 430, 28, L"Keyboard Commands", RGB(0, 0, 128), true);
        // 1010:9cb2: number-pad keycaps and matching direction-arrow sprites.
        for (int column = 1; column <= 3; ++column) {
            for (int row = 1; row <= 3; ++row) {
                if (column == 2 && row == 2) continue;
                sprite(column * 24 + 300, (row + 2) * 24, 0x1a5);
                const wchar_t digit = static_cast<wchar_t>(L'0' + (3 - row) * 3 + column);
                text(column * 24 + 306, (row + 2) * 24, 16, 20, std::wstring_view(&digit, 1));
                sprite(column * 42 + 270, row * 42 + 18, row * 3 + column + 0x1a3);
            }
        }
        sprite(84, 96, 0x1a5); text(90, 96, 16, 20, L"A");
        sprite(96, 120, 0x1a5); text(102, 120, 16, 20, L"Z");
        sprite(48, 120, 0x1a6); text(54, 120, 48, 20, L"Shift");
        sprite(354, 101, playerIllustrationSprite());
        text(12, 180, 420, 148,
            L"Use these simple keys...\n\n1) Use number pad (arrow keys) to move.\n2) Press [Shift]+arrow key to shoot in any direction.\n3) Press [A] and [Z] to choose weapon from arsenal.\n\nNote: [Num Lock] key must be off.");
    } else if (page == 10) {
        // 1010:a64a: ten ranked rows with separately colored columns.
        text(12, 12, 420, 28, L"High Scores", RGB(255, 0, 0), true);
        if (highScoreReadError_) {
            text(12, 60, 420, 80, L"Unable to read the high-score table.");
        } else {
            text(12, 48, 40, 20, L"#", RGB(80, 80, 80), false, true);
            text(60, 48, 180, 20, L"Name", RGB(80, 80, 80), false, true);
            text(252, 48, 150, 20, L"Score", RGB(80, 80, 80), false, true);
            text(396, 48, 48, 20, L"Skill", RGB(80, 80, 80), false, true);
            for (std::size_t index = 0; index < highScores_.Entries().size(); ++index) {
                const int y = static_cast<int>(index + 3) * 24;
                text(12, y, 40, 22, std::to_wstring(index + 1), RGB(80, 80, 80), false, true);
                const auto& entry = highScores_.Entries()[index];
                if (!entry.occupied) continue;
                text(60, y, 180, 22, std::wstring(entry.name.begin(), entry.name.end()), RGB(0, 0, 255), false, true);
                auto score = std::to_wstring(HighScoreTable::RankedScore(
                    entry.score, entry.heroBonus, entry.sphereBonus));
                for (int at = static_cast<int>(score.size()) - 3; at > 0; at -= 3) score.insert(at, L",");
                text(252, y, 144, 22, score, RGB(255, 255, 0), false, true);
                text(408, y, 36, 22, entry.skill == 1 ? L"EZ" : std::to_wstring(entry.skill), RGB(255, 0, 0), false, true);
            }
        }
    } else if (page == 12) {
        // 1010:b4ae / strings c4d6..c668. The previous fallback incorrectly
        // merged an acknowledgements draft into this separate preview page.
        text(12, 12, 420, 40, L"Drain Storm, Deluxe Version is coming soon.\nIt contains:", RGB(0, 0, 255));
        text(24, 60, 408, 64, L"New, never before seen challenge boards.\nFull-window, resizable weapon selector.\nConfigurable keyboard commands. Great for laptops.", RGB(0, 0, 0));
        for (int y : {63, 78, 93}) sprite(10, y, 0x1d + animationFrame % 4);
        text(12, 132, 420, 28, L"Coming Attractions", RGB(0, 160, 160), true);
        text(12, 204, 420, 18, L"Drain Storm Custom Editor.", RGB(255, 0, 0));
        text(12, 222, 420, 40, L"Create your own challenges, traps and adventures for friends. Or post them on the net for others to try.");
        text(12, 264, 360, 18, L"Drain Storm II.", RGB(255, 0, 0));
        text(12, 282, 336, 50, L"The Squid-squelching continues in this unbelievable sequel.");
        sprite(384, 252, 0x13a);
        sprite(360, 288, playerIllustrationSprite());
    } else {
        // Narrative callbacks a466/a4d8/a582/a966/a9ee share a heading
        // and fixed-width body, rather than a single default-font paragraph.
        const auto split = narrative.find(L"\r\n\r\n");
        const auto title = narrative.substr(0, split);
        const auto body = split == std::wstring_view::npos ? std::wstring_view{}
            : narrative.substr(split + 4);
        text(12, 12, 420, 28, page == 3 ? L"...continued" : title,
            page == 1 ? RGB(255, 0, 0) : RGB(80, 80, 80), true);
        if (page == 7) {
            constexpr std::array<std::wstring_view, 9> labels = {
                L"N", L"L", L"S", L"Q", L"T", L"M", L"P", L"F", L"H"};
            constexpr std::array<std::wstring_view, 9> commands = {
                L"Start a new game", L"Load a previously saved game",
                L"Save the current game", L"Quit the current game",
                L"Toggle sound", L"Read next message if there is one",
                L"Scroll through previous messages", L"Reread your fortunes",
                L"Display Help and Information"};
            for (std::size_t index = 0; index < labels.size(); ++index) {
                const int top = 48 + static_cast<int>(index) * 30;
                sprite(12, top, 0x1a5);
                text(18, top, 24, 20, labels[index], RGB(0, 0, 128));
                std::wstring command(commands[index]);
                if (index == 4)
                    command += soundEnabled_ ? L" (currently ON)" : L" (currently OFF)";
                text(60, top + 6, 372, 24, command, RGB(96, 0, 0));
            }
        } else {
            const int top = page == 2 ? 114 : page == 11 ? 66 : 48;
            // Long pages need a narrower fixed font to retain all text within
            // the recovered canvas on modern font rasterizers.
            HFONT compact = CreateFontW(13, 7, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                FIXED_PITCH | FF_MODERN, L"Courier New");
            SelectObject(dc, compact);
            SetTextColor(dc, RGB(128, 0, 0));
            RECT bodyBounds{page == 11 ? 48 : 12, top, 432, page == 3 ? 238 : 338};
            DrawTextW(dc, body.data(), static_cast<int>(body.size()), &bodyBounds,
                DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);
            SelectObject(dc, normal); DeleteObject(compact);
            if (page == 2) {
                for (int x = 156; x < 240; x += 12) sprite(x, 90, 0);
                sprite(192, 90, playerIllustrationSprite());
            }
            if (page == 3) text(96, 252, 300, 60, L"Good Luck!", RGB(96, 96, 0), true);
            if (page == 11) {
                RECT registration{12, 54, 432, 318};
                DrawEdge(dc, &registration, EDGE_SUNKEN, BF_RECT);
                sprite(20, 146, 0x1d + animationFrame % 4);
                sprite(20, 182, 0x1d + animationFrame % 4);
            }
            if (page == 12) sprite(384, 252, 0x13a);
        }
    }
    RestoreDC(dc, saved);
    DeleteObject(normal); DeleteObject(heading);
    return true;
}

void ApplicationState::DrawArsenal(HDC destination, int width, int scrollDirection) const {
    gameData_.DrawArsenal(destination, boardAtlas_, width, scrollDirection);
}

} // namespace drain_storm
