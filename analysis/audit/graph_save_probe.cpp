#include <iostream>
#include <sstream>
#include "application_state.h"

// Diagnostic navigation uses real connection tiles but does not simulate
// fighting to reach them. Saves are confined to a newly created temp folder.
int wmain(int argc, wchar_t** argv) {
    if (argc != 2) return 2;
    const std::filesystem::path assets(argv[1]);
    const auto folder = std::filesystem::temp_directory_path()
        / (L"drain-audit-graph-" + std::to_wstring(GetCurrentProcessId())
           + L"-" + std::to_wstring(GetTickCount64()));
    if (!std::filesystem::create_directory(folder)) return 3;
    const auto save = folder / L"probe.vkw";
    struct Step { std::uint16_t tile; std::size_t destination; };
    constexpr Step steps[] = {
        {0x29,2}, {0x23,3}, {0x27,6}, {0x27,5}, {0x27,8},
        {0x23,5}, {0x24,7}, {0x28,1}, {0x23,7}, {0x27,5},
        {0x27,8}, {0x23,5}, {0x23,6}, {0x23,3}, {0x28,7}, {0x23,3}
    };
    unsigned checkpoints = 0, loadFailures = 0, navigationFailures = 0;
    unsigned payloadChanges = 0, identityFailures = 0;
    for (int skill = 1; skill <= 4; ++skill) for (unsigned seed = 1; seed <= 16; ++seed) {
        drain_storm::ApplicationState app;
        app.resourceDirectory_ = assets.parent_path(); app.soundEnabled_ = false;
        if (!app.gameData_.Load(assets, 3, seed, skill)) return 4;
        unsigned stepIndex = 0;
        for (const auto step : steps) {
            if (!app.gameData_.PositionPlayerAtTile(step.tile)
                || !app.ActivateTransporter(step.tile <= 0x26)
                || app.CurrentLevel() != step.destination) {
                ++navigationFailures;
                std::cout << "navigation-failed skill=" << skill << " seed=" << seed
                    << " step=" << stepIndex << '\n';
                break;
            }
            if (stepIndex == 4) app.gameData_.level_[1][1] = 0x7e;
            if (stepIndex == 10) identityFailures += app.gameData_.level_[1][1] != 0x7e;
            std::ostringstream before(std::ios::binary);
            app.gameData_.WriteGame(before);
            if (!app.SaveGame(save) || !app.LoadGame(save)) {
                ++loadFailures;
                std::cout << "save-load-failed skill=" << skill << " seed=" << seed
                    << " step=" << stepIndex << " class=" << app.CurrentLevel() << '\n';
                break;
            }
            std::ostringstream after(std::ios::binary);
            app.gameData_.WriteGame(after);
            payloadChanges += before.str() != after.str();
            navigationFailures += app.boardHistory_.size() != app.currentBoardPath_.size();
            ++checkpoints; ++stepIndex;
        }
        identityFailures += !app.boardHistory_.empty();
    }
    unsigned mazeChecks=0, mazeFailures=0, factoryFlagFailures=0;
    constexpr Step mazeSteps[] = {{0x27,14},{0x27,15},{0x27,16},
        {0x23,15},{0x23,14},{0x23,13},{0x28,11},{0x23,13},
        {0x27,14},{0x27,15},{0x27,16}};
    for (int skill=1; skill<=4; ++skill) for (unsigned seed=1; seed<=16; ++seed) {
        drain_storm::ApplicationState app;
        app.resourceDirectory_=assets.parent_path(); app.soundEnabled_=false;
        if (!app.gameData_.Load(assets,13,seed,skill)) return 4;
        app.gameData_.genericRuntimeFlags_ |= 0x10;
        unsigned stepIndex=0;
        for (const auto step : mazeSteps) {
            if (!app.gameData_.PositionPlayerAtTile(step.tile)
                || !app.ActivateTransporter(step.tile<=0x26)
                || app.CurrentLevel()!=step.destination) { ++mazeFailures; break; }
            if (stepIndex==2) {
                app.gameData_.level_[1][1]=0x7e;
                app.gameData_.genericRuntimeFlags_ |= 0x20;
            }
            if (stepIndex==10 && app.gameData_.level_[1][1]!=0x7e) ++mazeFailures;
            const auto expectedFlags=stepIndex>=2 ? 0x30 : 0x10;
            factoryFlagFailures += (app.gameData_.genericRuntimeFlags_ & 0x30)!=expectedFlags;
            std::ostringstream before(std::ios::binary);
            app.gameData_.WriteGame(before);
            if (!app.SaveGame(save) || !app.LoadGame(save)) { ++mazeFailures; break; }
            std::ostringstream after(std::ios::binary);
            app.gameData_.WriteGame(after);
            mazeFailures += before.str()!=after.str();
            ++mazeChecks; ++stepIndex;
        }
    }
    std::cout << "maze checkpoints=" << mazeChecks << " failures=" << mazeFailures
        << " factory_flag_failures=" << factoryFlagFailures << '\n';
    const bool removedFile = std::filesystem::remove(save);
    const bool removedFolder = std::filesystem::remove(folder);
    std::cout << "graph checkpoints=" << checkpoints << " load_failures=" << loadFailures
        << " navigation_failures=" << navigationFailures << " payload_changes=" << payloadChanges
        << " identity_failures=" << identityFailures << " temp_cleanup="
        << (removedFile && removedFolder) << '\n';
    return 0;
}
