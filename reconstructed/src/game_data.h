#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <iosfwd>
#include <string>
#include <string_view>
#include <vector>

#include <windows.h>

namespace drain_storm {

class BitmapSurface;
class ApplicationState;

struct SpriteRecord {
    std::uint16_t id{};
    std::uint16_t sourceColumn{};
    std::uint16_t sourceRow{};
    std::uint16_t widthInCells{};
    std::uint16_t heightInCells{};
};

struct TileDescriptor {
    std::uint16_t sprite{};
    std::uint16_t flags{};
    std::uint16_t value{};
};

struct BoardOverlay {
    std::uint16_t type{};
    std::uint16_t column{};
    std::uint16_t row{};
    std::uint8_t animationFrame{}; // Per-effect lifetime; zero means not armed.
    std::uint16_t visualSprite{0xffff}; // Cached timer result, never serialized.
};

struct BoardActor {
    std::uint16_t type{};
    std::uint16_t column{};
    std::uint16_t row{};
    std::int16_t deltaColumn{};
    std::int16_t deltaRow{};
    std::uint16_t behavior{};
    std::uint16_t visualSprite{0xffff};
    std::uint16_t visualType{0xffff};
};

struct InventoryItem {
    std::uint16_t type{};
    std::uint16_t quantity{};
};

struct Projectile {
    std::uint16_t type{};
    int column{};
    int row{};
    int deltaColumn{};
    int deltaRow{};
};

enum class GameplaySound : int {
    None = -1,
    SingleThuk = 0,
    TripleThuk = 1,
    BombShot = 2,
    BombShotEight = 3,
    MissileLaunch = 4,
    MissileLaunchEight = 5,
    DeadXoid = 6,
    DeadXoidTwo = 7,
    DeadXoidThree = 8,
    Pop = 9,
    DoublePop = 10,
    TriplePop = 11,
    ExplosionSmall = 12,
    BoardBreak = 12,
    Crackle = 13,
    CasinoBet = 14,
    CasinoWin = 15,
    CasinoLose = 16,
    CasinoDraw = 17,
    Drill = 18,
    DoorOpen = 18,
    SpecialDoorOpen = 19,
    TreasureOne = 20,
    TreasureTwo = 21,
    Construction = 22,
    RadioBeep = 23,
    Explosion = 24,
    ExplosionLarge = 25,
    Yeow = 26,
    WakeUp = 27,
    PlayerHit = 28,
    PlayerHitTwice = 29,
    PlayerDies = 30,
};

class GameData {
public:
    static constexpr int kDeferredProceduralExit = 26;
    static constexpr std::size_t kBoardColumns = 40;
    static constexpr std::size_t kBoardRows = 36;
    static constexpr int kCellSize = 12;

