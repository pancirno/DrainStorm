// Parity audit harness: compiles unchanged reconstruction sources in isolation.
// Private access is used only to arrange isolated, deterministic engine states.
#include <array>
#include <cstdint>
#include <filesystem>
#include <iosfwd>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <windows.h>
#define private public
#include "application_state.h"
#undef private

using drain_storm::GameData;

int main() {
    GameData base;
    if (!base.Load(L"C:/Projekty/ds/reconstructed/assets/original/game-data", 17)) return 1;
    for (auto& column : base.level_) column.fill(0);
    base.actors_.clear();
    base.overlays_.clear();
    base.projectiles_.clear();
    base.inventory_ = {{0, 0}};
    base.playerColumn_ = 10;
    base.playerRow_ = 10;

    auto game = base;
    game.actors_ = {{0, 12, 11, 0, 0, 1}};
    game.projectiles_ = {{4, 11, 10, 1, 0}};
    game.AdvanceProjectiles();
    std::cout << "Fireball adjacent type-0 survivor count: " << game.actors_.size()
              << " (original: 0)\n";

    game = base;
    game.level_[12][10] = 3;
    game.projectiles_ = {{4, 11, 10, 1, 0}};
    game.AdvanceProjectiles();
    std::cout << "Fireball impact terrain: " << game.level_[12][10]
              << " (original: unchanged tile 3)\n";

    game = base;
    game.actors_ = {{2, 12, 10, 0, 0, 1}};
    game.projectiles_ = {{0, 11, 10, 1, 0}};
    game.AdvanceProjectiles();
    std::cout << "Bubble hitting immune type-2 remaining projectiles: "
              << game.projectiles_.size() << " (original: 0)\n";

    game = base;
    game.actors_ = {{0, 9, 10, 1, 0, 3}};
    game.projectiles_ = {{4, 10, 10, 1, 0}};
    game.AdvanceActors();
    game.ResolveActorCollision();
    std::cout << "Actor entering player/fireball square health: " << game.health_
              << " (original: 2; starts at 3)\n";

    game = base;
    game.SetRootBoard(false);
    std::cout << "Playable child mouse wait accepted: " << game.MovePlayerToward(10, 10)
              << " (original playable-board gate: accepted)\n";

    game = base;
    game.levelNumber_ = 2;
    game.playerColumn_ = 23;
    game.playerRow_ = 26;
    game.casinoStatus_ = 4;
    game.UseSelectedItem(1, 0);
    std::cout << "Rejected casino bet turn count: " << game.turnCounter_
              << " (original: 1)\n";

    game = base;
    game.ResolveOverlayEffect(4);
    std::wcout << L"Overlay-4 key message: " << game.TakeMessage()
               << L" (original: Moon Key)\n";

    for (std::uint32_t seed = 1; seed < 1000; ++seed) {
        game = base;
        game.randomState_ = seed;
        game.ResolveOverlayEffect(1);
        if (game.inventory_.size() > 1 && game.inventory_[1].type == 1) {
            std::cout << "Depth-one triple-cannon drop with seed " << seed
                      << " (original: prohibited at depth one)\n";
            break;
        }
    }

    drain_storm::ApplicationState state;
    for (int index = 0; index < 21; ++index) state.RecordGameplayMessage(L"message");
    const auto cursorAfterBatch = state.messageHistoryCursor_ - (21u - 1u);
    std::cout << "21-message batch computed cursor: " << cursorAfterBatch
              << ", retained count: " << state.messageHistory_.size()
              << " (out-of-range; deliberately not dereferenced)\n";
}
