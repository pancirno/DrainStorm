#include <cstdlib>
#include <cstring>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>

#include "application_state.h"
#include "game_data.h"
#include "high_scores.h"

namespace {

void Require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAILED: " << message << '\n';
        std::exit(EXIT_FAILURE);
    }
}

} // namespace

int wmain(int argumentCount, wchar_t** arguments) {
    Require(argumentCount == 2, "game-data directory argument");
    // Keep application sidecars and mutable save fixtures away from player data.
    const std::filesystem::path sourceGameDataDirectory = arguments[1];
    const std::filesystem::path reconstructedDirectory = std::filesystem::temp_directory_path()
        / (L"DrainStormRegression-" + std::to_wstring(GetCurrentProcessId())
            + L"-" + std::to_wstring(GetTickCount64()));
    Require(std::filesystem::create_directory(reconstructedDirectory), "create isolated test root");
    struct TestDirectory {
        std::filesystem::path path;
        ~TestDirectory() {
            std::error_code error;
            std::filesystem::remove_all(path, error);
        }
    } testDirectory{reconstructedDirectory};
    std::filesystem::create_directories(reconstructedDirectory / L"assets");
    std::filesystem::create_directory(reconstructedDirectory / L"bin");
    std::filesystem::copy(sourceGameDataDirectory.parent_path(),
        reconstructedDirectory / L"assets" / L"original", std::filesystem::copy_options::recursive);
    const std::filesystem::path gameDataDirectory =
        reconstructedDirectory / L"assets" / L"original" / L"game-data";
    const std::filesystem::path executablePath =
        reconstructedDirectory / L"bin" / L"DrainStorm.exe";
    {
        const auto scorePath = executablePath.parent_path() / L"DRAIN64.SCO";
        const auto keysPath = executablePath.parent_path() / L"DRAIN64.KEY";
        for (const auto& path : {scorePath, keysPath}) {
            std::ofstream broken(path, std::ios::binary);
            broken << "invalid-sidecar";
            Require(broken.good(), "write isolated malformed sidecar");
        }
        drain_storm::ApplicationState corruptStartup;
        Require(corruptStartup.LoadOriginalResources(executablePath)
                    && corruptStartup.HasPendingLifecycleMessage()
                    && corruptStartup.HighScoresText() == L"Unable to read the high-score table.",
                "corrupt optional sidecars report errors without preventing startup");
        const auto notice = corruptStartup.TakeLifecycleMessage();
        Require(notice.find(L"high-score") != std::wstring::npos
                    && notice.find(L"keyboard") != std::wstring::npos,
                "startup read notice identifies both failed sidecars");
        for (const auto& path : {scorePath, keysPath}) {
            std::ifstream unchanged(path, std::ios::binary);
            const std::string bytes{std::istreambuf_iterator<char>(unchanged), {}};
            Require(bytes == "invalid-sidecar", "startup never overwrites rejected player data");
            unchanged.close();
            std::filesystem::remove(path);
        }
    }
    const auto equipFirstInventoryItem = [](const std::filesystem::path& path,
                                            std::uint16_t itemType,
                                            std::uint16_t quantity = 1) {
        std::fstream save(path, std::ios::binary | std::ios::in | std::ios::out);
        save.seekg(130 + static_cast<std::streamoff>(
            drain_storm::GameData::kBoardColumns * drain_storm::GameData::kBoardRows * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        std::uint32_t inventoryCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        save.seekg(static_cast<std::streamoff>(actorCount * 12), std::ios::cur);
        save.read(reinterpret_cast<char*>(&inventoryCount), sizeof(inventoryCount));
        Require(inventoryCount != 0, "locate saved inventory fixture");
        save.seekp(save.tellg());
        save.write(reinterpret_cast<const char*>(&itemType), sizeof(itemType));
        save.write(reinterpret_cast<const char*>(&quantity), sizeof(quantity));
        Require(save.good(), "equip saved inventory fixture");
    };

    drain_storm::GameData levelOne;
    Require(levelOne.Load(gameDataDirectory, 1), "load level one");
    Require(levelOne.PlayerColumn() == 14 && levelOne.PlayerRow() == 32,
            "selector-1038 constructor start position");
    Require(levelOne.Actors().size() >= 22 && levelOne.Actors().size() <= 30,
            "selector-1038 creates two randomized captain groups");
    Require(levelOne.Actors()[0].type == 3 && levelOne.Actors()[0].column == 14 &&
                levelOne.Actors()[0].row == 10 && levelOne.Actors()[0].deltaColumn == 1 &&
                levelOne.Actors()[0].deltaRow == 0 && levelOne.Actors()[0].behavior == 3,
            "selector-1038 clockwise actor state");
    Require(levelOne.TileAt(12, 12) == 0x51 && levelOne.TileAt(13, 13) == 0x54,
            "selector-1038 center tile mutations");
    Require(levelOne.TileAt(7, 7) == 0x45 && levelOne.TileAt(9, 9) == 0x50,
            "selector-1038 bf2a block");
    Require(levelOne.TileAt(36, 1) == 0x90 && levelOne.TileAt(38, 3) == 0x98,
            "selector-1038 moving block seed");
    Require(levelOne.TileAt(14, 32) == 0x23, "selector-1038 start tile overwrite");
    Require(levelOne.IdentifySquare(14, 32).find(L"Effer-vessel") != std::wstring::npos
                && levelOne.IdentifySquare(7, 7) == L"Pyramid"
                && levelOne.IdentifySquare(0, 0) == L"Wall"
                && levelOne.IdentifySquare(36, 1)
                    == L"Gormaug\nNecromancer\n\n\nGorloth's vile roommate."
                && levelOne.IdentifySquare(38, 34)
                    == L"Gorloth\nDemon Lord",
            "right-click identification follows recovered square descriptions");
    Require(levelOne.TileAt(32, 14) == 0x10 && levelOne.TileAt(34, 16) == 0x17,
            "selector-1038 asymmetric object coordinates");
    Require(levelOne.PlayerSpriteIndex(false) == 0x152
                && levelOne.PlayerSpriteIndex(true) == 0x166
                && levelOne.PlayerSpriteIndex(true) == 0x167
                && levelOne.PlayerSpriteIndex(true) == 0x168
                && levelOne.PlayerSpriteIndex(true) == 0x169
                && levelOne.PlayerSpriteIndex(true) == 0x169
                && levelOne.PlayerSpriteIndex(false) == 0x152
                && levelOne.PlayerSpriteIndex(true) == 0x166,
            "FUN_1008_2d50 walking and capped Shift-attack sprite cycle");
    static_cast<void>(levelOne.PlayerSpriteIndex(false));

    drain_storm::GameData attackFacing;
    Require(attackFacing.Load(gameDataDirectory, 1)
                && attackFacing.UseSelectedItem(0, -1)
                && attackFacing.PlayerSpriteIndex(false) == 0x152
                && attackFacing.UseSelectedItem(-1, 0)
                && attackFacing.PlayerSpriteIndex(false) == 0x15a,
            "FUN_1008_1b22 turns the vessel only for horizontal arsenal actions");
    const std::filesystem::path facingPath = gameDataDirectory / L"horizontal-facing.VKW";
    Require(attackFacing.SaveGame(facingPath), "save horizontal-facing fixture");
    drain_storm::GameData restoredFacing;
    Require(restoredFacing.Load(gameDataDirectory, 1)
                && restoredFacing.LoadGame(facingPath)
                && restoredFacing.PlayerSpriteIndex(false) == 0x15a,
            "DAH64 v20 preserves horizontal vessel facing");
    std::filesystem::remove(facingPath);
    Require(attackFacing.Load(gameDataDirectory, 1)
                && attackFacing.PlayerSpriteIndex(false) == 0x152,
            "new board initialization restores right-facing vessel state");

    drain_storm::GameData mouseActions;
    Require(mouseActions.Load(gameDataDirectory, 1), "load board mouse-action fixture");
    const std::uint32_t mouseWaitTurn = mouseActions.TurnCount();
    Require(mouseActions.MovePlayerToward(static_cast<int>(mouseActions.PlayerColumn()),
                                          static_cast<int>(mouseActions.PlayerRow()))
                && mouseActions.TurnCount() == mouseWaitTurn + 1,
            "FUN_1008_6048 clicking the vessel dispatches a wait turn");
    const std::uint32_t beforeFarShiftClick = mouseActions.TurnCount();
    Require(!mouseActions.UseSelectedItemToward(
                    static_cast<int>(mouseActions.PlayerColumn()) + 2,
                    static_cast<int>(mouseActions.PlayerRow()))
                && mouseActions.TurnCount() == beforeFarShiftClick,
            "FUN_1008_6048 rejects nonadjacent Shift-clicks");
    Require(mouseActions.UseSelectedItemToward(
                    static_cast<int>(mouseActions.PlayerColumn()) + 1,
                    static_cast<int>(mouseActions.PlayerRow()))
                && mouseActions.TurnCount() == beforeFarShiftClick + 1,
            "FUN_1008_6048 dispatches an adjacent Shift-click");
    mouseActions.SetRootBoard(false);
    const std::uint32_t childMouseTurn = mouseActions.TurnCount();
    Require(mouseActions.MovePlayerToward(static_cast<int>(mouseActions.PlayerColumn()),
                                           static_cast<int>(mouseActions.PlayerRow()))
                && mouseActions.UseSelectedItemToward(
                    static_cast<int>(mouseActions.PlayerColumn()) + 1,
                    static_cast<int>(mouseActions.PlayerRow()))
                && !mouseActions.IdentifySquare(
                    mouseActions.PlayerColumn(), mouseActions.PlayerRow()).empty()
                && mouseActions.TurnCount() == childMouseTurn + 2,
            "FUN_1008_6048/FUN_1008_60f4 allow mouse actions on playable child boards");

    drain_storm::GameData powerPlantFirstTurn;
    Require(powerPlantFirstTurn.Load(gameDataDirectory, 1)
                && powerPlantFirstTurn.MovePlayer(0, 0)
                && powerPlantFirstTurn.TileAt(3, 3) == 0x1f
                && powerPlantFirstTurn.TileAt(3, 23) == 0x1f
                && powerPlantFirstTurn.TileAt(23, 3) == 0x1f
                && powerPlantFirstTurn.TileAt(23, 23) == 0x1f
                && powerPlantFirstTurn.Actors()[0].column == 15
                && powerPlantFirstTurn.Actors()[0].row == 10
                && powerPlantFirstTurn.Actors()[0].deltaColumn == 0
                && powerPlantFirstTurn.Actors()[0].deltaRow == 1
                && powerPlantFirstTurn.Actors()[16].behavior == 2,
            "intact Power Plant gates leave corner portals dormant");
    drain_storm::GameData powerPlantRingMotion;
    Require(powerPlantRingMotion.Load(gameDataDirectory, 1)
                && powerPlantRingMotion.MovePlayer(0, 0)
                && powerPlantRingMotion.MovePlayer(0, 0)
                && powerPlantRingMotion.Actors()[0].column == 15
                && powerPlantRingMotion.Actors()[0].row == 11
                && powerPlantRingMotion.Actors()[0].behavior == 3
                && powerPlantRingMotion.Actors()[0].deltaColumn == 0
                && powerPlantRingMotion.Actors()[0].deltaRow == 1,
            "FUN_1008_cdbe turns the Power Plant ring actors clockwise at corners");
    const std::filesystem::path powerPlantArenaPath =
        gameDataDirectory / L"selector-1038-arena-entry.VKW";
    Require(powerPlantFirstTurn.SaveGame(powerPlantArenaPath),
            "save selector-1038 arena-entry fixture");
    {
        std::fstream save(powerPlantArenaPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 1;
        const std::uint16_t playerRow = 19;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        Require(save.good(), "position selector-1038 arena-entry fixture");
    }
    drain_storm::GameData powerPlantArena;
    Require(powerPlantArena.Load(gameDataDirectory, 1)
                && powerPlantArena.LoadGame(powerPlantArenaPath)
                && powerPlantArena.MovePlayer(0, 0)
                && powerPlantArena.TileAt(10, 8) == 0x22
                && powerPlantArena.TileAt(15, 17) == 0x22
                && powerPlantArena.TileAt(8, 10) == 0x22
                && powerPlantArena.TileAt(17, 15) == 0x22
                && std::all_of(powerPlantArena.Actors().begin(),
                    powerPlantArena.Actors().begin() + 16,
                    [](const drain_storm::BoardActor& actor) {
                        return actor.behavior == 0;
                    }),
            "FUN_1008_e636 closes and freezes the central Power Plant arena");
    Require(powerPlantArena.SaveGame(powerPlantArenaPath),
            "save selector-1038 containment-repair fixture");
    {
        std::fstream save(powerPlantArenaPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint32_t repairTurn = 19;
        const std::uint16_t empty = 0;
        constexpr std::streamoff boardOffset = 130;
        save.seekp(21);
        save.write(reinterpret_cast<const char*>(&repairTurn), sizeof(repairTurn));
        save.seekp(boardOffset + static_cast<std::streamoff>((10 * 36 + 8) * 2));
        save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
        Require(save.good(), "break selector-1038 containment-repair fixture");
    }
    drain_storm::GameData powerPlantRepair;
    Require(powerPlantRepair.Load(gameDataDirectory, 1)
                && powerPlantRepair.LoadGame(powerPlantArenaPath)
                && powerPlantRepair.MovePlayer(0, 0)
                && powerPlantRepair.TileAt(10, 8) == 0x22,
            "FUN_1008_e636 repairs the containment ring on turn twenty");
    std::filesystem::remove(powerPlantArenaPath);

    const std::filesystem::path smartCardPath =
        gameDataDirectory / L"selector-1038-smart-card.VKW";
    Require(levelOne.SaveGame(smartCardPath), "save selector-1038 Smart Card fixture");
    {
        std::fstream save(smartCardPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 19;
        const std::uint16_t playerRow = 28;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        Require(save.good(), "position selector-1038 Smart Card fixture");
    }
    equipFirstInventoryItem(smartCardPath, 0x0e);
    drain_storm::GameData smartCardBoard;
    Require(smartCardBoard.Load(gameDataDirectory, 1)
                && smartCardBoard.LoadGame(smartCardPath)
                && smartCardBoard.UseSelectedItem(1, 0),
            "insert Smart Card into selector-1038 reader");
    Require(smartCardBoard.TileAt(20, 28) == 0xac
                && smartCardBoard.TileAt(20, 27) == 0xae
                && smartCardBoard.TileAt(23, 29) == 0
                && smartCardBoard.TileAt(23, 30) == 0
                && smartCardBoard.TileAt(23, 31) == 0
                && smartCardBoard.TakeMessage()
                    == L"The card engages... The security door opens."
                && smartCardBoard.TakeSoundEvent()
                    == drain_storm::GameplaySound::SpecialDoorOpen,
            "Smart Card opens the barrier without triggering unrelated portals");
    std::filesystem::remove(smartCardPath);

    drain_storm::GameData rejectedSmartCard;
    Require(rejectedSmartCard.Load(gameDataDirectory, 1), "load rejected Smart Card fixture");
    for (int selection = 0; selection < 10; ++selection) {
        Require(rejectedSmartCard.SelectNextItem(), "select rejected Smart Card");
    }
    const std::uint32_t rejectedSmartCardTurn = rejectedSmartCard.TurnCount();
    Require(rejectedSmartCard.Inventory()[rejectedSmartCard.SelectedItem()].type == 0x0e
                && rejectedSmartCard.UseSelectedItem(-1, 0)
                && rejectedSmartCard.TurnCount() == rejectedSmartCardTurn + 1
                && rejectedSmartCard.TakeMessage() == L"Nothing happens."
                && rejectedSmartCard.Inventory()[rejectedSmartCard.SelectedItem()].quantity == 1,
            "failed board-specific item use reports common feedback without consumption");

    for (const std::uint16_t inertKeyType : {std::uint16_t{16}, std::uint16_t{18}}) {
        const std::filesystem::path inertKeyPath = gameDataDirectory
            / (inertKeyType == 16 ? L"star-key-use.VKW" : L"fire-key-use.VKW");
        Require(levelOne.SaveGame(inertKeyPath), "save inert key fixture");
        equipFirstInventoryItem(inertKeyPath, inertKeyType, 1);
        drain_storm::GameData inertKey;
        Require(inertKey.Load(gameDataDirectory, 1) && inertKey.LoadGame(inertKeyPath),
                "load inert key fixture");
        const std::uint32_t inertKeyTurn = inertKey.TurnCount();
        Require(inertKey.UseSelectedItem(-1, 0)
                    && inertKey.TurnCount() == inertKeyTurn + 1
                    && inertKey.TakeMessage() == L"Nothing happens."
                    && inertKey.Inventory()[0].type == inertKeyType
                    && inertKey.Inventory()[0].quantity == 1,
                "Star and Fire Keys preserve common rejected-turn behavior");
        std::filesystem::remove(inertKeyPath);
    }

    const std::filesystem::path energySpherePath =
        gameDataDirectory / L"selector-1038-energy-sphere.VKW";
    Require(levelOne.SaveGame(energySpherePath), "save selector-1038 Energy Sphere fixture");
    {
        std::fstream save(energySpherePath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 11;
        const std::uint16_t playerRow = 12;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        Require(save.good(), "position selector-1038 Energy Sphere fixture");
    }
    equipFirstInventoryItem(energySpherePath, 4);
    drain_storm::GameData energySphere;
    Require(energySphere.Load(gameDataDirectory, 1)
                && energySphere.LoadGame(energySpherePath)
                && energySphere.UseSelectedItem(1, 0),
            "bomb selector-1038 Energy Sphere");
    Require(energySphere.TileAt(12, 12) == 0 && energySphere.TileAt(13, 12) == 0
                && energySphere.TileAt(12, 13) == 0 && energySphere.TileAt(13, 13) == 0
                && energySphere.TileAt(14, 32) == 0
                && energySphere.Score() == 24000
                && energySphere.Actors().size() >= 6
                && energySphere.Actors().size() <= 14
                && energySphere.TakeMessage() == L"You destroyed the Energy Sphere!",
            "Energy Sphere destruction applies four blasts and its 20,000-point award");
    equipFirstInventoryItem(energySpherePath, 11);
    drain_storm::GameData fireballSphere;
    Require(fireballSphere.Load(gameDataDirectory, 1)
                && fireballSphere.LoadGame(energySpherePath)
                && fireballSphere.UseSelectedItem(1, 0)
                && fireballSphere.TileAt(12, 12) == 0x51
                && fireballSphere.Score() == 0,
            "FUN_1008_ec6e rejects Fireball damage against the Energy Sphere");
    std::filesystem::remove(energySpherePath);

    const std::filesystem::path movingTargetPath =
        gameDataDirectory / L"selector-1038-moving-target.VKW";
    Require(levelOne.SaveGame(movingTargetPath), "save selector-1038 moving-target fixture");
    {
        std::fstream save(movingTargetPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 35;
        const std::uint16_t playerRow = 1;
        const std::uint16_t activeTargetTile = 0x87;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        constexpr std::streamoff boardOffset = 130;
        save.seekp(boardOffset + static_cast<std::streamoff>((36 * 36 + 1) * 2));
        save.write(reinterpret_cast<const char*>(&activeTargetTile), sizeof(activeTargetTile));
        Require(save.good(), "activate selector-1038 moving-target fixture");
    }
    equipFirstInventoryItem(movingTargetPath, 5, 10);
    drain_storm::GameData movingTarget;
    Require(movingTarget.Load(gameDataDirectory, 1)
                && movingTarget.LoadGame(movingTargetPath),
            "load selector-1038 moving-target fixture");
    const std::uint32_t targetScore = movingTarget.Score();
    Require(movingTarget.UseSelectedItem(1, 0), "bomb selector-1038 moving target");
    Require(movingTarget.TileAt(36, 1) == 0x87
                && movingTarget.Score() == targetScore + 2500
                && movingTarget.TakeSoundEvent() == drain_storm::GameplaySound::Yeow,
            "selector-1038 moving target retains tile, scores, and queues hit sample 0x1a");
    Require(movingTarget.SaveGame(movingTargetPath),
            "persist selector-1038 moving-target hit counter");
    {
        std::fstream save(movingTargetPath, std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        save.seekg(boardOffset + static_cast<std::streamoff>(40 * 36 * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        const std::streamoff actorsOffset = save.tellg();
        const std::array<std::uint16_t, 6> stationaryActor = {0, 0, 0, 0, 0, 1};
        for (std::uint32_t actor = 0; actor < actorCount; ++actor) {
            save.seekp(actorsOffset + static_cast<std::streamoff>(actor * 12));
            save.write(reinterpret_cast<const char*>(stationaryActor.data()),
                       sizeof(stationaryActor));
        }
        const std::uint16_t ninthHit = 9;
        save.seekp(-119 - static_cast<std::streamoff>(overlayCount), std::ios::end);
        save.write(reinterpret_cast<const char*>(&ninthHit), sizeof(ninthHit));
        Require(save.good(), "prepare selector-1038 score-cap fixture");
    }
    drain_storm::GameData restoredMovingTarget;
    Require(restoredMovingTarget.Load(gameDataDirectory, 1)
                && restoredMovingTarget.LoadGame(movingTargetPath)
                && restoredMovingTarget.UseSelectedItem(1, 0)
                && restoredMovingTarget.TileAt(36, 1) == 0x99
                && restoredMovingTarget.Score() == targetScore + 52500
                && !restoredMovingTarget.VictoryTriggered(),
            "selector-1038 tenth hit destroys one target and awards its kill bonus");
    Require(restoredMovingTarget.SaveGame(movingTargetPath),
            "save selector-1038 one-target-destroyed fixture");
    {
        std::fstream save(movingTargetPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t tenthHit = 10;
        save.seekp(-117 - static_cast<std::streamoff>(restoredMovingTarget.Overlays().size()), std::ios::end);
        save.write(reinterpret_cast<const char*>(&tenthHit), sizeof(tenthHit));
        Require(save.good(), "prepare selector-1038 final-target fixture");
    }
    equipFirstInventoryItem(movingTargetPath, 16);
    drain_storm::GameData completedPowerPlant;
    Require(completedPowerPlant.Load(gameDataDirectory, 1)
                && completedPowerPlant.LoadGame(movingTargetPath),
            "load selector-1038 final-target fixture");
    const std::uint32_t completedPowerPlantScore = completedPowerPlant.Score();
    Require(completedPowerPlant.UseSelectedItem(1, 0)
                && completedPowerPlant.VictoryTriggered()
                && completedPowerPlant.Score() == completedPowerPlantScore + 50000,
            "FUN_1008_e636 destroys the final target and raises the victory word");
    std::filesystem::remove(movingTargetPath);

    const std::filesystem::path movingTargetRuntimePath =
        gameDataDirectory / L"selector-1038-moving-target-runtime.VKW";
    Require(levelOne.SaveGame(movingTargetRuntimePath),
            "save selector-1038 moving-target runtime fixture");
    {
        std::fstream save(movingTargetRuntimePath, std::ios::binary | std::ios::in | std::ios::out);
        // This fixture tests target activation, not survival against newly spawned actors.
        const int durableHealth = 1000;
        save.seekp(34);
        save.write(reinterpret_cast<const char*>(&durableHealth), sizeof(durableHealth));
        constexpr std::streamoff boardOffset = 130;
        save.seekg(boardOffset + static_cast<std::streamoff>(40 * 36 * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        const std::streamoff actorsOffset = save.tellg();
        const std::array<std::uint16_t, 6> stationaryActor = {0, 0, 0, 0, 0, 1};
        for (std::uint32_t actor = 0; actor < actorCount; ++actor) {
            save.seekp(actorsOffset + static_cast<std::streamoff>(actor * 12));
            save.write(reinterpret_cast<const char*>(stationaryActor.data()),
                       sizeof(stationaryActor));
        }
        Require(save.good(), "write selector-1038 moving-target runtime fixture");
    }
    drain_storm::GameData movingTargetRuntime;
    Require(movingTargetRuntime.Load(gameDataDirectory, 1)
                && movingTargetRuntime.LoadGame(movingTargetRuntimePath),
            "load selector-1038 moving-target runtime fixture");
    for (int turn = 0; turn < 20; ++turn) {
        Require(movingTargetRuntime.MovePlayer(0, 0),
                "advance dormant selector-1038 target runtime");
    }
    Require(movingTargetRuntime.TileAt(36, 1) == 0x90,
            "FUN_1008_e636 keeps targets dormant before Energy Sphere destruction");
    Require(movingTargetRuntime.SaveGame(movingTargetRuntimePath),
            "save dormant selector-1038 target runtime");
    {
        std::fstream save(movingTargetRuntimePath,
                          std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        const std::uint16_t empty = 0;
        const std::uint8_t sphereDestroyed = 1;
        const int durableHealth = 1000;
        // Version 32: global Sphere latch precedes the final fourteen bytes.
        save.seekp(-15, std::ios::end);
        save.write(reinterpret_cast<const char*>(&sphereDestroyed), sizeof(sphereDestroyed));
        save.seekp(34);
        save.write(reinterpret_cast<const char*>(&durableHealth), sizeof(durableHealth));
        for (std::size_t column = 12; column <= 13; ++column) {
            for (std::size_t row = 12; row <= 13; ++row) {
                save.seekp(boardOffset
                    + static_cast<std::streamoff>((column * 36 + row) * 2));
                save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
            }
        }
        Require(save.good(), "destroy Energy Sphere in target wake-up fixture");
    }
    Require(movingTargetRuntime.Load(gameDataDirectory, 1)
                && movingTargetRuntime.LoadGame(movingTargetRuntimePath),
            "reload selector-1038 target wake-up fixture");
    for (int turn = 0; turn < 100 && movingTargetRuntime.TileAt(36, 1) == 0x90; ++turn) {
        Require(movingTargetRuntime.MovePlayer(0, 0),
                "advance selector-1038 moving-target runtime");
    }
    bool foundMovedTargetTile = false;
    for (std::size_t column = 0; column < drain_storm::GameData::kBoardColumns; ++column) {
        for (std::size_t row = 0; row < drain_storm::GameData::kBoardRows; ++row) {
            foundMovedTargetTile = foundMovedTargetTile
                || movingTargetRuntime.TileAt(column, row) == 0x87;
        }
    }
    Require(movingTargetRuntime.TileAt(36, 1) != 0x90 && foundMovedTargetTile,
            "FUN_1008_e636 awakens a target and installs active 0x87-0x8f tiles");
    std::filesystem::remove(movingTargetRuntimePath);

    const std::filesystem::path preActorRuntimePath =
        gameDataDirectory / L"selector-1038-pre-actor-runtime.VKW";
    Require(levelOne.SaveGame(preActorRuntimePath),
            "save selector-1038 pre-actor runtime fixture");
    {
        std::fstream save(preActorRuntimePath,
                          std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        const std::uint32_t spawnSeed = 22;
        const std::uint16_t playerColumn = 35;
        const std::uint16_t playerRow = 2;
        const int difficulty = 4;
        const std::uint16_t empty = 0;
        const std::uint16_t awakeTarget = 0x87;
        save.seekp(17);
        save.write(reinterpret_cast<const char*>(&spawnSeed), sizeof(spawnSeed));
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        save.seekp(62);
        save.write(reinterpret_cast<const char*>(&difficulty), sizeof(difficulty));
        save.seekp(boardOffset + static_cast<std::streamoff>((35 * 36 + 2) * 2));
        save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
        save.seekp(boardOffset + static_cast<std::streamoff>((36 * 36 + 1) * 2));
        save.write(reinterpret_cast<const char*>(&awakeTarget), sizeof(awakeTarget));
        save.seekp(boardOffset + static_cast<std::streamoff>((23 * 36 + 29) * 2));
        save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
        for (std::size_t column = 0; column < drain_storm::GameData::kBoardColumns; ++column) {
            for (std::size_t row = 0; row < drain_storm::GameData::kBoardRows; ++row) {
                const std::uint16_t tile = levelOne.TileAt(column, row);
                if (tile >= 0x18 && tile <= 0x22) {
                    save.seekp(boardOffset + static_cast<std::streamoff>((column * 36 + row) * 2));
                    save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
                }
            }
        }
        save.seekg(boardOffset + static_cast<std::streamoff>(40 * 36 * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        const std::streamoff actorsOffset = save.tellg();
        const std::array<std::uint16_t, 6> stationaryActor = {0, 0, 0, 0, 0, 1};
        for (std::uint32_t actor = 0; actor < actorCount; ++actor) {
            save.seekp(actorsOffset + static_cast<std::streamoff>(actor * 12));
            save.write(reinterpret_cast<const char*>(stationaryActor.data()),
                       sizeof(stationaryActor));
        }
        Require(save.good(), "write selector-1038 pre-actor runtime fixture");
    }
    drain_storm::GameData preActorRuntime;
    Require(preActorRuntime.Load(gameDataDirectory, 1)
                && preActorRuntime.LoadGame(preActorRuntimePath),
            "load selector-1038 pre-actor runtime fixture");
    const std::size_t actorsBeforePreRuntime = preActorRuntime.Actors().size();
    Require(preActorRuntime.MovePlayer(0, 0)
                && preActorRuntime.Actors().size() > actorsBeforePreRuntime,
            "1010:00ec releases target defenders before the actor pass");
    const bool releasedHunterPresent = std::any_of(
        preActorRuntime.Actors().begin()
            + static_cast<std::ptrdiff_t>(actorsBeforePreRuntime),
        preActorRuntime.Actors().end(), [](const drain_storm::BoardActor& actor) {
            return actor.behavior == 0;
        });
    Require(releasedHunterPresent,
            "first FUN_1008_e636 pass converts released right-half defenders to hunter mode");
    std::filesystem::remove(preActorRuntimePath);
    Require(levelOne.Actors()[16].column == 33 && levelOne.Actors()[16].row == 15 &&
                levelOne.Actors()[16].behavior == 2,
            "selector-1038 random actor coordinates");
    const std::filesystem::path actorLurePath =
        gameDataDirectory / L"common-actor-lure.VKW";
    Require(levelOne.SaveGame(actorLurePath), "save common actor-lure fixture");
    {
        std::fstream save(actorLurePath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 5;
        const std::uint16_t playerRow = 5;
        constexpr std::streamoff boardOffset = 130;
        const std::uint16_t empty = 0;
        const std::uint16_t lure = 7;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        for (int column = 19; column <= 22; ++column) {
            for (int row = 19; row <= 21; ++row) {
                save.seekp(boardOffset + static_cast<std::streamoff>((column * 36 + row) * 2));
                save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
            }
        }
        save.seekp(boardOffset + static_cast<std::streamoff>((22 * 36 + 20) * 2));
        save.write(reinterpret_cast<const char*>(&lure), sizeof(lure));
        save.seekg(boardOffset + static_cast<std::streamoff>(40 * 36 * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        const std::streamoff actorsOffset = save.tellg();
        for (std::uint32_t actor = 0; actor < actorCount; ++actor) {
            const std::array<std::uint16_t, 6> stationaryActor = {0, 0, 0, 0, 0, 1};
            save.seekp(actorsOffset + static_cast<std::streamoff>(actor * 12));
            save.write(reinterpret_cast<const char*>(stationaryActor.data()),
                       sizeof(stationaryActor));
        }
        const std::array<std::uint16_t, 6> lureActor = {0, 20, 20, 0, 0, 0};
        save.seekp(actorsOffset);
        save.write(reinterpret_cast<const char*>(lureActor.data()), sizeof(lureActor));
        Require(save.good(), "write common actor-lure fixture");
    }
    drain_storm::GameData actorLure;
    Require(actorLure.Load(gameDataDirectory, 1) && actorLure.LoadGame(actorLurePath)
                && actorLure.MovePlayer(0, 0),
            "advance common actor-lure fixture");
    Require(actorLure.Actors()[0].column == 21
                && actorLure.Actors()[0].row >= 19 && actorLure.Actors()[0].row <= 21,
            "FUN_1008_adee lure tile redirects hunter away from player");
    std::filesystem::remove(actorLurePath);

    const std::filesystem::path actorBreakPath =
        gameDataDirectory / L"common-actor-break.VKW";
    Require(levelOne.SaveGame(actorBreakPath), "save common actor-break fixture");
    {
        std::fstream save(actorBreakPath, std::ios::binary | std::ios::in | std::ios::out);
        // Dormant 1010:00ec targets consume no RNG before the actor pass.
        const std::uint32_t breakSeed = 19;
        const std::uint16_t empty = 0;
        const std::uint16_t destructible = 3;
        constexpr std::streamoff boardOffset = 130;
        save.seekp(17);
        save.write(reinterpret_cast<const char*>(&breakSeed), sizeof(breakSeed));
        for (std::size_t column = 0; column < drain_storm::GameData::kBoardColumns; ++column) {
            for (std::size_t row = 0; row < drain_storm::GameData::kBoardRows; ++row) {
                const std::uint16_t tile = levelOne.TileAt(column, row);
                if (tile >= 0x18 && tile <= 0x22) {
                    save.seekp(boardOffset + static_cast<std::streamoff>((column * 36 + row) * 2));
                    save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
                }
            }
        }
        for (int column = 19; column <= 21; ++column) {
            for (int row = 19; row <= 21; ++row) {
                save.seekp(boardOffset + static_cast<std::streamoff>((column * 36 + row) * 2));
                save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
            }
        }
        save.seekp(boardOffset + static_cast<std::streamoff>((19 * 36 + 19) * 2));
        save.write(reinterpret_cast<const char*>(&destructible), sizeof(destructible));
        save.seekg(boardOffset + static_cast<std::streamoff>(40 * 36 * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        const std::streamoff actorsOffset = save.tellg();
        const std::array<std::uint16_t, 6> stationaryActor = {0, 0, 0, 0, 0, 1};
        for (std::uint32_t actor = 0; actor < actorCount; ++actor) {
            save.seekp(actorsOffset + static_cast<std::streamoff>(actor * 12));
            save.write(reinterpret_cast<const char*>(stationaryActor.data()),
                       sizeof(stationaryActor));
        }
        const std::array<std::uint16_t, 6> breakingActor = {1, 20, 20, 0, 0, 0};
        save.seekp(actorsOffset);
        save.write(reinterpret_cast<const char*>(breakingActor.data()), sizeof(breakingActor));
        Require(save.good(), "write common actor-break fixture");
    }
    drain_storm::GameData actorBreak;
    Require(actorBreak.Load(gameDataDirectory, 1) && actorBreak.LoadGame(actorBreakPath)
                && actorBreak.MovePlayer(0, 0),
            "advance common actor-break fixture");
    Require(actorBreak.TileAt(19, 19) == 0
                && actorBreak.Actors()[0].column == 20 && actorBreak.Actors()[0].row == 20,
            "FUN_1008_8310 destructive hunter breaks terrain instead of moving");
    std::filesystem::remove(actorBreakPath);

    const std::filesystem::path cyclicActorBreakPath =
        gameDataDirectory / L"selector-1048-actor-break-cycle.VKW";
    drain_storm::GameData cyclicActorBreakSetup;
    Require(cyclicActorBreakSetup.Load(gameDataDirectory, 3)
                && cyclicActorBreakSetup.SaveGame(cyclicActorBreakPath),
            "save selector-1048 actor-break cycle fixture");
    {
        std::fstream save(cyclicActorBreakPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint32_t vulnerableTurn = 180;
        const std::uint16_t empty = 0;
        const std::uint16_t destructible = 3;
        constexpr std::streamoff boardOffset = 130;
        save.seekp(21);
        save.write(reinterpret_cast<const char*>(&vulnerableTurn), sizeof(vulnerableTurn));
        for (int column = 19; column <= 21; ++column) {
            for (int row = 19; row <= 21; ++row) {
                save.seekp(boardOffset + static_cast<std::streamoff>((column * 36 + row) * 2));
                save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
            }
        }
        save.seekp(boardOffset + static_cast<std::streamoff>((19 * 36 + 19) * 2));
        save.write(reinterpret_cast<const char*>(&destructible), sizeof(destructible));
        save.seekg(boardOffset + static_cast<std::streamoff>(40 * 36 * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        const std::streamoff actorsOffset = save.tellg();
        const std::array<std::uint16_t, 6> stationaryActor = {0, 0, 0, 0, 0, 1};
        for (std::uint32_t actor = 0; actor < actorCount; ++actor) {
            save.seekp(actorsOffset + static_cast<std::streamoff>(actor * 12));
            save.write(reinterpret_cast<const char*>(stationaryActor.data()),
                       sizeof(stationaryActor));
        }
        const std::array<std::uint16_t, 6> breakingActor = {1, 20, 20, 0, 0, 0};
        save.seekp(actorsOffset);
        save.write(reinterpret_cast<const char*>(breakingActor.data()), sizeof(breakingActor));
        Require(save.good(), "write selector-1048 actor-break cycle fixture");
    }
    drain_storm::GameData cyclicActorBreak;
    Require(cyclicActorBreak.Load(gameDataDirectory, 3)
                && cyclicActorBreak.LoadGame(cyclicActorBreakPath)
                && cyclicActorBreak.MovePlayer(0, 0),
            "advance selector-1048 actor-break cycle fixture");
    Require(cyclicActorBreak.TileAt(19, 19) == 0
                && cyclicActorBreak.Actors()[0].column == 20
                && cyclicActorBreak.Actors()[0].row == 20,
            "FUN_1010_4692 turn-180 break divisor makes destruction certain");
    std::filesystem::remove(cyclicActorBreakPath);

    const std::filesystem::path actorMinePath =
        gameDataDirectory / L"common-actor-mine.VKW";
    drain_storm::GameData actorMineSetup;
    Require(actorMineSetup.Load(gameDataDirectory, 1)
                && actorMineSetup.SaveGame(actorMinePath),
            "save common actor-mine setup");
    equipFirstInventoryItem(actorMinePath, 3, 1);
    Require(actorMineSetup.LoadGame(actorMinePath)
                && actorMineSetup.UseSelectedItem(0, -1)
                && !actorMineSetup.Projectiles().empty()
                && actorMineSetup.IdentifySquare(14, 31) == L"Mines (1)."
                && actorMineSetup.SaveGame(actorMinePath),
            "place and identify common stationary-mine fixture");
    {
        std::fstream save(actorMinePath, std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        save.seekg(boardOffset + static_cast<std::streamoff>(40 * 36 * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        const std::streamoff actorsOffset = save.tellg();
        const std::array<std::uint16_t, 6> stationaryActor = {0, 0, 0, 0, 0, 1};
        for (std::uint32_t actor = 0; actor < actorCount; ++actor) {
            save.seekp(actorsOffset + static_cast<std::streamoff>(actor * 12));
            save.write(reinterpret_cast<const char*>(stationaryActor.data()),
                       sizeof(stationaryActor));
        }
        const std::array<std::uint16_t, 6> mineActor = {0, 14, 31, 0, 0, 1};
        save.seekp(actorsOffset);
        save.write(reinterpret_cast<const char*>(mineActor.data()), sizeof(mineActor));
        Require(save.good(), "write common actor-mine fixture");
    }
    drain_storm::GameData actorMine;
    Require(actorMine.Load(gameDataDirectory, 1) && actorMine.LoadGame(actorMinePath),
            "load common actor-mine fixture");
    const std::uint32_t actorMineScore = actorMine.Score();
    Require(actorMine.MovePlayer(0, 0),
            "advance common actor-mine fixture");
    Require(actorMine.Projectiles().empty() && actorMine.Score() == actorMineScore + 10
                && std::none_of(actorMine.Overlays().begin(), actorMine.Overlays().end(),
                    [](const drain_storm::BoardOverlay& overlay) {
                        return overlay.type == 0x24;
                    })
                && std::any_of(actorMine.Overlays().begin(), actorMine.Overlays().end(),
                    [](const drain_storm::BoardOverlay& overlay) {
                        return overlay.type == 0x14 && overlay.column == 14 && overlay.row == 31;
                    }),
            "FUN_1008_aacc resolves triggered mine blast and actor score");
    for (int frame = 0; frame < 4; ++frame) actorMine.AdvanceAnimation();
    Require(std::any_of(actorMine.Overlays().begin(), actorMine.Overlays().end(),
                [](const drain_storm::BoardOverlay& overlay) {
                    return overlay.type == 0x14 && overlay.column == 14 && overlay.row == 31;
                }),
            "FUN_1008_8be6 retains transient effects through frame five");
    actorMine.AdvanceAnimation();
    Require(std::none_of(actorMine.Overlays().begin(), actorMine.Overlays().end(),
                [](const drain_storm::BoardOverlay& overlay) {
                    return overlay.type == 0x14;
                }),
            "FUN_1008_8be6 removes transient effects after frame five");
    std::filesystem::remove(actorMinePath);

    const std::filesystem::path actorFireballPath =
        gameDataDirectory / L"common-actor-fireball.VKW";
    const std::filesystem::path movingActorFireballPath =
        gameDataDirectory / L"common-moving-actor-fireball.VKW";
    drain_storm::GameData actorFireballSetup;
    Require(actorFireballSetup.Load(gameDataDirectory, 1)
                && actorFireballSetup.SaveGame(actorFireballPath),
            "save common actor-fireball fixture");
    equipFirstInventoryItem(actorFireballPath, 3, 1);
    Require(actorFireballSetup.LoadGame(actorFireballPath)
                && actorFireballSetup.UseSelectedItem(0, -1)
                && actorFireballSetup.Projectiles().size() == 1
                && actorFireballSetup.SaveGame(actorFireballPath),
            "place stationary fireball fixture");
    {
        std::fstream save(actorFireballPath, std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        save.seekg(boardOffset + static_cast<std::streamoff>(40 * 36 * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        const std::streamoff actorsOffset = save.tellg();
        const std::array<std::uint16_t, 6> stationaryActor = {0, 0, 0, 0, 0, 1};
        for (std::uint32_t actor = 0; actor < actorCount; ++actor) {
            save.seekp(actorsOffset + static_cast<std::streamoff>(actor * 12));
            save.write(reinterpret_cast<const char*>(stationaryActor.data()),
                       sizeof(stationaryActor));
        }
        const std::array<std::uint16_t, 6> fireballActor = {0, 14, 31, 0, 0, 1};
        save.seekp(actorsOffset);
        save.write(reinterpret_cast<const char*>(fireballActor.data()), sizeof(fireballActor));
        save.seekg(actorsOffset + static_cast<std::streamoff>(actorCount * 12));
        std::uint32_t inventoryCount{};
        std::uint32_t projectileCount{};
        save.read(reinterpret_cast<char*>(&inventoryCount), sizeof(inventoryCount));
        save.seekg(static_cast<std::streamoff>(inventoryCount * 4), std::ios::cur);
        save.read(reinterpret_cast<char*>(&projectileCount), sizeof(projectileCount));
        Require(projectileCount == 1, "locate common actor-fireball projectile");
        const std::uint16_t fireballType = 4;
        save.seekp(save.tellg());
        save.write(reinterpret_cast<const char*>(&fireballType), sizeof(fireballType));
        Require(save.good(), "write common actor-fireball fixture");
    }
    std::filesystem::copy_file(actorFireballPath, movingActorFireballPath,
                               std::filesystem::copy_options::overwrite_existing);
    {
        std::fstream save(movingActorFireballPath,
                          std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        save.seekg(boardOffset + static_cast<std::streamoff>(40 * 36 * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        const std::streamoff actorsOffset = save.tellg();
        const std::uint16_t movingTargetRow = 30;
        save.seekp(actorsOffset + 4);
        save.write(reinterpret_cast<const char*>(&movingTargetRow), sizeof(movingTargetRow));
        save.seekg(actorsOffset + static_cast<std::streamoff>(actorCount * 12));
        std::uint32_t inventoryCount{};
        std::uint32_t projectileCount{};
        save.read(reinterpret_cast<char*>(&inventoryCount), sizeof(inventoryCount));
        save.seekg(static_cast<std::streamoff>(inventoryCount * 4), std::ios::cur);
        save.read(reinterpret_cast<char*>(&projectileCount), sizeof(projectileCount));
        Require(projectileCount == 1, "locate moving actor-fireball projectile");
        const std::streamoff projectilesOffset = save.tellg();
        const int movingDeltaRow = -1;
        save.seekp(projectilesOffset + 16);
        save.write(reinterpret_cast<const char*>(&movingDeltaRow), sizeof(movingDeltaRow));
        Require(save.good(), "write moving actor-fireball fixture");
    }
    drain_storm::GameData movingActorFireball;
    Require(movingActorFireball.Load(gameDataDirectory, 1)
                && movingActorFireball.LoadGame(movingActorFireballPath),
            "load moving actor-fireball fixture");
    const std::uint32_t movingActorFireballScore = movingActorFireball.Score();
    Require(movingActorFireball.MovePlayer(0, 0)
                && movingActorFireball.Score() == movingActorFireballScore + 10
                && movingActorFireball.Projectiles().size() == 1
                && movingActorFireball.Projectiles()[0].type == 4
                && movingActorFireball.Projectiles()[0].row == 30
                && movingActorFireball.TakeSoundEvent() == drain_storm::GameplaySound::Crackle,
            "FUN_1008_77d0 moving fireball kills actor with crackle and survives impact");
    std::filesystem::remove(movingActorFireballPath);
    drain_storm::GameData actorFireball;
    Require(actorFireball.Load(gameDataDirectory, 1)
                && actorFireball.LoadGame(actorFireballPath),
            "load common actor-fireball fixture");
    const std::uint32_t actorFireballScore = actorFireball.Score();
    Require(actorFireball.MovePlayer(0, 0), "advance common actor-fireball fixture");
    Require(actorFireball.Health() == 3
                && actorFireball.Score() == actorFireballScore + 10
                && actorFireball.Projectiles().size() == 1
                && actorFireball.Projectiles()[0].type == 4
                && actorFireball.TakeSoundEvent() == drain_storm::GameplaySound::Crackle
                && std::any_of(actorFireball.Overlays().begin(), actorFireball.Overlays().end(),
                    [](const drain_storm::BoardOverlay& overlay) {
                        return overlay.type == 0x14 && overlay.column == 14 && overlay.row == 31;
                    }),
            "FUN_1008_8310 stationary fireball kills actor with crackle and survives impact");
    std::filesystem::remove(actorFireballPath);

    const std::filesystem::path actorImpactPath =
        gameDataDirectory / L"common-actor-player-impact.VKW";
    Require(levelOne.SaveGame(actorImpactPath), "save common actor-player impact fixture");
    {
        std::fstream save(actorImpactPath, std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        save.seekg(boardOffset + static_cast<std::streamoff>(40 * 36 * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        const std::streamoff actorsOffset = save.tellg();
        const std::array<std::uint16_t, 6> stationaryActor = {0, 0, 0, 0, 0, 1};
        for (std::uint32_t actor = 0; actor < actorCount; ++actor) {
            save.seekp(actorsOffset + static_cast<std::streamoff>(actor * 12));
            save.write(reinterpret_cast<const char*>(stationaryActor.data()),
                       sizeof(stationaryActor));
        }
        const std::array<std::uint16_t, 6> impactingActor = {0, 14, 32, 0, 0, 1};
        save.seekp(actorsOffset);
        save.write(reinterpret_cast<const char*>(impactingActor.data()), sizeof(impactingActor));
        Require(save.good(), "write common actor-player impact fixture");
    }
    drain_storm::GameData actorImpact;
    Require(actorImpact.Load(gameDataDirectory, 1) && actorImpact.LoadGame(actorImpactPath)
                && actorImpact.MovePlayer(0, 0),
            "advance common actor-player impact fixture");
    Require(actorImpact.Health() == 2
                && std::any_of(actorImpact.Overlays().begin(), actorImpact.Overlays().end(),
                    [](const drain_storm::BoardOverlay& overlay) {
                        return overlay.type == 0x18 && overlay.column == 14 && overlay.row == 32;
                    }),
            "FUN_1008_8310 player impact applies damage and native effect");
    Require(actorImpact.SaveGame(actorImpactPath),
            "save in-flight player-impact animation");
    drain_storm::GameData restoredActorImpact;
    Require(restoredActorImpact.Load(gameDataDirectory, 1)
                && restoredActorImpact.LoadGame(actorImpactPath),
            "restore in-flight player-impact animation");
    for (int frame = 0; frame < 5; ++frame) {
        restoredActorImpact.AdvanceAnimation();
        Require(restoredActorImpact.MovePlayer(0, 0),
                "rapid movement while player-impact animation runs");
    }
    Require(std::none_of(restoredActorImpact.Overlays().begin(),
                         restoredActorImpact.Overlays().end(),
                [](const drain_storm::BoardOverlay& overlay) {
                    return overlay.type == 0x18;
                }),
            "transient effects finish after reload even during rapid movement");
    std::filesystem::remove(actorImpactPath);

    const std::filesystem::path actorPortalPath =
        gameDataDirectory / L"common-actor-portal-passability.VKW";
    Require(levelOne.SaveGame(actorPortalPath), "save actor portal-passability fixture");
    {
        std::fstream save(actorPortalPath, std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        const std::uint16_t playerColumn = 30;
        const std::uint16_t playerRow = 10;
        const std::uint16_t empty = 0;
        const std::uint16_t wall = 3;
        const std::uint16_t portal = 0x10;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        const auto writeTile = [&](int column, int row, const std::uint16_t& tile) {
            save.seekp(boardOffset + static_cast<std::streamoff>((column * 36 + row) * 2));
            save.write(reinterpret_cast<const char*>(&tile), sizeof(tile));
        };
        writeTile(30, 10, empty);
        for (int column = 9; column <= 11; ++column) {
            for (int row = 9; row <= 11; ++row) writeTile(column, row, wall);
        }
        writeTile(10, 10, empty);
        writeTile(11, 10, portal);
        save.seekg(boardOffset + static_cast<std::streamoff>(40 * 36 * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        const std::streamoff actorsOffset = save.tellg();
        const std::array<std::uint16_t, 6> stationaryActor = {0, 0, 0, 0, 0, 1};
        for (std::uint32_t actor = 0; actor < actorCount; ++actor) {
            save.seekp(actorsOffset + static_cast<std::streamoff>(actor * 12));
            save.write(reinterpret_cast<const char*>(stationaryActor.data()),
                       sizeof(stationaryActor));
        }
        const std::array<std::uint16_t, 6> portalActor = {0, 10, 10, 0, 0, 0};
        save.seekp(actorsOffset);
        save.write(reinterpret_cast<const char*>(portalActor.data()), sizeof(portalActor));
        Require(save.good(), "write actor portal-passability fixture");
    }
    drain_storm::GameData actorPortal;
    Require(actorPortal.Load(gameDataDirectory, 1) && actorPortal.LoadGame(actorPortalPath)
                && actorPortal.MovePlayer(0, 0)
                && actorPortal.Actors()[0].column == 11
                && actorPortal.Actors()[0].row == 10,
            "FUN_1008_91ec lets spawned enemies leave through intact portal machinery");
    std::filesystem::remove(actorPortalPath);

    const auto writeCollidingActors = [](const std::filesystem::path& path,
                                          std::uint16_t firstType,
                                          std::uint16_t secondType) {
        std::fstream save(path, std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        save.seekg(boardOffset + static_cast<std::streamoff>(40 * 36 * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        Require(actorCount >= 2, "locate simultaneous player-impact actors");
        const std::streamoff actorsOffset = save.tellg();
        const std::array<std::uint16_t, 6> stationaryActor = {0, 0, 0, 0, 0, 1};
        for (std::uint32_t actor = 0; actor < actorCount; ++actor) {
            save.seekp(actorsOffset + static_cast<std::streamoff>(actor * 12));
            save.write(reinterpret_cast<const char*>(stationaryActor.data()),
                       sizeof(stationaryActor));
        }
        const std::array<std::uint16_t, 6> first = {firstType, 14, 32, 0, 0, 1};
        const std::array<std::uint16_t, 6> second = {secondType, 14, 32, 0, 0, 1};
        save.seekp(actorsOffset);
        save.write(reinterpret_cast<const char*>(first.data()), sizeof(first));
        save.write(reinterpret_cast<const char*>(second.data()), sizeof(second));
        Require(save.good(), "write simultaneous player-impact actors");
    };

    const auto impactEffectCount = [](const drain_storm::GameData& data) {
        return std::count_if(data.Overlays().begin(), data.Overlays().end(),
            [](const drain_storm::BoardOverlay& overlay) {
                return overlay.type == 0x18 && overlay.column == 14 && overlay.row == 32;
            });
    };

    const std::filesystem::path doubleImpactPath =
        gameDataDirectory / L"common-double-player-impact.VKW";
    Require(levelOne.SaveGame(doubleImpactPath), "save double player-impact fixture");
    writeCollidingActors(doubleImpactPath, 0, 0);
    drain_storm::GameData doubleImpact;
    Require(doubleImpact.Load(gameDataDirectory, 1) && doubleImpact.LoadGame(doubleImpactPath)
                && doubleImpact.MovePlayer(0, 0),
            "advance double player-impact fixture");
    Require(doubleImpact.Health() == 1 && impactEffectCount(doubleImpact) == 2
                && doubleImpact.TakeSoundEvent() == drain_storm::GameplaySound::PlayerHitTwice,
            "FUN_1008_286c removes every contact and promotes the repeated hit sound");
    std::filesystem::remove(doubleImpactPath);

    const std::filesystem::path lethalImpactPath =
        gameDataDirectory / L"common-lethal-player-impact.VKW";
    Require(levelOne.SaveGame(lethalImpactPath), "save lethal player-impact fixture");
    writeCollidingActors(lethalImpactPath, 0, 3);
    drain_storm::GameData lethalImpact;
    Require(lethalImpact.Load(gameDataDirectory, 1) && lethalImpact.LoadGame(lethalImpactPath)
                && lethalImpact.MovePlayer(0, 0),
            "advance lethal player-impact fixture");
    Require(lethalImpact.Health() == 0 && lethalImpact.PlayerSpriteIndex(false) == 0x3e
                && impactEffectCount(lethalImpact) == 2
                && lethalImpact.TakeSoundEvent() == drain_storm::GameplaySound::PlayerDies,
            "FUN_1008_2920 applies heavy damage and retains lethal sound priority");

    drain_storm::GameData childLethalImpact;
    Require(childLethalImpact.Load(gameDataDirectory, 1)
                && childLethalImpact.LoadGame(lethalImpactPath),
            "load child-board lethal player-impact fixture");
    childLethalImpact.SetTerminalDeathEnabled(false);
    Require(childLethalImpact.MovePlayer(0, 0) && childLethalImpact.Health() == 0
                && childLethalImpact.PlayerSpriteIndex(false) == 0x152
                && impactEffectCount(childLethalImpact) == 2
                && childLethalImpact.TakeSoundEvent()
                    == drain_storm::GameplaySound::PlayerHitTwice,
            "FUN_1008_2920 suppresses terminal death on explicitly scripted attract boards");
    std::filesystem::remove(lethalImpactPath);
    Require(levelOne.Health() == 3, "initial health");
    Require(levelOne.RecallDiscoveredLaw()
                && levelOne.TakeMessage() == L"You haven't read any fortunes yet.",
            "law recall reports an empty collection");
    Require(levelOne.Inventory().size() == 12, "original inventory slot count");
    Require(levelOne.ItemName(0) == L"Bubble" && levelOne.ItemName(20) == L"Emerald"
                && levelOne.ItemName(21) == L"Lucky Chip",
            "all twenty-two original arsenal descriptors are exported");
    const std::filesystem::path powerPlantRepellentPath =
        gameDataDirectory / L"selector-1038-repellent.VKW";
    drain_storm::GameData powerPlantRepellentSetup;
    Require(powerPlantRepellentSetup.Load(gameDataDirectory, 1)
                && powerPlantRepellentSetup.SaveGame(powerPlantRepellentPath),
            "save selector-1038 repellent fixture");
    equipFirstInventoryItem(powerPlantRepellentPath, 12, 1);
    drain_storm::GameData powerPlantRepellent;
    Require(powerPlantRepellent.Load(gameDataDirectory, 1)
                && powerPlantRepellent.LoadGame(powerPlantRepellentPath)
                && powerPlantRepellent.UseSelectedItem(1, 0)
                && powerPlantRepellent.RepellentEnergy() == 90,
            "FUN_1010_0bd2 drains ten repellent units on selector 1038");
    std::filesystem::remove(powerPlantRepellentPath);

    const std::filesystem::path commonRepellentPath =
        gameDataDirectory / L"common-repellent.VKW";
    drain_storm::GameData commonRepellentSetup;
    Require(commonRepellentSetup.Load(gameDataDirectory, 2)
                && commonRepellentSetup.SaveGame(commonRepellentPath),
            "save common repellent fixture");
    equipFirstInventoryItem(commonRepellentPath, 12, 1);
    drain_storm::GameData commonRepellent;
    Require(commonRepellent.Load(gameDataDirectory, 2)
                && commonRepellent.LoadGame(commonRepellentPath)
                && commonRepellent.UseSelectedItem(1, 0)
                && commonRepellent.RepellentEnergy() == 98
                && commonRepellent.PlayerSpriteIndex(false) == 0x16a,
            "FUN_1008_c55c drains two repellent units on common boards");
    std::filesystem::remove(commonRepellentPath);

    const std::filesystem::path smartBombPath =
        gameDataDirectory / L"selector-1038-smart-bomb.VKW";
    drain_storm::GameData smartBombSetup;
    Require(smartBombSetup.Load(gameDataDirectory, 1)
                && smartBombSetup.SaveGame(smartBombPath),
            "save selector-1038 Smart Bomb fixture");
    {
        std::fstream save(smartBombPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint32_t resistanceSeed = 1;
        constexpr std::streamoff boardOffset = 130;
        save.seekp(17);
        save.write(reinterpret_cast<const char*>(&resistanceSeed), sizeof(resistanceSeed));
        const std::uint16_t empty = 0;
        for (std::size_t column = 0; column < drain_storm::GameData::kBoardColumns; ++column) {
            for (std::size_t row = 0; row < drain_storm::GameData::kBoardRows; ++row) {
                const std::uint16_t tile = smartBombSetup.TileAt(column, row);
                if (tile >= 0x18 && tile <= 0x22) {
                    save.seekp(boardOffset + static_cast<std::streamoff>((column * 36 + row) * 2));
                    save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
                }
            }
        }
        save.seekg(boardOffset + static_cast<std::streamoff>(
            drain_storm::GameData::kBoardColumns
            * drain_storm::GameData::kBoardRows * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        const std::streamoff actorsOffset = save.tellg();
        for (std::uint32_t actor = 0; actor < actorCount; ++actor) {
            const std::uint16_t column = actor == 0
                ? 2 : static_cast<std::uint16_t>(10 + actor % 6);
            const std::uint16_t row = actor == 0
                ? 2 : static_cast<std::uint16_t>(10 + (actor / 6) % 6);
            const std::uint16_t stationary = 1;
            save.seekp(actorsOffset + static_cast<std::streamoff>(actor * 12 + 2));
            save.write(reinterpret_cast<const char*>(&column), sizeof(column));
            save.write(reinterpret_cast<const char*>(&row), sizeof(row));
            save.seekp(actorsOffset + static_cast<std::streamoff>(actor * 12 + 10));
            save.write(reinterpret_cast<const char*>(&stationary), sizeof(stationary));
        }
        Require(save.good() && actorCount > 1, "patch selector-1038 Smart Bomb actors");
    }
    equipFirstInventoryItem(smartBombPath, 13, 1);
    drain_storm::GameData smartBomb;
    Require(smartBomb.Load(gameDataDirectory, 1)
                && smartBomb.LoadGame(smartBombPath),
            "load selector-1038 Smart Bomb fixture");
    const std::size_t protectedActorCount = smartBomb.Actors().size();
    Require(smartBomb.UseSelectedItem(1, 0)
                && smartBomb.Actors().size() == protectedActorCount
                && smartBomb.TakeMessage()
                    == L"The Smart Bomb's effect is mitigated.",
            "FUN_1008_eec2 preserves force-field and randomly resistant actors");
    std::filesystem::remove(smartBombPath);
    Require(levelOne.Inventory()[1].type == 4 && levelOne.Inventory()[1].quantity >= 19,
            "initial inventory layout");
    Require(!levelOne.SelectPreviousItem(), "selection stops at first item");
    for (std::size_t index = 1; index < levelOne.Inventory().size(); ++index) {
        Require(levelOne.SelectNextItem(), "advance inventory selection");
    }
    Require(levelOne.SelectedItem() == 11, "last inventory item selected");
    Require(!levelOne.SelectNextItem(), "selection stops at last item");
    Require(levelOne.SelectVisibleArsenalItem(240) && levelOne.SelectedItem() == 7,
            "FUN_1008_5770 selects the first visible scrolled arsenal row");
    Require(!levelOne.SelectVisibleArsenalItem(275) && levelOne.SelectedItem() == 7,
            "FUN_1008_5770 preserves the one-pixel arsenal row boundary");
    Require(levelOne.SelectVisibleArsenalItem(384) && levelOne.SelectedItem() == 11,
            "FUN_1008_5770 selects the fifth visible scrolled arsenal row");
    Require(levelOne.SelectVisibleArsenalItem(419) && levelOne.SelectedItem() == 11
                && levelOne.SelectVisibleArsenalItem(420) && levelOne.SelectedItem() == 11,
            "FUN_1008_5770 extends the fifth row through the bottom boundary");
    Require(!levelOne.SelectVisibleArsenalItem(421) && levelOne.SelectedItem() == 11,
            "FUN_1008_5770 excludes the arsenal area's lower boundary");
    Require(levelOne.ScrollVisibleArsenal(true)
                && levelOne.SelectVisibleArsenalItem(240)
                && levelOne.SelectedItem() == 6,
            "FUN_1008_5a04 scrolls the arsenal viewport upward without moving selection");
    for (int scroll = 0; scroll < 7; ++scroll) {
        levelOne.ScrollVisibleArsenal(false);
    }
    Require(!levelOne.ScrollVisibleArsenal(false)
                && levelOne.SelectVisibleArsenalItem(384)
                && levelOne.SelectedItem() == 11,
            "FUN_1008_5a04 clamps the five-row arsenal viewport at its lower extent");
    const auto randomActorBeforeMove = levelOne.Actors()[16];
    Require(levelOne.MovePlayer(0, -1), "move onto a passable level-one square");
    Require(levelOne.PlayerColumn() == 14 && levelOne.PlayerRow() == 31,
            "movement updates player coordinates");
    Require(!levelOne.MovePlayerToward(20, 20), "mouse movement rejects distant squares");
    const auto& randomActorAfterMove = levelOne.Actors()[16];
    const int randomDeltaColumn = static_cast<int>(randomActorAfterMove.column) -
        static_cast<int>(randomActorBeforeMove.column);
    const int randomDeltaRow = static_cast<int>(randomActorAfterMove.row) -
        static_cast<int>(randomActorBeforeMove.row);
    Require(randomActorAfterMove.behavior == 2 && randomDeltaColumn >= -1 &&
                randomDeltaColumn <= 1 && randomDeltaRow >= -1 && randomDeltaRow <= 1,
            "right-half captain keeps its random behavior until security breach");

    drain_storm::GameData levelTwo;
    Require(levelTwo.Load(gameDataDirectory, 2), "load level two");
    Require(levelTwo.TileAt(7, 2) == 0x45 && levelTwo.TileAt(9, 4) == 0x50,
            "selector-1040 bf2a structures");
    Require(levelTwo.TileAt(11, 0) == 0x7f && levelTwo.TileAt(12, 1) == 0x82,
            "selector-1040 transporter row");
    Require(levelTwo.TileAt(35, 27) == 8 && levelTwo.TileAt(37, 29) == 0x0f,
            "selector-1040 asymmetric large object");
    Require(levelTwo.TileAt(17, 35) == 99 && levelTwo.TileAt(32, 30) == 104,
            "selector-1040 diagonal mutations");
    Require(levelTwo.TileAt(20, 35) == 0x6a && levelTwo.TileAt(23, 34) == 0x6c
                && levelTwo.TileAt(32, 31) == 0x6a,
            "selector-1040 randomized diagonal mutations");
    Require(levelTwo.TileAt(20, 27) == 0x69 && levelTwo.TileAt(20, 26) == 0x63
                && levelTwo.TileAt(28, 27) == 0x6b && levelTwo.TileAt(28, 26) == 0x67,
            "selector-1040 randomized machine tiles");
    Require(levelTwo.MovePlayer(0, 0) && levelTwo.MovePlayer(0, 0)
                && levelTwo.MovePlayer(0, 0),
            "selector-1040 accepts idle turns before casino retaliation");
    Require(levelTwo.TileAt(2, 31) != 0 && levelTwo.TileAt(34, 26) != 0,
            "selector-1040 machines remain dormant before a qualifying win");

    const std::filesystem::path casinoPath =
        gameDataDirectory / L"selector-1040-casino.VKW";
    Require(levelTwo.SaveGame(casinoPath), "save selector-1040 casino fixture");
    {
        std::fstream save(casinoPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 23;
        const std::uint16_t playerRow = 26;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        Require(save.good(), "position selector-1040 casino fixture");
    }
    equipFirstInventoryItem(casinoPath, 4, 2);
    drain_storm::GameData casino;
    Require(casino.Load(gameDataDirectory, 2) && casino.LoadGame(casinoPath)
                && casino.UseSelectedItem(1, 0),
            "place selector-1040 casino bet");
    Require(casino.Inventory()[0].type == 4 && casino.Inventory()[0].quantity == 1
                && casino.TakeMessage().find(L"Total Bet:\n1") != std::wstring::npos,
            "selector-1040 casino consumes and reports first bet");
    Require(casino.TakeSoundEvent() == drain_storm::GameplaySound::CasinoBet,
            "selector-1040 casino uses recovered wager sound");
    Require(casino.SaveGame(casinoPath), "persist selector-1040 casino bet");
    drain_storm::GameData restoredCasino;
    Require(restoredCasino.Load(gameDataDirectory, 2) && restoredCasino.LoadGame(casinoPath)
                && restoredCasino.UseSelectedItem(1, 0)
                && restoredCasino.TakeMessage().find(L"Total Bet:\n2") != std::wstring::npos,
            "selector-1040 casino bet state survives reload");
    Require(restoredCasino.SaveGame(casinoPath), "save selector-1040 casino deal fixture");
    {
        std::fstream save(casinoPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 25;
        const std::uint16_t playerRow = 26;
        const std::uint16_t empty = 0;
        constexpr std::streamoff boardOffset = 130;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        save.seekp(boardOffset + static_cast<std::streamoff>((26 * 36 + 26) * 2));
        save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
    }
    drain_storm::GameData casinoDeal;
    Require(casinoDeal.Load(gameDataDirectory, 2) && casinoDeal.LoadGame(casinoPath)
                && casinoDeal.MovePlayer(1, 0)
                && casinoDeal.TakeMessage().find(L"Your cards, sir") != std::wstring::npos,
            "selector-1040 casino lever deals first five cards");
    const auto holdButtons = std::count_if(casinoDeal.Overlays().begin(), casinoDeal.Overlays().end(),
        [](const drain_storm::BoardOverlay& overlay) { return overlay.type == 0x2b; });
    Require(holdButtons == 5 && casinoDeal.IdentifySquare(18, 23) == L"Card is Unheld",
            "selector-1040 casino creates five hold controls");
    Require(casinoDeal.TakeSoundEvent() == drain_storm::GameplaySound::CasinoDraw,
            "selector-1040 casino lever uses recovered draw sound");

    const std::filesystem::path casinoRoyalPath =
        gameDataDirectory / L"selector-1040-casino-royal.VKW";
    Require(casinoDeal.SaveGame(casinoRoyalPath), "save selector-1040 royal-flush fixture");
    {
        std::fstream save(casinoRoyalPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 25;
        const std::uint16_t playerRow = 26;
        const std::uint16_t empty = 0;
        constexpr std::streamoff boardOffset = 130;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        save.seekp(boardOffset + static_cast<std::streamoff>((26 * 36 + 26) * 2));
        save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
        constexpr std::array<std::uint16_t, 5> ranks = {1, 10, 11, 12, 13};
        constexpr std::array<std::uint16_t, 5> suits = {1, 1, 1, 1, 1};
        constexpr std::array<std::uint8_t, 5> held = {1, 1, 1, 1, 1};
        const std::uint16_t deckIndex = 5;
        const std::uint16_t ordinaryPoker = 0;
        const std::uint16_t bonusStage = 0;
        const std::uint16_t resultTicks = 0;
        // DAH64 v23 retains the procedural runtime triplet, transient frame,
        // first-target activation state, fortress flags, and alien-maze phases
        // and construction depth after the casino state.
        save.seekp(-89 - static_cast<std::streamoff>(casinoDeal.Overlays().size()), std::ios::end);
        save.write(reinterpret_cast<const char*>(ranks.data()), sizeof(ranks));
        save.write(reinterpret_cast<const char*>(suits.data()), sizeof(suits));
        save.write(reinterpret_cast<const char*>(held.data()), sizeof(held));
        save.write(reinterpret_cast<const char*>(&deckIndex), sizeof(deckIndex));
        save.write(reinterpret_cast<const char*>(&ordinaryPoker), sizeof(ordinaryPoker));
        save.write(reinterpret_cast<const char*>(&bonusStage), sizeof(bonusStage));
        save.write(reinterpret_cast<const char*>(&resultTicks), sizeof(resultTicks));
        Require(save.good(), "write selector-1040 royal-flush fixture");
    }
    drain_storm::GameData casinoRoyal;
    Require(casinoRoyal.Load(gameDataDirectory, 2) && casinoRoyal.LoadGame(casinoRoyalPath),
            "load selector-1040 royal-flush fixture");
    Require(casinoRoyal.MovePlayer(1, 0), "draw selector-1040 royal-flush fixture");
    const std::wstring royalResult = casinoRoyal.TakeMessage();
    const auto royalFollowups = casinoRoyal.TakeMessages();
    Require(royalResult.find(L"Royal Flush") != std::wstring::npos,
            "selector-1040 identifies royal flush");
    Require(casinoRoyal.TakeSoundEvent() == drain_storm::GameplaySound::CasinoWin,
            "selector-1040 winning hand uses recovered win sound");
    Require(std::any_of(casinoRoyal.Inventory().begin(), casinoRoyal.Inventory().end(),
                [](const drain_storm::InventoryItem& item) {
                    return item.type == 4 && item.quantity == 419;
                }),
            "selector-1040 royal flush pays original 200-to-1 return");
    const std::size_t retaliationActors = casinoRoyal.Actors().size();
    Require(casinoRoyal.MovePlayer(0, 0)
                && casinoRoyal.Actors().size() > retaliationActors,
            "selector-1040 large win releases advanced pit-boss group");
    Require(std::any_of(royalFollowups.begin(), royalFollowups.end(), [](const auto& message) {
                return message == L"You won a bonus prize!\n(Left of table.)";
            }),
            "selector-1040 first large win reveals left casino bonus");
    Require(casinoRoyal.TileAt(13, 19) == 0x5c
                && casinoRoyal.SaveGame(casinoRoyalPath),
            "selector-1040 opens and persists first bonus prize");
    drain_storm::GameData restoredCasinoRoyal;
    Require(restoredCasinoRoyal.Load(gameDataDirectory, 2)
                && restoredCasinoRoyal.LoadGame(casinoRoyalPath)
                && restoredCasinoRoyal.TileAt(13, 19) == 0x5c,
            "selector-1040 bonus progression survives reload");
    std::filesystem::remove(casinoRoyalPath);

    Require(casinoDeal.SaveGame(casinoPath), "save selector-1040 casino hold fixture");
    {
        std::fstream save(casinoPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 17;
        const std::uint16_t playerRow = 23;
        const std::uint16_t empty = 0;
        constexpr std::streamoff boardOffset = 130;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        save.seekp(boardOffset + static_cast<std::streamoff>((18 * 36 + 23) * 2));
        save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
    }
    drain_storm::GameData casinoHold;
    Require(casinoHold.Load(gameDataDirectory, 2) && casinoHold.LoadGame(casinoPath),
            "load selector-1040 casino hold fixture");
    Require(casinoHold.MovePlayer(1, 0), "step onto selector-1040 casino hold control");
    Require(casinoHold.TakeMessage() == L"Card is Held",
            "selector-1040 casino reports held card");
    Require(casinoHold.SaveGame(casinoPath), "persist selector-1040 held card");
    {
        std::fstream save(casinoPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 17;
        const std::uint16_t playerRow = 23;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
    }
    drain_storm::GameData restoredCasinoHold;
    Require(restoredCasinoHold.Load(gameDataDirectory, 2)
                && restoredCasinoHold.LoadGame(casinoPath)
                && restoredCasinoHold.IdentifySquare(18, 23) == L"Card is Held",
            "selector-1040 held card survives reload");
    Require(casinoHold.SaveGame(casinoPath), "save selector-1040 casino draw fixture");
    {
        std::fstream save(casinoPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 25;
        const std::uint16_t playerRow = 26;
        const std::uint16_t empty = 0;
        constexpr std::streamoff boardOffset = 130;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        save.seekp(boardOffset + static_cast<std::streamoff>((26 * 36 + 26) * 2));
        save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
    }
    drain_storm::GameData casinoDraw;
    Require(casinoDraw.Load(gameDataDirectory, 2) && casinoDraw.LoadGame(casinoPath)
                && casinoDraw.MovePlayer(1, 0),
            "pull selector-1040 casino draw lever");
    const std::wstring casinoResult = casinoDraw.TakeMessage();
    const auto remainingHoldButtons = std::count_if(
        casinoDraw.Overlays().begin(), casinoDraw.Overlays().end(),
        [](const drain_storm::BoardOverlay& overlay) { return overlay.type == 0x2b; });
    Require((casinoResult.find(L"WINNER") != std::wstring::npos
                || casinoResult == L"Game Over. Play Again.")
                && remainingHoldButtons == 0,
            "selector-1040 casino resolves draw and removes hold controls");
    const drain_storm::GameplaySound expectedCasinoResultSound =
        casinoResult.find(L"WINNER") != std::wstring::npos
            ? drain_storm::GameplaySound::CasinoWin
            : drain_storm::GameplaySound::CasinoLose;
    Require(casinoDraw.TakeSoundEvent() == expectedCasinoResultSound,
            "selector-1040 result uses recovered win-or-loss sound");
    drain_storm::GameData casinoTimeout = casinoDraw;
    for (int tick = 0; tick < 50; ++tick) casinoTimeout.AdvanceAnimation();
    const std::filesystem::path casinoTimeoutPath =
        gameDataDirectory / L"selector-1040-casino-timeout.VKW";
    Require(casinoTimeout.SaveGame(casinoTimeoutPath),
            "persist selector-1040 post-result display timer");
    drain_storm::GameData restoredCasinoTimeout;
    Require(restoredCasinoTimeout.Load(gameDataDirectory, 2)
                && restoredCasinoTimeout.LoadGame(casinoTimeoutPath),
            "reload selector-1040 post-result display timer");
    for (int tick = 0; tick < 52; ++tick) restoredCasinoTimeout.AdvanceAnimation();
    Require(std::none_of(restoredCasinoTimeout.Overlays().begin(),
                         restoredCasinoTimeout.Overlays().end(),
                [](const drain_storm::BoardOverlay& overlay) {
                    return overlay.type == 0x1e && overlay.row == 17
                        && overlay.column >= 17 && overlay.column <= 29
                        && (overlay.column - 17) % 3 == 0;
                }),
            "selector-1040 automatically clears resolved hand after native timeout");
    std::filesystem::remove(casinoTimeoutPath);
    Require(casinoDraw.SaveGame(casinoPath), "save selector-1040 post-draw fixture");
    {
        std::fstream save(casinoPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t bettingColumn = 23;
        const std::uint16_t bettingRow = 26;
        const std::uint32_t firstInventorySlot = 0;
        save.seekp(9);
        save.write(reinterpret_cast<const char*>(&firstInventorySlot), sizeof(firstInventorySlot));
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&bettingColumn), sizeof(bettingColumn));
        save.write(reinterpret_cast<const char*>(&bettingRow), sizeof(bettingRow));
        Require(save.good(), "position selector-1040 post-draw fixture");
    }
    equipFirstInventoryItem(casinoPath, 4, 2);
    drain_storm::GameData casinoReplay;
    Require(casinoReplay.Load(gameDataDirectory, 2) && casinoReplay.LoadGame(casinoPath)
                && casinoReplay.UseSelectedItem(1, 0),
            "start selector-1040 wager after completed hand");
    const std::wstring replayMessage = casinoReplay.TakeMessage();
    Require(replayMessage.find(L"Total Bet") != std::wstring::npos,
            "selector-1040 accepts wager after completed hand");
    Require(std::none_of(casinoReplay.Overlays().begin(), casinoReplay.Overlays().end(),
                [](const drain_storm::BoardOverlay& overlay) {
                    return overlay.type == 0x1e && overlay.row == 17
                        && overlay.column >= 17 && overlay.column <= 29
                        && (overlay.column - 17) % 3 == 0;
                }),
            "selector-1040 clears prior card objects before next wager");
    std::filesystem::remove(casinoPath);

    const std::filesystem::path casinoRejectionPath =
        gameDataDirectory / L"selector-1040-casino-rejection.VKW";
    Require(levelTwo.SaveGame(casinoRejectionPath),
            "save selector-1040 casino rejection fixture");
    {
        std::fstream save(casinoRejectionPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 23;
        const std::uint16_t playerRow = 26;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
    }
    equipFirstInventoryItem(casinoRejectionPath, 13, 2);
    drain_storm::GameData casinoRejection;
    Require(casinoRejection.Load(gameDataDirectory, 2)
                && casinoRejection.LoadGame(casinoRejectionPath)
                && casinoRejection.UseSelectedItem(1, 0)
                && casinoRejection.Inventory()[0].quantity == 2
                && casinoRejection.TakeMessage() == L"The house won't cover your bet.",
            "selector-1040 rejects unsupported casino inventory");
    std::filesystem::remove(casinoRejectionPath);

    const std::filesystem::path levelTwoForcePath =
        gameDataDirectory / L"selector-1040-force.VKW";
    Require(levelTwo.SaveGame(levelTwoForcePath), "save selector-1040 force fixture");
    {
        std::fstream save(levelTwoForcePath,
                          std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 12;
        const std::uint16_t playerRow = 4;
        const std::uint16_t empty = 0;
        constexpr std::streamoff boardOffset = 130;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        for (const std::uint16_t row : {std::uint16_t{3}, std::uint16_t{4}}) {
            save.seekp(boardOffset + static_cast<std::streamoff>(
                (playerColumn * drain_storm::GameData::kBoardRows + row) * 2));
            save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
        }
        save.seekp(boardOffset + static_cast<std::streamoff>(
            (13 * drain_storm::GameData::kBoardRows + playerRow) * 2));
        save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
        Require(save.good(), "position selector-1040 force fixture");
    }
    drain_storm::GameData levelTwoForce;
    Require(levelTwoForce.Load(gameDataDirectory, 2)
                && levelTwoForce.LoadGame(levelTwoForcePath)
                && levelTwoForce.MovePlayer(0, -1),
            "activate selector-1040 force boundary");
    Require(levelTwoForce.PlayerColumn() == 12 && levelTwoForce.PlayerRow() == 4
                && levelTwoForce.TakeMessage()
                    == L"A strange force deflects the Rubble Roadster.",
            "selector-1040 force cancels northward movement with exact message");
    Require(levelTwoForce.MovePlayer(1, -1)
                && levelTwoForce.PlayerColumn() == 13 && levelTwoForce.PlayerRow() == 4,
            "selector-1040 force preserves a diagonal command's horizontal component");
    std::filesystem::remove(levelTwoForcePath);

    drain_storm::GameData levelFour;
    Require(levelFour.Load(gameDataDirectory, 4), "load level four");
    Require(levelFour.TileAt(33, 5) == 0x7f && levelFour.TileAt(34, 6) == 0x82,
            "selector-1050 northeast transporter");
    Require(levelFour.TileAt(5, 29) == 0x7f && levelFour.TileAt(6, 30) == 0x82,
            "selector-1050 southwest transporter");
    Require(levelFour.Overlays().size() >= 6, "selector-1050 randomized pickups");
    const std::size_t firstScatteredPickup = levelFour.Overlays().size() - 6;
    Require(levelFour.Overlays()[firstScatteredPickup].type == 1 &&
                levelFour.Overlays()[firstScatteredPickup].column == 28 &&
                levelFour.Overlays()[firstScatteredPickup].row == 26,
            "selector-1050 recovered RNG sequence");
    Require(levelFour.Overlays().back().type == 2 && levelFour.Overlays().back().column == 22 &&
                levelFour.Overlays().back().row == 25,
            "selector-1050 recovered RNG sequence tail");

    drain_storm::GameData levelFourCountdown;
    const std::filesystem::path levelFourCountdownPath =
        gameDataDirectory / L"selector-1050-countdown.VKW";
    Require(levelFourCountdown.Load(gameDataDirectory, 4)
                && levelFourCountdown.SaveGame(levelFourCountdownPath),
            "save selector-1050 countdown board");
    {
        std::fstream save(levelFourCountdownPath,
                          std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        const std::uint16_t playerColumn = 20;
        const std::uint16_t playerRow = 20;
        const std::uint16_t empty = 0;
        const std::uint16_t wall = 0x60;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        for (int column = 19; column <= 21; ++column) {
            for (int row = 19; row <= 21; ++row) {
                const std::uint16_t tile = column == 20 && row == 20 ? empty : wall;
                save.seekp(boardOffset + static_cast<std::streamoff>((column * 36 + row) * 2));
                save.write(reinterpret_cast<const char*>(&tile), sizeof(tile));
            }
        }
        save.seekg(boardOffset + static_cast<std::streamoff>(40 * 36 * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        const std::streamoff actorsOffset = save.tellg();
        const std::array<std::uint16_t, 6> isolatedActor = {0, 0, 0, 0, 0, 0};
        for (std::uint32_t actor = 0; actor < actorCount; ++actor) {
            save.seekp(actorsOffset + static_cast<std::streamoff>(actor * 12));
            save.write(reinterpret_cast<const char*>(isolatedActor.data()),
                       sizeof(isolatedActor));
        }
        Require(save.good(), "isolate selector-1050 countdown player");
    }
    Require(levelFourCountdown.LoadGame(levelFourCountdownPath),
            "load isolated selector-1050 countdown board");
    const auto countTile = [](const drain_storm::GameData& board, std::uint16_t tile) {
        std::size_t count = 0;
        for (std::size_t column = 0; column < drain_storm::GameData::kBoardColumns; ++column) {
            for (std::size_t row = 0; row < drain_storm::GameData::kBoardRows; ++row) {
                count += board.TileAt(column, row) == tile ? 1u : 0u;
            }
        }
        return count;
    };
    Require(countTile(levelFourCountdown, 0x20) != 0,
            "selector-1050 countdown begins with sealed tiles");
    for (int turn = 0; turn < 24; ++turn) {
        Require(levelFourCountdown.UseSelectedItem(0, -1),
                "advance selector-1050 release countdown");
    }
    Require(countTile(levelFourCountdown, 0x20) == 0,
            "selector-1050 countdown releases sealed tiles");
    Require(!levelFourCountdown.Actors().empty()
                && std::all_of(levelFourCountdown.Actors().begin(),
                               levelFourCountdown.Actors().end(),
                               [](const drain_storm::BoardActor& actor) {
                                   return actor.behavior == 0;
                               }),
            "selector-1050 countdown releases spawned actors");
    std::filesystem::remove(levelFourCountdownPath);

    drain_storm::GameData pickupBoard;
    Require(pickupBoard.Load(gameDataDirectory, 4), "load collectible test board");
    const auto inventoryStock = [](const drain_storm::GameData& game) {
        std::uint32_t total = 0;
        for (const drain_storm::InventoryItem& item : game.Inventory()) {
            total += item.quantity;
        }
        return total;
    };

    const std::filesystem::path healthPickupPath =
        gameDataDirectory / L"health-pickup-roundtrip.VKW";
    Require(pickupBoard.SaveGame(healthPickupPath), "save health-pickup fixture");
    {
        std::fstream save(healthPickupPath, std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        save.seekg(boardOffset + static_cast<std::streamoff>(40 * 36 * 2));
        std::uint32_t overlayCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        Require(overlayCount != 0, "locate health-pickup overlay record");
        const std::array<std::uint16_t, 3> healthPickupRecord = {
            9, static_cast<std::uint16_t>(pickupBoard.PlayerColumn()),
            static_cast<std::uint16_t>(pickupBoard.PlayerRow())};
        save.seekp(save.tellg());
        save.write(reinterpret_cast<const char*>(healthPickupRecord.data()),
                   sizeof(healthPickupRecord));
        Require(save.good(), "write health-pickup overlay record");
    }
    drain_storm::GameData healthPickup;
    Require(healthPickup.Load(gameDataDirectory, 4)
                && healthPickup.LoadGame(healthPickupPath)
                && healthPickup.MovePlayer(0, 0)
                && healthPickup.Health() == 3
                && healthPickup.MovePlayer(0, -1)
                && healthPickup.MovePlayer(0, 1)
                && healthPickup.Health() == 4
                && healthPickup.TakeMessage() == L"You gained an extra life!"
                && healthPickup.SaveGame(healthPickupPath),
            "FUN_1008_a16a health treasure exceeds starting maximum");
    drain_storm::GameData restoredHealthPickup;
    Require(restoredHealthPickup.Load(gameDataDirectory, 4)
                && restoredHealthPickup.LoadGame(healthPickupPath)
                && restoredHealthPickup.Health() == 4,
            "above-starting health survives DAH64 round trip");
    std::filesystem::remove(healthPickupPath);

    const std::uint32_t stockBeforePickup = inventoryStock(pickupBoard);
    for (int step = 0; step < 4; ++step) {
        Require(pickupBoard.MovePlayer(0, -1), "walk toward map collectible");
    }
    Require(pickupBoard.MovePlayer(-1, 0) && pickupBoard.MovePlayer(-1, 0),
            "enter map collectible square");
    Require(pickupBoard.PlayerColumn() == 6 && pickupBoard.PlayerRow() == 2,
            "collectible fixture coordinates");
    Require(inventoryStock(pickupBoard) > stockBeforePickup,
            "random container adds recovered inventory stock");
    Require(pickupBoard.TakeMessage().starts_with(L"You found "),
            "random container emits its recovered inventory message");
    Require(pickupBoard.TakeSoundEvent() == drain_storm::GameplaySound::TreasureTwo,
            "type-one container selects recovered treasure-two sound");
    Require(std::none_of(pickupBoard.Overlays().begin(), pickupBoard.Overlays().end(),
                         [](const drain_storm::BoardOverlay& overlay) {
                             return overlay.column == 6 && overlay.row == 2;
                         }),
            "collected overlay node is removed");

    const std::filesystem::path blockedPickupPath =
        gameDataDirectory / L"blocked-pickup.VKW";
    drain_storm::GameData blockedPickupSource;
    Require(blockedPickupSource.Load(gameDataDirectory, 4)
                && blockedPickupSource.SaveGame(blockedPickupPath),
            "save blocked collectible fixture");
    {
        std::fstream save(blockedPickupPath,
                          std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        const std::uint16_t crud = 3;
        save.seekp(boardOffset + static_cast<std::streamoff>(
            (6 * drain_storm::GameData::kBoardRows + 2) * 2));
        save.write(reinterpret_cast<const char*>(&crud), sizeof(crud));
        Require(save.good(), "place destructible rock under collectible fixture");
    }
    drain_storm::GameData blockedPickup;
    Require(blockedPickup.Load(gameDataDirectory, 4)
                && blockedPickup.LoadGame(blockedPickupPath)
                && blockedPickup.IdentifySquare(6, 2) == L"Crud.",
            "load blocked collectible fixture");
    const std::uint32_t blockedStockBefore = inventoryStock(blockedPickup);
    for (int step = 0; step < 4; ++step) {
        Require(blockedPickup.MovePlayer(0, -1), "walk toward blocked collectible");
    }
    Require(blockedPickup.MovePlayer(-1, 0), "touch blocked collectible from adjacent square");
    Require(blockedPickup.PlayerColumn() == 7 && blockedPickup.PlayerRow() == 2
                && inventoryStock(blockedPickup) == blockedStockBefore,
            "1008:a0ca does not collect blocked treasure from an adjacent cell");
    Require(blockedPickup.MovePlayer(0, 0)
                && inventoryStock(blockedPickup) == blockedStockBefore,
            "waiting beside blocked treasure does not collect it");
    std::filesystem::remove(blockedPickupPath);

    const std::filesystem::path chestPath = gameDataDirectory / L"key-chest.VKW";
    drain_storm::GameData chestSource;
    Require(chestSource.Load(gameDataDirectory, 4) && chestSource.SaveGame(chestPath),
            "save key chest fixture");
    {
        std::fstream save(chestPath, std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        const std::uint32_t intactKeySeed = 1;
        const std::uint16_t chest[2][2] = {{0x7f, 0x81}, {0x80, 0x82}};
        save.seekp(17);
        save.write(reinterpret_cast<const char*>(&intactKeySeed), sizeof(intactKeySeed));
        for (std::uint16_t column = 0; column < 2; ++column) {
            for (std::uint16_t row = 0; row < 2; ++row) {
                save.seekp(boardOffset + static_cast<std::streamoff>(
                    ((column + 6) * drain_storm::GameData::kBoardRows + row + 6) * 2));
                save.write(reinterpret_cast<const char*>(&chest[column][row]),
                           sizeof(chest[column][row]));
            }
        }
        Require(save.good(), "place complete key chest fixture");
    }
    equipFirstInventoryItem(chestPath, 19);
    drain_storm::GameData chestBoard;
    Require(chestBoard.Load(gameDataDirectory, 4) && chestBoard.LoadGame(chestPath)
                && chestBoard.UseSelectedItem(-1, 0),
            "open common chest with Bronze Key");
    Require(chestBoard.TakeTransitionRequest() == 0,
            "opening a Bronze-Key chest does not request a map transition");
    std::size_t revealedTreasure = 0;
    std::size_t keyTurnEffects = 0;
    for (const drain_storm::BoardOverlay& overlay : chestBoard.Overlays()) {
        if (overlay.column >= 6 && overlay.column <= 7
            && overlay.row >= 6 && overlay.row <= 7
            && overlay.type > 0 && overlay.type < 0x12) {
            ++revealedTreasure;
        }
        if (overlay.column >= 6 && overlay.column <= 7
            && overlay.row >= 6 && overlay.row <= 7
            && overlay.type >= 0x19 && overlay.type <= 0x1c) {
            ++keyTurnEffects;
        }
    }
    Require(chestBoard.TileAt(6, 6) == 0 && chestBoard.TileAt(6, 7) == 0
                && chestBoard.TileAt(7, 6) == 0 && chestBoard.TileAt(7, 7) == 0
                && revealedTreasure == 4 && keyTurnEffects == 4
                && chestBoard.TakeMessage()
                    == L"The chest opens. The key remains intact!"
                && chestBoard.TakeSoundEvent() == drain_storm::GameplaySound::DoorOpen
                && std::any_of(chestBoard.Inventory().begin(), chestBoard.Inventory().end(),
                    [](const drain_storm::InventoryItem& item) {
                        return item.type == 19 && item.quantity == 1;
                    }),
            "Bronze Key opens chest, reveals four items, and preserves intact key");
    std::filesystem::remove(chestPath);

    const std::filesystem::path bronzeDoorPath =
        gameDataDirectory / L"bronze-key-door.VKW";
    drain_storm::GameData bronzeDoorSetup;
    Require(bronzeDoorSetup.Load(gameDataDirectory, 1)
                && bronzeDoorSetup.SaveGame(bronzeDoorPath),
            "save Bronze Key door fixture");
    {
        std::fstream save(bronzeDoorPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint32_t intactKeySeed = 3;
        const std::uint16_t playerColumn = 7;
        const std::uint16_t playerRow = 7;
        const std::uint16_t bronzeDoor = 0x5c;
        constexpr std::streamoff boardOffset = 130;
        save.seekp(17);
        save.write(reinterpret_cast<const char*>(&intactKeySeed), sizeof(intactKeySeed));
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        save.seekp(boardOffset + static_cast<std::streamoff>((6 * 36 + 7) * 2));
        save.write(reinterpret_cast<const char*>(&bronzeDoor), sizeof(bronzeDoor));
        Require(save.good(), "patch Bronze Key door fixture");
    }
    equipFirstInventoryItem(bronzeDoorPath, 19, 1);
    drain_storm::GameData bronzeDoorBoard;
    Require(bronzeDoorBoard.Load(gameDataDirectory, 1)
                && bronzeDoorBoard.LoadGame(bronzeDoorPath)
                && bronzeDoorBoard.UseSelectedItem(-1, 0)
                && bronzeDoorBoard.TileAt(6, 7) == 0
                && bronzeDoorBoard.TakeMessage()
                    == L"The door opens. The key remains intact!"
                && bronzeDoorBoard.TakeSoundEvent() == drain_storm::GameplaySound::Drill
                && std::any_of(bronzeDoorBoard.Inventory().begin(),
                    bronzeDoorBoard.Inventory().end(),
                    [](const drain_storm::InventoryItem& item) {
                        return item.type == 19 && item.quantity == 1;
                    }),
            "Bronze Key opens door without triggering unrelated Power Plant construction");
    std::filesystem::remove(bronzeDoorPath);

    drain_storm::GameData levelFourRuntime;
    Require(levelFourRuntime.Load(gameDataDirectory, 4), "load selector-1050 runtime board");
    const std::filesystem::path levelFourFloodPath =
        gameDataDirectory / L"selector-1050-flood.VKW";
    Require(levelFourRuntime.SaveGame(levelFourFloodPath), "save selector-1050 flood fixture");
    {
        // DAH64 v4 stores the selector-1050 countdown at offset 58. Disable this independent
        // early hazard phase so the fixture can isolate the turn-801 flood.
        std::fstream save(levelFourFloodPath,
                          std::ios::binary | std::ios::in | std::ios::out);
        const std::uint32_t expiredCountdown = 0;
        const std::uint16_t empty = 0;
        constexpr std::streamoff boardOffset = 130;
        save.seekp(58);
        save.write(reinterpret_cast<const char*>(&expiredCountdown), sizeof(expiredCountdown));
        for (std::size_t column = 0; column < drain_storm::GameData::kBoardColumns; ++column) {
            for (std::size_t row = 0; row < drain_storm::GameData::kBoardRows; ++row) {
                const std::uint16_t tile = levelFourRuntime.TileAt(column, row);
                if (tile >= 0x18 && tile <= 0x22) {
                    save.seekp(boardOffset + static_cast<std::streamoff>((column * 36 + row) * 2));
                    save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
                }
            }
        }
        save.seekg(boardOffset + static_cast<std::streamoff>(40 * 36 * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        const std::streamoff actorsOffset = save.tellg();
        const std::array<std::uint16_t, 6> stationaryActor = {0, 0, 0, 0, 0, 1};
        for (std::uint32_t actor = 0; actor < actorCount; ++actor) {
            save.seekp(actorsOffset + static_cast<std::streamoff>(actor * 12));
            save.write(reinterpret_cast<const char*>(stationaryActor.data()),
                       sizeof(stationaryActor));
        }
        Require(save.good(), "expire selector-1050 countdown fixture");
    }
    Require(levelFourRuntime.LoadGame(levelFourFloodPath), "load selector-1050 flood fixture");
    const std::size_t overlaysBeforeFlood = levelFourRuntime.Overlays().size();
    for (int turn = 0; turn < 801; ++turn) {
        Require(levelFourRuntime.UseSelectedItem(0, -1), "advance selector-1050 timer");
    }
    Require(levelFourRuntime.Overlays().size() == overlaysBeforeFlood,
            "selector-1050 flood waits beyond turn 800");
    Require(levelFourRuntime.UseSelectedItem(0, -1), "trigger selector-1050 flood");
    // Original's row = random(7) + column - 4 can fall outside the board.
    // The native port discards those writes while preserving the RNG draws.
    Require(levelFourRuntime.Overlays().size() > overlaysBeforeFlood
            && levelFourRuntime.Overlays().size() <= overlaysBeforeFlood + 24,
            "selector-1050 flood places only in-bounds hazards");
    for (std::size_t index = overlaysBeforeFlood; index < levelFourRuntime.Overlays().size(); ++index) {
        const auto& hazard = levelFourRuntime.Overlays()[index];
        Require(hazard.type == 0x16 && hazard.column >= 11 && hazard.column <= 34
                && hazard.row + 3 >= hazard.column && hazard.row <= hazard.column + 3,
                "selector-1050 flood retains original column-first hazard geometry");
    }
    Require(levelFourRuntime.Actors().empty(), "selector-1050 flood clears actors");
    std::filesystem::remove(levelFourFloodPath);

    drain_storm::GameData levelFive;
    Require(levelFive.Load(gameDataDirectory, 5), "load level five");
    Require(levelFive.TileAt(18, 18) == 0x10 && levelFive.TileAt(20, 20) == 0x17,
            "selector-1058 first guarded structure");
    Require(levelFive.TileAt(35, 3) == 0x24 && levelFive.TileAt(32, 7) == 0x27,
            "selector-1058 fixed tile mutations");
    Require(levelFive.Actors().size() >= 12, "selector-1058 stationary structure actors");
    const std::size_t firstStructureActor = levelFive.Actors().size() - 12;
    Require(levelFive.Actors()[firstStructureActor].type == 1 &&
                levelFive.Actors()[firstStructureActor].column == 18 &&
                levelFive.Actors()[firstStructureActor].row == 18,
            "selector-1058 stationary actor placement");
    Require(levelFive.Overlays().size() >= 9, "selector-1058 fixed and random overlays");

    const std::filesystem::path levelFiveSwitchPath =
        gameDataDirectory / L"level-five-switch-runtime.VKW";
    const auto saveLevelFiveAt = [&](std::uint16_t column, std::uint16_t row) {
        Require(levelFive.SaveGame(levelFiveSwitchPath), "save selector-1058 switch fixture");
        std::fstream output(levelFiveSwitchPath, std::ios::binary | std::ios::in | std::ios::out);
        output.seekp(29);
        output.write(reinterpret_cast<const char*>(&column), sizeof(column));
        output.write(reinterpret_cast<const char*>(&row), sizeof(row));
        Require(output.good(), "position selector-1058 switch fixture");
    };
    saveLevelFiveAt(36, 2);
    drain_storm::GameData levelFiveFirstSwitch;
    Require(levelFiveFirstSwitch.Load(gameDataDirectory, 5)
                && levelFiveFirstSwitch.LoadGame(levelFiveSwitchPath)
                && levelFiveFirstSwitch.MovePlayer(1, 0),
            "activate selector-1058 first floor switch");
    Require(levelFiveFirstSwitch.TileAt(31, 3) == 0x36
                && levelFiveFirstSwitch.TileAt(32, 3) == 0x3e
                && levelFiveFirstSwitch.TileAt(32, 4) == 0
                && levelFiveFirstSwitch.TileAt(32, 5) == 0x3f,
            "selector-1058 first chamber closes");
    Require(levelFiveFirstSwitch.MovePlayer(-1, 0)
                && levelFiveFirstSwitch.MovePlayer(1, 0)
                && levelFiveFirstSwitch.TileAt(31, 3) == 0x38
                && levelFiveFirstSwitch.TileAt(32, 3) == 0x3e
                && levelFiveFirstSwitch.TileAt(32, 4) == 0
                && levelFiveFirstSwitch.TileAt(32, 5) == 0x3f,
            "selector-1058 first chamber reopens");

    saveLevelFiveAt(36, 3);
    drain_storm::GameData levelFiveSecondSwitch;
    Require(levelFiveSecondSwitch.Load(gameDataDirectory, 5)
                && levelFiveSecondSwitch.LoadGame(levelFiveSwitchPath)
                && levelFiveSecondSwitch.MovePlayer(1, 0),
            "activate selector-1058 second floor switch");
    Require(levelFiveSecondSwitch.TileAt(34, 6) == 0x37
                && levelFiveSecondSwitch.TileAt(34, 7) == 0x3d
                && levelFiveSecondSwitch.TileAt(35, 7) == 0
                && levelFiveSecondSwitch.TileAt(36, 7) == 0x3c,
            "selector-1058 second chamber closes");

    Require(levelFive.SaveGame(levelFiveSwitchPath), "save selector-1058 probe fixture");
    std::uint16_t probeColumn{};
    std::uint16_t probeRow{};
    {
        std::fstream probe(levelFiveSwitchPath, std::ios::binary | std::ios::in | std::ios::out);
        probe.seekg(66);
        probe.read(reinterpret_cast<char*>(&probeColumn), sizeof(probeColumn));
        probe.read(reinterpret_cast<char*>(&probeRow), sizeof(probeRow));
        const std::uint16_t empty = 0;
        probe.seekp(29);
        probe.write(reinterpret_cast<const char*>(&probeColumn), sizeof(probeColumn));
        probe.write(reinterpret_cast<const char*>(&probeRow), sizeof(probeRow));
        constexpr std::streamoff boardOffset = 130;
        probe.seekp(boardOffset + static_cast<std::streamoff>(
            (probeColumn * drain_storm::GameData::kBoardRows + probeRow) * 2));
        probe.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
        probe.seekp(boardOffset + static_cast<std::streamoff>(
            ((probeColumn + 1) * drain_storm::GameData::kBoardRows + probeRow) * 2));
        probe.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
        Require(probe.good(), "breach selector-1058 probe fixture");
    }
    drain_storm::GameData levelFiveProbe;
    Require(levelFiveProbe.Load(gameDataDirectory, 5)
                && levelFiveProbe.LoadGame(levelFiveSwitchPath)
                && levelFiveProbe.MovePlayer(0, 0)
                && levelFiveProbe.TileAt(probeColumn + 1, probeRow) == 0x5d,
            "selector-1058 detects the breached probe pair");
    Require(levelFiveProbe.SaveGame(levelFiveSwitchPath),
            "persist selector-1058 probe activation");
    Require(probeRow > 0, "probe fixture has an adjacent entry square");
    {
        std::fstream save(levelFiveSwitchPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t entryRow = static_cast<std::uint16_t>(probeRow - 1);
        const std::uint16_t empty = 0;
        save.seekp(31);
        save.write(reinterpret_cast<const char*>(&entryRow), sizeof(entryRow));
        constexpr std::streamoff boardOffset = 130;
        save.seekp(boardOffset + static_cast<std::streamoff>(
            (probeColumn * drain_storm::GameData::kBoardRows + entryRow) * 2));
        save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
        Require(save.good(), "prepare movement into selector-1058 breached probe");
    }
    drain_storm::GameData restoredLevelFiveProbe;
    Require(restoredLevelFiveProbe.Load(gameDataDirectory, 5)
                && restoredLevelFiveProbe.LoadGame(levelFiveSwitchPath)
                && restoredLevelFiveProbe.MovePlayer(0, 1)
                && restoredLevelFiveProbe.TileAt(28, 10) == 59
                && restoredLevelFiveProbe.TileAt(29, 10) == 0
                && restoredLevelFiveProbe.TileAt(32, 9) == 59,
            "selector-1058 activates and persists the seventeen-cell mechanism");
    std::filesystem::remove(levelFiveSwitchPath);

    drain_storm::GameData levelSix;
    Require(levelSix.Load(gameDataDirectory, 6), "load level six");
    Require(levelSix.TileAt(10, 16) == 0x62 && levelSix.TileAt(10, 17) == 0x59 &&
                levelSix.TileAt(10, 18) == 0x62,
            "selector-1060 central tile mutations");
    Require(levelSix.TileAt(17, 14) == 0x10 && levelSix.TileAt(19, 16) == 0x17,
            "selector-1060 guarded structure");
    Require(levelSix.TileAt(5, 5) == 0x23 && levelSix.TileAt(5, 31) == 0x28 &&
                levelSix.TileAt(33, 3) == 0x27,
            "selector-1060 exits");
    Require(levelSix.Actors().size() >= 60, "selector-1060 repeated random actors");
    const std::size_t firstSelector1060Actor = levelSix.Actors().size() - 60;
    Require(levelSix.Actors()[firstSelector1060Actor].type == 4 &&
                levelSix.Actors()[firstSelector1060Actor].column == 5 &&
                levelSix.Actors()[firstSelector1060Actor].row == 10 &&
                levelSix.Actors()[firstSelector1060Actor].behavior == 2,
            "selector-1060 actor record");
    const std::filesystem::path moonKeyPath =
        gameDataDirectory / L"selector-1060-moon-key.VKW";
    Require(levelSix.SaveGame(moonKeyPath), "save selector-1060 Moon Key fixture");
    {
        std::fstream save(moonKeyPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 9;
        const std::uint16_t playerRow = 17;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        Require(save.good(), "position selector-1060 Moon Key fixture");
    }
    equipFirstInventoryItem(moonKeyPath, 17);
    drain_storm::GameData moonKeyBoard;
    Require(moonKeyBoard.Load(gameDataDirectory, 6) && moonKeyBoard.LoadGame(moonKeyPath)
                && moonKeyBoard.UseSelectedItem(1, 0),
            "use Moon Key on selector-1060 center door");
    Require(moonKeyBoard.TileAt(10, 16) == 0 && moonKeyBoard.TileAt(10, 17) == 0
                && moonKeyBoard.TileAt(10, 18) == 0
                && moonKeyBoard.TakeMessage() == L"The key turns... The door opens."
                && moonKeyBoard.TakeSoundEvent()
                    == drain_storm::GameplaySound::SpecialDoorOpen,
            "selector-1060 Moon Key opens the complete recovered doorway");
    std::filesystem::remove(moonKeyPath);
    const std::filesystem::path levelSixRuntimePath =
        gameDataDirectory / L"selector-1060-runtime.VKW";
    Require(levelSix.SaveGame(levelSixRuntimePath), "save selector-1060 runtime fixture");
    {
        std::fstream save(levelSixRuntimePath,
                          std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        const std::uint16_t empty = 0;
        const auto clearTile = [&](std::uint16_t column, std::uint16_t row) {
            save.seekp(boardOffset + static_cast<std::streamoff>(
                (column * drain_storm::GameData::kBoardRows + row) * 2));
            save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
        };
        clearTile(18, 11);
        clearTile(20, 22);
        clearTile(10, 17);
        Require(save.good(), "breach selector-1060 runtime objects");
    }
    drain_storm::GameData levelSixRuntime;
    Require(levelSixRuntime.Load(gameDataDirectory, 6)
                && levelSixRuntime.LoadGame(levelSixRuntimePath),
            "load selector-1060 runtime fixture");
    const std::size_t levelSixActorsBeforeRuntime = levelSixRuntime.Actors().size();
    Require(levelSixRuntime.UseSelectedItem(0, -1), "advance selector-1060 runtime");
    Require(levelSixRuntime.Actors().size() > levelSixActorsBeforeRuntime
                && levelSixRuntime.TileAt(18, 15) == 0x1b
                && levelSixRuntime.TileAt(18, 20) == 0x1b,
            "selector-1060 reacts to both transporters and opens its center machinery");
    Require(levelSixRuntime.SaveGame(levelSixRuntimePath),
            "persist selector-1060 one-shot flags");
    {
        // Isolate the one-shot flags from FUN_1008_81dc's now-restored
        // weapon-turn structure spawning on the following action.
        std::fstream save(levelSixRuntimePath,
                          std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        const std::uint16_t empty = 0;
        for (std::size_t column = 0; column < drain_storm::GameData::kBoardColumns; ++column) {
            for (std::size_t row = 0; row < drain_storm::GameData::kBoardRows; ++row) {
                const std::uint16_t tile = levelSixRuntime.TileAt(column, row);
                if (tile >= 0x18 && tile <= 0x22) {
                    save.seekp(boardOffset + static_cast<std::streamoff>((column * 36 + row) * 2));
                    save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
                }
            }
        }
        Require(save.good(), "isolate selector-1060 one-shot flags");
    }
    drain_storm::GameData restoredLevelSixRuntime;
    Require(restoredLevelSixRuntime.Load(gameDataDirectory, 6)
                && restoredLevelSixRuntime.LoadGame(levelSixRuntimePath),
            "restore selector-1060 runtime flags");
    const std::size_t restoredLevelSixActors = restoredLevelSixRuntime.Actors().size();
    Require(restoredLevelSixRuntime.UseSelectedItem(0, -1)
                && restoredLevelSixRuntime.Actors().size() == restoredLevelSixActors,
            "selector-1060 object triggers remain one-shot after save restore");
    std::filesystem::remove(levelSixRuntimePath);

    drain_storm::GameData levelSeven;
    Require(levelSeven.Load(gameDataDirectory, 7), "load level seven");
    Require(levelSeven.TileAt(19, 18) == 0x70 && levelSeven.TileAt(21, 20) == 0x78,
            "selector-1068 central device");
    Require(levelSeven.TileAt(8, 7) == 0x10 && levelSeven.TileAt(32, 31) == 0x17,
            "selector-1068 corner structures");
    Require(levelSeven.TileAt(18, 30) == 0x43 && levelSeven.TileAt(19, 30) == 0x41 &&
                levelSeven.TileAt(20, 33) == 0x28,
            "selector-1068 lower gate");
    Require(levelSeven.TileAt(30, 1) == 8 && levelSeven.TileAt(32, 3) == 0x0f &&
                levelSeven.TileAt(2, 27) == 0x27,
            "selector-1068 exit structure");
    Require(levelSeven.Actors().size() >= 27, "selector-1068 randomized actor groups");
    const std::size_t firstSelector1068Actor = levelSeven.Actors().size() - 27;
    Require(levelSeven.Actors()[firstSelector1068Actor].type == 0 &&
                levelSeven.Actors()[firstSelector1068Actor].column == 18 &&
                levelSeven.Actors()[firstSelector1068Actor].row == 17 &&
                levelSeven.Actors()[firstSelector1068Actor + 3].column == 18 &&
                levelSeven.Actors()[firstSelector1068Actor + 4].type == 1,
            "selector-1068 recovered RNG actor counts");

    const std::filesystem::path emeraldPath =
        gameDataDirectory / L"selector-1068-emerald.VKW";
    Require(levelSeven.SaveGame(emeraldPath), "save selector-1068 Emerald fixture");
    {
        std::fstream save(emeraldPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 18;
        const std::uint16_t playerRow = 18;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        Require(save.good(), "position selector-1068 Emerald fixture");
    }
    equipFirstInventoryItem(emeraldPath, 20);
    drain_storm::GameData emeraldBoard;
    Require(emeraldBoard.Load(gameDataDirectory, 7) && emeraldBoard.LoadGame(emeraldPath)
                && emeraldBoard.UseSelectedItem(1, 0),
            "use Emerald on selector-1068 central idol");
    Require(emeraldBoard.TileAt(19, 18) == 0x79
                && emeraldBoard.TileAt(20, 18) == 0x7a
                && emeraldBoard.TileAt(19, 19) == 0x7b
                && emeraldBoard.TileAt(20, 19) == 0x7c
                && emeraldBoard.TileAt(18, 30) == 0
                && emeraldBoard.TileAt(22, 30) == 0
                && emeraldBoard.Score() == 3000,
            "selector-1068 Emerald repairs the idol, opens its gate, and awards score");
    Require(emeraldBoard.TakeMessage().find(L"repaired the idol") != std::wstring::npos,
            "selector-1068 Emerald reports both recovered messages");
    std::filesystem::remove(emeraldPath);

    drain_storm::GameData levelSevenRuntime;
    Require(levelSevenRuntime.Load(gameDataDirectory, 7), "load selector-1068 runtime board");
    for (int step = 0; step < 4; ++step) {
        Require(levelSevenRuntime.MovePlayer(-1, 0), "approach selector-1068 structure");
    }
    for (int selection = 0; selection < 7; ++selection) {
        Require(levelSevenRuntime.SelectNextItem(), "select selector-1068 radio bomb");
    }
    Require(levelSevenRuntime.UseSelectedItem(-1, 0), "place selector-1068 radio bomb");
    Require(levelSevenRuntime.SelectNextItem(), "select selector-1068 detonator");
    Require(levelSevenRuntime.UseSelectedItem(-1, 0), "detonate selector-1068 radio bomb");
    Require(levelSevenRuntime.TileAt(31, 8) == 0,
            "selector-1068 runtime observes destroyed guarded structure");

    drain_storm::GameData levelEight;
    Require(levelEight.Load(gameDataDirectory, 8), "load procedural board");
    Require(levelEight.PlayerColumn() == 20 && levelEight.PlayerRow() == 7 &&
                levelEight.TileAt(20, 7) == 0x23,
            "procedural board start square");
    Require(levelEight.TileAt(12, 12) == 0x7f && levelEight.TileAt(13, 13) == 0x82 &&
                levelEight.TileAt(27, 22) == 0x7f && levelEight.TileAt(28, 23) == 0x82,
            "procedural board transporter grid");
    Require(levelEight.TileAt(10, 10) == 0 && levelEight.TileAt(30, 25) == 0,
            "procedural board central clearing");
    Require(levelEight.Actors().empty() && levelEight.Overlays().empty(),
            "procedural board initial dynamic state");
    const std::filesystem::path levelEightAlarmPath =
        gameDataDirectory / L"selector-c80e-alarm.VKW";
    Require(levelEight.SaveGame(levelEightAlarmPath), "save selector-c80e alarm fixture");
    {
        std::fstream save(levelEightAlarmPath,
                          std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        const std::uint16_t breached = 0;
        save.seekp(boardOffset + static_cast<std::streamoff>(
            (12 * drain_storm::GameData::kBoardRows + 12) * 2));
        save.write(reinterpret_cast<const char*>(&breached), sizeof(breached));
        Require(save.good(), "breach selector-c80e transporter fixture");
    }
    drain_storm::GameData levelEightAlarm;
    Require(levelEightAlarm.Load(gameDataDirectory, 8)
                && levelEightAlarm.LoadGame(levelEightAlarmPath)
                && levelEightAlarm.UseSelectedItem(0, -1),
            "trigger selector-c80e alarm runtime");
    const auto countLevelEightStructures = [](const drain_storm::GameData& board) {
        int count = 0;
        for (std::size_t column = 0; column < drain_storm::GameData::kBoardColumns; ++column) {
            for (std::size_t row = 0; row < drain_storm::GameData::kBoardRows; ++row) {
                count += board.TileAt(column, row) == 0x18;
            }
        }
        return count;
    };
    Require(countLevelEightStructures(levelEightAlarm) == 12
                && levelEightAlarm.TakeMessage() == L"An alarm sounds! You hear explosions.",
            "selector-c80e alarm constructs twelve guarded objects and reports exact message");
    Require(levelEightAlarm.SaveGame(levelEightAlarmPath),
            "persist selector-c80e alarm state");
    drain_storm::GameData restoredLevelEightAlarm;
    Require(restoredLevelEightAlarm.Load(gameDataDirectory, 8)
                && restoredLevelEightAlarm.LoadGame(levelEightAlarmPath)
                && restoredLevelEightAlarm.UseSelectedItem(0, -1)
                && countLevelEightStructures(restoredLevelEightAlarm) == 12
                && restoredLevelEightAlarm.TakeMessage().empty(),
            "selector-c80e alarm remains one-shot after save restore");
    std::filesystem::remove(levelEightAlarmPath);

    drain_storm::GameData levelNine;
    Require(levelNine.Load(gameDataDirectory, 9), "load selector-1078 map");
    Require(levelNine.TileAt(31, 32) == 8 && levelNine.TileAt(33, 34) == 0x0f,
            "selector-1078 asymmetric structure");
    Require(levelNine.TileAt(5, 5) == 0x10 && levelNine.TileAt(7, 7) == 0x17,
            "selector-1078 guarded structure");
    const std::filesystem::path levelNineWavePath =
        gameDataDirectory / L"selector-1078-wave.VKW";
    Require(levelNine.SaveGame(levelNineWavePath), "save selector-1078 wave fixture");
    {
        std::fstream save(levelNineWavePath,
                          std::ios::binary | std::ios::in | std::ios::out);
        const std::uint32_t waveTurn = 5;
        save.seekp(21);
        save.write(reinterpret_cast<const char*>(&waveTurn), sizeof(waveTurn));
        Require(save.good(), "set selector-1078 first wave turn");
    }
    drain_storm::GameData levelNineWave;
    Require(levelNineWave.Load(gameDataDirectory, 9) && levelNineWave.LoadGame(levelNineWavePath),
            "load selector-1078 wave fixture");
    const std::size_t levelNineActorsBeforeWave = levelNineWave.Actors().size();
    Require(levelNineWave.UseSelectedItem(0, -1)
                && levelNineWave.Actors().size() >= levelNineActorsBeforeWave + 9
                && levelNineWave.Actors().back().type == 0
                && levelNineWave.Actors().back().column == 32
                && levelNineWave.Actors().back().row == 33,
            "selector-1078 first timed actor wave");
    std::filesystem::remove(levelNineWavePath);

    drain_storm::GameData levelTen;
    Require(levelTen.Load(gameDataDirectory, 10), "load selector-33b4 procedural board");
    Require(levelTen.TileAt(10, 20) == 0x83 && levelTen.TileAt(11, 20) == 0x84 &&
                levelTen.TileAt(10, 21) == 0x85 && levelTen.TileAt(11, 21) == 0x86,
            "selector-33b4 central device");
    Require(levelTen.TileAt(4, 19) == 0x7e,
            "selector-33b4 sign placement");
    const std::filesystem::path levelTenSignPath =
        gameDataDirectory / L"selector-33b4-sign.VKW";
    Require(levelTen.SaveGame(levelTenSignPath), "save selector-33b4 sign fixture");
    {
        std::fstream save(levelTenSignPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 4;
        const std::uint16_t playerRow = 18;
        const std::uint16_t empty = 0;
        constexpr std::streamoff boardOffset = 130;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        save.seekp(boardOffset + static_cast<std::streamoff>((4 * 36 + 18) * 2));
        save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
        Require(save.good(), "position selector-33b4 sign fixture");
    }
    drain_storm::GameData levelTenSign;
    Require(levelTenSign.Load(gameDataDirectory, 10)
                && levelTenSign.LoadGame(levelTenSignPath)
                && levelTenSign.MovePlayer(0, 1)
                && levelTenSign.TakeMessage()
                    == L"You see here a signpost.\n\nIt reads:"
                && levelTenSign.TakeMessage() == L"The Xoid Fortress.",
            "FUN_1008_ba30 queues both selector-33b4 sign notices");
    drain_storm::ApplicationState levelTenSignHistory;
    Require(levelTenSignHistory.LoadOriginalResources(executablePath)
                && levelTenSignHistory.LoadGame(levelTenSignPath)
                && levelTenSignHistory.MovePlayer(0, 1)
                && levelTenSignHistory.TakeGameplayMessage()
                    == L"You see here a signpost.\n\nIt reads:"
                && levelTenSignHistory.NavigateMessageHistory(false)
                    == L"The Xoid Fortress.",
            "selector-33b4 sign sequence advances through M in the arsenal history");
    std::filesystem::remove(levelTenSignPath);
    Require(levelTen.TileAt(5, 15) == 0x1f && levelTen.TileAt(16, 15) == 0x1f &&
                levelTen.TileAt(16, 26) == 0x1f && levelTen.TileAt(5, 26) == 0x1f,
            "selector-33b4 guarded structures");
    Require(levelTen.TileAt(6, 24) == 0 && levelTen.TileAt(15, 24) == 0,
            "selector-33b4 chamber uses recovered tall clearing rectangle");
    Require(levelTen.TileAt(9, 15) == 0 && levelTen.TileAt(12, 15) == 0,
            "selector-33b4 upper passage uses recovered horizontal clearing line");
    Require(levelTen.Overlays().size() == 15,
            "selector-33b4 randomized and fixed overlays");
    Require(levelTen.PlayerColumn() >= 26 && levelTen.PlayerColumn() <= 35 &&
                levelTen.PlayerRow() >= 1 && levelTen.PlayerRow() <= 34 &&
                levelTen.TileAt(levelTen.PlayerColumn(), levelTen.PlayerRow()) == 0x23,
            "selector-33b4 randomized start square");

    const auto inventoryQuantity = [](const drain_storm::GameData& board,
                                      std::uint16_t type) {
        std::uint32_t quantity = 0;
        for (const drain_storm::InventoryItem& item : board.Inventory()) {
            if (item.type == type) quantity += item.quantity;
        }
        return quantity;
    };
    const std::filesystem::path levelTenEmeraldPath =
        gameDataDirectory / L"selector-33b4-emerald.VKW";
    Require(levelTen.SaveGame(levelTenEmeraldPath), "save selector-33b4 emerald fixture");
    {
        std::fstream save(levelTenEmeraldPath,
                          std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 10;
        const std::uint16_t playerRow = 20;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        Require(save.good(), "position vessel inside selector-33b4 emerald device");
    }
    drain_storm::GameData levelTenEmerald;
    Require(levelTenEmerald.Load(gameDataDirectory, 10)
                && levelTenEmerald.LoadGame(levelTenEmeraldPath),
            "load selector-33b4 emerald fixture");
    const std::uint32_t emeraldsBefore = inventoryQuantity(levelTenEmerald, 20);
    Require(levelTenEmerald.UseSelectedItem(0, -1)
                && levelTenEmerald.TileAt(10, 20) == 0
                && levelTenEmerald.TileAt(11, 20) == 0
                && levelTenEmerald.TileAt(10, 21) == 0
                && levelTenEmerald.TileAt(11, 21) == 0
                && inventoryQuantity(levelTenEmerald, 20) == emeraldsBefore + 1
                && levelTenEmerald.TakeMessage() == L"You pick up the Giant Emerald!",
            "FUN_1010_37a2 clears the central device and grants one Emerald");
    Require(levelTenEmerald.SaveGame(levelTenEmeraldPath),
            "persist selector-33b4 emerald one-shot phase");
    drain_storm::GameData levelTenEmeraldReloaded;
    Require(levelTenEmeraldReloaded.Load(gameDataDirectory, 10)
                && levelTenEmeraldReloaded.LoadGame(levelTenEmeraldPath)
                && levelTenEmeraldReloaded.UseSelectedItem(0, -1)
                && inventoryQuantity(levelTenEmeraldReloaded, 20) == emeraldsBefore + 1
                && levelTenEmeraldReloaded.TakeMessage().empty(),
            "selector-33b4 Emerald phase remains one-shot after reload");
    std::filesystem::remove(levelTenEmeraldPath);

    const std::filesystem::path levelTenBarrierPath =
        gameDataDirectory / L"selector-33b4-barrier.VKW";
    Require(levelTen.SaveGame(levelTenBarrierPath), "save selector-33b4 barrier fixture");
    {
        std::fstream save(levelTenBarrierPath,
                          std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        const std::uint16_t empty = 0;
        const std::uint16_t closedBarrier = 0x20;
        save.seekp(boardOffset + static_cast<std::streamoff>((5 * 36 + 15) * 2));
        save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
        save.seekp(boardOffset + static_cast<std::streamoff>((9 * 36 + 19) * 2));
        save.write(reinterpret_cast<const char*>(&closedBarrier), sizeof(closedBarrier));
        Require(save.good(), "breach selector-33b4 corner sentinel");
    }
    drain_storm::GameData levelTenBarrier;
    Require(levelTenBarrier.Load(gameDataDirectory, 10)
                && levelTenBarrier.LoadGame(levelTenBarrierPath),
            "load selector-33b4 barrier fixture");
    std::size_t innerBarrierBefore = 0;
    for (std::size_t column = 9; column <= 12; ++column) {
        for (std::size_t row = 19; row <= 22; ++row) {
            innerBarrierBefore += levelTenBarrier.TileAt(column, row) == 0x20 ? 1u : 0u;
        }
    }
    Require(innerBarrierBefore == 1 && levelTenBarrier.UseSelectedItem(0, -1),
            "trigger selector-33b4 breached-sentinel phase");
    bool sealsRemain = false;
    std::size_t convertedBarrier = 0;
    for (std::size_t column = 0; column < drain_storm::GameData::kBoardColumns; ++column) {
        for (std::size_t row = 0; row < drain_storm::GameData::kBoardRows; ++row) {
            sealsRemain = sealsRemain || levelTenBarrier.TileAt(column, row) == 0x21;
            if (column >= 9 && column <= 12 && row >= 19 && row <= 22) {
                convertedBarrier += levelTenBarrier.TileAt(column, row) == 0x22 ? 1u : 0u;
            }
        }
    }
    Require(!sealsRemain && convertedBarrier >= innerBarrierBefore,
            "FUN_1010_37a2 removes fortress seals and converts its inner barrier");
    Require(levelTenBarrier.SaveGame(levelTenBarrierPath),
            "persist selector-33b4 barrier one-shot phase");
    {
        std::fstream save(levelTenBarrierPath,
                          std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        const std::uint16_t diagnosticSeal = 0x21;
        save.seekp(boardOffset);
        save.write(reinterpret_cast<const char*>(&diagnosticSeal), sizeof(diagnosticSeal));
        Require(save.good(), "inject post-phase selector-33b4 diagnostic seal");
    }
    drain_storm::GameData levelTenBarrierReloaded;
    Require(levelTenBarrierReloaded.Load(gameDataDirectory, 10)
                && levelTenBarrierReloaded.LoadGame(levelTenBarrierPath)
                && levelTenBarrierReloaded.UseSelectedItem(0, -1)
                && levelTenBarrierReloaded.TileAt(0, 0) == 0x21,
            "selector-33b4 barrier phase remains one-shot after reload");
    std::filesystem::remove(levelTenBarrierPath);

    drain_storm::GameData levelEleven;
    Require(levelEleven.Load(gameDataDirectory, 11), "load selector-cb9c procedural board");
    Require(!levelEleven.Overlays().empty() && levelEleven.Overlays().size() <= 4 &&
                std::all_of(levelEleven.Overlays().begin(), levelEleven.Overlays().end(),
                            [](const drain_storm::BoardOverlay& overlay) {
                                return overlay.type == 2 || overlay.type == 7;
                            }),
            "selector-cb9c randomized pickups");
    Require(levelEleven.PlayerColumn() >= 1 && levelEleven.PlayerColumn() <= 37 &&
                levelEleven.PlayerRow() >= 1 && levelEleven.PlayerRow() <= 33 &&
                levelEleven.TileAt(levelEleven.PlayerColumn(), levelEleven.PlayerRow()) == 0x23 &&
                levelEleven.TileAt(levelEleven.PlayerColumn() + 2,
                                   levelEleven.PlayerRow() + 2) == 0,
            "selector-cb9c generic entry reserves adjacent square without sign");
    drain_storm::GameData signedLevelEleven;
    Require(signedLevelEleven.Load(gameDataDirectory, 11, 1, 1, 0, true)
        && signedLevelEleven.TileAt(signedLevelEleven.PlayerColumn() + 2,
            signedLevelEleven.PlayerRow() + 2) == 0x7e,
        "selector-cb9c hub entry adds introductory sign");
    Require(levelEleven.PositionPlayerAtTile(0x27), "selector-cb9c generated exit");
    const auto countGuardedCenters = [](const drain_storm::GameData& board) {
        int count = 0;
        for (std::size_t column = 0; column < drain_storm::GameData::kBoardColumns; ++column) {
            for (std::size_t row = 0; row < drain_storm::GameData::kBoardRows; ++row) {
                count += board.TileAt(column, row) == 0x18;
            }
        }
        return count;
    };
    const std::filesystem::path levelElevenRuntimePath =
        gameDataDirectory / L"selector-cb9c-runtime.VKW";
    Require(levelEleven.SaveGame(levelElevenRuntimePath),
            "save selector-cb9c runtime fixture");
    {
        std::fstream save(levelElevenRuntimePath,
                          std::ios::binary | std::ios::in | std::ios::out);
        const std::uint32_t structureTurn = 490;
        save.seekp(21);
        save.write(reinterpret_cast<const char*>(&structureTurn), sizeof(structureTurn));
        Require(save.good(), "set selector-cb9c structure threshold");
    }
    drain_storm::GameData levelElevenRuntime;
    Require(levelElevenRuntime.Load(gameDataDirectory, 11)
                && levelElevenRuntime.LoadGame(levelElevenRuntimePath),
            "load selector-cb9c runtime fixture");
    const int centersBeforeLevelElevenRuntime = countGuardedCenters(levelElevenRuntime);
    const std::size_t overlaysBeforeLevelElevenRuntime = levelElevenRuntime.Overlays().size();
    Require(levelElevenRuntime.UseSelectedItem(0, -1)
                && countGuardedCenters(levelElevenRuntime) == centersBeforeLevelElevenRuntime + 1
                && levelElevenRuntime.Overlays().size() == overlaysBeforeLevelElevenRuntime + 9,
            "selector-cb9c turn-490 structure escalation");
    Require(levelElevenRuntime.SaveGame(levelElevenRuntimePath),
            "persist selector-cb9c runtime phases");
    drain_storm::GameData restoredLevelElevenRuntime;
    Require(restoredLevelElevenRuntime.Load(gameDataDirectory, 11)
                && restoredLevelElevenRuntime.LoadGame(levelElevenRuntimePath),
            "restore selector-cb9c runtime phases");
    const int restoredLevelElevenCenters = countGuardedCenters(restoredLevelElevenRuntime);
    Require(restoredLevelElevenRuntime.UseSelectedItem(0, -1)
                && countGuardedCenters(restoredLevelElevenRuntime)
                    == restoredLevelElevenCenters,
            "selector-cb9c structure phase remains one-shot after save restore");
    std::filesystem::remove(levelElevenRuntimePath);

    drain_storm::GameData levelTwelve;
    Require(levelTwelve.Load(gameDataDirectory, 12), "load final arena class");
    Require(levelTwelve.PlayerColumn() == 15 && levelTwelve.PlayerRow() == 33 &&
                levelTwelve.TileAt(15, 33) == 0x23 && levelTwelve.TileAt(30, 33) == 0x27,
            "final arena connection squares");
    Require(levelTwelve.TileAt(24, 33) == 0x7e && levelTwelve.TileAt(1, 0) == 0x38 &&
                levelTwelve.TileAt(38, 35) == 0x3b,
            "final arena sign and patterned perimeter");
    Require(levelTwelve.TileAt(14, 32) == 0x3f && levelTwelve.TileAt(16, 33) == 0x3a &&
                levelTwelve.TileAt(23, 33) == 0x3b && levelTwelve.TileAt(31, 32) == 0x3f,
            "final arena framed connections");
    Require(levelTwelve.TileAt(3, 32) == 0x80 && levelTwelve.TileAt(20, 33) == 0x82 &&
                levelTwelve.TileAt(36, 32) == 0x7f,
            "final arena transporter row");
    Require(levelTwelve.Overlays().size() == 13 && levelTwelve.Overlays()[0].type == 0x0e &&
                std::count_if(levelTwelve.Overlays().begin() + 1,
                              levelTwelve.Overlays().end(),
                              [](const drain_storm::BoardOverlay& overlay) {
                                  return overlay.type == 1 || overlay.type == 2;
                              }) == 12,
            "final arena randomized overlay grid");

    drain_storm::GameData mazeStageOne;
    Require(mazeStageOne.Load(gameDataDirectory, 13), "load maze stage one");
    Require(mazeStageOne.TileAt(mazeStageOne.PlayerColumn(), mazeStageOne.PlayerRow()) == 0x23 &&
                mazeStageOne.PositionPlayerAtTile(0x27) && mazeStageOne.PositionPlayerAtTile(0x28),
            "maze stage-one connection squares");
    Require(mazeStageOne.Actors().size() == 6 && mazeStageOne.Overlays().size() >= 10,
            "maze stage-one actors and overlays");
    Require(mazeStageOne.TileAt(14, 14) == 8 && mazeStageOne.TileAt(15, 15) == 0x1a &&
                mazeStageOne.TileAt(22, 22) == 0x0f,
            "maze stage-one central guarded structures");
    const auto mazeHasRoute = [](const drain_storm::GameData& board,
                                 std::uint16_t destinationTile) {
        std::array<std::array<bool, drain_storm::GameData::kBoardRows>,
                   drain_storm::GameData::kBoardColumns> visited{};
        std::deque<std::pair<std::size_t, std::size_t>> pending;
        pending.emplace_back(board.PlayerColumn(), board.PlayerRow());
        visited[board.PlayerColumn()][board.PlayerRow()] = true;
        constexpr std::array<std::pair<int, int>, 4> directions = {
            std::pair{-1, 0}, std::pair{1, 0}, std::pair{0, -1}, std::pair{0, 1}};
        while (!pending.empty()) {
            const auto [column, row] = pending.front();
            pending.pop_front();
            if (board.TileAt(column, row) == destinationTile) return true;
            for (const auto [deltaColumn, deltaRow] : directions) {
                const int nextColumn = static_cast<int>(column) + deltaColumn;
                const int nextRow = static_cast<int>(row) + deltaRow;
                if (nextColumn < 0 || nextRow < 0
                    || nextColumn >= static_cast<int>(drain_storm::GameData::kBoardColumns)
                    || nextRow >= static_cast<int>(drain_storm::GameData::kBoardRows)
                    || visited[static_cast<std::size_t>(nextColumn)][static_cast<std::size_t>(nextRow)]) {
                    continue;
                }
                const std::uint16_t tile = board.TileAt(static_cast<std::size_t>(nextColumn),
                                                        static_cast<std::size_t>(nextRow));
                if (tile != 0 && tile != 0x23 && tile != 0x27 && tile != 0x28) continue;
                visited[static_cast<std::size_t>(nextColumn)][static_cast<std::size_t>(nextRow)] = true;
                pending.emplace_back(static_cast<std::size_t>(nextColumn),
                                     static_cast<std::size_t>(nextRow));
            }
        }
        return false;
    };
    Require(mazeHasRoute(mazeStageOne, 0x27), "maze stage-one exit remains reachable");

    drain_storm::GameData mazeRuntimeSource;
    Require(mazeRuntimeSource.Load(gameDataDirectory, 13), "load maze runtime source");
    const std::uint16_t mazeRuntimeColumn = static_cast<std::uint16_t>(
        mazeRuntimeSource.PlayerColumn() + 1);
    const std::uint16_t mazeRuntimeRow = static_cast<std::uint16_t>(
        mazeRuntimeSource.PlayerRow());
    const std::filesystem::path mazeRuntimePath =
        gameDataDirectory / L"maze-runtime.VKW";
    Require(mazeRuntimeSource.SaveGame(mazeRuntimePath), "save maze runtime fixture");
    {
        std::fstream save(mazeRuntimePath, std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        const std::uint16_t dormantWall = 0x2b;
        save.seekp(boardOffset + static_cast<std::streamoff>(
            (mazeRuntimeColumn * drain_storm::GameData::kBoardRows + mazeRuntimeRow) * 2));
        save.write(reinterpret_cast<const char*>(&dormantWall), sizeof(dormantWall));
        Require(save.good(), "seed maze runtime wall fixture");
    }
    drain_storm::GameData mazeRuntime;
    Require(mazeRuntime.Load(gameDataDirectory, 13) && mazeRuntime.LoadGame(mazeRuntimePath)
                && mazeRuntime.UseSelectedItem(0, -1),
            "advance maze class runtime");
    Require(mazeRuntime.TileAt(mazeRuntimeColumn, mazeRuntimeRow) == 0x36,
            "maze runtime activates adjacent wall forms through FUN_1010_8f56");
    std::filesystem::remove(mazeRuntimePath);

    drain_storm::GameData mazeStageThree;
    Require(mazeStageThree.Load(gameDataDirectory, 15), "load maze stage three");
    Require(mazeStageThree.TileAt(mazeStageThree.PlayerColumn(), mazeStageThree.PlayerRow()) == 0x23
                && mazeStageThree.PositionPlayerAtTile(0x27),
            "maze stage-three connection squares");

    drain_storm::GameData convertedArena;
    Require(convertedArena.Load(gameDataDirectory, 16), "load converted-terrain arena");
    Require(convertedArena.PlayerColumn() == 8 && convertedArena.PlayerRow() == 8 &&
                convertedArena.TileAt(8, 8) == 0x23 && convertedArena.TileAt(6, 6) == 0x7e,
            "converted-terrain arena entry chamber");
    Require(convertedArena.TileAt(6, 28) == 8 && convertedArena.TileAt(7, 29) == 0x18 &&
                convertedArena.TileAt(34, 8) == 0x0f,
            "converted-terrain arena guarded chambers");
    Require(convertedArena.Actors().size() == 1 && convertedArena.Actors()[0].type == 3 &&
                convertedArena.Actors()[0].column >= 11 &&
                convertedArena.Actors()[0].column <= 38 &&
                convertedArena.Actors()[0].row >= 11 && convertedArena.Actors()[0].row <= 34,
            "converted-terrain arena actor");
    std::size_t convertedPassages{};
    for (std::size_t column = 0; column < drain_storm::GameData::kBoardColumns; ++column)
        for (std::size_t row = 0; row < drain_storm::GameData::kBoardRows; ++row)
            convertedPassages += convertedArena.TileAt(column, row) == 0x5e ? 1u : 0u;
    Require(convertedPassages > 100, "converted-terrain arena preserves alien maze passages");

    const std::filesystem::path convertedArenaRuntimePath =
        gameDataDirectory / L"selector-3a96-runtime.VKW";
    Require(convertedArena.SaveGame(convertedArenaRuntimePath),
            "save converted-terrain runtime fixture");
    {
        std::fstream save(convertedArenaRuntimePath,
                          std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        const std::uint16_t empty = 0;
        const std::uint16_t dormantPassage = 0x5e;
        const auto writeTile = [&](std::size_t column, std::size_t row,
                                   const std::uint16_t& tile) {
            save.seekp(boardOffset + static_cast<std::streamoff>(
                (column * drain_storm::GameData::kBoardRows + row) * 2));
            save.write(reinterpret_cast<const char*>(&tile), sizeof(tile));
        };
        writeTile(33, 7, empty);
        writeTile(33, 29, empty);
        writeTile(7, 29, empty);
        writeTile(30, 5, dormantPassage);
        writeTile(30, 30, dormantPassage);
        writeTile(5, 30, dormantPassage);
        Require(save.good(), "breach all three selector-3a96 chamber sentinels");
    }
    drain_storm::GameData convertedArenaRuntime;
    Require(convertedArenaRuntime.Load(gameDataDirectory, 16)
                && convertedArenaRuntime.LoadGame(convertedArenaRuntimePath)
                && convertedArenaRuntime.UseSelectedItem(0, -1),
            "advance selector-3a96 chamber runtime");
    const std::size_t releasedCoreActors = static_cast<std::size_t>(std::count_if(
        convertedArenaRuntime.Actors().begin(), convertedArenaRuntime.Actors().end(),
        [](const drain_storm::BoardActor& actor) {
            return actor.type == 2 && actor.column == 20 && actor.row == 18;
        }));
    Require(convertedArenaRuntime.TileAt(30, 5) == 0
                && convertedArenaRuntime.TileAt(30, 30) == 0
                && convertedArenaRuntime.TileAt(5, 30) == 0
                && convertedArenaRuntime.TileAt(20, 16) == 0x5c
                && releasedCoreActors >= 4 && releasedCoreActors <= 6
                && std::any_of(convertedArenaRuntime.Overlays().begin(),
                               convertedArenaRuntime.Overlays().end(),
                    [](const drain_storm::BoardOverlay& overlay) {
                        return overlay.type == 4 && overlay.column == 20 && overlay.row == 18;
                    }),
            "FUN_1010_3d50 opens three quadrants and activates the alien core");
    Require(convertedArenaRuntime.SaveGame(convertedArenaRuntimePath),
            "persist selector-3a96 runtime phases");
    {
        std::fstream save(convertedArenaRuntimePath,
                          std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        const std::uint16_t dormantPassage = 0x5e;
        save.seekp(boardOffset + static_cast<std::streamoff>((30 * 36 + 5) * 2));
        save.write(reinterpret_cast<const char*>(&dormantPassage), sizeof(dormantPassage));
        Require(save.good(), "inject post-phase selector-3a96 diagnostic passage");
    }
    drain_storm::GameData convertedArenaReloaded;
    Require(convertedArenaReloaded.Load(gameDataDirectory, 16)
                && convertedArenaReloaded.LoadGame(convertedArenaRuntimePath)
                && convertedArenaReloaded.UseSelectedItem(0, -1)
                && convertedArenaReloaded.TileAt(30, 5) == 0x5e
                && static_cast<std::size_t>(std::count_if(
                    convertedArenaReloaded.Actors().begin(),
                    convertedArenaReloaded.Actors().end(),
                    [](const drain_storm::BoardActor& actor) {
                        return actor.type == 2 && actor.column == 20 && actor.row == 18;
                    })) <= releasedCoreActors,
            "selector-3a96 core release remains one-shot after reload");
    std::filesystem::remove(convertedArenaRuntimePath);

    drain_storm::GameData genericDepthOne;
    Require(genericDepthOne.Load(gameDataDirectory, 17), "load generic procedural depth one");
    Require(genericDepthOne.TileAt(genericDepthOne.PlayerColumn(), genericDepthOne.PlayerRow()) == 0x23
                && genericDepthOne.PlayerColumn() <= 37 && genericDepthOne.PlayerRow() <= 33 &&
                genericDepthOne.TileAt(genericDepthOne.PlayerColumn() + 2,
                                       genericDepthOne.PlayerRow() + 2) == 0x7e,
            "generic procedural depth-one entry and sign");
    Require(genericDepthOne.PositionPlayerAtTile(0x27), "generic procedural generated exit");
    Require(!genericDepthOne.Overlays().empty(), "generic procedural randomized pickups");

    const std::filesystem::path genericRuntimePath =
        gameDataDirectory / L"generic-depth-runtime.VKW";
    drain_storm::GameData genericRuntimeSetup;
    Require(genericRuntimeSetup.Load(gameDataDirectory, 17)
                && genericRuntimeSetup.SaveGame(genericRuntimePath),
            "save generic procedural runtime fixture");
    {
        std::fstream save(genericRuntimePath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint32_t firstStructureThreshold = 331;
        save.seekp(21);
        save.write(reinterpret_cast<const char*>(&firstStructureThreshold),
                   sizeof(firstStructureThreshold));
        Require(save.good(), "advance generic procedural runtime fixture");
    }
    drain_storm::GameData genericRuntime;
    Require(genericRuntime.Load(gameDataDirectory, 17)
                && genericRuntime.LoadGame(genericRuntimePath)
                && genericRuntime.MovePlayer(0, 0)
                && genericRuntime.SaveGame(genericRuntimePath),
            "trigger generic procedural first escalation");
    {
        std::ifstream save(genericRuntimePath, std::ios::binary);
        int spawnChance{};
        int breakDivisor{};
        std::uint8_t runtimeFlags{};
        save.seekg(-56 - static_cast<std::streamoff>(genericRuntime.Overlays().size()), std::ios::end);
        save.read(reinterpret_cast<char*>(&spawnChance), sizeof(spawnChance));
        save.read(reinterpret_cast<char*>(&breakDivisor), sizeof(breakDivisor));
        save.read(reinterpret_cast<char*>(&runtimeFlags), sizeof(runtimeFlags));
        Require(save.good() && spawnChance == 25 && breakDivisor == 10
                    && (runtimeFlags & 0x07u) == 0x07u,
                "FUN_1008_dd76 restores procedural spawn and break escalation");
    }
    std::filesystem::remove(genericRuntimePath);

    drain_storm::GameData genericDepthTwo;
    Require(genericDepthTwo.Load(gameDataDirectory, 18), "load generic procedural depth two");
    Require(genericDepthTwo.TileAt(genericDepthTwo.PlayerColumn(), genericDepthTwo.PlayerRow()) == 0x23
                && !genericDepthTwo.PositionPlayerAtTile(0x7e)
                && genericDepthTwo.PositionPlayerAtTile(0x27),
            "generic procedural depth-two connections omit the initial sign");

    drain_storm::GameData genericDepthFour;
    Require(genericDepthFour.Load(gameDataDirectory, 20), "load generic procedural depth four");
    Require(genericDepthFour.TileAt(genericDepthFour.PlayerColumn(), genericDepthFour.PlayerRow()) == 0x23
                && genericDepthFour.PositionPlayerAtTile(0x27),
            "generic procedural depth-four connections");

    drain_storm::GameData genericDepthFive;
    Require(genericDepthFive.Load(gameDataDirectory, 21), "load generic procedural depth five");
    Require(genericDepthFive.TileAt(genericDepthFive.PlayerColumn(), genericDepthFive.PlayerRow()) == 0x23
                && genericDepthFive.PositionPlayerAtTile(0x27),
            "generic procedural depth-five connections");

    drain_storm::GameData genericDepthNine;
    Require(genericDepthNine.Load(gameDataDirectory, 25), "load generic procedural depth nine");
    Require(genericDepthNine.TileAt(genericDepthNine.PlayerColumn(), genericDepthNine.PlayerRow()) == 0x23
                && genericDepthNine.PositionPlayerAtTile(0x27),
            "generic procedural depth-nine terminal recursion boundary");

    drain_storm::GameData demoGame;
    Require(demoGame.LoadDemoGame(gameDataDirectory), "initialize original attract-mode RNG chain");
    Require(demoGame.LevelNumber() == 2,
            "attract-mode seed selects the original first showcase-board branch");
    Require(demoGame.Inventory().size() == 12 && demoGame.Inventory()[1].quantity == 20 &&
                demoGame.Inventory()[3].quantity == 21 && demoGame.Inventory()[5].quantity == 13 &&
                demoGame.Inventory()[6].quantity == 9 && demoGame.Inventory()[11].quantity == 6,
            "attract-mode randomized inventory quantities");
    drain_storm::GameData newGame;
    Require(newGame.LoadNewGame(gameDataDirectory), "initialize playable new-game RNG chain");
    Require(newGame.LevelNumber() == 17 && newGame.Inventory().size() == 3
                && newGame.Inventory()[0].type == 0 && newGame.Inventory()[1].type == 4
                && (newGame.Inventory()[2].type == 2 || newGame.Inventory()[2].type == 3),
            "playable new-game starts at procedural depth one with compact arsenal");
    bool initialUpDrainPresent = false;
    for (std::size_t column = 0; column < drain_storm::GameData::kBoardColumns; ++column) {
        for (std::size_t row = 0; row < drain_storm::GameData::kBoardRows; ++row) {
            initialUpDrainPresent = initialUpDrainPresent
                || (newGame.TileAt(column, row) >= 0x23
                    && newGame.TileAt(column, row) <= 0x26);
        }
    }
    Require(!initialUpDrainPresent
                && newGame.TileAt(newGame.PlayerColumn(), newGame.PlayerRow()) == 0,
            "FUN_1008_0630 erases the first playable stage's upward drain");
    Require(newGame.UseSelectedItem(1, 0) && newGame.Projectiles().size() == 1
                && newGame.Projectiles()[0].type == 0,
            "playable new-game Bubble launches the original bubble projectile");
    drain_storm::GameData hardestNewGame;
    Require(hardestNewGame.LoadNewGame(gameDataDirectory, 4)
                && hardestNewGame.Difficulty() == 4,
            "new-game preserves selected Drain Death difficulty");
    Require(!hardestNewGame.LoadNewGame(gameDataDirectory, 0)
                && !hardestNewGame.LoadNewGame(gameDataDirectory, 5),
            "new-game rejects invalid difficulty values");
    drain_storm::GameData hardestLevelFour;
    Require(hardestLevelFour.Load(gameDataDirectory, 4, 1, 4)
                && hardestLevelFour.Overlays().size() == levelFour.Overlays().size() + 3,
            "selector-1050 pickup count scales with difficulty");
    drain_storm::GameData hardestLevelSix;
    Require(hardestLevelSix.Load(gameDataDirectory, 6, 1, 4)
                && hardestLevelSix.Actors().size() == 240,
            "selector-1060 actor count scales with difficulty");

    drain_storm::GameData weapons;
    Require(weapons.Load(gameDataDirectory, 1), "load weapon test board");
    Require(weapons.UseSelectedItem(0, -1), "use unlimited primary weapon");
    Require(weapons.Projectiles().size() == 1 && weapons.Projectiles()[0].type == 0,
            "primary weapon projectile type");
    const drain_storm::GameplaySound primarySound = weapons.TakeSoundEvent();
    Require(primarySound == drain_storm::GameplaySound::SingleThuk,
            "primary weapon sound is not replaced by a first-turn structure event");
    Require(weapons.Inventory()[0].quantity == 0, "unlimited primary is not consumed");
    Require(weapons.SelectNextItem(), "select bomb inventory item");
    const std::uint16_t bombsBefore = weapons.Inventory()[1].quantity;
    Require(weapons.UseSelectedItem(-1, 0), "use directional bomb");
    Require(weapons.Inventory()[1].quantity == bombsBefore - 1, "consumable quantity decrements");
    const drain_storm::GameplaySound bombSound = weapons.TakeSoundEvent();
    Require(bombSound == drain_storm::GameplaySound::BombShot
                || bombSound == drain_storm::GameplaySound::BoardBreak,
            "bomb or moving-target terrain sound event");
    Require(weapons.SelectNextItem(), "select triple projectile item");
    const std::uint16_t tripleBefore = weapons.Inventory()[2].quantity;
    const std::size_t projectilesBefore = weapons.Projectiles().size();
    Require(weapons.UseSelectedItem(0, -1), "use triple projectile item");
    Require(weapons.Projectiles().size() == projectilesBefore + 3,
            "triple weapon creates three projectiles");
    Require(weapons.Inventory()[2].quantity == tripleBefore,
            "type-one triple weapon is not consumed");
    const int projectileColumn = weapons.Projectiles()[0].column;
    weapons.AdvanceAnimation();
    Require(weapons.Projectiles()[0].column == projectileColumn,
            "animation timer does not advance projectile turns");

    drain_storm::GameData objectDamage;
    Require(objectDamage.Load(gameDataDirectory, 1), "load object-damage test board");
    const std::filesystem::path objectDamagePath =
        gameDataDirectory / L"common-object-damage-isolated.VKW";
    Require(objectDamage.SaveGame(objectDamagePath), "save isolated object-damage fixture");
    {
        std::fstream save(objectDamagePath, std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        save.seekg(boardOffset + static_cast<std::streamoff>(40 * 36 * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        const std::streamoff actorsOffset = save.tellg();
        const std::uint16_t stationary = 1;
        for (std::uint32_t actor = 0; actor < actorCount; ++actor) {
            save.seekp(actorsOffset + static_cast<std::streamoff>(actor * 12 + 10));
            save.write(reinterpret_cast<const char*>(&stationary), sizeof(stationary));
        }
        Require(save.good(), "isolate object-damage fixture from moving actors");
    }
    Require(objectDamage.LoadGame(objectDamagePath), "load isolated object-damage fixture");
    Require(objectDamage.TileAt(5, 32) == 0x1a, "even 3x3 object center fixture");
    Require(objectDamage.SelectNextItem(), "select object-damage bomb");
    for (int shot = 0; shot < 12 && objectDamage.TileAt(5, 32) != 0; ++shot) {
        Require(objectDamage.UseSelectedItem(-1, 0), "advance bomb toward 3x3 object");
    }
    for (std::size_t row = 31; row <= 33; ++row) {
        for (std::size_t column = 4; column <= 6; ++column) {
            Require(objectDamage.TileAt(column, row) == 0,
                    "destroyed 3x3 object clears every tile");
        }
    }
    Require(objectDamage.TakeSoundEvent() == drain_storm::GameplaySound::BoardBreak,
            "3x3 destruction sound event");
    Require(objectDamage.Score() == 350, "type-0x1a object awards recovered score");
    std::filesystem::remove(objectDamagePath);

    const std::filesystem::path fireballObjectPath =
        gameDataDirectory / L"common-fireball-object.VKW";
    drain_storm::GameData fireballObjectSetup;
    Require(fireballObjectSetup.Load(gameDataDirectory, 1)
                && fireballObjectSetup.SaveGame(fireballObjectPath),
            "save fireball object-damage fixture");
    {
        std::fstream save(fireballObjectPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 7;
        const std::uint16_t playerRow = 32;
        const std::uint32_t fireballDropSeed = 15;
        save.seekp(17);
        save.write(reinterpret_cast<const char*>(&fireballDropSeed), sizeof(fireballDropSeed));
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        Require(save.good(), "position fireball object-damage fixture");
    }
    equipFirstInventoryItem(fireballObjectPath, 11, 1);
    drain_storm::GameData fireballObject;
    Require(fireballObject.Load(gameDataDirectory, 1)
                && fireballObject.LoadGame(fireballObjectPath)
                && fireballObject.UseSelectedItem(-1, 0),
            "fire fireball into 3x3 object");
    for (std::size_t row = 31; row <= 33; ++row) {
        for (std::size_t column = 4; column <= 6; ++column) {
            Require(fireballObject.TileAt(column, row) == fireballObjectSetup.TileAt(column, row),
                    "1008:7c60 fireball leaves every 3x3 object tile intact");
        }
    }
    Require(fireballObject.Projectiles().empty() && fireballObject.Score() == 0,
            "1008:7c60 consumes type-4 fireball without terrain score or treasure");
    std::filesystem::remove(fireballObjectPath);

    const std::filesystem::path selectorFireballPath =
        gameDataDirectory / L"selector-1050-fireball-object.VKW";
    drain_storm::GameData selectorFireballSetup;
    Require(selectorFireballSetup.Load(gameDataDirectory, 4)
                && selectorFireballSetup.SaveGame(selectorFireballPath),
            "save selector-1050 fireball object fixture");
    {
        std::fstream save(selectorFireballPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint32_t selectorDropSeed = 28;
        const std::uint16_t playerColumn = 7;
        const std::uint16_t playerRow = 7;
        constexpr std::streamoff boardOffset = 130;
        constexpr std::uint16_t objectTiles[3][3] = {
            {8, 9, 10}, {11, 0x18, 12}, {13, 14, 15}};
        save.seekp(17);
        save.write(reinterpret_cast<const char*>(&selectorDropSeed), sizeof(selectorDropSeed));
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        for (int row = 0; row < 3; ++row) {
            for (int column = 0; column < 3; ++column) {
                save.seekp(boardOffset + static_cast<std::streamoff>(
                    ((4 + column) * 36 + 6 + row) * 2));
                save.write(reinterpret_cast<const char*>(&objectTiles[row][column]),
                           sizeof(objectTiles[row][column]));
            }
        }
        Require(save.good(), "write selector-1050 fireball object fixture");
    }
    equipFirstInventoryItem(selectorFireballPath, 11, 1);
    drain_storm::GameData selectorFireball;
    Require(selectorFireball.Load(gameDataDirectory, 4)
                && selectorFireball.LoadGame(selectorFireballPath)
                && selectorFireball.UseSelectedItem(-1, 0)
                && selectorFireball.TileAt(5, 7) == 0x18
                && selectorFireball.TileAt(4, 6) == 8
                && selectorFireball.Projectiles().empty(),
            "1008:7c60 does not dispatch selector-1050 treasure on fireball terrain impact");
    std::filesystem::remove(selectorFireballPath);

    const auto writeBlockedPlacementFixture = [&](const std::filesystem::path& path,
                                                   std::uint16_t itemType) {
        drain_storm::GameData setup;
        Require(setup.Load(gameDataDirectory, 1) && setup.SaveGame(path),
                "save blocked weapon-placement fixture");
        std::fstream save(path, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 7;
        const std::uint16_t playerRow = 7;
        const std::uint16_t solidTile = 3;
        constexpr std::streamoff boardOffset = 130;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        save.seekp(boardOffset + static_cast<std::streamoff>((6 * 36 + 7) * 2));
        save.write(reinterpret_cast<const char*>(&solidTile), sizeof(solidTile));
        Require(save.good(), "patch blocked weapon-placement fixture");
        save.close();
        equipFirstInventoryItem(path, itemType, 1);
    };
    const std::filesystem::path blockedMinePath =
        gameDataDirectory / L"blocked-mine.VKW";
    writeBlockedPlacementFixture(blockedMinePath, 3);
    drain_storm::GameData blockedMine;
    Require(blockedMine.Load(gameDataDirectory, 1)
                && blockedMine.LoadGame(blockedMinePath),
            "load blocked mine fixture");
    const std::uint32_t blockedMineTurn = blockedMine.TurnCount();
    Require(blockedMine.UseSelectedItem(-1, 0)
                && blockedMine.TurnCount() == blockedMineTurn + 1
                && blockedMine.Projectiles().empty()
                && blockedMine.TakeMessage() == L"You can't lay a mine there."
                && blockedMine.Inventory()[0].quantity == 1,
            "FUN_1008_1fe2 rejects a mine on impassable terrain without consumption");
    std::filesystem::remove(blockedMinePath);

    const std::filesystem::path blockedRadioPath =
        gameDataDirectory / L"blocked-radio-bomb.VKW";
    writeBlockedPlacementFixture(blockedRadioPath, 8);
    drain_storm::GameData blockedRadio;
    Require(blockedRadio.Load(gameDataDirectory, 1)
                && blockedRadio.LoadGame(blockedRadioPath),
            "load blocked radio-bomb fixture");
    const std::uint32_t blockedRadioTurn = blockedRadio.TurnCount();
    Require(blockedRadio.UseSelectedItem(-1, 0)
                && blockedRadio.TurnCount() == blockedRadioTurn + 1
                && blockedRadio.Projectiles().empty()
                && blockedRadio.TakeMessage()
                    == L"You can't lay a radio bomb there."
                && blockedRadio.Inventory()[0].quantity == 1,
            "FUN_1008_1fe2 rejects a radio bomb on impassable terrain without consumption");
    std::filesystem::remove(blockedRadioPath);

    drain_storm::GameData emptyDetonator;
    Require(emptyDetonator.Load(gameDataDirectory, 2), "load empty detonator fixture");
    for (int selection = 0; selection < 8; ++selection) {
        Require(emptyDetonator.SelectNextItem(), "select empty detonator");
    }
    Require(emptyDetonator.Inventory()[emptyDetonator.SelectedItem()].type == 9,
            "empty detonator item type");
    const std::uint16_t emptyDetonatorsBefore =
        emptyDetonator.Inventory()[emptyDetonator.SelectedItem()].quantity;
    Require(emptyDetonator.UseSelectedItem(1, 0)
                && emptyDetonator.Inventory()[emptyDetonator.SelectedItem()].quantity
                    == emptyDetonatorsBefore - 1
                && emptyDetonator.TakeSoundEvent()
                    == drain_storm::GameplaySound::RadioBeep,
            "FUN_1008_a732 consumes an empty detonator use and emits radio beep");

    drain_storm::GameData charges;
    Require(charges.Load(gameDataDirectory, 1), "load charge test board");
    for (int selection = 0; selection < 7; ++selection) {
        Require(charges.SelectNextItem(), "select stationary charge");
    }
    Require(charges.Inventory()[charges.SelectedItem()].type == 8, "charge item type");
    Require(charges.UseSelectedItem(1, 0), "place stationary charge");
    Require(charges.Projectiles().size() == 1 && charges.Projectiles()[0].type == 3,
            "stationary charge projectile");
    Require(charges.SelectNextItem(), "select detonator");
    const std::uint16_t detonatorsBefore = charges.Inventory()[charges.SelectedItem()].quantity;
    const std::filesystem::path chargeImpactPath =
        gameDataDirectory / L"common-charge-impact.VKW";
    Require(charges.SaveGame(chargeImpactPath), "save common charge-impact fixture");
    {
        std::fstream save(chargeImpactPath, std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        save.seekg(boardOffset + static_cast<std::streamoff>(40 * 36 * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        const std::streamoff actorsOffset = save.tellg();
        const std::array<std::uint16_t, 6> stationaryActor = {0, 0, 0, 0, 0, 1};
        for (std::uint32_t actor = 0; actor < actorCount; ++actor) {
            save.seekp(actorsOffset + static_cast<std::streamoff>(actor * 12));
            save.write(reinterpret_cast<const char*>(stationaryActor.data()),
                       sizeof(stationaryActor));
        }
        const std::array<std::uint16_t, 6> blastActor = {3, 15, 32, 0, 0, 1};
        save.seekp(actorsOffset);
        save.write(reinterpret_cast<const char*>(blastActor.data()), sizeof(blastActor));
        Require(save.good(), "write common charge-impact fixture");
    }
    drain_storm::GameData chargeImpact;
    Require(chargeImpact.Load(gameDataDirectory, 1) && chargeImpact.LoadGame(chargeImpactPath),
            "load common charge-impact fixture");
    const std::uint32_t chargeImpactScore = chargeImpact.Score();
    Require(chargeImpact.UseSelectedItem(1, 0)
                && chargeImpact.Score() == chargeImpactScore + 250,
            "FUN_1008_a980 charge blast awards native captain score");
    std::filesystem::remove(chargeImpactPath);
    Require(charges.UseSelectedItem(1, 0), "detonate placed charge");
    Require(charges.Projectiles().empty(), "detonation removes charge");
    Require(charges.Inventory()[charges.SelectedItem()].quantity == detonatorsBefore - 1,
            "successful detonation consumes detonator use");
    const drain_storm::GameplaySound detonationSound = charges.TakeSoundEvent();
    Require(detonationSound == drain_storm::GameplaySound::Explosion
                || detonationSound == drain_storm::GameplaySound::BoardBreak,
            "detonation or moving-target terrain sound event");

    drain_storm::GameData levelThree;
    Require(levelThree.Load(gameDataDirectory, 3), "load level three");
    Require(levelThree.PlayerColumn() == 3 && levelThree.PlayerRow() == 2,
            "map start marker coordinates");
    Require(levelThree.Actors().size() == 62, "map and selector-1048 actor constructors");
    Require(levelThree.TileAt(31, 5) == 0x45 && levelThree.TileAt(33, 11) == 0x50,
            "selector-1048 bf2a structures");
    Require(levelThree.TileAt(38, 1) == 99 && levelThree.TileAt(38, 16) == 104,
            "selector-1048 vertical mutations");
    Require(levelThree.TileAt(24, 9) == 0x18 && levelThree.TileAt(17, 20) == 0x18
                && levelThree.TileAt(27, 10) == 0x18,
            "selector-1048 procedural structures follow recovered LCG");
    const std::filesystem::path levelThreeRuntimePath =
        gameDataDirectory / L"selector-1048-runtime.VKW";
    drain_storm::GameData levelThreeRuntime;
    Require(levelThreeRuntime.Load(gameDataDirectory, 3), "load selector-1048 runtime board");
    Require(levelThreeRuntime.SaveGame(levelThreeRuntimePath), "save selector-1048 runtime fixture");
    {
        // DAH64 version 7 begins its column-major board at byte 124; the
        // selector-1058 probe state precedes the fixed board payload.
        constexpr std::streamoff boardOffset = 130;
        constexpr std::streamoff removedCenter = (24 * 36 + 9) * 2;
        std::fstream save(levelThreeRuntimePath,
                          std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t emptyTile = 0;
        save.seekp(boardOffset + removedCenter);
        save.write(reinterpret_cast<const char*>(&emptyTile), sizeof(emptyTile));
        Require(save.good(), "prepare destroyed selector-1048 structure fixture");
    }
    Require(levelThreeRuntime.LoadGame(levelThreeRuntimePath), "load selector-1048 runtime fixture");
    const std::size_t effectsBeforeRebuild = levelThreeRuntime.Overlays().size();
    Require(levelThreeRuntime.UseSelectedItem(0, -1), "advance selector-1048 rebuild turn");
    std::size_t rebuiltStructures = 0;
    for (std::size_t column = 0; column < drain_storm::GameData::kBoardColumns; ++column) {
        for (std::size_t row = 0; row < drain_storm::GameData::kBoardRows; ++row) {
            rebuiltStructures += levelThreeRuntime.TileAt(column, row) == 0x18 ? 1u : 0u;
        }
    }
    Require(rebuiltStructures == 3, "selector-1048 runtime replenishes destroyed structure");
    Require(levelThreeRuntime.Overlays().size() == effectsBeforeRebuild + 9,
            "selector-1048 rebuild creates nine construction effects");
    std::filesystem::remove(levelThreeRuntimePath);

    const std::filesystem::path levelThreeLeverPath =
        gameDataDirectory / L"selector-1048-lever.VKW";
    Require(levelThree.TileAt(30, 28) == 0x38
                && levelThree.SaveGame(levelThreeLeverPath),
            "save selector-1048 lower lever fixture");
    {
        std::fstream save(levelThreeLeverPath,
                          std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t playerColumn = 38;
        const std::uint16_t playerRow = 19;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&playerColumn), sizeof(playerColumn));
        save.write(reinterpret_cast<const char*>(&playerRow), sizeof(playerRow));
        Require(save.good(), "position selector-1048 lower lever fixture");
    }
    drain_storm::GameData levelThreeLever;
    Require(levelThreeLever.Load(gameDataDirectory, 3)
                && levelThreeLever.LoadGame(levelThreeLeverPath),
            "load selector-1048 lower lever fixture");
    const std::size_t actorsBeforeLever = levelThreeLever.Actors().size();
    Require(levelThreeLever.MovePlayer(0, 0), "activate selector-1048 lower lever");
    Require(levelThreeLever.TileAt(30, 28) == 0x36
                && levelThreeLever.TileAt(31, 28) == 0x3b
                && levelThreeLever.TileAt(29, 32) == 0x37
                && levelThreeLever.TileAt(32, 21) == 0x18
                && levelThreeLever.Actors().size() >= actorsBeforeLever + 3,
            "selector-1048 lower lever opens chamber and releases guards");
    std::filesystem::remove(levelThreeLeverPath);
    const auto& perimeterActor = levelThree.Actors()[22];
    Require(perimeterActor.type <= 1 && perimeterActor.column == 37 && perimeterActor.row == 24 &&
                perimeterActor.deltaColumn == 1 && perimeterActor.behavior == 3,
            "selector-1048 perimeter actor state");
    Require(std::any_of(levelThree.Overlays().begin(), levelThree.Overlays().end(),
                        [](const drain_storm::BoardOverlay& overlay) {
                            return overlay.type == 3 && overlay.column == 0 && overlay.row == 8;
                        }),
            "selector-1048 optional overlay follows recovered LCG");
    const auto actorsBeforeMove = levelThree.Actors();
    Require(levelThree.MovePlayer(-1, 0), "level-three passable movement");
    bool actorMoved = false;
    for (std::size_t index = 0; index < actorsBeforeMove.size(); ++index) {
        if (actorsBeforeMove[index].column != levelThree.Actors()[index].column ||
            actorsBeforeMove[index].row != levelThree.Actors()[index].row) {
            actorMoved = true;
            break;
        }
    }
    Require(actorMoved, "actors advance after a player turn");
    Require(levelThree.MovePlayer(0, 0), "advance selector-1048 perimeter corner turn");
    Require(std::any_of(levelThree.Actors().begin(), levelThree.Actors().end(),
                        [](const drain_storm::BoardActor& actor) {
                            return actor.column == 39 && actor.row == 24
                                && actor.deltaColumn == 0 && actor.deltaRow == 1
                                && actor.behavior == 3;
                        }),
            "selector-1048 perimeter actors turn clockwise at upper-right corner");

    drain_storm::GameData weaponTurn;
    Require(weaponTurn.Load(gameDataDirectory, 3), "load weapon-turn test board");
    const auto weaponTurnActors = weaponTurn.Actors();
    Require(weaponTurn.UseSelectedItem(0, -1), "accept weapon turn");
    bool actorMovedAfterWeapon = weaponTurn.Actors().size() != weaponTurnActors.size();
    for (std::size_t index = 0;
         !actorMovedAfterWeapon && index < weaponTurnActors.size(); ++index) {
        actorMovedAfterWeapon =
            weaponTurnActors[index].column != weaponTurn.Actors()[index].column ||
            weaponTurnActors[index].row != weaponTurn.Actors()[index].row;
    }
    Require(actorMovedAfterWeapon, "actors advance after an accepted weapon turn");

    drain_storm::GameData levelThreeNavigation;
    Require(levelThreeNavigation.Load(gameDataDirectory, 3), "load selector-1048 navigation board");
    Require(levelThreeNavigation.PositionPlayerAtTile(0x27), "find selector-1048 first exit");
    Require(levelThreeNavigation.MovePlayer(0, 0)
                && levelThreeNavigation.TakeTransitionRequest() == 0,
            "standing on a transporter does not activate it");
    Require(levelThreeNavigation.ActivateTransporter(false), "activate selector-1048 first exit");
    Require(levelThreeNavigation.TakeTransitionRequest() == 6,
            "selector-1048 first exit targets selector-1060");
    Require(levelThreeNavigation.PositionPlayerAtTile(0x28), "find selector-1048 second exit");
    Require(levelThreeNavigation.ActivateTransporter(false), "activate selector-1048 second exit");
    Require(levelThreeNavigation.TakeTransitionRequest() == 7,
            "selector-1048 second exit targets selector-1068");
    Require(levelThreeNavigation.PositionPlayerAtTile(0x2a), "find selector-1048 fourth exit");
    Require(levelThreeNavigation.ActivateTransporter(false), "activate selector-1048 fourth exit");
    Require(levelThreeNavigation.TakeTransitionRequest() == 11,
            "selector-1048 fourth exit targets selector-cb9c");

    drain_storm::GameData levelSixNavigation;
    Require(levelSixNavigation.Load(gameDataDirectory, 6), "load selector-1060 navigation board");
    levelSixNavigation.SetRootBoard(false);
    Require(levelSixNavigation.PositionPlayerAtTile(0x23), "find selector-1060 parent exit");
    Require(levelSixNavigation.ActivateTransporter(true), "activate selector-1060 parent exit");
    Require(levelSixNavigation.TakeTransitionRequest() == -1,
            "selector-1060 entry returns to shared selector-1048 root");
    Require(levelSixNavigation.PositionPlayerAtTile(0x27), "find selector-1060 shared-board exit");
    Require(levelSixNavigation.ActivateTransporter(false), "activate selector-1060 shared-board exit");
    Require(levelSixNavigation.TakeTransitionRequest() == 5,
            "selector-1060 first exit targets selector-1058");
    Require(levelSixNavigation.PositionPlayerAtTile(0x28), "find selector-1060 child exit");
    Require(levelSixNavigation.ActivateTransporter(false), "activate selector-1060 child exit");
    Require(levelSixNavigation.TakeTransitionRequest() == 4,
            "selector-1060 second exit targets selector-1050");

    drain_storm::GameData levelFiveNavigation;
    Require(levelFiveNavigation.Load(gameDataDirectory, 5), "load selector-1058 navigation board");
    Require(levelFiveNavigation.PositionPlayerAtTile(0x23), "find selector-1058 selector-1060 edge");
    Require(levelFiveNavigation.ActivateTransporter(true), "activate selector-1058 selector-1060 edge");
    Require(levelFiveNavigation.TakeTransitionRequest() == 6,
            "selector-1058 tile 0x23 targets selector-1060");
    Require(levelFiveNavigation.PositionPlayerAtTile(0x24), "find selector-1058 selector-1068 edge");
    Require(levelFiveNavigation.ActivateTransporter(true), "activate selector-1058 selector-1068 edge");
    Require(levelFiveNavigation.TakeTransitionRequest() == 7,
            "selector-1058 tile 0x24 targets selector-1068");

    drain_storm::GameData levelSevenNavigation;
    Require(levelSevenNavigation.Load(gameDataDirectory, 7), "load selector-1068 navigation board");
    levelSevenNavigation.SetRootBoard(false);
    Require(levelSevenNavigation.PositionPlayerAtTile(0x23), "find selector-1068 parent exit");
    Require(levelSevenNavigation.ActivateTransporter(true), "activate selector-1068 parent exit");
    Require(levelSevenNavigation.TakeTransitionRequest() == -1,
            "selector-1068 entry returns to shared selector-1048 root");
    Require(levelSevenNavigation.PositionPlayerAtTile(0x27), "find selector-1068 shared-board exit");
    Require(levelSevenNavigation.ActivateTransporter(false), "activate selector-1068 shared-board exit");
    Require(levelSevenNavigation.TakeTransitionRequest() == 5,
            "selector-1068 first exit targets selector-1058");

    drain_storm::GameData levelFourNavigation;
    Require(levelFourNavigation.Load(gameDataDirectory, 4), "load selector-1050 navigation board");
    Require(levelFourNavigation.PositionPlayerAtTile(0x27), "find selector-1050 child exit");
    Require(levelFourNavigation.ActivateTransporter(false), "activate selector-1050 child exit");
    Require(levelFourNavigation.TakeTransitionRequest() == 10,
            "selector-1050 exit targets selector-33b4");

    drain_storm::GameData levelTenNavigation;
    Require(levelTenNavigation.Load(gameDataDirectory, 10), "load selector-33b4 navigation board");
    levelTenNavigation.SetRootBoard(false);
    Require(levelTenNavigation.ActivateTransporter(true), "activate selector-33b4 parent exit");
    Require(levelTenNavigation.TakeTransitionRequest() == -1,
            "selector-33b4 start tile returns to parent");

    drain_storm::GameData levelElevenNavigation;
    Require(levelElevenNavigation.Load(gameDataDirectory, 11), "load selector-cb9c navigation board");
    levelElevenNavigation.SetRootBoard(false);
    Require(levelElevenNavigation.ActivateTransporter(true), "activate selector-cb9c parent exit");
    Require(levelElevenNavigation.TakeTransitionRequest() == -1,
            "selector-cb9c start tile returns to parent");

    const auto requireLevelElevenExit = [&](std::uint32_t seed, int expectedLevel,
                                            const wchar_t* fixtureName) {
        const std::filesystem::path path = gameDataDirectory / fixtureName;
        drain_storm::GameData setup;
        Require(setup.Load(gameDataDirectory, 11)
                    && setup.PositionPlayerAtTile(0x27)
                    && setup.SaveGame(path),
                "save selector-cb9c child-factory fixture");
        {
            std::fstream save(path, std::ios::binary | std::ios::in | std::ios::out);
            save.seekp(17);
            save.write(reinterpret_cast<const char*>(&seed), sizeof(seed));
            Require(save.good(), "seed selector-cb9c child-factory fixture");
        }
        drain_storm::GameData board;
        Require(board.Load(gameDataDirectory, 11) && board.LoadGame(path)
                    && board.ActivateTransporter(false)
                    && board.TakeTransitionRequest() == expectedLevel
                    && board.ActivateTransporter(false)
                    && board.TakeTransitionRequest() == expectedLevel,
                "FUN_1010_ce62 chooses and caches recovered child class");
        std::filesystem::remove(path);
    };
    requireLevelElevenExit(10, 8, L"selector-cb9c-c80e-child.VKW");
    requireLevelElevenExit(38, 12, L"selector-cb9c-final-child.VKW");
    requireLevelElevenExit(5, 13, L"selector-cb9c-maze-child.VKW");
    requireLevelElevenExit(1, 11, L"selector-cb9c-recursive-child.VKW");

    drain_storm::GameData finalArenaNavigation;
    Require(finalArenaNavigation.Load(gameDataDirectory, 12)
                && finalArenaNavigation.PositionPlayerAtTile(0x27)
                && finalArenaNavigation.ActivateTransporter(false)
                && finalArenaNavigation.TakeTransitionRequest() == 11,
            "FUN_1010_d5d4 final-arena exit targets selector-cb9c");

    drain_storm::GameData mazeNavigation;
    Require(mazeNavigation.Load(gameDataDirectory, 13), "load maze navigation stage one");
    Require(mazeNavigation.PositionPlayerAtTile(0x27) && mazeNavigation.ActivateTransporter(false)
                && mazeNavigation.TakeTransitionRequest() == 14,
            "maze stage one advances to stage two");
    Require(mazeNavigation.Load(gameDataDirectory, 13)
                && mazeNavigation.PositionPlayerAtTile(0x28) && mazeNavigation.ActivateTransporter(false)
                && mazeNavigation.TakeTransitionRequest() == 11,
            "maze stage one alternate exit targets selector-cb9c");
    Require(mazeNavigation.Load(gameDataDirectory, 15)
                && mazeNavigation.PositionPlayerAtTile(0x27) && mazeNavigation.ActivateTransporter(false)
                && mazeNavigation.TakeTransitionRequest() == 16,
            "maze stage three advances to converted-terrain arena");

    drain_storm::GameData genericNavigation;
    Require(genericNavigation.Load(gameDataDirectory, 17), "load generic return fixture");
    genericNavigation.SetRootBoard(false);
    Require(genericNavigation.PositionPlayerAtTile(0x23)
                && genericNavigation.ActivateTransporter(true)
                && genericNavigation.TakeTransitionRequest() == -1,
            "generic upward entry drain returns through retained parent");
    Require(genericNavigation.Load(gameDataDirectory, 17)
                && genericNavigation.PositionPlayerAtTile(0x27)
                && genericNavigation.ActivateTransporter(false)
                && genericNavigation.TakeTransitionRequest() == 18,
            "generic procedural depth one advances to depth two");
    // Destination selection is deferred to the graph owner, which first checks
    // for an existing child without consuming a random roll.
    for (std::uint32_t seed = 1; seed <= 40; ++seed) {
        Require(genericNavigation.Load(gameDataDirectory, 21, seed)
                    && genericNavigation.PositionPlayerAtTile(0x27)
                    && genericNavigation.ActivateTransporter(false),
                "activate generic procedural depth-five branch");
        const int request = genericNavigation.TakeTransitionRequest();
        Require(request == drain_storm::GameData::kDeferredProceduralExit,
                "depth-five exit defers the random choice until cache lookup");
    }
    Require(genericNavigation.Load(gameDataDirectory, 25)
                && genericNavigation.PositionPlayerAtTile(0x27)
                && genericNavigation.ActivateTransporter(false)
                && genericNavigation.TakeTransitionRequest() == 3,
            "generic procedural depth nine must return to selector-1048 class");

    const std::filesystem::path savedGamePath = gameDataDirectory / L"native-save-roundtrip.VKW";
    const auto savedPlayerColumn = weaponTurn.PlayerColumn();
    const auto savedPlayerRow = weaponTurn.PlayerRow();
    const auto savedActors = weaponTurn.Actors();
    const auto savedInventory = weaponTurn.Inventory();
    const auto savedScore = weaponTurn.Score();
    const auto savedSelection = weaponTurn.SelectedItem();
    Require(weaponTurn.SaveGame(savedGamePath), "save complete native game state");
    Require(weaponTurn.SelectNextItem(), "mutate state after save");
    Require(weaponTurn.LoadGame(savedGamePath), "reload complete native game state");
    Require(weaponTurn.PlayerColumn() == savedPlayerColumn
                && weaponTurn.PlayerRow() == savedPlayerRow,
            "saved player coordinates round trip");
    Require(weaponTurn.Actors().size() == savedActors.size()
                && weaponTurn.Inventory().size() == savedInventory.size()
                && weaponTurn.Score() == savedScore
                && weaponTurn.SelectedItem() == savedSelection,
            "saved actor inventory and score state round trip");
    const std::filesystem::path difficultySavePath =
        gameDataDirectory / L"native-difficulty-roundtrip.VKW";
    Require(hardestNewGame.SaveGame(difficultySavePath), "save selected difficulty");
    drain_storm::GameData restoredDifficulty;
    Require(restoredDifficulty.Load(gameDataDirectory, 1)
                && restoredDifficulty.LoadGame(difficultySavePath)
                && restoredDifficulty.Difficulty() == 4,
            "selected difficulty round trips through native save");
    std::filesystem::remove(difficultySavePath);
    {
        std::ofstream corrupt(savedGamePath, std::ios::binary | std::ios::trunc);
        corrupt << "not a save";
    }
    Require(!weaponTurn.LoadGame(savedGamePath), "reject invalid saved-game signature");
    std::filesystem::remove(savedGamePath);

    const std::filesystem::path wrappedDeathPath =
        gameDataDirectory / L"wrapped-score-death-lifecycle.VKW";
    drain_storm::GameData wrappedDeathSource;
    Require(wrappedDeathSource.Load(gameDataDirectory, 1)
                && wrappedDeathSource.SaveGame(wrappedDeathPath),
            "save wrapped-score death lifecycle fixture");
    writeCollidingActors(wrappedDeathPath, 0, 3);
    {
        std::fstream save(wrappedDeathPath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint32_t wrappedNegativeScore = 4'000'000'000u;
        save.seekp(25);
        save.write(reinterpret_cast<const char*>(&wrappedNegativeScore),
                   sizeof(wrappedNegativeScore));
        Require(save.good(), "write wrapped-score death lifecycle fixture");
    }
    drain_storm::ApplicationState wrappedDeathState;
    Require(wrappedDeathState.LoadOriginalResources(executablePath)
                && wrappedDeathState.LoadGame(wrappedDeathPath)
                && wrappedDeathState.MovePlayer(0, 0),
            "start wrapped-score fatal animation");
    for (int frame = 0; frame < 5; ++frame) wrappedDeathState.AdvanceAnimation();
    Require(wrappedDeathState.TakeLifecycleMessage().find(L"did not fare well")
                    != std::wstring::npos
                && wrappedDeathState.IsDemoMode()
                && !wrappedDeathState.HasPendingHighScore(),
            "FUN_1008_1b22 uses signed long score for death tier and ranking");
    std::filesystem::remove(wrappedDeathPath);
    const std::filesystem::path quitScorePath =
        gameDataDirectory / L"quit-score-lifecycle.VKW";
    drain_storm::GameData quitScoreSource;
    Require(quitScoreSource.Load(gameDataDirectory, 1)
                && quitScoreSource.SaveGame(quitScorePath),
            "save quit-score lifecycle fixture");
    {
        std::fstream save(quitScorePath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint32_t qualifyingScore = 2'000'000'000u;
        save.seekp(25);
        save.write(reinterpret_cast<const char*>(&qualifyingScore), sizeof(qualifyingScore));
        Require(save.good(), "write quit-score lifecycle fixture");
    }
    drain_storm::ApplicationState unsavedQuitState;
    Require(unsavedQuitState.LoadOriginalResources(executablePath)
                && unsavedQuitState.LoadGame(quitScorePath)
                && unsavedQuitState.QuitCurrentGame(true)
                && unsavedQuitState.IsDemoMode()
                && unsavedQuitState.HasPendingHighScore(),
            "FUN_1008_1efe submits an unsaved abandoned run");
    drain_storm::ApplicationState savedQuitState;
    Require(savedQuitState.LoadOriginalResources(executablePath)
                && savedQuitState.LoadGame(quitScorePath)
                && savedQuitState.QuitCurrentGame(false)
                && savedQuitState.IsDemoMode()
                && !savedQuitState.HasPendingHighScore(),
            "FUN_1008_1efe omits a saved abandoned run from high scores");
    {
        std::fstream save(quitScorePath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint32_t zeroScore = 0;
        save.seekp(25);
        save.write(reinterpret_cast<const char*>(&zeroScore), sizeof(zeroScore));
        Require(save.good(), "write zero-score lifecycle fixture");
    }
    drain_storm::ApplicationState zeroScoreQuitState;
    Require(zeroScoreQuitState.LoadOriginalResources(executablePath)
                && zeroScoreQuitState.LoadGame(quitScorePath)
                && zeroScoreQuitState.QuitCurrentGame(true)
                && zeroScoreQuitState.HasPendingHighScore(),
            "signed Win16 high-score comparison accepts zero above an empty -1 slot");
    drain_storm::ApplicationState lawRecallState;
    Require(lawRecallState.LoadOriginalResources(executablePath)
                && lawRecallState.LoadGame(quitScorePath)
                && lawRecallState.RecallDiscoveredLaw()
                && lawRecallState.CurrentGameplayMessage()
                    == L"You haven't read any fortunes yet."
                && lawRecallState.TakeLifecycleMessage().empty(),
            "FUN_1008_2c84 keeps playable law recall in the arsenal history");
    std::filesystem::remove(quitScorePath);

    const std::filesystem::path transitionSourcePath =
        gameDataDirectory / L"graph-transition-source.VKW";
    const std::filesystem::path graphSavePath =
        gameDataDirectory / L"graph-roundtrip.VKW";
    drain_storm::GameData transitionSource;
    Require(transitionSource.Load(gameDataDirectory, 3), "load graph transition source");
    Require(transitionSource.PositionPlayerAtTile(0x27), "position graph transition source");
    Require(transitionSource.SaveGame(transitionSourcePath), "save graph transition source");
    drain_storm::ApplicationState graphState;
    Require(graphState.LoadOriginalResources(executablePath), "load application graph resources");
    Require(graphState.IsDemoMode()
                && (graphState.CurrentLevel() == 1 || graphState.CurrentLevel() == 2
                    || graphState.CurrentLevel() == 9 || graphState.CurrentLevel() == 10)
                && !graphState.MovePlayer(1, 0),
            "application startup enters a showcase-board attract mode and blocks manual movement");
    for (int tick = 0; tick < 2000; ++tick) graphState.AdvanceAnimation();
    Require(graphState.IsDemoMode()
                && (graphState.CurrentLevel() == 1 || graphState.CurrentLevel() == 2
                    || graphState.CurrentLevel() == 9 || graphState.CurrentLevel() == 10)
                && !graphState.MovePlayer(1, 0),
            "FUN_1010_dafe sentinel starts another randomized attract demonstration");
    const auto originalBindings = graphState.KeyBindingTable();
    auto changedBindings = originalBindings;
    changedBindings[9] = 'X';
    Require(graphState.SetKeyBindings(changedBindings) && graphState.KeyBinding(9) == 'X'
                && graphState.SetKeyBindings(originalBindings),
            "keyboard command table updates and persists transactionally");
    Require(graphState.IdentifySquare(graphState.PlayerColumn(), graphState.PlayerRow()).empty(),
            "FUN_1008_60f4 blocks right-click identification during attract mode");
    Require(graphState.SoundEnabled() && !graphState.ToggleSound()
                && graphState.ToggleSound() && graphState.SoundEnabled(),
            "application sound toggle follows original T binding");
    Require(!graphState.RecallDiscoveredLaw(),
            "FUN_1008_6f46 blocks fortune recall during attract mode");
    Require(graphState.LoadGame(transitionSourcePath), "load legacy single-board save");
    const std::wstring identifiedPlayer = graphState.IdentifySquare(
        graphState.PlayerColumn(), graphState.PlayerRow());
    Require(identifiedPlayer.find(L"Effer-vessel") != std::wstring::npos
                && graphState.CurrentGameplayMessage() == identifiedPlayer
                && graphState.NavigateMessageHistory(true) == identifiedPlayer,
            "FUN_1008_60f4 appends root-board identification to message history");
    Require(!graphState.IsDemoMode() && graphState.CurrentLevel() == 3
                && graphState.ActivateTransporter(false)
                && graphState.CurrentLevel() == 6,
            "application follows selector-1048 child edge");

    const std::filesystem::path fortuneMessagePath =
        gameDataDirectory / L"fortune-message-history.VKW";
    drain_storm::GameData fortuneMessageSource;
    Require(fortuneMessageSource.Load(gameDataDirectory, 3)
                && fortuneMessageSource.SaveGame(fortuneMessagePath),
            "save fortune-cookie message fixture");
    {
        std::fstream save(fortuneMessagePath,
                          std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        std::streamoff offset = boardOffset + 40 * 36 * 2;
        const auto skipRecords = [&save, &offset](std::streamoff recordSize) {
            std::uint32_t count{};
            save.seekg(offset);
            save.read(reinterpret_cast<char*>(&count), sizeof(count));
            offset += static_cast<std::streamoff>(sizeof(count))
                + static_cast<std::streamoff>(count) * recordSize;
        };
        skipRecords(6); // Stable on-disk overlay fields, excluding runtime age.
        skipRecords(12); // Six serialized fields, excluding presentation caches.
        std::uint32_t inventoryCount{};
        save.seekg(offset);
        save.read(reinterpret_cast<char*>(&inventoryCount), sizeof(inventoryCount));
        Require(inventoryCount != 0, "locate fortune-cookie inventory fixture");
        const drain_storm::InventoryItem fortuneCookie{0x0f, 1};
        save.seekp(offset + static_cast<std::streamoff>(sizeof(inventoryCount)));
        save.write(reinterpret_cast<const char*>(&fortuneCookie), sizeof(fortuneCookie));
        Require(save.good(), "write fortune-cookie inventory fixture");
    }
    drain_storm::ApplicationState fortuneMessageState;
    Require(fortuneMessageState.LoadOriginalResources(executablePath),
            "load fortune-cookie application resources");
    Require(fortuneMessageState.LoadGame(fortuneMessagePath),
            "load fortune-cookie application fixture");
    Require(fortuneMessageState.UseSelectedItem(1, 0),
            "use fortune cookie through application state");
    Require(fortuneMessageState.TakeGameplayMessage()
                == L"It's a fortune cookie...\n\nIt says:",
            "FUN_1008_2c08 initially selects its fortune introduction");
    Require(!fortuneMessageState.NavigateMessageHistory(false).empty()
                && fortuneMessageState.CurrentGameplayMessage()
                    != L"It's a fortune cookie...\n\nIt says:",
            "FUN_1008_2c08 exposes its two notices through M in original order");
    std::filesystem::remove(fortuneMessagePath);

    const std::filesystem::path proceduralSignPath =
        gameDataDirectory / L"procedural-sign-history.VKW";
    drain_storm::GameData proceduralSignSource;
    Require(proceduralSignSource.Load(gameDataDirectory, 17)
                && proceduralSignSource.PositionPlayerAtTile(0x7e)
                && proceduralSignSource.SaveGame(proceduralSignPath),
            "save procedural signpost history fixture");
    const std::uint16_t proceduralSignColumn =
        static_cast<std::uint16_t>(proceduralSignSource.PlayerColumn());
    const std::uint16_t proceduralSignRow =
        static_cast<std::uint16_t>(proceduralSignSource.PlayerRow());
    const std::uint16_t proceduralApproachColumn =
        proceduralSignColumn == 0 ? 1 : proceduralSignColumn - 1;
    const int proceduralSignStep = proceduralSignColumn == 0 ? -1 : 1;
    {
        std::fstream save(proceduralSignPath,
                          std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        const std::uint16_t empty = 0;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&proceduralApproachColumn),
                   sizeof(proceduralApproachColumn));
        save.write(reinterpret_cast<const char*>(&proceduralSignRow),
                   sizeof(proceduralSignRow));
        save.seekp(boardOffset + static_cast<std::streamoff>(
            (proceduralApproachColumn * 36 + proceduralSignRow) * 2));
        save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
        Require(save.good(), "position procedural signpost history fixture");
    }
    drain_storm::ApplicationState proceduralSignState;
    Require(proceduralSignState.LoadOriginalResources(executablePath)
                && proceduralSignState.LoadGame(proceduralSignPath)
                && proceduralSignState.MovePlayer(proceduralSignStep, 0)
                && proceduralSignState.TakeGameplayMessage()
                    == L"You see here a signpost.\n\nIt reads:"
                && proceduralSignState.NavigateMessageHistory(false)
                    == L"Welcome to your shower drain.\n\nHey, it's tiled!",
            "FUN_1008_df52 supplies the procedural signpost's second message");
    std::filesystem::remove(proceduralSignPath);

    const std::filesystem::path genericDownSourcePath =
        gameDataDirectory / L"graph-generic-down-source.VKW";
    drain_storm::GameData genericDownSource;
    Require(genericDownSource.Load(gameDataDirectory, 17)
                && genericDownSource.PositionPlayerAtTile(0x27),
            "locate procedural downward-drain application fixture");
    const std::uint16_t drainColumn =
        static_cast<std::uint16_t>(genericDownSource.PlayerColumn());
    const std::uint16_t drainRow =
        static_cast<std::uint16_t>(genericDownSource.PlayerRow());
    const std::uint16_t approachColumn = drainColumn == 0 ? 1 : drainColumn - 1;
    const int drainStep = drainColumn == 0 ? -1 : 1;
    Require(genericDownSource.SaveGame(genericDownSourcePath),
            "save procedural downward-drain application fixture");
    {
        std::fstream save(genericDownSourcePath,
                          std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        const std::uint16_t empty = 0;
        save.seekp(29);
        save.write(reinterpret_cast<const char*>(&approachColumn), sizeof(approachColumn));
        save.write(reinterpret_cast<const char*>(&drainRow), sizeof(drainRow));
        save.seekp(boardOffset
            + static_cast<std::streamoff>((approachColumn * 36 + drainRow) * 2));
        save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
        Require(save.good(), "position procedural drain approach");
    }
    drain_storm::ApplicationState genericDownState;
    Require(genericDownState.LoadOriginalResources(executablePath)
                && genericDownState.LoadGame(genericDownSourcePath)
                && genericDownState.MovePlayer(drainStep, 0)
                && genericDownState.CurrentLevel() == 18,
            "FUN_1008_ba30 automatically follows a downward drain on entry");
    std::filesystem::remove(genericDownSourcePath);
    Require(!graphState.IdentifySquare(graphState.PlayerColumn(), graphState.PlayerRow()).empty(),
            "FUN_1008_60f4 allows right-click identification on retained playable boards");
    Require(graphState.SaveGame(graphSavePath), "save retained board graph");
    {
        std::ifstream input(graphSavePath, std::ios::binary);
        std::vector<char> bytes{std::istreambuf_iterator<char>(input),
                                std::istreambuf_iterator<char>()};
        Require(bytes.size() > 11 && std::equal(bytes.begin(), bytes.begin() + 5, "DAG64"),
                "board graph save signature");
        std::uint32_t historyCount{};
        std::memcpy(&historyCount, bytes.data() + 7, sizeof(historyCount));
        Require(historyCount == 1, "board graph saves retained parent");
        std::uint32_t sharedCount{};
        std::memcpy(&sharedCount, bytes.data() + 11, sizeof(sharedCount));
        Require(sharedCount == 4, "hub graph saves all preconstructed shared boards");
        constexpr std::array<char, 5> boardSignature = {'D', 'A', 'H', '6', '4'};
        const auto lastBoard = std::find_end(bytes.begin(), bytes.end(),
                                             boardSignature.begin(), boardSignature.end());
        Require(lastBoard != bytes.end(), "find current board in graph save");
        const std::streamoff currentBoardOffset =
            static_cast<std::streamoff>(std::distance(bytes.begin(), lastBoard));
        std::fstream output(graphSavePath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t returnColumn = 5;
        const std::uint16_t returnRow = 5;
        output.seekp(currentBoardOffset + 29);
        output.write(reinterpret_cast<const char*>(&returnColumn), sizeof(returnColumn));
        output.write(reinterpret_cast<const char*>(&returnRow), sizeof(returnRow));
        Require(output.good(), "position restored child at parent exit");
    }
    const std::filesystem::path childDeathGraphPath =
        gameDataDirectory / L"graph-child-death-lifecycle.VKW";
    Require(std::filesystem::copy_file(graphSavePath, childDeathGraphPath,
                std::filesystem::copy_options::overwrite_existing),
            "copy descendant death lifecycle fixture");
    {
        std::ifstream input(childDeathGraphPath, std::ios::binary);
        std::vector<char> bytes{std::istreambuf_iterator<char>(input),
                                std::istreambuf_iterator<char>()};
        constexpr std::array<char, 5> boardSignature = {'D', 'A', 'H', '6', '4'};
        const auto lastBoard = std::find_end(bytes.begin(), bytes.end(),
                                             boardSignature.begin(), boardSignature.end());
        Require(lastBoard != bytes.end(), "find descendant death board");
        const std::streamoff offset =
            static_cast<std::streamoff>(std::distance(bytes.begin(), lastBoard));
        std::fstream save(childDeathGraphPath,
                          std::ios::binary | std::ios::in | std::ios::out);
        constexpr std::streamoff boardOffset = 130;
        const std::uint16_t empty = 0;
        save.seekp(offset + boardOffset + static_cast<std::streamoff>((5 * 36 + 5) * 2));
        save.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
        save.seekg(offset + boardOffset + static_cast<std::streamoff>(40 * 36 * 2));
        std::uint32_t overlayCount{};
        std::uint32_t actorCount{};
        save.read(reinterpret_cast<char*>(&overlayCount), sizeof(overlayCount));
        save.seekg(static_cast<std::streamoff>(overlayCount * 6), std::ios::cur);
        save.read(reinterpret_cast<char*>(&actorCount), sizeof(actorCount));
        Require(actorCount >= 2, "descendant death board has collision actors");
        const std::streamoff actorsOffset = save.tellg();
        const std::array<std::uint16_t, 6> stationary = {0, 0, 0, 0, 0, 1};
        for (std::uint32_t actor = 0; actor < actorCount; ++actor) {
            save.seekp(actorsOffset + static_cast<std::streamoff>(actor * 12));
            save.write(reinterpret_cast<const char*>(stationary.data()), sizeof(stationary));
        }
        const std::array<std::uint16_t, 6> lightHit = {0, 5, 5, 0, 0, 1};
        const std::array<std::uint16_t, 6> heavyHit = {3, 5, 5, 0, 0, 1};
        save.seekp(actorsOffset);
        save.write(reinterpret_cast<const char*>(lightHit.data()), sizeof(lightHit));
        save.write(reinterpret_cast<const char*>(heavyHit.data()), sizeof(heavyHit));
        Require(save.good(), "write descendant death lifecycle fixture");
    }
    drain_storm::ApplicationState childDeathState;
    Require(childDeathState.LoadOriginalResources(executablePath)
                && childDeathState.LoadGame(childDeathGraphPath)
                && childDeathState.CurrentLevel() == 6
                && childDeathState.MovePlayer(0, 0),
            "start descendant fatal animation");
    for (int frame = 0; frame < 5; ++frame) childDeathState.AdvanceAnimation();
    Require(childDeathState.TakeLifecycleMessage().find(L"Game Over")
                    != std::wstring::npos,
            "FUN_1008_1b22 runs game over on retained playable descendants");
    std::filesystem::remove(childDeathGraphPath);
    drain_storm::ApplicationState restoredGraphState;
    Require(restoredGraphState.LoadOriginalResources(executablePath),
            "load restored graph resources");
    Require(restoredGraphState.LoadGame(graphSavePath)
                && restoredGraphState.CurrentLevel() == 6,
            "restore active child and retained graph");
    const auto positionCurrentGraphBoard = [&](std::uint16_t column, std::uint16_t row,
                                                const char* message) {
        std::ifstream input(graphSavePath, std::ios::binary);
        std::vector<char> bytes{std::istreambuf_iterator<char>(input),
                                std::istreambuf_iterator<char>()};
        constexpr std::array<char, 5> boardSignature = {'D', 'A', 'H', '6', '4'};
        const auto lastBoard = std::find_end(bytes.begin(), bytes.end(),
                                             boardSignature.begin(), boardSignature.end());
        Require(lastBoard != bytes.end(), message);
        const std::streamoff offset =
            static_cast<std::streamoff>(std::distance(bytes.begin(), lastBoard));
        std::fstream output(graphSavePath, std::ios::binary | std::ios::in | std::ios::out);
        output.seekp(offset + 29);
        output.write(reinterpret_cast<const char*>(&column), sizeof(column));
        output.write(reinterpret_cast<const char*>(&row), sizeof(row));
        Require(output.good(), message);
    };

    drain_storm::GameData selector1060Route;
    Require(selector1060Route.Load(gameDataDirectory, 6)
                && selector1060Route.PositionPlayerAtTile(0x27),
            "find selector-1060 cross-link coordinates");
    Require(restoredGraphState.SaveGame(graphSavePath), "save graph before selector-1058 link");
    positionCurrentGraphBoard(static_cast<std::uint16_t>(selector1060Route.PlayerColumn()),
                              static_cast<std::uint16_t>(selector1060Route.PlayerRow()),
                              "position selector-1060 cross-link");
    Require(restoredGraphState.LoadGame(graphSavePath) && restoredGraphState.ActivateTransporter(false)
                && restoredGraphState.CurrentLevel() == 5,
            "shared graph enters selector-1058 from selector-1060");

    drain_storm::GameData selector1058Route;
    Require(selector1058Route.Load(gameDataDirectory, 5)
                && selector1058Route.PositionPlayerAtTile(0x23),
            "find selector-1058 return coordinates");
    Require(restoredGraphState.SaveGame(graphSavePath), "save graph before shared return");
    positionCurrentGraphBoard(static_cast<std::uint16_t>(selector1058Route.PlayerColumn()),
                              static_cast<std::uint16_t>(selector1058Route.PlayerRow()),
                              "position selector-1058 shared return");
    Require(restoredGraphState.LoadGame(graphSavePath) && restoredGraphState.ActivateTransporter(true)
                && restoredGraphState.CurrentLevel() == 6,
            "selector-1058 returns to retained selector-1060 instance");

    Require(selector1060Route.PositionPlayerAtTile(0x23),
            "find selector-1060 root-return coordinates");
    Require(restoredGraphState.SaveGame(graphSavePath), "save graph before root return");
    positionCurrentGraphBoard(static_cast<std::uint16_t>(selector1060Route.PlayerColumn()),
                              static_cast<std::uint16_t>(selector1060Route.PlayerRow()),
                              "position selector-1060 root return");
    Require(restoredGraphState.LoadGame(graphSavePath) && restoredGraphState.ActivateTransporter(true)
                && restoredGraphState.CurrentLevel() == 3,
            "shared selector returns to retained selector-1048 root");

    drain_storm::GameData selector1048ProceduralRoute;
    Require(selector1048ProceduralRoute.Load(gameDataDirectory, 3)
                && selector1048ProceduralRoute.PositionPlayerAtTile(0x2a),
            "find selector-1048 procedural child coordinates");
    Require(restoredGraphState.SaveGame(graphSavePath),
            "save graph before procedural child entry");
    positionCurrentGraphBoard(
        static_cast<std::uint16_t>(selector1048ProceduralRoute.PlayerColumn()),
        static_cast<std::uint16_t>(selector1048ProceduralRoute.PlayerRow()),
        "position selector-1048 procedural child edge");
    Require(restoredGraphState.LoadGame(graphSavePath)
                && restoredGraphState.ActivateTransporter(false)
                && restoredGraphState.CurrentLevel() == 11,
            "enter a procedural child board");
    constexpr std::array<std::pair<int, int>, 8> adjacentSteps = {{
        {0, -1}, {1, 0}, {0, 1}, {-1, 0}, {1, -1}, {1, 1}, {-1, 1}, {-1, -1}}};
    const auto openStep = std::find_if(adjacentSteps.begin(), adjacentSteps.end(),
        [&](const auto& step) { return restoredGraphState.MovePlayer(step.first, step.second); });
    Require(openStep != adjacentSteps.end() && restoredGraphState.CurrentLevel() == 11,
            "advance away from the procedural child entrance");
    const std::uint32_t retainedChildTurn = restoredGraphState.CurrentTurn();

    int proceduralReturnColumn = -1;
    int proceduralReturnRow = -1;
    for (int column = 0; column < 40 && proceduralReturnColumn < 0; ++column) {
        for (int row = 0; row < 36; ++row) {
            if (restoredGraphState.CurrentTileAt(column, row) == 0x23) {
                proceduralReturnColumn = column;
                proceduralReturnRow = row;
                break;
            }
        }
    }
    Require(proceduralReturnColumn >= 0, "locate generated procedural child return");
    Require(restoredGraphState.SaveGame(graphSavePath),
            "save graph before procedural child return");
    positionCurrentGraphBoard(static_cast<std::uint16_t>(proceduralReturnColumn),
                              static_cast<std::uint16_t>(proceduralReturnRow),
                              "position procedural child return");
    Require(restoredGraphState.LoadGame(graphSavePath),
            "reload procedural child return fixture");
    Require(restoredGraphState.ActivateTransporter(true),
            "activate procedural child return");
    Require(restoredGraphState.CurrentLevel() == 3,
            "procedural child returns to selector-1048");
    Require(restoredGraphState.SaveGame(graphSavePath),
            "persist cached procedural child");
    drain_storm::ApplicationState cachedGraphState;
    Require(cachedGraphState.LoadOriginalResources(executablePath)
                && cachedGraphState.LoadGame(graphSavePath)
                && cachedGraphState.ActivateTransporter(false)
                && cachedGraphState.CurrentLevel() == 11
                && cachedGraphState.CurrentTurn() == retainedChildTurn,
            "re-enter the same persisted procedural child instance");
    std::filesystem::remove(transitionSourcePath);
    std::filesystem::remove(graphSavePath);

    const std::filesystem::path highScorePath = gameDataDirectory / L"high-score-roundtrip.bin";
    drain_storm::HighScoreTable highScores;
    Require(highScores.Qualifies(0), "zero score qualifies above original empty -1 slot");
    Require(!highScores.Qualifies(0x80000000u)
                && !highScores.Qualifies(4'000'000'000u),
            "wrapped negative Win16 long scores do not qualify");
    Require(highScores.Submit("ZERO", 0, 1), "submit original zero high score");
    Require(highScores.Submit("ALPHA", 1200, 4), "submit first high score");
    Require(highScores.Submit("BETA", 9000, 2), "submit leading high score");
    Require(!highScores.Submit("INVALID", 10000, 5), "reject invalid high-score Skill");
    Require(highScores.Entries()[0].name == "BETA" && highScores.Entries()[1].name == "ALPHA",
            "high scores sort descending");
    drain_storm::HighScoreTable nameScores;
    Require(nameScores.Submit("ABCDEFGHIJKLMNO", 0, 1)
                && nameScores.Entries()[0].name == "ABCDEFGHIJKL"
                && nameScores.Submit("", 1, 1)
                && nameScores.Entries()[0].occupied
                && nameScores.Entries()[0].name.empty()
                && nameScores.Format().find(L"No expeditions") == std::wstring::npos,
            "high-score records retain twelve-character and blank original names");
    const std::filesystem::path blankNameScorePath =
        gameDataDirectory / L"high-score-blank-name-roundtrip.bin";
    Require(nameScores.Save(blankNameScorePath), "save blank-name high-score table");
    drain_storm::HighScoreTable restoredNameScores;
    Require(restoredNameScores.Load(blankNameScorePath)
                && restoredNameScores.Entries()[0].occupied
                && restoredNameScores.Entries()[0].name.empty(),
            "DSH64 count preserves occupied blank-name records");
    std::filesystem::remove(blankNameScorePath);
    drain_storm::HighScoreTable heroScores;
    Require(heroScores.Submit("REGULAR", 9'000'000, 2)
                && heroScores.Submit("HERO", 0, 1, true)
                && heroScores.Entries()[0].name == "HERO"
                && heroScores.Entries()[0].heroBonus
                && heroScores.Format().find(L"10000000") != std::wstring::npos,
            "FUN_1010_ed28 applies the recovered ten-million-point hero ranking bonus");
    const std::filesystem::path heroScorePath =
        gameDataDirectory / L"high-score-hero-roundtrip.bin";
    Require(heroScores.Save(heroScorePath), "save DSH64 hero-bonus table");
    drain_storm::HighScoreTable restoredHeroScores;
    Require(restoredHeroScores.Load(heroScorePath)
                && restoredHeroScores.Entries()[0].heroBonus
                && restoredHeroScores.Entries()[0].name == "HERO",
            "DSH64 version three preserves hero ranking metadata");
    std::filesystem::remove(heroScorePath);
    Require(highScores.Save(highScorePath), "save high scores");
    drain_storm::HighScoreTable restoredHighScores;
    Require(restoredHighScores.Load(highScorePath), "load high scores");
    Require(restoredHighScores.Entries()[0].score == 9000
                && restoredHighScores.Entries()[0].skill == 2
                && restoredHighScores.Format().find(L"Skill 2") != std::wstring::npos
                && restoredHighScores.Format().find(L"Skill 4") != std::wstring::npos,
            "high-score round trip and display");
    {
        std::ofstream legacy(highScorePath, std::ios::binary | std::ios::trunc);
        constexpr std::array<char, 5> signature = {'D', 'S', 'H', '6', '4'};
        const std::uint16_t versionOne = 1;
        const std::uint16_t oneRecord = 1;
        std::array<char, 32> legacyName{};
        std::copy_n("BETA", 4, legacyName.begin());
        const std::uint32_t legacyScore = 9000;
        const std::uint16_t impossibleLegacySelector = 12;
        legacy.write(signature.data(), static_cast<std::streamsize>(signature.size()));
        legacy.write(reinterpret_cast<const char*>(&versionOne), sizeof(versionOne));
        legacy.write(reinterpret_cast<const char*>(&oneRecord), sizeof(oneRecord));
        legacy.write(legacyName.data(), static_cast<std::streamsize>(legacyName.size()));
        legacy.write(reinterpret_cast<const char*>(&legacyScore), sizeof(legacyScore));
        legacy.write(reinterpret_cast<const char*>(&impossibleLegacySelector),
                     sizeof(impossibleLegacySelector));
        Require(legacy.good(), "write DSH64 version-one migration fixture");
    }
    Require(restoredHighScores.Load(highScorePath)
                && restoredHighScores.Entries()[0].skill == 1
                && restoredHighScores.Format().find(L"Skill EZ") != std::wstring::npos,
            "migrate legacy board-selector metadata to original EZ Skill");
    Require(restoredHighScores.Save(highScorePath), "resave migrated DSH64 version-two table");
    {
        std::fstream invalidSkill(highScorePath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint16_t invalid = 5;
        invalidSkill.seekp(45);
        invalidSkill.write(reinterpret_cast<const char*>(&invalid), sizeof(invalid));
    }
    Require(!restoredHighScores.Load(highScorePath), "reject invalid DSH64 Skill metadata");
    Require(highScores.Save(highScorePath), "restore signed high-score fixture");
    {
        std::fstream invalidScore(highScorePath, std::ios::binary | std::ios::in | std::ios::out);
        const std::uint32_t wrappedNegative = 0x80000000u;
        invalidScore.seekp(41);
        invalidScore.write(reinterpret_cast<const char*>(&wrappedNegative),
                           sizeof(wrappedNegative));
    }
    Require(!restoredHighScores.Load(highScorePath),
            "reject impossible wrapped-negative DSH64 score");
    Require(highScores.Save(highScorePath), "restore high-score corruption fixture");
    {
        std::fstream corrupt(highScorePath, std::ios::binary | std::ios::in | std::ios::out);
        corrupt.put('X');
    }
    Require(!restoredHighScores.Load(highScorePath), "reject corrupt high-score signature");
    std::filesystem::remove(highScorePath);

    drain_storm::GameData invalidLevel;
    Require(!invalidLevel.Load(gameDataDirectory, 0), "reject level zero");
    Require(!invalidLevel.Load(gameDataDirectory, 26), "reject level twenty-six");

    std::cout << "All game-data checks passed.\n";
    return EXIT_SUCCESS;
}