    bool Load(const std::filesystem::path& gameDataDirectory, std::size_t levelNumber = 1,
              std::uint32_t initialRandomState = 1, int difficulty = 1, int boardDepth = 0,
              bool introductorySign = false);
    bool LoadDemoGame(const std::filesystem::path& gameDataDirectory, int difficulty = 1,
                      std::uint32_t initialRandomState = 1);
    bool LoadNewGame(const std::filesystem::path& gameDataDirectory, int difficulty = 1,
                     std::uint32_t initialRandomState = 1);
    bool SaveGame(const std::filesystem::path& path) const;
    bool LoadGame(const std::filesystem::path& path);
    void AdoptPlayerStateFrom(const GameData& source, bool keepConstructedRandomState = false);
    bool PositionPlayerAtTile(std::uint16_t tile) noexcept;
    int TakeTransitionRequest() noexcept;
    void AdvanceAnimation() noexcept;
    bool MovePlayer(int deltaColumn, int deltaRow) noexcept;
    bool MovePlayerToward(int column, int row) noexcept;
    bool UseSelectedItemToward(int column, int row) noexcept;
    bool ActivateTransporter(bool upward) noexcept;
    bool UseSelectedItem(int deltaColumn, int deltaRow) noexcept;
    [[nodiscard]] int Health() const noexcept { return health_; }
    [[nodiscard]] std::uint32_t Score() const noexcept { return score_; }
    [[nodiscard]] std::uint32_t TurnCount() const noexcept { return turnCounter_; }
    [[nodiscard]] bool VictoryTriggered() const noexcept { return victoryTriggered_; }
    [[nodiscard]] bool EnergySphereDestroyed() const noexcept { return energySphereDestroyed_; }
    void SetRootBoard(bool root) noexcept { rootBoard_ = root; }
    void SetTerminalDeathEnabled(bool enabled) noexcept { terminalDeathEnabled_ = enabled; }
    [[nodiscard]] std::uint16_t PlayerSpriteIndex(bool shiftHeld) const noexcept;
    [[nodiscard]] std::size_t LevelNumber() const noexcept { return levelNumber_; }
    [[nodiscard]] int Difficulty() const noexcept { return difficulty_; }
    [[nodiscard]] std::size_t PlayerColumn() const noexcept { return playerColumn_; }
    [[nodiscard]] std::size_t PlayerRow() const noexcept { return playerRow_; }
    [[nodiscard]] int RepellentEnergy() const noexcept { return repellentEnergy_; }
    [[nodiscard]] const std::vector<BoardActor>& Actors() const noexcept { return actors_; }
    [[nodiscard]] const std::vector<BoardOverlay>& Overlays() const noexcept { return overlays_; }
    GameplaySound TakeSoundEvent() noexcept;
    std::wstring TakeMessage();
    std::vector<std::wstring> TakeMessages();
    std::wstring TakeDialogMessage();
    bool SelectPreviousItem() noexcept;
    bool SelectNextItem() noexcept;
    bool SelectVisibleArsenalItem(int clientY) noexcept;
    bool ScrollVisibleArsenal(bool upward) noexcept;
    void DrawArsenal(HDC destination, const BitmapSurface& atlas, int width = 118,
                     int scrollDirection = 0) const;
    [[nodiscard]] std::size_t SelectedItem() const noexcept { return selectedItem_; }
    [[nodiscard]] const std::vector<InventoryItem>& Inventory() const noexcept { return inventory_; }
    [[nodiscard]] std::wstring_view ItemName(std::size_t type) const noexcept {
        return type < itemNames_.size() ? std::wstring_view(itemNames_[type]) : std::wstring_view{};
    }
    [[nodiscard]] const std::vector<Projectile>& Projectiles() const noexcept { return projectiles_; }
    [[nodiscard]] std::size_t DiscoveredLawCount() const noexcept;
    bool RecallDiscoveredLaw() noexcept;
    [[nodiscard]] std::uint16_t TileAt(std::size_t column, std::size_t row) const;
    [[nodiscard]] std::wstring IdentifySquare(std::size_t column, std::size_t row) const;
    void DrawLevel(HDC destination, const BitmapSurface& atlas) const;

private:
    friend class ApplicationState;

