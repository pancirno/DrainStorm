#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <windows.h>

#include "bitmap_surface.h"
#include "game_data.h"
#include "high_scores.h"

namespace drain_storm {

class ApplicationState {
public:
    static constexpr std::size_t kBoardColumns = 0x28;
    static constexpr std::size_t kBoardRows = 0x24;
    static constexpr std::uint16_t kBoardCellDirty = 0x0001;
    static constexpr std::size_t kKeyBindingCount = 19;
    using KeyBindings = std::array<UINT, kKeyBindingCount>;

    void MarkEntireBoardDirty() noexcept;
    void AdvanceAnimation() noexcept;
    [[nodiscard]] bool IsDemoMode() const noexcept { return demoMode_; }
    bool MovePlayer(int deltaColumn, int deltaRow) noexcept;
    bool MovePlayerToward(int column, int row) noexcept;
    bool UseSelectedItemToward(int column, int row) noexcept;
    bool ActivateTransporter(bool upward) noexcept;
    bool UseSelectedItem(int deltaColumn, int deltaRow) noexcept;
    std::wstring TakeGameplayMessage();
    void PostGameplayNotice(std::wstring message);
    std::wstring TakeLifecycleMessage();
    [[nodiscard]] bool HasPendingLifecycleMessage() const noexcept {
        return !pendingLifecycleMessage_.empty();
    }
    [[nodiscard]] std::wstring CurrentGameplayMessage() const;
    [[nodiscard]] bool HasMoreGameplayMessages() const noexcept;
    [[nodiscard]] int Health() const noexcept;
    [[nodiscard]] std::uint32_t Score() const noexcept;
    [[nodiscard]] std::size_t CurrentLevel() const noexcept;
    [[nodiscard]] std::uint32_t CurrentTurn() const noexcept { return gameData_.turnCounter_; }
    [[nodiscard]] int CurrentDifficulty() const noexcept;
    [[nodiscard]] int PlayerColumn() const noexcept;
    [[nodiscard]] int PlayerRow() const noexcept;
    [[nodiscard]] std::uint16_t CurrentTileAt(int column, int row) const;
    [[nodiscard]] std::wstring IdentifySquare(int column, int row);
    [[nodiscard]] std::wstring NavigateMessageHistory(bool previous);
    [[nodiscard]] std::wstring HighScoresText() const;
    [[nodiscard]] bool HasPendingHighScore() const noexcept;
    bool SubmitPendingHighScore(const std::wstring& name);
    bool SelectPreviousItem() noexcept;
    bool SelectNextItem() noexcept;
    bool SelectVisibleArsenalItem(int clientY) noexcept;
    bool ScrollVisibleArsenal(bool upward) noexcept;
    bool RecallDiscoveredLaw() noexcept;
    bool ToggleSound() noexcept;
    [[nodiscard]] UINT KeyBinding(std::size_t index) const noexcept;
    [[nodiscard]] const KeyBindings& KeyBindingTable() const noexcept { return keyBindings_; }
    bool SetKeyBindings(const KeyBindings& bindings);
    bool ResetKeyBindings();
    [[nodiscard]] bool SoundEnabled() const noexcept { return soundEnabled_; }
    [[nodiscard]] bool IsBoardCellDirty(std::size_t column, std::size_t row) const;
    bool LoadOriginalResources(const std::filesystem::path& executablePath);
    bool NewGame(int difficulty = 1);
    bool QuitCurrentGame(bool submitScore);
    bool SaveGame(const std::filesystem::path& path) const;
    bool LoadGame(const std::filesystem::path& path);
    void DrawBoard(HDC destination, int pressedDemoButton = -1) const;
    bool AdvanceDrainAnimation() noexcept;
    [[nodiscard]] bool IsDrainAnimating() const noexcept { return drainAnimationBoard_.has_value(); }
    [[nodiscard]] RECT DemoPanelBounds() const noexcept;
    [[nodiscard]] int DemoButtonAt(int x, int y) const noexcept;
    std::wstring HelpPageText(int page) const;
    static void DrawControlSurroundings(HDC destination, const RECT& client,
                                       RECT arsenal, RECT board);
    bool DrawHelpPage(HDC destination, int page, const RECT& bounds,
                      std::uint32_t animationFrame = 0, std::wstring_view narrative = {}) const;
    void DrawMessagePanel(HDC destination, int width) const;
    void DrawArsenal(HDC destination, int width = 118, int scrollDirection = 0) const;

private:
    void DrawDemoPanel(HDC destination, int pressedButton) const;
    void RecordGameplayMessage(std::wstring message);
    void PlayPendingGameplaySound() noexcept;
    bool PrepareHubBranches();
    void HandleBoardTransition();
    void HandleEndState();
    [[nodiscard]] bool IsFatalHitAnimating() const noexcept;

    struct BoardHistoryEntry {
        GameData board;
        std::uint16_t returnTile{};
    };

    struct CachedBoardEntry {
        std::vector<std::uint16_t> path;
        GameData board;
    };

    using BoardColumn = std::array<std::uint16_t, kBoardRows>;
    std::array<BoardColumn, kBoardColumns> boardFlags_{};
    BitmapSurface boardAtlas_{};
    GameData gameData_{};
    // Presentation-only source board: destination state is committed atomically.
    std::optional<GameData> drainAnimationBoard_{};
    unsigned drainAnimationFrame_{};
    std::vector<BoardHistoryEntry> boardHistory_{};
    std::unordered_map<std::size_t, GameData> sharedBoards_{};
    std::vector<std::uint16_t> currentBoardPath_{};
    std::vector<CachedBoardEntry> cachedBoards_{};
    std::filesystem::path resourceDirectory_{};
    std::filesystem::path highScorePath_{};
    std::filesystem::path keyBindingPath_{};
    HighScoreTable highScores_{};
    std::wstring pendingLifecycleMessage_{};
    bool highScoreReadError_{};
    std::vector<std::wstring> messageHistory_{};
    std::size_t messageHistoryCursor_{};
    std::size_t messageReviewBoundary_{};
    std::uint32_t pendingHighScore_{};
    std::uint16_t pendingHighScoreSkill_{};
    bool pendingHighScoreAvailable_{};
    bool pendingHighScoreHeroBonus_{};
    bool pendingHighScoreSphereBonus_{};
    bool soundEnabled_{true};
    bool demoMode_{};
    std::size_t demoStep_{};
    int demoTick_{};
    std::uint32_t demoSeed_{1};
    KeyBindings keyBindings_{};

    bool LoadKeyBindings();
    bool SaveKeyBindings() const;
    bool RestartDemo() noexcept;
};

} // namespace drain_storm