    bool WriteGame(std::ostream& stream) const;
    bool ReadGame(std::istream& stream);
    void QueueSound(GameplaySound sound) noexcept;
    using BoardColumn = std::array<std::uint16_t, kBoardRows>;
    std::array<BoardColumn, kBoardColumns> level_{};
    std::vector<SpriteRecord> sprites_{};
    std::vector<TileDescriptor> tiles_{};
    std::vector<std::int16_t> overlaySprites_{};
    std::vector<BoardOverlay> overlays_{};
    std::vector<BoardActor> actors_{};
    std::vector<std::uint16_t> itemSprites_{};
    std::vector<std::uint16_t> itemDisplayKinds_{};
    std::vector<std::wstring> itemNames_{};
    std::vector<InventoryItem> inventory_{};
    std::vector<Projectile> projectiles_{};
    std::size_t selectedItem_{};
    std::size_t firstVisibleItem_{};
    std::uint32_t animationFrame_{};
    std::uint8_t transientOverlayFrame_{};
    mutable std::uint8_t playerAttackFrame_{};
    std::uint32_t randomState_{1};
    std::uint32_t turnCounter_{};
    std::uint32_t score_{};
    std::size_t levelNumber_{};
    // Original board +0x3c is construction depth, not board-class selector.
    std::uint16_t boardDepth_{1};
    bool depthFromSave_{};
    std::size_t playerColumn_{};
    std::size_t playerRow_{};
    bool playerFacesRight_{true};
    bool rootBoard_{true};
    // FUN_1008_2920 only enters the terminal death path for a non-demo game
    // regardless of active-board ancestry. This graph-owned bit is not
    // serialized inside the board payload.
    bool terminalDeathEnabled_{true};
    // FUN_1008_e636 writes application offset 0x100 after both mobile Power
    // Plant targets have been destroyed. ApplicationState consumes it at the
    // end of the same accepted action, so it is intentionally transient.
    bool victoryTriggered_{};
    int health_{3};
    // Set on a fatal playable hit for the remainder of the accepted turn.
    // A later life pickup raises health but does not undo the death event.
    bool fatalHitLatched_{};
    int difficulty_{1};
    int repellentEnergy_{};
    GameplaySound pendingSound_{GameplaySound::None};
    std::vector<std::wstring> pendingMessages_{};
    std::wstring pendingDialogMessage_{};
    std::array<bool, 47> discoveredLaws_{};
    std::vector<std::uint8_t> discoveredLawOrder_{};
    std::uint32_t lawRecallCursor_{};
    int pendingTransition_{};
    bool levelFourNortheastTriggered_{};
    bool levelFourSouthwestTriggered_{};
    bool levelFourFloodTriggered_{};
    std::uint32_t levelFourCountdown_{24};
    int levelTwoSequenceState_{-1};
    int levelTwoSequenceMode_{1};
    std::uint16_t casinoTopChestCount_{7};
    std::array<bool, 4> levelSevenStructureActive_{};
    int levelSevenSpawnDivisor_{25};
    bool levelSevenEndPhase_{};
    std::uint16_t levelFiveProbeColumn_{};
    std::uint16_t levelFiveProbeRow_{};
    bool levelFiveFirstProbeBroken_{};
    bool levelFiveSecondProbeBroken_{};
    bool levelFiveProbeTransformed_{};
    bool levelFiveFirstSwitchActive_{};
    bool levelFiveSecondSwitchActive_{};
    bool levelSixStructuresSpawned_{};
    bool levelSixUpperTransporterTriggered_{};
    bool levelSixLowerTransporterTriggered_{};
    bool levelSixCenterOpened_{};
    bool levelEightAlarmTriggered_{};
    std::uint16_t levelThreeSpawnChance_{25};
    std::uint16_t levelTwelveSpawnChance_{};
    std::uint16_t levelTwelveMaximumGroup_{1};
    std::uint8_t levelElevenRuntimeFlags_{};
    bool levelTenEmeraldClaimed_{};
    bool levelTenBarrierOpened_{};
    std::array<bool, 3> levelSixteenChambersActive_{{true, true, true}};
    bool levelSixteenCoreActivated_{};
    int genericSpawnChance_{10};
    int genericBreakDivisor_{20};
    std::uint8_t genericRuntimeFlags_{};
    std::array<std::uint16_t, 2> levelOneTargetHits_{};
    std::array<std::uint16_t, 2> levelOneTargetColumns_{36, 36};
    std::array<std::uint16_t, 2> levelOneTargetRows_{1, 32};
    std::array<std::uint8_t, 2> levelOneTargetDefeated_{};
    void ReconcileLevelOneTargets() noexcept;
    std::size_t LevelOneTargetAt(int column, int row) const noexcept;
    std::pair<int, int> StructureSpawnParameters() const noexcept;
    void RefreshFinalArenaSpawnParameters() noexcept;
    int ActorBreakDivisor() const noexcept;
    std::uint32_t levelOneFirstTargetActivationTurn_{};
    bool levelOneSecurityGateOpened_{};
    bool levelOneCornerPortalsActivated_{};
    std::uint16_t levelOneSpawnChance_{};
    std::uint16_t levelOneMaximumGroup_{1};
    bool energySphereDestroyed_{};
    struct TerrainAnimation {
        std::uint16_t tile{0xffff};
        std::uint16_t sprite{};
    };
    std::vector<std::array<TerrainAnimation, kBoardRows>> terrainAnimation_{kBoardColumns};
    void AdvanceRandomAnimation() noexcept;
    bool levelOneArenaEntered_{};
    std::uint16_t casinoStatus_{};
    std::uint16_t casinoBetCount_{};
    std::uint16_t casinoBetItemType_{0xffff};
    std::array<std::uint16_t, 10> casinoDeck_{};
    std::array<std::uint16_t, 5> casinoRanks_{};
    std::array<std::uint16_t, 5> casinoSuits_{};
    std::array<std::uint8_t, 5> casinoHeld_{};
    std::uint16_t casinoDeckIndex_{};
    std::uint16_t casinoWildRank_{};
    std::uint16_t casinoBonusStage_{};
    std::uint16_t casinoResultTicks_{};
    // Presentation only: the original marquee has a global scroll cursor.
    std::uint16_t casinoAnnouncement_{};
    std::size_t casinoScrollOffset_{};
    void SetCasinoAnnouncement(std::uint16_t announcement) noexcept;
    [[nodiscard]] std::wstring_view CasinoAnnouncementText() const noexcept;
    [[nodiscard]] std::wstring CasinoAnnouncementWindow() const;

    void ResolveActorCollision() noexcept;
    void SpawnStructureActors() noexcept;
    void AdvanceActors() noexcept;
    bool ResolveActorTurnCollision(std::size_t actorIndex) noexcept;
    void AdvanceProjectiles() noexcept;
    void AddProjectile(std::uint16_t type, int column, int row,
                       int deltaColumn, int deltaRow) noexcept;
    bool DamageBoardSquare(int column, int row, int strength,
                           std::vector<Projectile>* spawnedProjectiles = nullptr) noexcept;
    bool AffectRadioBlastSquare(int column, int row) noexcept;
    void ApplyRadioExplosion(int centerColumn, int centerRow) noexcept;
    int RollObjectTreasure(int impactClass) noexcept;
    [[nodiscard]] std::wstring_view SignpostText() const noexcept;
    void AwardActorScore(std::uint16_t type) noexcept;
    void AwardTileScore(std::uint16_t tile) noexcept;
    void ConsumeSelectedItem() noexcept;
    void AddInventoryItem(std::uint16_t type, std::uint16_t quantity) noexcept;
    void AddRandomizedInventoryItem(std::uint16_t type) noexcept;
    void ResetCasinoRound() noexcept;
    void MaintainCasinoStructures() noexcept;
    void AdvanceCasinoRetaliation(int requestedMode) noexcept;
    void ResolveTriggeredMines() noexcept;
    void ResolveOverlayEffect(std::uint16_t type) noexcept;
    void ResolveOverlaysAtPlayer() noexcept;
    bool PlaceLevelThreeStructure(bool showConstructionEffects) noexcept;
    bool PlaceRandomGuardedStructure(bool showConstructionEffects) noexcept;
    bool PlaceMoonRoomStructures() noexcept;
    void ActivateLevelSixteenRegion(std::size_t left, std::size_t top,
                                    std::size_t right, std::size_t bottom) noexcept;
    void AdvanceBoardRuntime(bool movementAction, int deltaColumn, int deltaRow,
                             bool preActorPhase) noexcept;
    void AdvanceBoardRuntimeCore(bool movementAction, int deltaColumn, int deltaRow,
                                 bool preActorPhase) noexcept;
    void DetectBoardTransition() noexcept;
    void AdvanceRepellent() noexcept;
    void ArmTransientOverlayAnimation() noexcept;
    bool DetonateCharges() noexcept;
    int NextRandom(int upperBound) noexcept;
    std::uint16_t SelectProceduralStructureType(int depth) noexcept;
};

} // namespace drain_storm
