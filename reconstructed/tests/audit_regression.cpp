#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include "application_state.h"

// Exercise the production Win32 routing without showing windows or dialogs.
namespace drain_storm {
ATOM RegisterMainWindowClass(HINSTANCE, HICON);
ATOM RegisterChildWindowClass(HINSTANCE, const wchar_t*, WNDPROC, HBRUSH);
LRESULT CALLBACK ArsenalWindowProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK BoardWindowProcedure(HWND, UINT, WPARAM, LPARAM);
HWND CreateMainWindow(HINSTANCE, int, ApplicationState&);
}

namespace {
void Require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAILED: " << message << '\n';
        std::exit(EXIT_FAILURE);
    }
}
}

int wmain(int argc, wchar_t** argv) {
    Require(argc == 2, "asset directory argument");
    drain_storm::GameData base;
    Require(base.Load(argv[1], 17), "load descriptors");
    for (auto& column : base.level_) column.fill(0);
    base.actors_.clear();
    base.overlays_.clear();
    base.projectiles_.clear();
    base.inventory_ = {{0, 0}};
    base.playerColumn_ = 10;
    base.playerRow_ = 10;

    {
        auto casino = base;
        casino.levelNumber_ = 2;
        casino.difficulty_ = 2;
        casino.playerColumn_ = 20;
        casino.playerRow_ = 4;
        casino.level_[21][4] = 3;
        const auto turn = casino.turnCounter_;
        Require(casino.MovePlayer(1, -1) && casino.playerColumn_ == 20
                    && casino.playerRow_ == 4 && casino.turnCounter_ == turn + 1,
                "casino force line accepts a stationary turn when horizontal deflection is blocked");
    }
    for (const int level : {3, 5}) {
        const auto mapPath = std::filesystem::path(argv[1]) / L"levels"
            / (L"level_0" + std::to_wstring(level) + L".map");
        std::ifstream map(mapPath, std::ios::binary);
        std::array<char, drain_storm::GameData::kBoardRows
            * (drain_storm::GameData::kBoardColumns + 1)> bytes{};
        map.read(bytes.data(), static_cast<std::streamsize>(bytes.size()));
        Require(map.gcount() == static_cast<std::streamsize>(bytes.size()),
                "read original map actor-order fixture");
        drain_storm::GameData board;
        Require(board.Load(argv[1], level), "construct original map actor-order fixture");
        std::size_t actorIndex = 0;
        for (std::size_t column = 0; column < drain_storm::GameData::kBoardColumns; ++column) {
            for (std::size_t row = 0; row < drain_storm::GameData::kBoardRows; ++row) {
                const char marker = bytes[row * (drain_storm::GameData::kBoardColumns + 1) + column];
                const int type = marker == 'g' ? 0 : marker == 'm' ? 1
                    : marker == 'd' ? 2 : marker == 'z' ? 4 : -1;
                if (type < 0) continue;
                Require(actorIndex < board.actors_.size()
                            && board.actors_[actorIndex].column == column
                            && board.actors_[actorIndex].row == row
                            && board.actors_[actorIndex].type == type,
                        "map actors retain original column-major creation order");
                ++actorIndex;
            }
        }
        Require(actorIndex != 0, "map actor-order fixture contains actor markers");
    }
    {
        drain_storm::GameData moon;
        Require(moon.Load(argv[1], 6, 1, 2), "construct Moon Room pickup stack");
        moon.playerColumn_ = 31;
        moon.playerRow_ = 10;
        moon.pendingMessages_.clear();
        moon.ResolveOverlaysAtPlayer();
        Require(!moon.pendingMessages_.empty()
                    && moon.pendingMessages_.front() == L"You gained an extra life!",
                "linked-list head order collects newest Moon Room pickup first");
    }
    for (int boardClass : {4, 17}) {
        for (const auto [playerX, playerY, stepX] : {
                 std::tuple{9, 10, 1}, std::tuple{12, 10, -1},
                 std::tuple{9, 11, 1}, std::tuple{12, 11, -1}}) {
            auto chest = base;
            chest.levelNumber_ = boardClass;
            chest.playerColumn_ = playerX;
            chest.playerRow_ = playerY;
            chest.level_[10][10] = 0x7f; chest.level_[11][10] = 0x80;
            chest.level_[10][11] = 0x81; chest.level_[11][11] = 0x82;
            chest.inventory_ = {{19, 1}};
            chest.selectedItem_ = 0;
            chest.randomState_ = 19;
            auto expected = chest;
            std::array<int, 4> expectedLoot{};
            for (int column = 0; column < 2; ++column)
                for (int row = 0; row < 2; ++row)
                    expectedLoot[column * 2 + row] = expected.RollObjectTreasure(19);
            Require(chest.UseSelectedItem(stepX, 0), "open each selected chest corner");
            constexpr std::uint16_t effects[2][2] = {{0x19, 0x1a}, {0x1b, 0x1c}};
            for (int column = 0; column < 2; ++column) {
                for (int row = 0; row < 2; ++row) {
                    const int x = 10 + column, y = 10 + row;
                    Require(std::any_of(chest.overlays_.begin(), chest.overlays_.end(),
                            [=](const drain_storm::BoardOverlay& overlay) {
                                return overlay.column == x && overlay.row == y
                                    && overlay.type == effects[row][column];
                            }), "Bronze Key effects keep original chest quadrant orientation");
                    const int loot = expectedLoot[column * 2 + row];
                    Require(loot < 0 || std::any_of(chest.overlays_.begin(), chest.overlays_.end(),
                            [=](const drain_storm::BoardOverlay& overlay) {
                                return overlay.column == x && overlay.row == y
                                    && overlay.type == loot;
                            }), "Bronze Key loot follows original column-major RNG order");
                }
            }
        }
    }
    {
        bool foundDropSeed = false;
        for (std::uint32_t seed = 1; seed <= 500 && !foundDropSeed; ++seed) {
            auto preview = base;
            preview.randomState_ = seed;
            if (preview.NextRandom(10) != 2 || preview.NextRandom(10) == 2) continue;
            auto blast = base;
            blast.randomState_ = seed;
            blast.actors_ = {{2, 11, 9, 0, 0, 0}, {2, 9, 11, 0, 0, 0}};
            blast.ApplyRadioExplosion(10, 10);
            const auto hasDrop = [&blast](int x, int y) {
                return std::any_of(blast.overlays_.begin(), blast.overlays_.end(),
                    [=](const drain_storm::BoardOverlay& overlay) {
                        return overlay.type == 1 && overlay.column == x && overlay.row == y;
                    });
            };
            Require(hasDrop(11, 9) && !hasDrop(9, 11),
                    "radio blast visits northeast lieutenant before southwest");
            foundDropSeed = true;
        }
        Require(foundDropSeed, "find radio-blast ordered-drop seed");
    }
    {
        auto chain = base;
        chain.actors_ = {{0, 11, 10, 0, 0, 0}, {0, 12, 10, 0, 0, 0}};
        chain.projectiles_ = {{2, 10, 10, 1, 0}};
        chain.AdvanceProjectiles();
        Require(chain.actors_.empty() && chain.projectiles_.size() == 7
                    && std::none_of(chain.projectiles_.begin(), chain.projectiles_.end(),
                        [](const drain_storm::Projectile& projectile) {
                            return projectile.column == 11 && projectile.row == 10;
                        }), "missile fragments move and chain-hit actors on the impact turn");

        auto terrain = base;
        terrain.level_[11][10] = 3;
        terrain.actors_ = {{0, 12, 10, 0, 0, 0}};
        terrain.projectiles_ = {{2, 10, 10, 1, 0}};
        terrain.AdvanceProjectiles();
        Require(terrain.actors_.empty(),
                "terrain-impact missile fragments collide in the same update");

        auto capped = base;
        capped.actors_ = {{0, 11, 10, 0, 0, 0}};
        capped.projectiles_.assign(149, {0, 1, 1, 0, 0});
        capped.projectiles_.push_back({2, 10, 10, 1, 0});
        capped.AdvanceProjectiles();
        Require(capped.projectiles_.size() == 150
                    && capped.projectiles_.back().column == 10
                    && capped.projectiles_.back().row == 9,
                "missile fragmentation respects the live 150-projectile cap");
    }
    {
        auto smart = base;
        smart.inventory_ = {{13, 1}};
        smart.selectedItem_ = 0;
        smart.actors_ = {{3, 10, 10, 0, 0, 1}, {3, 30, 10, 0, 0, 1}};
        smart.playerColumn_ = 20;
        smart.playerRow_ = 20;
        smart.randomState_ = 1;
        Require(smart.UseSelectedItem(1, 0), "detonate common Smart Bomb");
        const auto left = std::count_if(smart.actors_.begin(), smart.actors_.end(),
            [](const drain_storm::BoardActor& actor) { return actor.column < 20; });
        const auto right = std::count_if(smart.actors_.begin(), smart.actors_.end(),
            [](const drain_storm::BoardActor& actor) { return actor.column > 20; });
        Require(left == 2 && right == 7,
                "Smart Bomb splits right captain first and assigns RNG in reverse order");

        auto capped = base;
        capped.inventory_ = {{13, 1}};
        capped.selectedItem_ = 0;
        capped.actors_.assign(298, {0, 1, 1, 0, 0, 1});
        capped.actors_.insert(capped.actors_.begin(), {3, 10, 10, 0, 0, 1});
        capped.actors_.push_back({3, 30, 10, 0, 0, 1});
        capped.randomState_ = 1;
        Require(capped.UseSelectedItem(1, 0), "detonate actor-cap Smart Bomb");
        Require(std::none_of(capped.actors_.begin(), capped.actors_.end(),
                    [](const drain_storm::BoardActor& actor) {
                        return actor.column > 20;
                    }), "captain at full capacity cannot append lieutenants before its removal");
    }
    {
        auto mines = base;
        mines.randomState_ = 1;
        mines.actors_ = {{3, 10, 10, 0, 0, 0}};
        mines.overlays_ = {{0x24, 10, 10}, {0x24, 11, 10}};
        mines.ResolveTriggeredMines();
        Require(mines.actors_.empty(),
                "second overlapping mine can hit lieutenants split by the first blast");
        auto single = base;
        single.randomState_ = 1;
        single.actors_ = {{3, 10, 10, 0, 0, 0}};
        single.overlays_ = {{0x24, 10, 10}};
        single.ResolveTriggeredMines();
        Require(single.actors_.size() == 7,
                "new lieutenants are excluded from their spawning square's actor snapshot");
    }

    {
        drain_storm::GameData gauntlet;
        Require(gauntlet.Load(argv[1], 4, 2), "construct Gauntlet save fixture");
        const auto offboard = std::count_if(gauntlet.overlays_.begin(), gauntlet.overlays_.end(),
            [](const drain_storm::BoardOverlay& overlay) {
                return overlay.row >= drain_storm::GameData::kBoardRows;
            });
        Require(offboard != 0, "Gauntlet fixture includes original off-board pickup");
        std::stringstream bytes;
        Require(gauntlet.WriteGame(bytes), "save Gauntlet with off-board pickup");
        drain_storm::GameData loaded;
        Require(loaded.Load(argv[1], 4) && loaded.ReadGame(bytes)
                    && loaded.overlays_.size() == gauntlet.overlays_.size(),
                "load Gauntlet without moving or discarding off-board pickups");
    }
    {
        drain_storm::GameData plant;
        Require(plant.Load(argv[1], 1, 1, 3), "construct Power Plant gate fixture");
        plant.AdvanceBoardRuntime(false, 0, 0, false);
        Require(!plant.levelOneSecurityGateOpened_ && !plant.levelOneCornerPortalsActivated_
                    && plant.level_[3][3] == 0x1f,
                "intact Power Plant gates remain dormant");
        plant.level_[23][29] = 0;
        plant.AdvanceBoardRuntime(false, 0, 0, false);
        Require(plant.levelOneSecurityGateOpened_ && !plant.levelOneCornerPortalsActivated_
                    && plant.StructureSpawnParameters() == std::pair{12, 5},
                "security breach releases defenders without activating portals");
        plant.level_[21][13] = 0;
        plant.level_[2][2] = 0;
        plant.AdvanceBoardRuntime(false, 0, 0, false);
        Require(plant.levelOneCornerPortalsActivated_ && plant.level_[2][2] == 0x10
                    && plant.level_[3][3] == 0x1d
                    && plant.StructureSpawnParameters() == std::pair{12, 8},
                "second breach reconstructs complete corner portals once");
        std::stringstream bytes;
        Require(plant.WriteGame(bytes), "save independent Power Plant gate flags");
        drain_storm::GameData restored;
        Require(restored.Load(argv[1], 1) && restored.ReadGame(bytes)
                    && restored.levelOneSecurityGateOpened_
                    && restored.levelOneCornerPortalsActivated_,
                "reload both Power Plant gate flags");
        drain_storm::GameData late;
        Require(late.Load(argv[1], 1, 1, 3), "construct late Power Plant breach fixture");
        late.actors_ = {{0, 10, 10, 0, -1, 2}};
        late.playerColumn_ = 1; late.playerRow_ = 19;
        late.AdvanceBoardRuntime(false, 0, 0, false);
        Require(late.levelOneArenaEntered_
                    && late.StructureSpawnParameters() == std::pair{9, 1}
                    && late.actors_[0].deltaColumn == 0 && late.actors_[0].deltaRow == -1,
                "arena lever updates spawn words before ring-turn branch");
        late.playerColumn_ = 20; late.playerRow_ = 20;
        late.level_[23][29] = 0;
        late.AdvanceBoardRuntime(false, 0, 0, false);
        Require(late.StructureSpawnParameters() == std::pair{12, 5},
                "late security breach overrides earlier arena spawn words");
        late.level_[21][13] = 0;
        late.AdvanceBoardRuntime(false, 0, 0, false);
        Require(late.StructureSpawnParameters() == std::pair{12, 8},
                "late corner breach overrides group size even inside arena phase");
        std::stringstream lateSave;
        Require(late.WriteGame(lateSave) && restored.ReadGame(lateSave)
                    && restored.StructureSpawnParameters() == std::pair{12, 8},
                "save retains last-writer Power Plant spawn state");
        late.energySphereDestroyed_ = true;
        late.levelOneFirstTargetActivationTurn_ = 10;
        late.turnCounter_ = 15;
        late.level_[late.levelOneTargetColumns_[0]][late.levelOneTargetRows_[0]] = 0x87;
        auto expected = late;
        expected.NextRandom(2);
        late.AdvanceBoardRuntime(false, 0, 0, false);
        Require(late.randomState_ == expected.randomState_,
                "lower boss consumes wake-up roll before its twelve-turn delay gate");
    }
    {
        drain_storm::GameData plant;
        Require(plant.Load(argv[1], 1) && plant.DamageBoardSquare(12, 12, 1)
                    && plant.energySphereDestroyed_,
                "destroying the actual Energy Sphere sets global spawn latch");
        auto destination = base;
        destination.AdoptPlayerStateFrom(plant);
        destination.level_[5][5] = 0x22;
        const auto randomBefore = destination.randomState_;
        const auto actorsBefore = destination.actors_.size();
        destination.SpawnStructureActors();
        Require(destination.energySphereDestroyed_
                    && destination.randomState_ == randomBefore
                    && destination.actors_.size() == actorsBefore,
                "sphere latch suppresses the entire spawner scan across boards");
        std::stringstream bytes;
        Require(destination.WriteGame(bytes), "save global sphere latch");
        drain_storm::GameData restored;
        Require(restored.Load(argv[1], 17) && restored.ReadGame(bytes)
                    && restored.energySphereDestroyed_,
                "reload global sphere latch on another board");
    }
    {
        drain_storm::GameData hub;
        Require(hub.Load(argv[1], 3, 1, 2), "construct actual hub map");
        hub.playerColumn_ = 38;
        hub.playerRow_ = 19;
        const auto barrierCount = [&hub]() {
            int count = 0;
            for (int column = 30; column <= 34; ++column)
                for (int row = 19; row <= 23; ++row)
                    count += hub.level_[column][row] == 0x5f;
            return count;
        };
        Require(hub.level_[30][28] == 0x38 && barrierCount() != 0,
                "hub lever fixture begins with sealed machinery");
        hub.AdvanceBoardRuntime(false, 0, 0, false);
        Require(hub.level_[30][28] == 0x36 && hub.level_[30][29] == 0
                    && hub.level_[31][30] == 0 && barrierCount() == 0
                    && hub.level_[32][21] == 0x18,
                "hub lever opens original rectangle and releases enclosure");
        Require(std::none_of(hub.actors_.begin(), hub.actors_.end(),
                    [](const drain_storm::BoardActor& actor) {
                        return actor.column == 21 && actor.row == 32;
                    }), "hub lever never releases actors at transposed coordinates");
        Require(std::any_of(hub.actors_.begin(), hub.actors_.end(),
                    [](const drain_storm::BoardActor& actor) {
                        return actor.column == 32 && actor.row == 21;
                    }), "hub lever releases actors at original structure center");
    }
    for (int skill = 1; skill <= 4; ++skill) {
        const auto check = [&](std::size_t level, std::pair<int, int> spawn,
                               int breakDivisor, const char* message) {
            drain_storm::GameData board;
            Require(board.Load(argv[1], level, 1, skill)
                        && board.StructureSpawnParameters() == spawn
                        && board.ActorBreakDivisor() == breakDivisor, message);
        };
        check(1, {0, 1}, 5, "Power Plant initial difficulty parameters");
        check(2, {4, 1}, 20, "class-two initial difficulty parameters");
        check(4, {(skill + 3) * 2, 1}, 20, "Gauntlet initial difficulty parameters");
        check(5, {skill + 10, 1}, 85 - 10 * skill,
              "Breakout initial difficulty parameters");
        check(7, {3, 3}, 15, "Statue Room initial difficulty parameters");
        check(8, {6, 1}, 25, "Chest Room initial difficulty parameters");
        check(9, {4, 1}, 75, "Xoid Fortress initial difficulty parameters");
        check(10, {skill * 3, skill + 2}, 20,
              "Casino initial difficulty parameters");
        check(12, {0, 1}, 100, "final arena counts all 120 initial structures");
    }
    for (const auto [remaining, expected] : std::array<std::pair<int, std::pair<int, int>>, 6>{
            {{119, {1, 1}}, {99, {2, 1}}, {59, {3, 1}},
             {29, {4, 1}}, {19, {6, 1}}, {9, {10, 3}}}}) {
        auto arena = base;
        Require(arena.Load(argv[1], 12, 1, 2), "load final arena threshold fixture");
        int removed = 0;
        for (int column = 3; column <= 36 && removed < 120 - remaining; column += 3)
            for (int row = 2; row <= 29 && removed < 120 - remaining; row += 3) {
                arena.level_[column][row] = 0;
                ++removed;
            }
        Require(arena.StructureSpawnParameters() == std::pair{0, 1},
            "final arena destruction does not change the previous actor turn");
        arena.AdvanceBoardRuntime(false, 0, 0, false);
        Require(arena.StructureSpawnParameters() == expected
            && arena.ActorBreakDivisor() == 100,
            "final arena stores original post-actor spawn tier without altering break divisor");
        if (remaining == 9) {
            std::stringstream save;
            Require(arena.WriteGame(save), "persist final arena escalation");
            auto restored = base;
            Require(restored.ReadGame(save)
                && restored.StructureSpawnParameters() == expected,
                "final arena stored spawn tier survives save and reload");
            auto oldBytes = save.str(); oldBytes[5] = 30;
            oldBytes.resize(oldBytes.size() - 5 * sizeof(std::uint16_t));
            std::stringstream oldSave(oldBytes);
            Require(restored.ReadGame(oldSave)
                && restored.StructureSpawnParameters() == expected,
                "version-30 final arena save derives its missing spawn tier");
        }
    }
    for (int roll = 1; roll <= 5; ++roll) {
        bool foundSeed = false;
        for (std::uint32_t seed = 1; seed <= 100 && !foundSeed; ++seed) {
            auto preview = base;
            preview.randomState_ = seed;
            if (preview.NextRandom(4) == 1 || preview.NextRandom(5) != roll)
                continue;
            auto pickup = base;
            pickup.randomState_ = seed;
            pickup.ResolveOverlayEffect(3);
            constexpr std::array<std::uint16_t, 5> rewards = {13, 12, 7, 11, 8};
            Require(std::any_of(pickup.inventory_.begin(), pickup.inventory_.end(),
                        [roll, &rewards](const drain_storm::InventoryItem& item) {
                            return item.type == rewards[roll - 1];
                        }), "type-three treasure grants original five-entry reward table");
            foundSeed = true;
        }
        Require(foundSeed, "find all five non-life type-three treasure rolls");
    }
    for (int strength : {1, 2, 3}) {
        auto chest = base;
        chest.level_[12][12] = 0x7f;
        chest.level_[13][12] = 0x80;
        chest.level_[12][13] = 0x81;
        chest.level_[13][13] = 0x82;
        Require(chest.DamageBoardSquare(13, 13, strength),
                "explosive impact accepts any chest quadrant");
        for (int x = 12; x <= 13; ++x)
            for (int y = 12; y <= 13; ++y)
                Require(chest.level_[x][y] == 0,
                        "explosive chest callback clears all four quadrants");
        Require(std::count_if(chest.overlays_.begin(), chest.overlays_.end(),
                    [](const drain_storm::BoardOverlay& overlay) {
                        return overlay.type == 0x14 && overlay.column >= 12
                            && overlay.column <= 13 && overlay.row >= 12
                            && overlay.row <= 13;
                    }) == 4, "explosive chest paints four hit effects");
    }
    {
        auto procedural = base;
        procedural.levelNumber_ = 17;
        for (std::uint32_t seed = 1; seed <= 64; ++seed) {
            procedural.randomState_ = seed;
            Require(procedural.RollObjectTreasure(4) == -1
                        && procedural.randomState_ == seed,
                    "procedural explosive chests neither reward nor draw RNG");
            auto expected = procedural;
            const int loot = expected.NextRandom(5) == 5 ? 2 : 1;
            Require(procedural.RollObjectTreasure(19) == loot
                        && procedural.randomState_ == expected.randomState_,
                    "procedural key chest uses five-outcome treasure table");
        }
    }
    {
        drain_storm::GameData depthNine;
        Require(depthNine.Load(argv[1], 25, 1, 4)
                    && depthNine.level_[31][19] == 0x7f,
                "depth-nine guarded placement preserves an earlier chest quadrant");
    }
    for (const auto [type, name] : {
             std::pair{1, L"Triple-Shot Cannon"}, std::pair{6, L"Bomb Burst"},
             std::pair{7, L"Missile Burst"}, std::pair{8, L"Radio Controlled Bomb"},
             std::pair{15, L"Fortune Cookie"}}) {
        auto pickup = base;
        pickup.AddRandomizedInventoryItem(static_cast<std::uint16_t>(type));
        Require(std::any_of(pickup.pendingMessages_.begin(),
                            pickup.pendingMessages_.end(),
                    [name](const std::wstring& message) {
                        return message.find(name) != std::wstring::npos;
                    }), "pickup notice uses original full item name");
    }
    {
        bool damagedSentinel = false;
        for (std::uint32_t seed = 1; seed <= 200 && !damagedSentinel; ++seed) {
            drain_storm::GameData maze;
            Require(maze.Load(argv[1], 16, seed, 2), "construct alien maze mine fixture");
            Require(maze.level_[7][29] != 0 && maze.levelSixteenChambersActive_[2],
                    "alien maze southwest sentinel initially intact");
            maze.overlays_.push_back({0x24, 5, 29});
            maze.AdvanceBoardRuntime(false, 0, 0, false);
            if (maze.level_[7][29] != 0) continue;
            Require(maze.levelSixteenChambersActive_[2],
                    "derived callback sees intact sentinel before same-turn mine blast");
            maze.AdvanceBoardRuntime(false, 0, 0, false);
            Require(!maze.levelSixteenChambersActive_[2],
                    "alien maze opens damaged quadrant on following callback");
            damagedSentinel = true;
        }
        Require(damagedSentinel, "exercise mine damage to alien maze sentinel");
    }
    {
        drain_storm::GameData full;
        Require(full.Load(argv[1], 7, 1, 2), "construct Statue Room actor-cap fixture");
        full.actors_.assign(300, {0, 1, 1, 0, 0, 0});
        full.levelSevenSpawnDivisor_ = 1;
        auto room = full;
        room.actors_.clear();
        room.AdvanceBoardRuntime(false, 0, 0, false);
        full.AdvanceBoardRuntime(false, 0, 0, false);
        Require(room.actors_.size() == 4 && full.actors_.size() == 300
                    && room.randomState_ == full.randomState_,
                "Statue Room special wave honors actor cap without omitting type RNG");
    }
    {
        drain_storm::HighScoreTable scores;
        const auto missing = std::filesystem::path(argv[1]) / L"audit-missing-sidecar.bin";
        Require(!std::filesystem::exists(missing) && scores.Load(missing),
                "missing high-score sidecar is normal first-run state");
        Require(!scores.Load(std::filesystem::path(argv[1])),
                "existing unreadable high-score target reports a read error");
        drain_storm::ApplicationState sidecar;
        sidecar.keyBindingPath_ = missing;
        Require(sidecar.LoadKeyBindings(), "missing key sidecar uses defaults normally");
        sidecar.keyBindingPath_ = std::filesystem::path(argv[1]);
        Require(!sidecar.LoadKeyBindings(), "existing unreadable key target is not missing");
        sidecar.highScoreReadError_ = true;
        Require(sidecar.HighScoresText() == L"Unable to read the high-score table.",
                "high-score read error remains visible in help content");
        for (int index = 0; index < 10; ++index)
            Require(scores.Submit("ORDINARY", 1'000'000u + index, 2),
                    "fill high-score table without bonuses");
        Require(!scores.Qualifies(20'000u)
                    && scores.Qualifies(20'000u, false, true)
                    && scores.Submit("SPHERE", 20'000u, 2, false, true)
                    && scores.Entries()[0].sphereBonus
                    && scores.Format().find(L"5020000") != std::wstring::npos,
                "Energy Sphere adds five million to ranking but not raw score");
        const auto scorePath = std::filesystem::path(argv[1]) / L"sphere-score-roundtrip.bin";
        Require(scores.Save(scorePath), "save Sphere-bonus high-score fixture");
        drain_storm::HighScoreTable restored;
        const bool roundTrip = restored.Load(scorePath)
            && restored.Entries()[0].sphereBonus
            && restored.Entries()[0].score == 20'000u;
        std::filesystem::remove(scorePath);
        Require(roundTrip, "persist Sphere bonus independently of raw score");

        drain_storm::ApplicationState ended;
        ended.resourceDirectory_ = std::filesystem::path(argv[1]).parent_path();
        ended.highScores_ = drain_storm::HighScoreTable{};
        for (int index = 0; index < 10; ++index)
            Require(ended.highScores_.Submit("ORDINARY", 1'000'000u + index, 2),
                    "fill death-submission high-score fixture");
        ended.gameData_ = base;
        ended.gameData_.score_ = 20'000u;
        ended.gameData_.health_ = 0;
        ended.gameData_.energySphereDestroyed_ = true;
        ended.HandleEndState();
        Require(ended.pendingHighScoreAvailable_ && ended.pendingHighScoreSphereBonus_
                    && ended.pendingHighScore_ == 20'000u && ended.IsDemoMode(),
                "fatal Sphere run qualifies by ranked bonus and returns to demo");
    }

    const auto resources = std::filesystem::path(argv[1]).parent_path();
    {
        drain_storm::ApplicationState hub;
        hub.resourceDirectory_ = resources;
        Require(hub.gameData_.Load(argv[1], 3, 19, 3)
                    && hub.gameData_.PositionPlayerAtTile(0x27),
                "prepare seeded hub departure");
        std::uint32_t expectedRandom = hub.gameData_.randomState_;
        for (const std::size_t level : {6u, 7u, 2u, 11u, 5u}) {
            drain_storm::GameData expected;
            Require(expected.Load(argv[1], level, expectedRandom, 3,
                        level == 5 ? hub.gameData_.boardDepth_ + 2
                                   : hub.gameData_.boardDepth_ + 1),
                    "construct hub branch oracle");
            expectedRandom = expected.randomState_;
        }
        Require(hub.ActivateTransporter(false) && hub.CurrentLevel() == 6
                    && hub.gameData_.randomState_ == expectedRandom,
                "hub constructs all five branches in original order and keeps constructor RNG");
        Require(hub.sharedBoards_.contains(5) && hub.sharedBoards_.contains(6)
                    && hub.sharedBoards_.contains(7) && hub.cachedBoards_.size() == 2,
                "hub retains shared room and both unvisited side branches");
    }
    {
        drain_storm::ApplicationState fresh;
        fresh.resourceDirectory_ = resources;
        Require(fresh.gameData_.Load(argv[1], 4, 31, 2)
                    && fresh.gameData_.PositionPlayerAtTile(0x27),
                "prepare ordinary fresh child");
        drain_storm::GameData expected;
        Require(expected.Load(argv[1], 10, fresh.gameData_.randomState_, 2,
                    fresh.gameData_.boardDepth_ + 1), "construct ordinary child oracle");
        Require(fresh.ActivateTransporter(false) && fresh.CurrentLevel() == 10
                    && fresh.gameData_.randomState_ == expected.randomState_,
                "fresh child retains its constructor-advanced random stream");
    }
    {
        drain_storm::ApplicationState cached;
        cached.resourceDirectory_ = resources;
        Require(cached.gameData_.Load(argv[1], 21, 13, 2)
                    && cached.gameData_.PositionPlayerAtTile(0x27),
                "prepare depth-five cached exit");
        drain_storm::GameData child;
        Require(child.Load(argv[1], 22, 29, 2), "prepare cached depth-six child");
        cached.cachedBoards_.push_back({{0x27}, child});
        const auto before = cached.gameData_.randomState_;
        Require(cached.ActivateTransporter(false) && cached.CurrentLevel() == 22
                    && cached.gameData_.randomState_ == before,
                "cached depth-five exit skips destination roll and carries current RNG");
    }
    {
        drain_storm::ApplicationState rare;
        rare.resourceDirectory_ = resources;
        Require(rare.gameData_.Load(argv[1], 11, 11, 2)
                    && rare.gameData_.PositionPlayerAtTile(0x27),
                "prepare recursive branch");
        bool foundRare = false, foundExhausted = false;
        for (std::uint32_t seed = 1; seed <= 1000; ++seed) {
            auto candidate = rare.gameData_;
            candidate.randomState_ = seed;
            candidate.DetectBoardTransition();
            if (candidate.TakeTransitionRequest() == 8) {
                rare.gameData_ = candidate;
                rare.gameData_.pendingTransition_ = 8;
                foundRare = true;
                break;
            }
        }
        Require(foundRare, "find rare recursive branch before dispatch");
        rare.HandleBoardTransition();
        Require(rare.CurrentLevel() == 8 && rare.gameData_.TileAt(20, 5) == 0x27,
                "rare recursive child receives caller-specific forward drain");
        for (std::uint32_t seed = 1; seed <= 1000; ++seed) {
            auto candidate = rare.gameData_;
            candidate.levelNumber_ = 11;
            candidate.level_[20][5] = 0x27;
            candidate.playerColumn_ = 20;
            candidate.playerRow_ = 5;
            candidate.levelElevenRuntimeFlags_ = 0;
            candidate.genericRuntimeFlags_ |= 0x30;
            candidate.randomState_ = seed;
            candidate.DetectBoardTransition();
            if (candidate.TakeTransitionRequest() == 11) {
                foundExhausted = true;
                break;
            }
        }
        Require(foundExhausted, "exhausted factories fall back to recursive child");
    }

    auto game = base;
    {
        auto sphere = base;
        sphere.levelNumber_ = 1;
        sphere.level_[12][12] = 0x51;
        sphere.level_[14][32] = 0x23;
        sphere.level_[32][14] = 3;
        Require(sphere.DamageBoardSquare(12, 12, 2), "destroy sphere directly");
        Require(sphere.level_[14][32] == 0 && sphere.level_[32][14] == 3,
            "sphere clears original column 14 row 32, not transposed coordinate");
        int constructionEffects = 0;
        for (const auto& overlay : sphere.overlays_) {
            if (overlay.type == 0x16) {
                ++constructionEffects;
                Require(overlay.column == 14 && overlay.row == 32,
                    "sphere construction effect uses original coordinate");
            }
        }
        Require(constructionEffects == 1, "sphere queues exactly one construction effect");
    }
    {
        int rareRolls = 0;
        for (int depth = 1; depth <= 9; ++depth) {
            for (std::uint32_t seed = 1; seed <= 100; ++seed) {
                auto board = base;
                board.randomState_ = seed;
                const auto next = seed * 0x343fdu + 0x269ec3u;
                const bool rare = depth >= 5 && ((next >> 16u) & 0x7fffu) % 20 == 19;
                Require(board.SelectProceduralStructureType(depth) == (rare ? 0x1a : 0x18),
                    "original depth-gated one-in-twenty structure selection");
                Require(board.randomState_ == (depth >= 5 ? next : seed),
                    "structure selection consumes exactly one roll only from depth five");
                rareRolls += rare;
            }
        }
        Require(rareRolls > 0, "selection regression exercises rare outcome");
        for (int skill = 1; skill <= 4; ++skill) {
            int rareStructures = 0;
            for (std::uint32_t seed = 1; seed <= 40; ++seed) {
                drain_storm::GameData board;
                Require(board.Load(argv[1], 21, seed, skill), "generate depth-five structure fixture");
                for (const auto& column : board.level_)
                    for (const auto tile : column) rareStructures += tile == 0x1a;
            }
            Require(rareStructures > 0, "procedural constructor places rare structures at every skill");
        }
    }
    // Final targets have stable identities, independent of which half or
    // scan order they occupy. Exercise direct impacts, motion and persistence.
    {
        auto boss = base;
        boss.levelNumber_ = 1; boss.levelOneArenaEntered_ = true;
        boss.turnCounter_ = 100; boss.levelOneFirstTargetActivationTurn_ = 1;
        boss.levelOneTargetColumns_ = {10, 24}; boss.levelOneTargetRows_ = {20, 10};
        const auto paintTargets = [](auto& board) {
            for (std::size_t id = 0; id < 2; ++id)
                for (unsigned y = 0; y < 3; ++y) for (unsigned x = 0; x < 3; ++x)
                    board.level_[board.levelOneTargetColumns_[id] + x][board.levelOneTargetRows_[id] + y]
                        = static_cast<std::uint16_t>(0x87 + y * 3 + x);
        };
        paintTargets(boss);
        for (int strength : {0, 1, 4})
            Require(!boss.DamageBoardSquare(11, 21, strength), "boss rejects ineffective impact classes");
        for (int hit = 0; hit < 9; ++hit)
            Require(boss.DamageBoardSquare(10 + hit % 3, 20 + hit / 3, 2), "all nine boss tiles map to their identity");
        Require(boss.levelOneTargetHits_[0] == 9 && boss.levelOneTargetHits_[1] == 0,
            "lower-half first boss does not damage upper-half second boss");
        auto sameHalf = boss;
        for (int x = 24; x < 27; ++x) for (int y = 10; y < 13; ++y) sameHalf.level_[x][y] = 0;
        sameHalf.levelOneTargetRows_[1] = 26;
        paintTargets(sameHalf);
        sameHalf.DamageBoardSquare(10, 20, 2);
        sameHalf.AdvanceBoardRuntime(false, 0, 0, false);
        Require(!sameHalf.VictoryTriggered() && sameHalf.level_[24][26] == 0x87
            && sameHalf.levelOneTargetHits_[1] == 0, "both bosses in one half still require separate defeats");
        std::stringstream save;
        Require(boss.WriteGame(save), "save crossed boss identities");
        auto legacyBytes = save.str();
        legacyBytes.resize(legacyBytes.size() - 27); legacyBytes[5] = 25;
        std::stringstream legacySave(legacyBytes);
        auto legacyBoss = base;
        Require(legacyBoss.ReadGame(legacySave) && legacyBoss.level_ == boss.level_
            && legacyBoss.levelOneTargetHits_ == boss.levelOneTargetHits_
            && legacyBoss.LevelOneTargetAt(10, 20) != legacyBoss.LevelOneTargetAt(24, 10),
            "version-25 boss save migrates without resetting board or saved damage counters");
        auto corruptBytes = save.str(); corruptBytes[corruptBytes.size() - 27] = static_cast<char>(0xff);
        std::stringstream corruptSave(corruptBytes);
        auto rejected = base;
        Require(!rejected.ReadGame(corruptSave) && rejected.level_ == base.level_,
            "invalid saved boss coordinates are rejected atomically");
        auto restored = base;
        Require(restored.ReadGame(save) && restored.levelOneTargetRows_ == boss.levelOneTargetRows_,
            "boss identity persists when loading from a different board class");
        Require(restored.DamageBoardSquare(12, 22, 3), "radio blast gives first boss its tenth hit");
        restored.AdvanceBoardRuntime(false, 0, 0, false);
        Require(restored.level_[10][20] == 0x99 && restored.level_[24][10] == 0x87
            && !restored.VictoryTriggered() && restored.score_ == 72500,
            "only defeated boss explodes, scores once, and does not grant early victory");
        restored.AdvanceBoardRuntime(false, 0, 0, false);
        Require(restored.score_ == 72500, "defeated boss cannot award score again");
        auto occupiedRubble = restored;
        for (int x = 24; x < 27; ++x) for (int y = 10; y < 13; ++y) occupiedRubble.level_[x][y] = 0;
        occupiedRubble.levelOneTargetColumns_[1] = occupiedRubble.levelOneTargetColumns_[0];
        occupiedRubble.levelOneTargetRows_[1] = occupiedRubble.levelOneTargetRows_[0];
        paintTargets(occupiedRubble);
        std::stringstream rubbleSave;
        Require(occupiedRubble.WriteGame(rubbleSave), "save living boss occupying defeated boss position");
        auto rubbleReloaded = base;
        Require(rubbleReloaded.ReadGame(rubbleSave) && rubbleReloaded.DamageBoardSquare(10, 20, 2)
            && rubbleReloaded.levelOneTargetHits_[0] == 10 && rubbleReloaded.levelOneTargetHits_[1] == 1,
            "a defeated boss identity is never reused by a living boss crossing its rubble");
        for (int hit = 0; hit < 10; ++hit) restored.DamageBoardSquare(24, 10, 2);
        restored.AdvanceBoardRuntime(false, 0, 0, false);
        Require(restored.VictoryTriggered() && restored.score_ == 145000,
            "victory requires ten hits on each independent boss");

        bool crossedWithMissile = false;
        for (unsigned seed = 1; seed <= 200 && !crossedWithMissile; ++seed) {
            auto moving = base;
            moving.levelNumber_ = 1; moving.turnCounter_ = 100;
            moving.levelOneArenaEntered_ = true; moving.levelOneFirstTargetActivationTurn_ = 1;
            moving.levelOneTargetColumns_ = {10, 28}; moving.levelOneTargetRows_ = {17, 23};
            moving.playerColumn_ = 11; moving.playerRow_ = 35;
            moving.randomState_ = seed; moving.projectiles_ = {{2, 11, 20, 0, -1}};
            paintTargets(moving);
            moving.AdvanceBoardRuntime(false, 0, 0, true);
            if (moving.levelOneTargetRows_[0] != 18) continue;
            Require(moving.levelOneTargetHits_[0] == 1 && moving.levelOneTargetHits_[1] == 0,
                "boss moving across middle onto a missile retains its damage identity");
            crossedWithMissile = true;
        }
        Require(crossedWithMissile, "exercise actual boss crossing during movement");
        bool sawDefensiveWave = false;
        for (unsigned seed = 1; seed <= 2000 && !sawDefensiveWave; ++seed) {
            auto wave = base;
            wave.levelNumber_ = 1;
            wave.levelOneArenaEntered_ = true;
            wave.levelOneTargetDefeated_[1] = 1;
            wave.levelOneTargetColumns_ = {10, 28};
            wave.levelOneTargetRows_ = {17, 23};
            wave.playerColumn_ = 35;
            wave.playerRow_ = 33;
            wave.randomState_ = seed;
            for (int y = 0; y < 3; ++y)
                for (int x = 0; x < 3; ++x)
                    wave.level_[10 + x][17 + y] =
                        static_cast<std::uint16_t>(0x87 + y * 3 + x);
            wave.AdvanceBoardRuntime(false, 0, 0, true);
            if (wave.actors_.empty()) continue;
            Require(wave.actors_.size() == 8
                        && std::none_of(wave.actors_.begin(), wave.actors_.end(),
                            [](const drain_storm::BoardActor& actor) {
                                return (actor.column == 11 && actor.row == 18)
                                    || actor.behavior != 0;
                            }), "boss defensive wave surrounds center with eight idle actors");
            for (const auto& actor : wave.actors_) {
                const bool corner = actor.column != 11 && actor.row != 18;
                Require(actor.type == 0 || actor.type == (corner ? 2 : 1),
                        "boss wave corner and edge types follow original rules");
            }
            sawDefensiveWave = true;
        }
        Require(sawDefensiveWave, "exercise boss defensive wave random branch");
        for (unsigned seed = 1; seed <= 50; ++seed) {
            auto dormant = base; dormant.levelNumber_ = 1; dormant.randomState_ = seed;
            dormant.levelOneTargetColumns_ = {10, 10}; dormant.levelOneTargetRows_ = {17, 20};
            dormant.playerColumn_ = 11; dormant.playerRow_ = 35;
            paintTargets(dormant);
            for (int x = 0; x < 3; ++x) for (int y = 0; y < 3; ++y)
                dormant.level_[10 + x][20 + y] = static_cast<std::uint16_t>(0x90 + y * 3 + x);
            dormant.AdvanceBoardRuntime(false, 0, 0, true);
            Require(dormant.level_[10][20] == 0x90 && dormant.level_[12][22] == 0x98,
                "moving boss cannot overwrite the other dormant target");
        }
        auto missing = base; missing.levelNumber_ = 1; missing.levelOneArenaEntered_ = true;
        missing.turnCounter_ = 100;
        missing.AdvanceBoardRuntime(false, 0, 0, false);
        Require(!missing.VictoryTriggered(), "missing target graphics alone never count as defeating both bosses");
    }

    for (int skill = 1; skill <= 4; ++skill) {
        for (std::size_t level = 1; level <= 25; ++level) {
            auto board = base;
            Require(board.Load(argv[1], level, 1, skill), "load all 100 board/skill combinations");
            std::stringstream save;
            Require(board.WriteGame(save), "serialize all board/skill combinations");
            auto restored = base;
            Require(restored.ReadGame(save) && restored.Difficulty() == skill && restored.level_ == board.level_,
                "all 100 board/skill combinations round-trip without regenerating terrain");
        }
        for (std::size_t level = 13; level <= 16; ++level) {
            auto board = base;
            Require(board.Load(argv[1], level, 1, skill), "load difficulty-scaled maze");
            const auto captains = std::count_if(board.actors_.begin(), board.actors_.end(),
                [](const auto& actor) { return actor.type == 3; });
            if (level < 16) {
                const int extra = level > 13 || skill > 2 ? 1 : 0;
                Require(board.actors_.size() == static_cast<std::size_t>((skill + 2) * 2 + extra)
                    && captains == extra, "86a0 exact maze enemy counts at every skill/stage");
                Require(board.StructureSpawnParameters().first == (skill + static_cast<int>(level - 12)) * 2,
                    "86a0 maze structure spawn formula");
                if (skill == 1) Require(std::none_of(board.actors_.begin(), board.actors_.end(),
                    [](const auto& actor) { return actor.type == 2; }), "Beginner maze has no randomized lieutenants");
            } else {
                Require(captains == skill && board.actors_.size() == static_cast<std::size_t>(skill),
                    "3a96 alien-maze captains scale with skill");
                Require(board.StructureSpawnParameters().first == (skill + 2) * 2,
                    "3a96 alien-maze spawn formula");
            }
            Require(board.ActorBreakDivisor() == (6 - skill) * 10, "maze break divisor follows skill");
            std::stringstream save;
            Require(board.WriteGame(save), "save maze skill");
            auto restored = base;
            Require(restored.ReadGame(save) && restored.level_ == board.level_
                && restored.actors_.size() == board.actors_.size() && restored.Difficulty() == skill,
                "save/load preserves corrected populations and board layout");
        }
        for (int depth : {3, 8, 20}) {
            auto constructed = base;
            Require(constructed.Load(argv[1], 11, 1, skill, depth), "construct recursive branch at actual depth");
            auto oracle = base; oracle.randomState_ = 1;
            const int pickups = oracle.NextRandom(skill + 3);
            Require(constructed.overlays_.size() == static_cast<std::size_t>(pickups),
                "cb9c pickup roll uses skill plus three");
            auto board = base; board.levelNumber_ = 11;
            board.boardDepth_ = static_cast<std::uint16_t>(depth); board.difficulty_ = skill;
            for (unsigned phase : {0u, 1u, 3u, 7u, 15u}) {
                board.levelElevenRuntimeFlags_ = static_cast<std::uint8_t>(phase);
                int chance = depth / 2, divisor = std::max(10, 100 - depth - 15 * skill);
                if (phase & 1) { chance += 2; divisor -= 5; }
                if (phase & 2) { chance *= 2; divisor /= 2; }
                if (phase & 4) { chance = 25; divisor = 10; }
                if (phase & 8) { chance = 50; divisor = 5; }
                Require(board.StructureSpawnParameters() == std::pair{chance, skill}
                    && board.ActorBreakDivisor() == divisor, "cb9c/d06a depth/skill escalation parameters");
            }
        }
        for (int depth = 1; depth <= 9; ++depth) {
            auto board = base; board.levelNumber_ = 16 + depth; board.difficulty_ = skill;
            board.turnCounter_ = 6; board.genericRuntimeFlags_ = 0;
            board.AdvanceBoardRuntime(false, 0, 0, false);
            const int expected = std::min(skill + 5, depth + skill - 1);
            Require(board.genericSpawnChance_ == expected, "dd76 updates at turn six on every depth");
            board.turnCounter_ = 51;
            board.AdvanceBoardRuntime(false, 0, 0, false);
            board.AdvanceBoardRuntime(false, 0, 0, false);
            Require(board.genericSpawnChance_ == expected + 2, "turn-six update does not erase later escalation");
            board.genericSpawnChance_ = 18; // Simulate the old save's incorrect value.
            std::stringstream save;
            Require(board.WriteGame(save), "write legacy procedural migration fixture");
            auto bytes = save.str(); bytes.resize(bytes.size() - 27); bytes[5] = 25;
            std::stringstream legacy(bytes);
            auto loaded = base;
            Require(loaded.ReadGame(legacy) && loaded.genericSpawnChance_ == expected + 2
                && loaded.level_ == board.level_, "legacy spawn repair preserves saved terrain");
        }
    }
    // Overlapping batches must each retain five frames, including after save.
    game.overlays_ = {{0x14, 15, 15}, {0x1e, 20, 20}, {1, 21, 21}};
    game.ArmTransientOverlayAnimation();
    for (int tick = 0; tick < 3; ++tick) game.AdvanceAnimation();
    game.overlays_.push_back({0x16, 16, 15});
    game.ArmTransientOverlayAnimation();
    Require(game.overlays_[0].animationFrame == 4 && game.overlays_.back().animationFrame == 1,
        "new effect starts at frame one without rewinding old effect");
    std::stringstream effectsSave;
    Require(game.WriteGame(effectsSave), "save overlapping effect ages");
    auto effects = base;
    Require(effects.ReadGame(effectsSave) && effects.overlays_[0].animationFrame == 4
        && effects.overlays_.back().animationFrame == 1, "effect ages survive save/load");
    effects.AdvanceAnimation();
    Require(effects.overlays_[0].animationFrame == 5, "old effect displays fifth frame");
    effects.AdvanceAnimation();
    Require(effects.overlays_.size() == 3 && effects.overlays_.back().animationFrame == 3,
        "old effect expires without deleting newer effect or persistent overlays");
    effects.ArmTransientOverlayAnimation();
    Require(effects.overlays_.back().animationFrame == 3, "unrelated action does not rewind effect");
    effects.AdvanceAnimation(); effects.AdvanceAnimation();
    Require(effects.overlays_.back().animationFrame == 5, "new effect receives fifth frame");
    effects.AdvanceAnimation();
    Require(effects.overlays_.size() == 2 && effects.transientOverlayFrame_ == 0,
        "only persistent overlays remain after both batches finish");
    auto oldEffects = effectsSave.str();
    oldEffects[5] = 23;
    oldEffects.resize(oldEffects.size() - game.overlays_.size() - 33);
    std::stringstream oldEffectsStream(oldEffects);
    Require(effects.ReadGame(oldEffectsStream) && effects.overlays_[0].animationFrame == 4
        && effects.overlays_.back().animationFrame == 4, "v23 shared frame migrates to per-effect ages");
    auto invalidEffects = effectsSave.str(); invalidEffects[invalidEffects.size() - 34] = 6;
    std::stringstream invalidEffectsStream(invalidEffects);
    Require(!effects.ReadGame(invalidEffectsStream), "reject invalid saved effect age");

    // Isolated free regions outside level three's placement range. All other
    // terrain must survive, and insufficient room must leave no partial event.
    game = base;
    for (auto& column : game.level_) column.fill(0x36);
    constexpr POINT sites[] = {{1, 1}, {30, 1}, {34, 28}};
    for (const auto& site : sites)
        for (int x = 0; x < 3; ++x)
            for (int y = 0; y < 3; ++y) game.level_[site.x + x][site.y + y] = 0;
    game.playerColumn_ = 2; game.playerRow_ = 2;
    const auto untouched = game.level_;
    Require(!game.PlaceMoonRoomStructures() && game.level_ == untouched && game.overlays_.empty(),
        "Moon Room does not place on player or commit a partial construction event");
    game.playerColumn_ = 10; game.playerRow_ = 10;
    Require(game.PlaceMoonRoomStructures() && game.overlays_.size() == 27,
        "Moon Room places all three structures over full recovered coordinate range");
    int centers = 0;
    for (std::size_t x = 0; x < game.kBoardColumns; ++x)
        for (std::size_t y = 0; y < game.kBoardRows; ++y) {
            centers += game.level_[x][y] == 0x18;
            if (untouched[x][y] != 0) Require(game.level_[x][y] == untouched[x][y],
                "Moon Room construction preserves all occupied terrain");
        }
    Require(centers == 3, "Moon Room constructs exactly three structures");
    game = base; game.levelNumber_ = 6; game.turnCounter_ = 1001;
    game.levelSixUpperTransporterTriggered_ = true;
    game.levelSixLowerTransporterTriggered_ = true;
    game.levelSixCenterOpened_ = true;
    std::uint32_t eventSeed = 0;
    for (; eventSeed < 10000; ++eventSeed) {
        auto probe = game; probe.randomState_ = eventSeed;
        if (probe.NextRandom(100) == 1) break;
    }
    Require(eventSeed < 10000, "find deterministic Moon Room event roll");
    for (auto& column : game.level_) column.fill(0x36);
    game.level_[32][10] = 0x37; game.level_[32][23] = 0x37;
    game.randomState_ = eventSeed;
    game.AdvanceBoardRuntime(false, 0, 0, false);
    Require(!game.levelSixStructuresSpawned_ && game.overlays_.empty(),
        "packed Moon Room leaves construction event pending");
    for (const auto& site : sites)
        for (int x = 0; x < 3; ++x)
            for (int y = 0; y < 3; ++y) game.level_[site.x + x][site.y + y] = 0;
    game.randomState_ = eventSeed;
    game.AdvanceBoardRuntime(false, 0, 0, false);
    Require(game.levelSixStructuresSpawned_ && game.overlays_.size() == 27,
        "Moon Room event completes when space becomes available");
    game.randomState_ = eventSeed;
    game.AdvanceBoardRuntime(false, 0, 0, false);
    Require(game.overlays_.size() == 27, "Moon Room construction event is one-shot");
    game = base;
    {
        drain_storm::ApplicationState messages;
        Require(!messages.HasMoreGameplayMessages(), "empty history has no more prompt");
        messages.gameData_.pendingMessages_ = {L"first", L"second"};
        messages.TakeGameplayMessage();
        Require(messages.HasMoreGameplayMessages(), "queued second message enables more prompt");
        (void)messages.NavigateMessageHistory(false);
        Require(!messages.HasMoreGameplayMessages(), "last message clears more prompt");
        (void)messages.NavigateMessageHistory(false);
        Require(!messages.HasMoreGameplayMessages(), "past-end message clears more prompt");
        (void)messages.NavigateMessageHistory(true);
        Require(messages.messageHistoryCursor_ < messages.messageReviewBoundary_,
            "reviewing a previously read message selects old-message color");
        messages.gameData_.pendingMessages_ = {L"fresh", L"next"};
        messages.TakeGameplayMessage();
        Require(messages.messageHistoryCursor_ == messages.messageReviewBoundary_,
            "new message batch resets the review boundary");
    }
    game.levelNumber_ = 2;
    for (std::uint16_t announcement = 0; announcement < 5; ++announcement) {
        game.SetCasinoAnnouncement(announcement);
        Require(game.CasinoAnnouncementWindow() == std::wstring(13, L' '), "marquee blank lead-in");
        const auto rng = game.randomState_;
        for (int tick = 0; tick < 13; ++tick) game.AdvanceAnimation();
        Require(game.CasinoAnnouncementWindow() == game.CasinoAnnouncementText().substr(0, 13),
            "marquee advances to original text without input");
        auto result = game;
        if (announcement == 2 || announcement == 4) {
            result.casinoStatus_ = 3;
            result.casinoResultTicks_ = 37;
            result.casinoDeckIndex_ = 5;
            result.casinoRanks_.fill(1); result.casinoSuits_.fill(1);
            for (std::size_t card = 0; card < result.casinoDeck_.size(); ++card)
                result.casinoDeck_[card] = static_cast<std::uint16_t>(card);
        }
        std::stringstream casinoSave;
        Require(result.WriteGame(casinoSave), "write casino presentation");
        auto restored = base;
        Require(restored.ReadGame(casinoSave) && restored.casinoAnnouncement_ == announcement
            && restored.casinoScrollOffset_ == 13 && restored.CasinoAnnouncementWindow() == result.CasinoAnnouncementWindow()
            && restored.casinoResultTicks_ == result.casinoResultTicks_, "casino result and marquee survive reload");
        auto olderCasino = casinoSave.str(); olderCasino[5] = 24; olderCasino.resize(olderCasino.size() - 33);
        std::stringstream olderCasinoStream(olderCasino);
        Require(restored.ReadGame(olderCasinoStream), "v24 saves remain readable");
        Require(game.randomState_ == rng && game.turnCounter_ == 0,
            "marquee animation consumes neither gameplay turns nor random numbers");
        game.casinoScrollOffset_ = game.CasinoAnnouncementText().size() + 14;
        game.AdvanceAnimation();
        Require(game.casinoScrollOffset_ == 0, "marquee wraps after trailing blank window");
    }
    game.SetCasinoAnnouncement(3);
    game.ResetCasinoRound();
    Require(game.casinoAnnouncement_ == 0 && game.casinoScrollOffset_ == 0,
        "fresh casino table resets announcement");
    // Offscreen GDI regressions compare restored UI graphics with the actual
    // original atlas pixels, rather than just testing sprite IDs.
    {
        drain_storm::BitmapSurface atlas;
        Require(atlas.Load(std::filesystem::path(argv[1]).parent_path() / L"bitmap_BOARD_BITMAPS.bmp"),
            "load rendering atlas");
        HDC rendered = CreateCompatibleDC(nullptr), expected = CreateCompatibleDC(nullptr);
        BITMAPINFO info{};
        info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        info.bmiHeader.biWidth = 480; info.bmiHeader.biHeight = -432;
        info.bmiHeader.biPlanes = 1; info.bmiHeader.biBitCount = 32;
        info.bmiHeader.biCompression = BI_RGB;
        void* pixels{};
        HBITMAP actualBitmap = CreateDIBSection(rendered, &info, DIB_RGB_COLORS, &pixels, nullptr, 0);
        void* actualPixels = pixels;
        HBITMAP expectedBitmap = CreateDIBSection(expected, &info, DIB_RGB_COLORS, &pixels, nullptr, 0);
        Require(actualBitmap && expectedBitmap, "allocate offscreen rendering surfaces");
        auto oldActual = SelectObject(rendered, actualBitmap);
        auto oldExpected = SelectObject(expected, expectedBitmap);
        const auto compareSprite = [&](int x, int y, std::size_t index, int visibleWidth = -1) {
            const auto& sprite = base.sprites_[index];
            atlas.DrawRegion(expected, x, y, sprite.sourceColumn * 12, sprite.sourceRow * 12,
                sprite.widthInCells * 12, sprite.heightInCells * 12);
            for (int dy = 0; dy < sprite.heightInCells * 12; ++dy)
                for (int dx = 0; dx < (visibleWidth < 0 ? sprite.widthInCells * 12 : visibleWidth); ++dx)
                    if (GetPixel(rendered, x + dx, y + dy) != GetPixel(expected, x + dx, y + dy)) {
                        std::cerr << "Sprite " << index << " differs at " << dx << ',' << dy << '\n';
                        Require(false, "restored graphics match original atlas pixels");
                    }
        };
        PatBlt(rendered, 0, 0, 480, 432, WHITENESS);
        drain_storm::ApplicationState::DrawControlSurroundings(rendered, RECT{0, 0, 480, 432},
            RECT{12, 12, 132, 420}, RECT{144, 12, 468, 420});
        Require(GetPixel(rendered, 138, 200) == RGB(192, 192, 192), "control gap is gray rather than black");
        Require(GetPixel(rendered, 20, 20) == RGB(255, 255, 255), "control surround does not erase child content");
        Require(GetPixel(rendered, 10, 20) != GetPixel(rendered, 132, 20), "control surround has contrasting bevel edges");
        base.DrawArsenal(rendered, atlas, 120);
        Require(GetPixel(rendered, 0, 60) == RGB(128, 128, 128)
                    && GetPixel(rendered, 119, 60) == RGB(0, 0, 0)
                    && GetPixel(rendered, 0, 300) == RGB(128, 128, 128)
                    && GetPixel(rendered, 119, 300) == RGB(0, 0, 0),
                "arsenal list and score status restore original vertical inset edges");
        for (unsigned frame = 0; frame < 40; ++frame) {
            game = base; game.animationFrame_ = frame;
            game.level_[15][15] = 0x23; game.level_[16][15] = 0x27;
            game.level_[17][15] = 0x1a; game.level_[18][15] = 0x51;
            game.level_[19][15] = 0x59; game.level_[20][15] = 0xad;
            game.AdvanceRandomAnimation();
            const auto rng = game.randomState_;
            game.DrawLevel(rendered, atlas);
            compareSprite(180, 180, frame % 8 < 4 ? 0x42 + frame % 4 : 0x41);
            compareSprite(192, 180, frame % 8 < 4 ? 0x2c + frame % 4 : 0x2b);
            constexpr std::uint16_t machinery[] = {0x6d, 0x6f, 0x73};
            compareSprite(204, 180, machinery[frame % 3]);
            compareSprite(216, 180, 0xaf + (frame % 9) * 4);
            compareSprite(228, 180, 0xd3 + (frame % 40) / 10);
            compareSprite(240, 180, 0x1c8 + frame % 3);
            Require(game.randomState_ == rng && game.turnCounter_ == 0, "terrain painting leaves gameplay state untouched");
        }
        // Original 1008:9933..998e selects sprites by drain family, not
        // branch index. Check every frame of the complete presentation cycle.
        for (unsigned frame = 0; frame < 120; ++frame) {
            game = base; game.animationFrame_ = frame;
            for (unsigned branch = 0; branch < 8; ++branch)
                game.level_[10 + branch][15] = static_cast<std::uint16_t>(0x23 + branch);
            game.DrawLevel(rendered, atlas);
            for (unsigned branch = 0; branch < 8; ++branch) {
                const unsigned spriteBase = branch < 4 ? 0x41 : 0x2b;
                compareSprite((10 + branch) * 12, 180,
                    spriteBase + (frame % 8 < 4 ? 1 + frame % 4 : 0));
            }
        }
        for (std::uint16_t actor : std::array<std::uint16_t, 2>{2, 3}) {
            std::array<COLORREF, 144> first{};
            bool changed = false;
            game = base; game.actors_ = {{actor, 15, 15}};
            for (unsigned frame = 0; frame < 120; ++frame) {
                game.animationFrame_ = frame;
                game.AdvanceRandomAnimation();
                game.DrawLevel(rendered, atlas);
                for (int y = 0; y < 12; ++y)
                    for (int x = 0; x < 12; ++x) {
                        const auto pixel = GetPixel(rendered, 180 + x, 180 + y);
                        if (frame == 0) first[y * 12 + x] = pixel;
                        else changed = changed || pixel != first[y * 12 + x];
                    }
            }
            Require(changed, "formerly static enemy has animated frames");
        }
        game = base;
        game.level_[15][15] = 0x1a;
        game.animationFrame_ = 2;
        game.AdvanceRandomAnimation();
        game.energySphereDestroyed_ = true;
        for (unsigned frame = 3; frame < 10; ++frame) {
            game.animationFrame_ = frame;
            game.AdvanceRandomAnimation();
            game.DrawLevel(rendered, atlas);
            compareSprite(180, 180, 0x73);
        }
        Require(game.randomState_ == base.randomState_,
                "Sphere shutdown freezes machinery without consuming RNG");
        for (std::uint32_t seed = 1; seed <= 32; ++seed) {
            game = base; game.randomState_ = seed; game.animationFrame_ = 4;
            game.level_[2][2] = 0xaf; game.level_[3][2] = 0x83;
            game.level_[4][2] = 0xa2;
            game.actors_ = {{2, 20, 20}, {2, 21, 20}, {3, 22, 20}};
            game.overlays_ = {{1, 25, 20}, {1, 26, 20}};
            auto oracle = game;
            const auto skull = 0x1d2 + oracle.NextRandom(2);
            const auto sphere = 0x83 + (oracle.NextRandom(8) == 1 ? 0x103 : 0xff);
            if (oracle.NextRandom(3) == 1) oracle.NextRandom(2);
            const auto first = oracle.NextRandom(2) == 2 ? 8 : 7;
            const auto second = oracle.NextRandom(2) == 2 ? 8 : 7;
            oracle.NextRandom(2); // Captain, independently of lieutenants.
            const auto newest = oracle.NextRandom(3) == 1 ? 0x47 : 0xffff;
            const auto oldest = oracle.NextRandom(3) == 1 ? 0x47 : 0xffff;
            game.AdvanceRandomAnimation();
            Require(game.randomState_ == oracle.randomState_
                        && game.terrainAnimation_[2][2].sprite == skull
                        && game.terrainAnimation_[3][2].sprite == sphere
                        && game.actors_[0].visualSprite == first
                        && game.actors_[1].visualSprite == second
                        && game.overlays_[1].visualSprite == newest
                        && game.overlays_[0].visualSprite == oldest,
                    "animation rolls follow original terrain/actor/head-first overlay order");
            const auto afterTick = game.randomState_;
            game.DrawLevel(rendered, atlas); game.DrawLevel(rendered, atlas);
            Require(game.randomState_ == afterTick, "cached animation repaint never rolls gameplay RNG");
            auto hidden = base; hidden.randomState_ = seed; hidden.animationFrame_ = 4;
            hidden.level_[10][10] = 0xaf; // Player occupancy.
            hidden.level_[20][20] = 0xaf; hidden.actors_ = {{0, 20, 20}};
            hidden.level_[21][20] = 0xaf; hidden.projectiles_ = {{0, 21, 20, 1, 0}};
            hidden.level_[22][20] = 3;
            hidden.overlays_ = {{1, 10, 10}, {1, 20, 20}, {1, 21, 20}, {1, 22, 20}};
            hidden.AdvanceRandomAnimation();
            Require(hidden.randomState_ == seed,
                    "occupied and hidden terrain/treasure consume no animation rolls");
        }
        game = base;
        game.overlays_ = {{0x14, 15, 15, 4}, {0x16, 16, 15, 1}};
        game.DrawLevel(rendered, atlas);
        compareSprite(180, 180, 0x63);
        compareSprite(192, 180, 0x5b);
        // Full-frame compositing must obey the original terrain/occupancy
        // exclusions, not just paint every stored overlay over the terrain.
        for (std::uint16_t pickup : std::array<std::uint16_t, 5>{1, 2, 3, 0xd, 0x11}) {
            game = base;
            game.overlays_ = {{pickup, 15, 15}};
            for (std::uint16_t terrain : std::array<std::uint16_t, 3>{3, 0x36, 0}) {
                game.level_[15][15] = terrain;
                game.DrawLevel(rendered, atlas);
                compareSprite(180, 180, terrain != 0 ? game.tiles_[terrain].sprite
                    : pickup == 1 ? game.overlaySprites_[pickup] : 0x57);
                Require(game.overlays_.size() == 1 && game.inventory_.size() == 1,
                    "concealing or revealing an item does not collect or delete it");
            }
        }
        game = base;
        game.level_[15][15] = 3;
        game.overlays_ = {{0x16, 15, 15}};
        game.transientOverlayFrame_ = 1;
        game.DrawLevel(rendered, atlas);
        compareSprite(180, 180, 0x5b); // Construction effects may cover solid tiles.
        game = base;
        game.actors_ = {{2, 15, 15}};
        game.projectiles_ = {{0, 15, 15, 0, 0}};
        game.overlays_ = {{0x16, 15, 15, 1}};
        game.DrawLevel(rendered, atlas);
        compareSprite(180, 180, 0x5b); // Late effect pass covers actors and projectiles.
        for (std::uint32_t frame = 0; frame < 10; ++frame) {
            game = base; game.animationFrame_ = frame;
            game.overlays_ = {{2, 15, 15}, {0x2b, 16, 15}, {0x2d, 17, 15}};
            game.projectiles_ = {{3, 18, 15, 0, 0}, {4, 19, 15, 1, 0}};
            const auto rng = game.randomState_;
            game.DrawLevel(rendered, atlas);
            compareSprite(180, 180, 0x57 + frame % 4);
            compareSprite(192, 180, frame % 5 < 3 ? 0xfb : 0);
            constexpr std::uint16_t markerFrames[] = {0xfc, 0xfd, 0xfe, 0xff, 0xfe, 0xfd, 0xfc, 0xfc, 0xfc, 0xfc};
            compareSprite(204, 180, markerFrames[frame]);
            compareSprite(216, 180, 0x1d + frame % 4);
            compareSprite(228, 180, 0x21 + frame % 4);
            Require(game.randomState_ == rng && game.animationFrame_ == frame && game.turnCounter_ == 0,
                "board painting consumes neither animation ticks nor gameplay RNG/turns");
        }
        for (std::uint16_t projectile = 0; projectile <= 4; ++projectile) {
            game = base;
            // Death sprite avoids depending on the physical Shift key in this test.
            game.health_ = 0; game.terminalDeathEnabled_ = true;
            game.actors_ = {{0, 10, 10}};
            game.overlays_ = {{2, 10, 10}, {0x14, 10, 10}};
            game.transientOverlayFrame_ = 1;
            game.projectiles_ = {{projectile, 10, 10, 1, 0}};
            game.DrawLevel(rendered, atlas);
            compareSprite(120, 120, 0x3e);
        }
        for (int health : {0, 3}) {
            game = base;
            game.health_ = health; game.terminalDeathEnabled_ = true;
            game.overlays_ = {{0x18, 10, 10, 1}};
            game.actors_ = {{0, 10, 10}};
            game.projectiles_ = {{4, 10, 10, 1, 0}};
            for (unsigned frame = 0; frame < 5; ++frame) {
                game.DrawLevel(rendered, atlas);
                compareSprite(120, 120, 0x3a + frame);
                game.AdvanceAnimation();
            }
            Require(game.overlays_.empty(), "player hit effect expires after its five frames");
            // Use the deterministic wreck pose to check normal priority resumes.
            game.health_ = 0;
            game.DrawLevel(rendered, atlas);
            compareSprite(120, 120, 0x3e + game.animationFrame_ % 3);
        }
        game = base;
        game.health_ = 0; game.terminalDeathEnabled_ = true;
        game.overlays_ = {{0x18, 11, 10, 1}};
        game.DrawLevel(rendered, atlas);
        compareSprite(120, 120, 0x3e);
        compareSprite(132, 120, 0x3a);
        for (std::uint16_t projectile : std::array<std::uint16_t, 2>{3, 4}) {
            game = base;
            game.actors_ = {{2, 15, 15}};
            game.projectiles_ = {{projectile, 15, 15, 0, 0}};
            game.DrawLevel(rendered, atlas);
            compareSprite(180, 180, 7);
        }
        for (int direction : {-1, 0, 1}) {
            PatBlt(rendered, 0, 0, 480, 432, WHITENESS);
            game = base; game.health_ = 3;
            game.DrawArsenal(rendered, atlas, 118, direction);
            compareSprite(93, 198, direction < 0 ? 0x99 : direction > 0 ? 0x9a : 0x9b);
            compareSprite(2, 72, 0x154);
            // Original selection frame covers the marker's last two columns.
            compareSprite(92, 259, 0x96, 22);
        }
        for (std::uint16_t wild : std::array<std::uint16_t, 3>{0, 2, 7}) {
            game = base; game.levelNumber_ = 2; game.casinoWildRank_ = wild;
            game.DrawLevel(rendered, atlas);
            Require(game.casinoScrollOffset_ == 0, "repainting does not advance marquee");
            Require(GetPixel(rendered, 216, 156) == RGB(0, 0, 0), "marquee draws opaque black lead-in");
            for (int column = 22; column <= 26; ++column) {
                const std::size_t sprite = wild == 0 ? 0xf0 : column == 22
                    ? (wild == 2 ? 0x134 : 0x136) : column == 23 ? 0x135 : column + 0x11f;
                compareSprite(column * 12, 16 * 12, sprite);
            }
        }
        game = base; game.levelNumber_ = 2; game.casinoStatus_ = 0;
        game.DrawLevel(rendered, atlas);
        for (int card = 0; card < 5; ++card) {
            compareSprite((17 + card * 3) * 12, 17 * 12, 0x12a);
            compareSprite((17 + card * 3 + 1) * 12, 18 * 12, 0x12e);
        }
        game.casinoStatus_ = 2;
        game.casinoRanks_.fill(1); game.casinoSuits_.fill(1);
        game.overlays_ = {{0x1e, 17, 17, 1}};
        for (unsigned phase = 1; phase <= 5; ++phase) {
            game.DrawLevel(rendered, atlas);
            compareSprite(17 * 12, 17 * 12, phase <= 2 ? 0xf0 : phase <= 4 ? 0x12a : 0x11a);
            if (phase < 5) game.AdvanceAnimation();
        }
        game.AdvanceAnimation();
        Require(game.overlays_.empty(), "casino card flip expires after five phases");
        drain_storm::ApplicationState help;
        help.resourceDirectory_ = std::filesystem::path(argv[1]).parent_path();
        help.gameData_ = base;
        help.PostGameplayNotice(L"Sound Off");
        Require(help.CurrentGameplayMessage() == L"Sound Off"
            && help.NavigateMessageHistory(true) == L"Sound Off",
            "auxiliary notices enter arsenal history without modal delivery");
        Require(help.boardAtlas_.Load(std::filesystem::path(argv[1]).parent_path() / L"bitmap_BOARD_BITMAPS.bmp"),
            "load illustrated help atlas");
        Require(help.highScores_.Submit("PLAYER", 123456, 4, true), "seed high-score rendering");
        for (int page = 0; page <= 12; ++page) {
            PatBlt(rendered, 0, 0, 480, 432, WHITENESS);
            const auto initialMap = GetMapMode(rendered);
            if (page == 0) {
                RECT panel{0, 0, 118, 432};
                FillRect(rendered, &panel, static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH)));
                help.gameData_.health_ = 5;
                help.gameData_.score_ = 123456;
                help.gameData_.pendingMessages_ = {L"You see here a signpost.\n\nIt reads:", L"Second message"};
                help.TakeGameplayMessage();
                help.DrawArsenal(rendered, 118);
                const auto originalColor = GetTextColor(rendered);
                help.DrawMessagePanel(rendered, 118);
                Require(GetTextColor(rendered) == originalColor, "message renderer restores text color");
                // Long tokens must terminate even in a narrower-than-character panel.
                help.DrawMessagePanel(expected, 3);
            } else {
                Require(help.DrawHelpPage(rendered, page, RECT{0, 0, 444, 348}), "render illustrated help page");
            }
            Require(GetMapMode(rendered) == initialMap, "help renderer restores caller DC mapping");
            Require(GetPixel(rendered, 479, 431) == RGB(255, 255, 255), "help rendering respects page bounds");
            GdiFlush();
            BITMAPFILEHEADER header{};
            header.bfType = 0x4d42;
            header.bfOffBits = sizeof(header) + sizeof(BITMAPINFOHEADER);
            header.bfSize = header.bfOffBits + 480 * 432 * 4;
            std::ofstream snapshot("help-render-" + std::to_string(page) + ".bmp", std::ios::binary);
            snapshot.write(reinterpret_cast<const char*>(&header), sizeof(header));
            snapshot.write(reinterpret_cast<const char*>(&info.bmiHeader), sizeof(info.bmiHeader));
            snapshot.write(static_cast<const char*>(actualPixels), 480 * 432 * 4);
            Require(snapshot.good(), "write offscreen help snapshot");
        }
        Require(!help.DrawHelpPage(rendered, 13, RECT{0, 0, 444, 348}), "reject invalid help page");
        help.gameData_ = base;
        help.gameData_.health_ = 0; help.gameData_.terminalDeathEnabled_ = true;
        for (int page : {2, 6, 12}) {
            Require(help.DrawHelpPage(rendered, page, RECT{0, 0, 444, 348}, 1),
                "draw state-dependent help player illustration");
            const int x = page == 2 ? 192 : page == 6 ? 354 : 360;
            const int y = page == 2 ? 90 : page == 6 ? 101 : 288;
            compareSprite(x, y, 0x3f);
        }
        const auto helpRng = help.gameData_.randomState_;
        const auto helpTurns = help.gameData_.turnCounter_;
        for (std::uint32_t frame = 0; frame < 10; ++frame) {
            Require(help.DrawHelpPage(rendered, 4, RECT{0, 0, 444, 348}, frame), "animate elements page");
            compareSprite(60, 144, frame % 8 < 4 ? 0x2c + frame % 4 : 0x2b);
            compareSprite(84, 144, frame % 8 < 4 ? 0x42 + frame % 4 : 0x41);
            compareSprite(204, 144, frame % 5 == 1 ? 0x6d : 0x6e);
            compareSprite(120, 300, 0x57 + frame % 4);
            Require(help.DrawHelpPage(rendered, 5, RECT{0, 0, 444, 348}, frame), "animate weapons page");
            compareSprite(24, 232, 0x21 + frame % 4);
            compareSprite(24, 304, 0x1d + frame % 4);
            compareSprite(12, 64, 0); // Floor continues between weapon rows.
        }
        Require(help.gameData_.randomState_ == helpRng && help.gameData_.turnCounter_ == helpTurns,
            "help animation does not consume gameplay RNG or turns");
        Require(game.Load(argv[1], 6), "load Moon Room rendering snapshot");
        game.AdvanceBoardRuntime(false, 0, 0, false);
        game.DrawLevel(rendered, atlas);
        BITMAPFILEHEADER moonHeader{};
        moonHeader.bfType = 0x4d42;
        moonHeader.bfOffBits = sizeof(moonHeader) + sizeof(info.bmiHeader);
        moonHeader.bfSize = moonHeader.bfOffBits + 480 * 432 * 4;
        std::ofstream moonSnapshot("moon-room-render.bmp", std::ios::binary);
        moonSnapshot.write(reinterpret_cast<const char*>(&moonHeader), sizeof(moonHeader));
        moonSnapshot.write(reinterpret_cast<const char*>(&info.bmiHeader), sizeof(info.bmiHeader));
        GdiFlush();
        moonSnapshot.write(static_cast<const char*>(actualPixels), 480 * 432 * 4);
        Require(moonSnapshot.good(), "write Moon Room rendering snapshot");
        for (const int level : {2, 10}) {
            Require(help.gameData_.Load(argv[1], level), "load demo panel rendering board");
            help.demoMode_ = true;
            const RECT panel = help.DemoPanelBounds();
            const auto rng = help.gameData_.randomState_;
            for (unsigned frame : {0u, 3u, 7u}) {
                help.gameData_.animationFrame_ = frame;
                for (int pressed : {-1, 0, 1, 2}) {
                    help.DrawBoard(rendered, pressed);
                    const unsigned phase = frame % 10;
                    constexpr unsigned dozer[] = {10, 11, 10, 12};
                    const std::size_t icons[] = {help.gameData_.PlayerSpriteIndex(false),
                        phase > 5 ? 11 - phase : phase + 1, dozer[frame % 4]};
                    for (int button = 0; button < 3; ++button) {
                        const int offset = button == pressed ? 1 : 0;
                        compareSprite(panel.left + 30 + button * 84 + offset,
                            panel.top + 102 + offset, icons[button]);
                    }
                    Require(GetPixel(rendered, panel.left + 3, panel.top + 90) == RGB(192,192,192),
                        "demo panel has original light-gray background");
                }
            }
            Require(help.gameData_.randomState_ == rng, "demo panel painting leaves gameplay RNG alone");
            help.DrawBoard(rendered);
            GdiFlush();
            std::ofstream demoSnapshot("demo-panel-" + std::to_string(level) + ".bmp", std::ios::binary);
            demoSnapshot.write(reinterpret_cast<const char*>(&moonHeader), sizeof(moonHeader));
            demoSnapshot.write(reinterpret_cast<const char*>(&info.bmiHeader), sizeof(info.bmiHeader));
            demoSnapshot.write(static_cast<const char*>(actualPixels), 480 * 432 * 4);
            Require(demoSnapshot.good(), "write demo panel visual regression snapshot");
            help.demoMode_ = false;
            help.DrawBoard(rendered);
            help.gameData_.DrawLevel(expected, atlas);
            GdiFlush();
            Require(std::memcmp(actualPixels, pixels, 480 * 432 * 4) == 0,
                "starting gameplay removes the entire demo panel without leaving graphics behind");
        }
        for (std::uint16_t tile = 0x23; tile <= 0x2a; ++tile) {
            help.gameData_ = base;
            auto source = base;
            source.level_[10][10] = tile;
            help.drainAnimationBoard_ = source;
            help.drainAnimationFrame_ = 0;
            const auto rng = help.gameData_.randomState_;
            const auto ticks = help.gameData_.animationFrame_;
            for (unsigned frame = 0; frame < 5; ++frame) {
                Require(help.IsDrainAnimating(), "drain animation remains active for five frames");
                help.DrawBoard(rendered);
                compareSprite(120, 120, (tile <= 0x26 ? 0x35 : 0x30) + frame);
                help.AdvanceAnimation();
                Require(help.gameData_.animationFrame_ == ticks && help.gameData_.randomState_ == rng,
                    "drain presentation does not advance destination gameplay or animation");
                Require(help.AdvanceDrainAnimation(), "advance original drain frame");
            }
            Require(!help.IsDrainAnimating() && !help.AdvanceDrainAnimation(),
                "drain animation terminates after five frames");
            help.DrawBoard(rendered);
            help.gameData_.DrawLevel(expected, atlas);
            GdiFlush();
            Require(std::memcmp(actualPixels, pixels, 480 * 432 * 4) == 0,
                "destination replaces source preview without stale drain graphics");
        }
        help.gameData_ = base;
        help.gameData_.level_[10][10] = 0x27;
        help.gameData_.pendingTransition_ = 18;
        help.HandleBoardTransition();
        Require(help.IsDrainAnimating() && help.gameData_.LevelNumber() == 18
            && help.drainAnimationBoard_->LevelNumber() == 17,
            "real forward transition commits destination and retains source only for drawing");
        const auto drainSave = std::filesystem::path(L"drain-animation-roundtrip.vkw");
        Require(help.SaveGame(drainSave) && help.LoadGame(drainSave)
            && !help.IsDrainAnimating() && help.gameData_.LevelNumber() == 18,
            "saving mid-animation stores destination and loading discards presentation state");
        std::filesystem::remove(drainSave);
        Require(help.gameData_.PositionPlayerAtTile(0x23) && help.ActivateTransporter(true)
            && help.IsDrainAnimating() && help.gameData_.LevelNumber() == 17
            && help.drainAnimationBoard_->LevelNumber() == 18,
            "return transition animates its source and restores the exact parent");
        Require(help.NewGame() && !help.IsDrainAnimating(), "new game cancels stale drain presentation");
        SelectObject(rendered, oldActual); SelectObject(expected, oldExpected);
        DeleteObject(actualBitmap); DeleteObject(expectedBitmap);
        DeleteDC(rendered); DeleteDC(expected);
    }
    // 1010:65e9..688d: the Moon Room's two right-hand chambers remain
    // intact until their seal is destroyed; stepping on the switch disarms it.
    for (int chamberRow : {10, 23}) {
        for (std::uint16_t seal : std::array<std::uint16_t, 3>{0x37, 0x5c, 0}) {
            Require(game.Load(argv[1], 6), "load Moon Room trap regression");
            game.actors_.clear();
            game.overlays_.clear();
            game.playerColumn_ = 5; game.playerRow_ = 5;
            game.level_[32][chamberRow] = seal;
            game.AdvanceBoardRuntime(false, 0, 0, false);
            const bool released = seal == 0;
            Require(game.actors_.size() == (released ? 4u : 0u), "only destroyed seal releases four trap actors");
            for (int row : {chamberRow - 1, chamberRow + 1})
                for (int column : {31, 32})
                    Require((game.level_[column][row] == 0) == released,
                        "Moon Room chamber tiles survive intact and switch-opened seals");
            game.AdvanceBoardRuntime(false, 0, 0, false);
            Require(game.actors_.size() == (released ? 4u : 0u), "Moon Room trap releases only once");
        }
    }
    Require(game.Load(argv[1], 6), "load lower Moon Room sentinel regression");
    game.actors_.clear(); game.playerColumn_ = 5; game.playerRow_ = 5;
    game.level_[32][23] = 0;
    game.level_[31][22] = 0; // Upper-left corner is not this chamber's sentinel.
    game.AdvanceBoardRuntime(false, 0, 0, false);
    Require(game.actors_.size() == 4, "lower Moon Room trap tests bottom-left corner");
    for (int chamberRow : {10, 23}) {
        Require(game.Load(argv[1], 6), "load Moon Room floor-switch regression");
        game.actors_.clear();
        game.playerColumn_ = 25; game.playerRow_ = chamberRow == 10 ? 9 : 24;
        game.AdvanceBoardRuntime(true, 1, 0, false);
        Require(game.level_[32][chamberRow] == 0x5c && game.actors_.empty(),
            "Moon Room floor switch opens seal without releasing trap");
    }
    // a0ca compares both coordinates; none of the eight neighboring cells
    // qualifies, regardless of the pickup's underlying terrain.
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            for (const std::uint16_t terrain : std::array<std::uint16_t, 2>{0, 3}) {
                game = base;
                game.level_[11][11] = terrain;
                game.overlays_ = {{1, 11, 11}};
                game.playerColumn_ = 11 + dx;
                game.playerRow_ = 11 + dy;
                const auto seed = game.randomState_;
                game.ResolveOverlaysAtPlayer();
                Require(game.inventory_.size() == 1 && game.overlays_.size() == 1
                    && game.randomState_ == seed && game.TakeMessages().empty(),
                    "adjacent pickup cannot grant inventory, consume RNG or emit a notice");
            }
        }
    }
    game = base;
    game.level_[11][10] = 3;
    game.overlays_ = {{1, 11, 10}};
    Require(game.MovePlayer(0, 0) && !game.MovePlayer(1, 0)
        && game.inventory_.size() == 1 && game.overlays_.size() == 1,
        "wait and blocked entry leave treasure untouched");
    Require(game.DamageBoardSquare(11, 10, 1) && game.level_[11][10] == 0,
        "clear Crud underneath colorful treasure");
    game.TakeMessages();
    Require(game.MovePlayer(0, 0) && game.inventory_.size() == 1,
        "clearing nearby terrain does not remotely collect treasure");
    Require(game.MovePlayer(1, 0) && game.inventory_.size() > 1
        && game.TakeMessage().starts_with(L"You found "),
        "entering the cleared pickup cell grants treasure");
    for (const auto& overlay : game.overlays_)
        Require(overlay.type != 1, "collected treasure node removed");
    const auto stock = game.inventory_.size();
    Require(game.MovePlayer(0, 0) && game.inventory_.size() == stock
        && game.TakeMessages().empty(), "collected pickup cannot grant twice");

    game = base;
    game.actors_ = {{0, 12, 11, 0, 0, 1}, {1, 13, 11, 0, 0, 1},
                    {4, 11, 9, 0, 0, 1}, {0, 14, 10, 0, 0, 1}};
    game.projectiles_ = {{4, 11, 10, 1, 0}};
    game.AdvanceProjectiles();
    Require(game.actors_.size() == 1 && game.actors_[0].column == 14,
        "1008:790c fireball visits every neighboring actor but not distance two");
    Require(game.score_ == 30 && game.projectiles_.size() == 1,
        "fireball scores all three victims and survives");

    for (const std::uint32_t seed : {1u, 2u, 3u, 4u, 5u}) {
        game = base;
        game.randomState_ = seed;
        game.actors_ = {{2, 13, 11, 0, 0, 1}};
        game.projectiles_ = {{4, 11, 10, 1, 0}};
        const auto next = seed * 0x343fdu + 0x269ec3u;
        const bool dies = ((next >> 16u) & 0x7fffu) % 3u == 0;
        game.AdvanceProjectiles();
        Require(game.actors_.empty() == dies,
            "1008:7969 resistant diagonal actor uses distance-plus-one roll");
    }

    game = base;
    game.level_[12][10] = 3;
    game.projectiles_ = {{4, 11, 10, 1, 0}};
    game.AdvanceProjectiles();
    Require(game.level_[12][10] == 3 && game.projectiles_.empty() && game.score_ == 0,
        "1008:7c60 fireball stops on terrain without destroying it");

    for (const std::uint16_t actorType : std::array<std::uint16_t, 2>{2, 3}) {
        game = base;
        game.actors_ = {{actorType, 12, 10, 0, 0, 1}};
        game.projectiles_ = {{0, 11, 10, 1, 0}};
        game.AdvanceProjectiles();
        Require(game.projectiles_.empty() && game.actors_.size() == 1 && game.score_ == 0,
            "1008:7a1d immune enemy absorbs bubble without damage");
    }

    game = base;
    game.SetRootBoard(false);
    Require(game.MovePlayerToward(10, 10) && !game.IdentifySquare(10, 10).empty(),
        "6048/60f4 playable ancestry is not the demo flag");
    drain_storm::ApplicationState demo;
    demo.demoMode_ = true;
    for (const auto level : {1, 2, 5, 6, 9, 10, 16}) {
        demo.gameData_.levelNumber_ = level;
        const RECT bounds = demo.DemoPanelBounds();
        const int x = level == 5 || level == 10 ? 12 : level == 6 ? 228 : level == 16 ? 24 : 144;
        const int y = level == 5 || level == 6 || level == 10 ? 12 : 276;
        Require(bounds.left == x && bounds.top == y && bounds.right == x + 240 && bounds.bottom == y + 144,
            "198e demo panel follows the current board class");
        for (int button = 0; button < 3; ++button) {
            Require(demo.DemoButtonAt(x + 24 + button * 84, y + 96) == button
                && demo.DemoButtonAt(x + 47 + button * 84, y + 119) == button
                && demo.DemoButtonAt(x + 48 + button * 84, y + 119) == -1,
                "demo sprite button hit regions match original rectangles");
        }
        demo.demoMode_ = false;
        Require(demo.DemoButtonAt(x + 24, y + 96) == -1, "no demo controls in gameplay");
        demo.demoMode_ = true;
    }
    demo.demoMode_ = false;
    for (unsigned lever = 0; lever < 2; ++lever) {
        drain_storm::GameData fast;
        Require(fast.Load(argv[1], 5), "load tunnel lever timing fixture");
        fast.actors_.clear(); fast.projectiles_.clear(); fast.overlays_.clear();
        fast.playerColumn_ = 36; fast.playerRow_ = 2 + lever;
        fast.level_[36][2 + lever] = fast.level_[37][2 + lever] = 0;
        auto waited = fast;
        for (unsigned toggle = 0; toggle < 4; ++toggle) {
            Require(fast.MovePlayer(1, 0) && waited.MovePlayer(1, 0), "enter tunnel lever");
            const unsigned left = lever == 0 ? 31 : 34;
            const unsigned top = lever == 0 ? 3 : 6;
            constexpr std::uint16_t closed[2][3][3] = {
                {{0x38, 0x3e, 0x39}, {0x37, 0, 0x37}, {0x3a, 0x3f, 0x3b}},
                {{0x38, 0x36, 0x39}, {0x3d, 0, 0x3c}, {0x3a, 0x36, 0x3b}}};
            constexpr std::uint16_t open[2][3][3] = {
                {{0x36, 0x3e, 0x36}, {0, 0, 0}, {0x36, 0x3f, 0x36}},
                {{0x37, 0, 0x37}, {0x3d, 0, 0x3c}, {0x37, 0, 0x37}}};
            for (unsigned y = 0; y < 3; ++y)
                for (unsigned x = 0; x < 3; ++x)
                    Require(fast.TileAt(left + x, top + y) ==
                        (toggle % 2 == 0 ? open[lever][y][x] : closed[lever][y][x]),
                        "lever writes original ce88/cafa tunnel tiles on every toggle");
            for (unsigned tick = 0; tick < 6; ++tick) waited.AdvanceAnimation();
            Require(fast.MovePlayer(-1, 0) && waited.MovePlayer(-1, 0), "leave lever mid-animation");
            Require(fast.level_ == waited.level_ && fast.randomState_ == waited.randomState_,
                "moving during lever animation preserves the same board and RNG as waiting");
        }
        for (unsigned tick = 0; tick < 6; ++tick) fast.AdvanceAnimation();
        Require(std::none_of(fast.overlays_.begin(), fast.overlays_.end(), [](const auto& overlay) {
            return overlay.type == 0x14 || overlay.type == 0x16;
        }), "overlapping lever animations expire without leaving stale tiles");
    }
    demo.gameData_ = base;
    demo.demoMode_ = true;
    Require(!demo.MovePlayerToward(10, 10) && !demo.UseSelectedItemToward(11, 10)
        && demo.IdentifySquare(10, 10).empty(), "application still blocks demo mouse input");

    constexpr const wchar_t* keyMessages[] = {L"You found the Moon Key!",
        L"You found the Star Key!", L"You found the Fire Key!"};
    constexpr std::uint16_t keyTypes[] = {17, 16, 18};
    for (std::uint16_t index = 0; index < 3; ++index) {
        game = base;
        game.ResolveOverlayEffect(4 + index);
        Require(game.TakeMessage() == keyMessages[index]
            && game.inventory_.back().type == keyTypes[index], "a2b4/a2d1/a2ee key identity");
    }

    for (const std::size_t count : {1u, 20u, 21u, 50u}) {
        drain_storm::ApplicationState state;
        state.RecordGameplayMessage(L"old message");
        for (std::size_t index = 0; index < count; ++index)
            state.gameData_.pendingMessages_.push_back(std::to_wstring(index));
        const auto oldest = count > 20 ? count - 20 : 0;
        Require(state.TakeGameplayMessage() == std::to_wstring(oldest),
            "55e0 history selects first retained message after batch eviction");
        Require(state.messageHistory_.size() <= 20
            && state.messageHistoryCursor_ < state.messageHistory_.size(), "bounded history cursor");
        Require(state.TakeGameplayMessage().empty(), "message batch drained exactly once");
    }

    // 8310 completes each actor before moving the next one.
    game = base;
    game.actors_ = {{0, 9, 10, 1, 0, 3}};
    game.projectiles_ = {{4, 10, 10, 1, 0}};
    game.AdvanceActors();
    Require(game.health_ == 2 && game.actors_.empty() && game.score_ == 0
        && game.projectiles_.size() == 1, "player collision precedes projectile collision");
    game = base;
    game.actors_ = {{0, 11, 10, 0, 0, 1}, {0, 12, 10, -1, 0, 3}};
    game.projectiles_ = {{1, 11, 10, 1, 0}};
    game.AdvanceActors();
    Require(game.actors_.size() == 1 && game.actors_[0].column == 11
        && game.projectiles_.empty() && game.score_ == 10,
        "earlier death frees a destination for the next actor in the same turn");
    game = base;
    game.actors_ = {{1, 12, 10, 0, 0, 0}};
    game.projectiles_ = {{4, 12, 10, 1, 0}};
    game.level_[11][10] = 3;
    game.genericBreakDivisor_ = 1;
    game.AdvanceActors();
    Require(game.actors_.size() == 1 && game.level_[11][10] == 0
        && game.projectiles_.size() == 1, "terrain-breaking actor skips collision phase that turn");
    game = base;
    game.actors_ = {{0, 12, 10, -1, 0, 3}};
    game.level_[11][10] = 1;
    game.projectiles_ = {{4, 12, 10, 1, 0}};
    game.AdvanceActors();
    Require(game.actors_.size() == 1 && game.actors_[0].behavior == 0,
        "blocked vector actor changes behavior without collision dispatch");
    game = base;
    game.actors_ = {{3, 20, 20, 0, 0, 1}};
    game.projectiles_ = {{1, 20, 20, 1, 0}};
    game.randomState_ = 1;
    game.AdvanceActors();
    Require(!game.actors_.empty() && game.score_ == 250, "captain collision spawns lieutenants");
    bool splitMoved = false;
    for (const auto& actor : game.actors_)
        splitMoved = splitMoved || actor.column != 20 || actor.row != 20;
    Require(splitMoved, "new lieutenants receive their turn before AdvanceActors returns");

    const auto ownsCannon = [](const drain_storm::GameData& board) {
        for (const auto& item : board.inventory_) if (item.type == 1) return true;
        return false;
    };
    for (std::uint32_t seed = 1; seed <= 256; ++seed) {
        game = base;
        game.randomState_ = seed;
        game.boardDepth_ = 1;
        game.ResolveOverlayEffect(1);
        Require(!ownsCannon(game), "a1ee depth-one colorful treasure never grants cannon");
        for (int depth = 1; depth <= 3; ++depth) {
            game = base;
            game.boardDepth_ = static_cast<std::uint16_t>(depth);
            game.randomState_ = seed;
            const auto next = seed * 0x343fdu + 0x269ec3u;
            const bool grant = depth >= 3 && (((next >> 16u) & 0x7fffu) % 3u + 1) == 3;
            game.ResolveOverlayEffect(2);
            Require(ownsCannon(game) == grant, "a249 type-two depth/ownership cannon gate");
            if (grant) Require(game.randomState_ == next, "cannon branch consumes exactly one roll");
            game = base;
            game.boardDepth_ = static_cast<std::uint16_t>(depth);
            game.inventory_.push_back({1, 3});
            game.randomState_ = seed;
            game.ResolveOverlayEffect(2);
            Require(game.inventory_[1].quantity == 3, "a270 does not grant duplicate cannon stock");
        }
    }
    bool checkedDepthTwoGrant = false;
    bool checkedDepthTwoReject = false;
    for (std::uint32_t seed = 1; seed < 10000; ++seed) {
        auto state = seed;
        const auto roll = [&state](unsigned bound) {
            state = state * 0x343fdu + 0x269ec3u;
            return ((state >> 16u) & 0x7fffu) % bound + 1;
        };
        if (roll(12) != 6) continue;
        const bool grant = roll(5) == 5;
        const auto afterGate = state;
        // Exclude recursive containers in the rejection fixture.
        if (!grant && (roll(30) == 12 || roll(17) > 10)) continue;
        game = base;
        game.boardDepth_ = 2;
        game.randomState_ = seed;
        game.ResolveOverlayEffect(1);
        Require(ownsCannon(game) == grant, "a1fc depth-two extra one-in-five gate");
        if (grant) { checkedDepthTwoGrant = true;
            Require(game.randomState_ == afterGate, "depth-two grant consumes two rolls"); }
        else checkedDepthTwoReject = true;
        if (checkedDepthTwoGrant && checkedDepthTwoReject) break;
    }
    Require(checkedDepthTwoGrant && checkedDepthTwoReject, "both depth-two branch fixtures covered");

    game = base;
    game.boardDepth_ = 42;
    std::stringstream saved;
    Require(game.WriteGame(saved), "write v23 construction depth");
    auto loaded = base;
    Require(loaded.ReadGame(saved) && loaded.boardDepth_ == 42, "depth survives board save/load");
    loaded.AdoptPlayerStateFrom(base);
    Require(loaded.boardDepth_ == 42, "player transfer does not replace board depth");
    auto noticeSource = base;
    noticeSource.pendingMessages_ = {L"first", L"second"};
    loaded.AdoptPlayerStateFrom(noticeSource);
    Require(loaded.TakeMessage() == L"first" && loaded.TakeMessage() == L"second",
        "pending notices survive player transfer in chronological order");
    auto legacy = saved.str();
    legacy[5] = 22;
    legacy.resize(legacy.size() - sizeof(std::uint16_t) - base.overlays_.size() - 33);
    std::stringstream legacyStream(legacy);
    Require(loaded.ReadGame(legacyStream) && loaded.boardDepth_ == 1,
        "v22 procedural-root save remains readable with inferred depth");
    drain_storm::ApplicationState traversal;
    traversal.gameData_ = base;
    traversal.demoMode_ = false;
    traversal.resourceDirectory_ = std::filesystem::path(argv[1]).parent_path();
    traversal.gameData_.level_[10][10] = 0x27;
    Require(traversal.ActivateTransporter(false) && traversal.CurrentLevel() == 18
        && traversal.gameData_.boardDepth_ == 2, "new child receives construction depth plus one");
    Require(traversal.ActivateTransporter(true) && traversal.CurrentLevel() == 17
        && traversal.gameData_.boardDepth_ == 1, "returning to parent retains its original depth");
    Require(traversal.ActivateTransporter(false) && traversal.gameData_.boardDepth_ == 2,
        "cached child depth does not increment on revisit");

    // ba30/+18: every common class supports all four blue return tiles.
    for (std::size_t level = 1; level <= 25; ++level) {
        if (level == 5) continue;
        for (std::uint16_t tile = 0x23; tile <= 0x26; ++tile) {
            game = base; game.levelNumber_ = level; game.rootBoard_ = false;
            game.level_[10][10] = tile;
            game.DetectBoardTransition();
            Require(game.TakeTransitionRequest() == -1, "common blue-drain return on every class");
            game.rootBoard_ = true; game.DetectBoardTransition();
            Require(game.TakeTransitionRequest() == 0, "root has no parent transition");
        }
    }
    {
        drain_storm::ApplicationState graph;
        graph.resourceDirectory_ = std::filesystem::path(argv[1]).parent_path();
        graph.soundEnabled_ = false;
        Require(graph.gameData_.Load(argv[1], 3), "load branching hub");
        // Walk into real connection tiles rather than invoking U/D directly.
        const auto walk = [&](std::uint16_t tile, std::size_t destination, std::uint16_t landing) {
            auto& board = graph.gameData_;
            if (!board.PositionPlayerAtTile(tile)) {
                std::cerr << "Missing tile " << tile << " on level " << board.LevelNumber() << '\n';
                Require(false, "source has connection tile");
            }
            const auto x = board.PlayerColumn(), y = board.PlayerRow();
            const int dx = x > 0 ? 1 : -1;
            board.playerColumn_ = x - dx;
            board.level_[board.playerColumn_][y] = 0;
            board.actors_.clear(); board.projectiles_.clear(); board.overlays_.clear();
            board.health_ = 1000;
            Require(graph.MovePlayer(dx, 0) && graph.CurrentLevel() == destination,
                "walking into connection loads correct board");
            Require(graph.CurrentTileAt(graph.PlayerColumn(), graph.PlayerRow()) == landing,
                "transition lands on correct reciprocal connection");
            Require(graph.boardHistory_.size() == graph.currentBoardPath_.size(),
                "graph ancestry and traversal path stay aligned");
        };
        walk(0x29, 2, 0x23); // casino
        graph.gameData_.casinoBonusStage_ = 2;
        walk(0x23, 3, 0x29);
        Require(graph.boardHistory_.empty(), "casino returns to root without phantom history");
        walk(0x29, 2, 0x23);
        Require(graph.gameData_.casinoBonusStage_ == 2, "casino state survives revisit");
        walk(0x23, 3, 0x29);
        walk(0x27, 6, 0x23);
        graph.gameData_.score_ = 12345;
        walk(0x28, 4, 0x23);
        walk(0x27, 10, 0x23);
        walk(0x23, 4, 0x27);
        walk(0x23, 6, 0x28);
        walk(0x27, 5, 0x23);
        walk(0x27, 8, 0x23);
        graph.gameData_.level_[1][1] = 0x7e;
        walk(0x23, 5, 0x27);
        walk(0x24, 7, 0x27); // other parent, never previously visited
        Require(graph.boardHistory_.size() == 1 && graph.gameData_.boardDepth_ == 2,
            "cross-link restores sibling ancestry and depth");
        walk(0x28, 1, 0x23); // previously omitted Power Plant edge
        walk(0x23, 7, 0x28);
        walk(0x27, 5, 0x24);
        walk(0x27, 8, 0x23);
        Require(graph.gameData_.level_[1][1] == 0x7e,
            "shared board descendant retains identity through either arm");
        const auto graphPath = std::filesystem::temp_directory_path()
            / (L"drain-branch-regression-" + std::to_wstring(GetCurrentProcessId()) + L".VKW");
        Require(graph.SaveGame(graphPath) && graph.LoadGame(graphPath), "round-trip branching save");
        std::filesystem::remove(graphPath);
        walk(0x23, 5, 0x27);
        walk(0x23, 6, 0x27);
        walk(0x23, 3, 0x27);
        Require(graph.boardHistory_.empty() && graph.gameData_.rootBoard_,
            "cross-linked branch unwinds completely to original root");
        walk(0x28, 7, 0x23);
        walk(0x23, 3, 0x28);
        Require(graph.boardHistory_.empty(), "second branch returns to its own hub exit");
        walk(0x27, 6, 0x23);
        // Reproduce the phantom ancestor written by the previous implementation.
        auto phantom = graph.gameData_;
        Require(phantom.Load(argv[1], 5), "load legacy phantom ancestor");
        graph.boardHistory_.push_back({phantom, 0x23});
        graph.currentBoardPath_.push_back(0x23);
        walk(0x23, 3, 0x27);
        Require(graph.boardHistory_.empty(), "recover legacy shared-return stack when hub is retained");

        struct Edge { std::size_t source; std::uint16_t tile; std::size_t target; };
        for (const auto edge : {Edge{3, 0x2a, 11}, Edge{8, 0x27, 11}, Edge{11, 0x27, 11},
                               Edge{12, 0x27, 11}, Edge{13, 0x27, 14}, Edge{13, 0x28, 11},
                               Edge{14, 0x27, 15}, Edge{15, 0x27, 16}, Edge{17, 0x27, 18}}) {
            graph.boardHistory_.clear(); graph.currentBoardPath_.clear();
            graph.sharedBoards_.clear(); graph.cachedBoards_.clear();
            Require(graph.gameData_.Load(argv[1], edge.source), "load forward-edge fixture");
            // c80e has no down drain on construction; exercise its recovered
            // c8cc handler with an explicit connection fixture.
            if (edge.source == 8) graph.gameData_.level_[10][10] = 0x27;
            if (edge.source == 11) graph.gameData_.levelElevenRuntimeFlags_ = 0x20;
            walk(edge.tile, edge.target, 0x23);
            graph.gameData_.level_[1][1] = 0x7e;
            walk(0x23, edge.source, edge.tile);
            Require(graph.boardHistory_.empty(), "recursive/maze branch returns to exact instance");
            walk(edge.tile, edge.target, 0x23);
            Require(graph.gameData_.level_[1][1] == 0x7e, "recursive/maze child state survives revisit");
        }
    }

    for (int rejection = 0; rejection < 5; ++rejection) {
        game = base;
        game.levelNumber_ = 2;
        game.playerColumn_ = 23; game.playerRow_ = 26;
        game.inventory_ = {{4, 5}};
        game.repellentEnergy_ = 100;
        game.casinoStatus_ = rejection == 0 ? 4 : rejection == 1 ? 2 : 1;
        if (rejection == 2) game.inventory_ = {{0, 0}};
        if (rejection >= 3) {
            game.casinoBetCount_ = rejection == 4 ? 20 : 1;
            game.casinoBetItemType_ = rejection == 4 ? 4 : 5;
        }
        game.actors_ = {{0, 30, 30, -1, 0, 3}};
        game.projectiles_ = {{0, 5, 5, 1, 0}};
        const auto quantity = game.inventory_[0].quantity;
        Require(game.UseSelectedItem(1, 0) && game.turnCounter_ == 1
            && game.repellentEnergy_ == 98 && game.actors_[0].column == 29
            && game.projectiles_[0].column == 6 && game.inventory_[0].quantity == quantity,
            "every rejected casino wager advances world without consuming stock");
    }

    for (std::uint16_t bonus = 0; bonus < 3; ++bonus) {
        game = base;
        game.levelNumber_ = 2; game.playerColumn_ = 26; game.playerRow_ = 26;
        game.casinoStatus_ = 2; game.casinoBetCount_ = 2; game.casinoBetItemType_ = 4;
        game.casinoBonusStage_ = bonus; game.casinoRanks_ = {1, 10, 11, 12, 13};
        game.casinoSuits_.fill(1); game.casinoHeld_.fill(1);
        game.AdvanceBoardRuntime(true, 1, 0, false);
        drain_storm::ApplicationState messages;
        messages.gameData_ = game;
        Require(messages.TakeGameplayMessage().find(L"Royal Flush") != std::wstring::npos,
            "winner message appears first");
        Require(messages.NavigateMessageHistory(false).size() > 0, "retaliation message is reviewable");
        const auto prize = messages.NavigateMessageHistory(false);
        Require(prize == (bonus == 0 ? L"You won a bonus prize!\n(Left of table.)"
            : bonus == 1 ? L"You won a bonus prize!\n(Right of table.)"
            : L"BONUS! You have gained access to the treasure room."),
            "complete bonus notice survives 99-character history limit");
        if (bonus == 2) Require(messages.NavigateMessageHistory(false)
            == L"You're too good.\nCasino is closed...", "closure is a separate complete message");
        if (bonus == 0) Require(game.level_[13][19] == 0x5c,
            "casino left bonus door uses original column and row");
        if (bonus == 1) Require(game.level_[35][19] == 0x5c,
            "casino right bonus door uses original column and row");
        if (bonus == 2) {
            for (int column : {7, 8, 9, 32, 33, 34})
                for (int row = 2; row <= 4; ++row)
                    Require(game.level_[column][row] == 0,
                        "casino final bonus clears original treasure-room columns");
        }
    }

    const auto evaluateCasino = [&](std::array<std::uint16_t, 5> ranks,
                                    std::array<std::uint16_t, 5> suits,
                                    std::uint16_t wild, std::uint32_t turns,
                                    std::int32_t score) {
        auto hand = base;
        hand.levelNumber_ = 2; hand.playerColumn_ = 26; hand.playerRow_ = 26;
        hand.casinoStatus_ = 2; hand.casinoBetCount_ = 1; hand.casinoBetItemType_ = 4;
        hand.casinoRanks_ = ranks; hand.casinoSuits_ = suits; hand.casinoHeld_.fill(1);
        hand.casinoWildRank_ = wild; hand.turnCounter_ = turns; hand.score_ = score;
        hand.levelTwoSequenceState_ = -1;
        hand.AdvanceBoardRuntime(true, 1, 0, false);
        return hand;
    };
    const auto payout = [](const drain_storm::GameData& hand) {
        for (const auto& item : hand.inventory_)
            if (item.type == 4) return item.quantity;
        return std::uint16_t{0};
    };
    Require(payout(evaluateCasino({2, 2, 4, 8, 10}, {1, 2, 3, 4, 1}, 0, 1, 0)) == 0,
        "casino does not pay ordinary low pair");
    Require(payout(evaluateCasino({1, 10, 11, 12, 13}, {1, 2, 3, 4, 1}, 0, 1, 0)) == 8,
        "casino pays mixed-suit ace-high straight");
    Require(payout(evaluateCasino({2, 2, 2, 2, 3}, {1, 2, 3, 4, 1}, 2, 1, 0)) == 20,
        "casino preserves original four-wild-card classification");
    const auto earlyWin = evaluateCasino({1, 3, 5, 8, 11}, {1, 1, 1, 1, 1}, 0, 1, 2000);
    const auto lateWin = evaluateCasino({1, 3, 5, 8, 11}, {1, 1, 1, 1, 1}, 0, 1601, 0);
    Require(earlyWin.levelTwoSequenceMode_ == 2 && lateWin.levelTwoSequenceMode_ == 3,
        "casino retaliation uses elapsed turns rather than score");
    for (bool right : {false, true}) {
        auto machine = base;
        machine.levelNumber_ = 2; machine.casinoStatus_ = 0;
        machine.levelTwoSequenceMode_ = 1;
        machine.levelTwoSequenceState_ = right ? 104 : 4;
        machine.AdvanceBoardRuntime(false, 0, 0, false);
        Require(machine.level_[right ? 36 : 4][right ? 30 : 31]
            == (right ? 0x3f : 0x3e),
            "casino machine restores its directional gate");
    }
    auto activeWin = base;
    activeWin.levelNumber_ = 2; activeWin.playerColumn_ = 26; activeWin.playerRow_ = 26;
    activeWin.casinoStatus_ = 2; activeWin.casinoBetCount_ = 1;
    activeWin.casinoBetItemType_ = 4;
    activeWin.casinoRanks_ = {1, 3, 6, 8, 10};
    activeWin.casinoSuits_.fill(1); activeWin.casinoHeld_.fill(1);
    activeWin.levelTwoSequenceMode_ = 1; activeWin.levelTwoSequenceState_ = 2;
    activeWin.AdvanceBoardRuntime(true, 1, 0, false);
    Require(activeWin.levelTwoSequenceMode_ == 1 && activeWin.levelTwoSequenceState_ == 4,
        "casino win advances but does not replace an active retaliation cycle");
    bool captainSpawned = false;
    for (std::uint32_t seed = 1; seed <= 1000 && !captainSpawned; ++seed) {
        auto machine = base;
        machine.levelNumber_ = 2; machine.level_[4][33] = 0x1e;
        machine.levelTwoSequenceMode_ = 3; machine.levelTwoSequenceState_ = 0;
        machine.randomState_ = seed;
        machine.AdvanceCasinoRetaliation(0);
        for (const auto& actor : machine.actors_) {
            if (actor.type != 3) continue;
            captainSpawned = true;
            Require(actor.column == 36 && actor.row == 30,
                "left casino machine releases captain at original remote gate");
        }
    }
    Require(captainSpawned, "find mode-three casino captain spawn seed");
    drain_storm::GameData defendedCasino;
    Require(defendedCasino.Load(argv[1], 2, 1), "load casino defense fixture");
    defendedCasino.actors_.clear(); defendedCasino.level_[11][0] = 0;
    defendedCasino.turnCounter_ = 1;
    defendedCasino.AdvanceBoardRuntime(false, 0, 0, false);
    Require(defendedCasino.actors_.size() >= 3 && defendedCasino.casinoTopChestCount_ == 6,
        "casino chest loss releases captains from intact top structures");
    const auto defenderCount = defendedCasino.actors_.size();
    defendedCasino.AdvanceBoardRuntime(false, 0, 0, false);
    Require(defendedCasino.actors_.size() == defenderCount,
        "casino chest loss triggers defense only once");
    defendedCasino.level_[4][33] = 0; defendedCasino.turnCounter_ = 200;
    defendedCasino.AdvanceBoardRuntime(false, 0, 0, false);
    Require(defendedCasino.level_[4][33] == 0x1e,
        "casino periodically reconstructs destroyed corner machine");
    std::stringstream casinoDefenseSave;
    Require(defendedCasino.WriteGame(casinoDefenseSave), "save casino chest observation");
    auto restoredDefense = base;
    Require(restoredDefense.ReadGame(casinoDefenseSave)
        && restoredDefense.casinoTopChestCount_ == 6,
        "casino chest observation survives save and reload");
    auto oldDefenseBytes = casinoDefenseSave.str();
    oldDefenseBytes[5] = 28;
    oldDefenseBytes.resize(oldDefenseBytes.size() - 14);
    std::stringstream oldDefenseSave(oldDefenseBytes);
    Require(restoredDefense.ReadGame(oldDefenseSave)
        && restoredDefense.casinoTopChestCount_ == 6,
        "version-28 casino save derives chest observation from terrain");

    drain_storm::GameData alienMaze;
    Require(alienMaze.Load(argv[1], 16, 1), "load alien maze for dormant-wall audit");
    int ordinaryOutsideEntry = 0, dormantWalls = 0;
    for (int column = 0; column < 40; ++column) {
        for (int row = 0; row < 36; ++row) {
            if (column >= 5 && column <= 9 && row >= 5 && row <= 9) continue;
            const auto tile = alienMaze.level_[column][row];
            ordinaryOutsideEntry += tile >= 0x36 && tile <= 0x40;
            dormantWalls += tile == 0x5d;
        }
    }
    Require(ordinaryOutsideEntry == 0 && dormantWalls > 100,
        "alien maze leaves non-entry walls dormant until activation");
    const auto originalWall = [](int mask) {
        std::array<bool, 11> candidates{}; candidates.fill(true);
        const auto disable = [&candidates](std::initializer_list<int> indices) {
            for (int index : indices) candidates[index] = false;
        };
        if (mask & 1) disable({1,3,4,7}); else disable({2,5,6,8,9,10});
        if (mask & 2) disable({1,2,5,6}); else disable({3,4,7,8,9,10});
        if (mask & 4) disable({0,4,5,8}); else disable({2,3,6,7,9,10});
        if (mask & 8) disable({0,2,3,9}); else disable({4,5,6,7,8,10});
        constexpr std::array<int, 11> tiles = {
            0x36,0x37,0x38,0x39,0x3b,0x3a,0x3c,0x3d,0x3e,0x3f,0x40};
        const auto selected = std::find(candidates.begin(), candidates.end(), true);
        return selected == candidates.end() ? 3 : tiles[static_cast<std::size_t>(
            std::distance(candidates.begin(), selected))];
    };
    for (int mask = 0; mask < 16; ++mask) {
        for (auto& column : alienMaze.level_) column.fill(0);
        alienMaze.level_[10][10] = 0x5d;
        if (mask & 1) alienMaze.level_[11][10] = 0x5d;
        if (mask & 2) alienMaze.level_[9][10] = 0x5d;
        if (mask & 4) alienMaze.level_[10][11] = 0x5d;
        if (mask & 8) alienMaze.level_[10][9] = 0x5d;
        alienMaze.ActivateLevelSixteenRegion(9, 9, 11, 11);
        Require(alienMaze.level_[10][10] == originalWall(mask),
            "maze wall shaping follows east-west-south-north neighbor order");
    }
    for (std::size_t mazeClass : {13u, 14u, 15u}) {
        drain_storm::GameData maze;
        Require(maze.Load(argv[1], mazeClass, 1), "load hidden-wall maze");
        for (int column = static_cast<int>(maze.playerColumn_) - 1;
             column <= static_cast<int>(maze.playerColumn_) + 1; ++column) {
            for (int row = static_cast<int>(maze.playerRow_) - 1;
                 row <= static_cast<int>(maze.playerRow_) + 1; ++row) {
                if (column < 0 || row < 0 || column >= 40 || row >= 36) continue;
                Require(maze.level_[column][row] < 0x2b || maze.level_[column][row] > 0x35,
                    "maze entrance reveals neighboring walls before first turn");
            }
        }
    }
    drain_storm::ApplicationState fatalPickup;
    fatalPickup.resourceDirectory_ = std::filesystem::path(argv[1]).parent_path();
    fatalPickup.demoMode_ = false; fatalPickup.soundEnabled_ = false;
    auto& fatalBoard = fatalPickup.gameData_;
    Require(fatalBoard.Load(argv[1], 17, 1), "load fatal-hit pickup fixture");
    for (auto& column : fatalBoard.level_) column.fill(0);
    fatalBoard.projectiles_.clear();
    fatalBoard.playerColumn_ = 10; fatalBoard.playerRow_ = 10;
    fatalBoard.health_ = 1; fatalBoard.terminalDeathEnabled_ = true;
    fatalBoard.actors_ = {{0, 11, 10, 0, 0, 0}};
    fatalBoard.overlays_ = {{9, 11, 10}};
    Require(fatalPickup.MovePlayer(1, 0) && !fatalPickup.IsDemoMode()
        && fatalPickup.TakeLifecycleMessage().empty()
        && !fatalPickup.MovePlayer(0, 0),
        "fatal hit keeps its animation board and blocks another turn");
    for (int frame = 0; frame < 4; ++frame) {
        fatalPickup.AdvanceAnimation();
        Require(!fatalPickup.IsDemoMode() && fatalPickup.TakeLifecycleMessage().empty(),
            "fatal hit completes all five visible frames before game over");
    }
    fatalPickup.AdvanceAnimation();
    Require(fatalPickup.IsDemoMode() && !fatalPickup.TakeLifecycleMessage().empty(),
        "same-turn life pickup cannot cancel deferred fatal game over");

    auto resistedImpact = base;
    resistedImpact.level_[10][10] = 0x10;
    resistedImpact.level_[11][11] = 0x19;
    Require(resistedImpact.DamageBoardSquare(10, 10, 1)
        && resistedImpact.level_[11][11] == 0x19
        && !resistedImpact.overlays_.empty()
        && resistedImpact.TakeSoundEvent() == drain_storm::GameplaySound::ExplosionSmall,
        "reinforced structure shows impact when bomb is resisted");
    Require(resistedImpact.TakeMessage() == L"The bomb had no effect on the portal!",
        "resisted bomb restores original portal warning");
    std::uint16_t unbreakable = 0;
    for (std::uint16_t tile = 1; tile < resistedImpact.tiles_.size(); ++tile)
        if ((resistedImpact.tiles_[tile].flags & 5u) == 4u) {
            unbreakable = tile; break;
        }
    Require(unbreakable != 0, "find indestructible solid tile");
    resistedImpact.overlays_.clear(); resistedImpact.level_[11][10] = unbreakable;
    resistedImpact.projectiles_ = {{1, 10, 10, 1, 0}};
    resistedImpact.AdvanceProjectiles();
    Require(resistedImpact.level_[11][10] == unbreakable
        && !resistedImpact.overlays_.empty()
        && resistedImpact.TakeSoundEvent() == drain_storm::GameplaySound::ExplosionSmall,
        "indestructible wall preserves terrain but displays projectile impact");
    bool foundHiddenTreasure = false;
    for (std::uint32_t seed = 1; seed <= 10000 && !foundHiddenTreasure; ++seed) {
        auto rock = base;
        rock.level_[10][10] = 3; rock.randomState_ = seed;
        rock.DamageBoardSquare(10, 10, 1);
        foundHiddenTreasure = rock.TakeMessage()
            == L"There was treasure hidden under the rockpile!";
    }
    Require(foundHiddenTreasure, "hidden-rock treasure uses original notice");

    for (int itemType : {6, 7}) {
        auto eightWay = base;
        eightWay.inventory_ = {{static_cast<std::uint16_t>(itemType), 1}};
        Require(eightWay.UseSelectedItem(1, 0) && eightWay.projectiles_.size() == 8,
            "eight-way item launches every projectile");
        Require(eightWay.projectiles_[1].deltaColumn == -1
            && eightWay.projectiles_[1].deltaRow == 0,
            "eight-way shots enumerate column before row velocity");
    }
    auto missileFragments = base;
    missileFragments.level_[10][10] = 3;
    std::vector<drain_storm::Projectile> fragments;
    Require(missileFragments.DamageBoardSquare(10, 10, 2, &fragments)
        && fragments.size() == 8
        && fragments[1].deltaColumn == -1 && fragments[1].deltaRow == 0,
        "missile terrain fragments enumerate column before row velocity");
    for (std::size_t mazeClass : {13u, 14u, 15u}) {
        auto sign = base;
        sign.levelNumber_ = mazeClass;
        Require(sign.SignpostText() == L"The Labyrinth.",
            "hidden maze classes use original Labyrinth sign");
    }
    auto emptySign = base; emptySign.levelNumber_ = 9;
    Require(emptySign.SignpostText().empty(), "class-nine sign callback is empty");
    auto bosses = base; bosses.levelNumber_ = 1;
    bosses.levelOneTargetColumns_ = {10, 24};
    bosses.levelOneTargetRows_ = {20, 10};
    bosses.levelOneTargetDefeated_.fill(0);
    Require(bosses.IdentifySquare(11, 21).find(L"Gormaug") != std::wstring::npos
        && bosses.IdentifySquare(25, 11) == L"Gorloth\nDemon Lord",
        "boss identification follows tracked moving rectangles and names");
    auto betPrompt = base;
    betPrompt.levelNumber_ = 2; betPrompt.playerColumn_ = 23; betPrompt.playerRow_ = 26;
    betPrompt.inventory_ = {{4, 1}};
    Require(betPrompt.UseSelectedItem(1, 0)
        && betPrompt.CasinoAnnouncementText()
            == L"You may continue betting, or pull the lever to deal.",
        "first casino bet updates the scrolling prompt");
    auto moonSwitch = base;
    moonSwitch.levelNumber_ = 6; moonSwitch.playerColumn_ = 25; moonSwitch.playerRow_ = 24;
    moonSwitch.levelSixStructuresSpawned_ = true;
    moonSwitch.levelSixUpperTransporterTriggered_ = true;
    moonSwitch.levelSixLowerTransporterTriggered_ = true;
    moonSwitch.levelSixCenterOpened_ = true;
    moonSwitch.level_[32][23] = 0x37;
    moonSwitch.AdvanceBoardRuntime(false, 0, 0, false);
    Require(moonSwitch.level_[32][23] == 0x5c
        && moonSwitch.TakeSoundEvent() == drain_storm::GameplaySound::Construction,
        "Moon Room seal switch requests construction sound");

    auto bubbleTerrain = base;
    bubbleTerrain.level_[11][10] = 0x23;
    bubbleTerrain.actors_ = {{0, 11, 10, 0, 0, 0}};
    bubbleTerrain.projectiles_ = {{0, 10, 10, 1, 0}};
    bubbleTerrain.AdvanceProjectiles();
    Require(bubbleTerrain.actors_.empty() && bubbleTerrain.level_[11][10] == 0x23
        && std::any_of(bubbleTerrain.overlays_.begin(), bubbleTerrain.overlays_.end(),
            [](const auto& overlay) { return overlay.type == 0x16; })
        && bubbleTerrain.TakeSoundEvent() == drain_storm::GameplaySound::Pop,
        "bubble hit paints and sounds on occupied terrain without replacing it");

    auto captainSplit = base;
    captainSplit.inventory_ = {{13, 1}};
    captainSplit.actors_ = {{3, 30, 30, 0, 0, 0}, {3, 31, 30, 0, 0, 0}};
    Require(captainSplit.UseSelectedItem(1, 0), "use captain-split Smart Bomb");
    bool foundReplacement = false;
    for (const auto& actor : captainSplit.actors_)
        if (actor.type == 2) {
            foundReplacement = true;
            Require(actor.behavior == 0, "captain replacements retain pursuit behavior");
        }
    Require(foundReplacement, "Smart Bomb produces captain replacements");

    auto identification = base;
    identification.levelNumber_ = 2; identification.level_[10][10] = 2;
    identification.playerColumn_ = 20; identification.playerRow_ = 20;
    identification.overlays_ = {{0x14, 11, 10}};
    Require(identification.IdentifySquare(10, 10) == L"Poker Table."
        && identification.IdentifySquare(11, 10) == L"Empty",
        "casino table identifies and transient effects fall through to terrain");
    auto flooded = base;
    flooded.levelNumber_ = 4; flooded.turnCounter_ = 801;
    flooded.levelFourFloodTriggered_ = false;
    flooded.levelFourCountdown_ = 0;
    flooded.levelFourNortheastTriggered_ = true;
    flooded.levelFourSouthwestTriggered_ = true;
    flooded.actors_ = {{0, 1, 1, 0, 0, 0}, {0, 2, 2, 0, 0, 0},
                       {0, 3, 3, 0, 0, 0}};
    flooded.AdvanceBoardRuntime(false, 0, 0, false);
    const auto hasFloodEffect = [&flooded](int column, int row) {
        return std::any_of(flooded.overlays_.begin(), flooded.overlays_.end(),
            [=](const auto& overlay) {
                return overlay.type == 0x16 && overlay.column == column && overlay.row == row;
            });
    };
    Require(flooded.actors_.empty() && hasFloodEffect(2, 2) && hasFloodEffect(3, 3)
        && !hasFloodEffect(1, 1),
        "Gauntlet flood paints backward removal effects except actor zero");
    auto gauntletHazard = base;
    gauntletHazard.levelNumber_ = 4; gauntletHazard.randomState_ = 1;
    gauntletHazard.levelFourCountdown_ = 0;
    gauntletHazard.levelFourNortheastTriggered_ = false;
    gauntletHazard.levelFourSouthwestTriggered_ = true;
    gauntletHazard.levelFourFloodTriggered_ = true;
    gauntletHazard.level_[33][5] = 0;
    gauntletHazard.AdvanceBoardRuntime(false, 0, 0, false);
    Require(gauntletHazard.level_[28][26] == 0x22
        && gauntletHazard.level_[26][28] != 0x22,
        "Gauntlet transporter hazards retain original column/row draw order");
    auto guardedPlacement = base;
    guardedPlacement.levelNumber_ = 11; guardedPlacement.randomState_ = 1;
    guardedPlacement.playerColumn_ = 38; guardedPlacement.playerRow_ = 34;
    for (auto& column : guardedPlacement.level_) column.fill(0);
    Require(guardedPlacement.PlaceRandomGuardedStructure(true)
        && guardedPlacement.level_[7][5] == 0x18
        && guardedPlacement.overlays_.size() == 9,
        "guarded structure rolls original column before row and paints effects");
    auto alarmPlacement = base;
    alarmPlacement.levelNumber_ = 8; alarmPlacement.randomState_ = 1;
    alarmPlacement.playerColumn_ = 39; alarmPlacement.playerRow_ = 35;
    for (auto& column : alarmPlacement.level_) column.fill(0);
    alarmPlacement.AdvanceBoardRuntime(false, 0, 0, false);
    Require(alarmPlacement.levelEightAlarmTriggered_
        && alarmPlacement.level_[7][5] == 0x18
        && alarmPlacement.overlays_.size() >= 9
        && alarmPlacement.TakeSoundEvent() == drain_storm::GameplaySound::Construction,
        "alarm placement restores original axes, construction effects and sound");
    auto arsenalViewport = base;
    arsenalViewport.inventory_.clear();
    for (std::uint16_t item = 1; item <= 12; ++item)
        arsenalViewport.inventory_.push_back({item, 1});
    arsenalViewport.selectedItem_ = 0; arsenalViewport.firstVisibleItem_ = 1;
    Require(arsenalViewport.SelectPreviousItem() && arsenalViewport.selectedItem_ == 0
        && arsenalViewport.firstVisibleItem_ == 0,
        "previous at first item still brings selection into view");
    arsenalViewport.selectedItem_ = 11; arsenalViewport.firstVisibleItem_ = 0;
    Require(arsenalViewport.SelectNextItem() && arsenalViewport.selectedItem_ == 11
        && arsenalViewport.firstVisibleItem_ == 7,
        "next at last item still brings selection into view");
    arsenalViewport.firstVisibleItem_ = 1;
    Require(arsenalViewport.SelectVisibleArsenalItem(420)
        && arsenalViewport.selectedItem_ == 5,
        "bottom arsenal boundary selects fifth item rather than hidden sixth");
    arsenalViewport.inventory_ = {{2, 1}, {3, 1}, {4, 1}, {1, 1}, {5, 1}, {6, 1}};
    arsenalViewport.selectedItem_ = 1; arsenalViewport.firstVisibleItem_ = 1;
    arsenalViewport.ConsumeSelectedItem();
    Require(arsenalViewport.selectedItem_ == 2 && arsenalViewport.firstVisibleItem_ == 1,
        "item consumption preserves an already visible arsenal viewport");
    auto fortuneRecall = base;
    fortuneRecall.discoveredLawOrder_ = {1, 2, 3};
    fortuneRecall.discoveredLaws_.fill(false);
    for (std::uint8_t law : fortuneRecall.discoveredLawOrder_)
        fortuneRecall.discoveredLaws_[law] = true;
    fortuneRecall.lawRecallCursor_ = 2;
    fortuneRecall.inventory_ = {{0x0f, 1}};
    fortuneRecall.selectedItem_ = 0;
    for (std::uint32_t seed = 1; seed < 100; ++seed) {
        auto oracle = fortuneRecall; oracle.randomState_ = seed;
        if (!fortuneRecall.discoveredLaws_[oracle.NextRandom(47) - 1]) {
            fortuneRecall.randomState_ = seed; break;
        }
    }
    Require(fortuneRecall.UseSelectedItem(1, 0)
        && fortuneRecall.discoveredLawOrder_.size() == 4
        && fortuneRecall.lawRecallCursor_ == 2,
        "new fortune preserves the current recall position");
    fortuneRecall.pendingMessages_.clear();
    Require(fortuneRecall.RecallDiscoveredLaw() && fortuneRecall.lawRecallCursor_ == 1
        && fortuneRecall.TakeMessage().starts_with(L"#2 - "),
        "fortune recall keeps original ordinal prefix and reverse cursor");
    auto hubReplenishment = base;
    hubReplenishment.levelNumber_ = 3; hubReplenishment.difficulty_ = 2;
    hubReplenishment.levelThreeSpawnChance_ = 6;
    hubReplenishment.playerColumn_ = 39; hubReplenishment.playerRow_ = 35;
    for (auto& column : hubReplenishment.level_) column.fill(0);
    hubReplenishment.level_[2][9] = 0x60;
    hubReplenishment.level_[31][4] = 0x5f;
    bool sawFirstGate = false, sawSecondGate = false, sawChest = false;
    bool sawThresholdIncrease = false;
    for (std::uint32_t seed = 1; seed < 2000; ++seed) {
        auto trial = hubReplenishment; trial.randomState_ = seed;
        Require(trial.PlaceLevelThreeStructure(true), "hub places replenishment structure");
        sawThresholdIncrease |= trial.levelThreeSpawnChance_ == 7;
        if (trial.level_[2][9] == 0x5c) {
            sawFirstGate = true;
            Require(std::any_of(trial.overlays_.begin(), trial.overlays_.end(),
                [](const auto& overlay) { return overlay.type == 0x16
                    && overlay.column == 2 && overlay.row == 9; }),
                "first hub reward paints its construction effect");
        }
        if (trial.level_[31][4] == 0x5c) sawSecondGate = true;
        for (int column = 11; column <= 25; ++column)
            for (int row = 6; row <= 19; ++row)
                sawChest |= trial.level_[column][row] == 0x7f;
        if (sawFirstGate && sawSecondGate && sawChest && sawThresholdIncrease) {
            std::stringstream save;
            Require(trial.WriteGame(save), "persist dynamic hub spawn threshold");
            auto restored = base;
            Require(restored.ReadGame(save)
                && restored.levelThreeSpawnChance_ == trial.levelThreeSpawnChance_,
                "dynamic hub spawn threshold survives save and reload");
            auto olderBytes = save.str();
            olderBytes[5] = 30;
            olderBytes.resize(olderBytes.size() - 5 * sizeof(std::uint16_t));
            std::stringstream olderSave(olderBytes);
            Require(restored.ReadGame(olderSave)
                && restored.levelThreeSpawnChance_ == restored.difficulty_ + 24,
                "version-30 hub save derives its missing spawn threshold");
            break;
        }
    }
    Require(sawFirstGate && sawSecondGate && sawChest && sawThresholdIncrease,
        "zero-center hub replenishment can open both gates and place bonus chest");
    hubReplenishment.levelThreeSpawnChance_ = 26;
    hubReplenishment.turnCounter_ = 6;
    hubReplenishment.AdvanceBoardRuntime(false, 0, 0, false);
    Require(hubReplenishment.levelThreeSpawnChance_ == 6,
        "hub turn-six spawn reset runs after the preceding actor phase");

    auto tileEightActor = base;
    tileEightActor.playerColumn_ = 20; tileEightActor.playerRow_ = 20;
    tileEightActor.actors_ = {{0, 10, 10, 1, 0, 3}};
    tileEightActor.level_[11][10] = 8;
    tileEightActor.AdvanceActors();
    Require(tileEightActor.actors_[0].column == 11,
        "actor entry includes the first machinery tile eight");
    for (int terrain : {0, 0x23}) {
        auto dodgingActor = base;
        dodgingActor.playerColumn_ = 20; dodgingActor.playerRow_ = 20;
        dodgingActor.actors_ = {{2, 10, 10, 1, 0, 3}};
        dodgingActor.level_[11][10] = static_cast<std::uint16_t>(terrain);
        dodgingActor.projectiles_ = {{0, 11, 10, 0, 0}};
        for (std::uint32_t seed = 1; seed < 100; ++seed) {
            auto oracle = dodgingActor; oracle.randomState_ = seed;
            if (oracle.NextRandom(2) == 2) {
                dodgingActor.randomState_ = seed; break;
            }
        }
        dodgingActor.AdvanceActors();
        Require(dodgingActor.actors_[0].column == 10,
            "lieutenant avoids projectile on empty or actor-passable terrain");
    }
    auto selfOccupied = base;
    selfOccupied.playerColumn_ = 20; selfOccupied.playerRow_ = 20;
    selfOccupied.actors_ = {{0, 10, 10, 0, 0, 2}};
    selfOccupied.randomState_ = 6;
    selfOccupied.AdvanceActors();
    Require(selfOccupied.actors_[0].column == 11 && selfOccupied.actors_[0].row == 10,
        "random actor retries when zero-vector candidate is its own occupied cell");
    auto bossRemains = base;
    bossRemains.levelNumber_ = 1;
    bossRemains.levelOneTargetColumns_ = {10, 13};
    bossRemains.levelOneTargetRows_ = {10, 10};
    bossRemains.levelOneTargetDefeated_ = {0, 1};
    bossRemains.levelOneTargetHits_ = {0, 10};
    bossRemains.playerColumn_ = 20; bossRemains.playerRow_ = 11;
    for (int column = 0; column < 3; ++column)
        for (int row = 0; row < 3; ++row) {
            bossRemains.level_[10 + column][10 + row]
                = static_cast<std::uint16_t>(0x87 + column + row * 3);
            bossRemains.level_[13 + column][10 + row]
                = static_cast<std::uint16_t>(0x99 + column + row * 3);
        }
    for (std::uint32_t seed = 1; seed < 10000; ++seed) {
        auto oracle = bossRemains; oracle.randomState_ = seed;
        if (oracle.NextRandom(50) != 50 && oracle.NextRandom(3) == 1) {
            bossRemains.randomState_ = seed; break;
        }
    }
    bossRemains.AdvanceBoardRuntime(true, 1, 0, true);
    Require(bossRemains.levelOneTargetColumns_[0] == 10
        && bossRemains.levelOneTargetRows_[0] == 10
        && bossRemains.level_[13][10] == 0x99,
        "surviving boss cannot move across defeated boss wreckage");
    drain_storm::GameData breakout;
    Require(breakout.Load(argv[1], 5, 1), "load Breakout switch fixture");
    breakout.playerColumn_ = 37; breakout.playerRow_ = 2;
    breakout.AdvanceBoardRuntime(true, 1, 0, false);
    Require(breakout.levelFiveFirstSwitchActive_, "first Breakout switch latches open");
    breakout.level_[31][3] = 0;
    std::stringstream switchSave;
    Require(breakout.WriteGame(switchSave), "save Breakout switch state");
    auto restoredSwitch = base;
    Require(restoredSwitch.ReadGame(switchSave) && restoredSwitch.levelFiveFirstSwitchActive_,
        "Breakout switch flag survives damaged anchor and reload");
    restoredSwitch.AdvanceBoardRuntime(true, 1, 0, false);
    Require(restoredSwitch.level_[31][3] == 0x38
        && !restoredSwitch.levelFiveFirstSwitchActive_,
        "Breakout switch restores framed perimeter after anchor damage");
    auto mechanism = breakout;
    mechanism.levelFiveFirstProbeBroken_ = true;
    mechanism.levelFiveProbeTransformed_ = false;
    mechanism.playerColumn_ = mechanism.levelFiveProbeColumn_;
    mechanism.playerRow_ = mechanism.levelFiveProbeRow_;
    mechanism.AdvanceBoardRuntime(false, 0, 0, false);
    Require(!mechanism.levelFiveProbeTransformed_,
        "Breakout hidden mechanism ignores stationary callbacks");
    mechanism.AdvanceBoardRuntime(true, 1, 0, false);
    Require(mechanism.levelFiveProbeTransformed_
        && mechanism.TakeSoundEvent() == drain_storm::GameplaySound::Construction,
        "Breakout hidden mechanism activates on movement with construction sound");

    drain_storm::GameData fortressEasy, fortressHard;
    Require(fortressEasy.Load(argv[1], 10, 1, 1) && fortressHard.Load(argv[1], 10, 1, 4),
        "Fortress constructors across difficulty");
    Require(fortressEasy.randomState_ != fortressHard.randomState_,
        "1010:3428 Fortress placement consumes difficulty-dependent rolls");
    {
        const auto instance = GetModuleHandleW(nullptr);
        Require(drain_storm::RegisterMainWindowClass(instance, nullptr)
                    && drain_storm::RegisterChildWindowClass(instance, L"DrainStormArsenalWindow",
                        drain_storm::ArsenalWindowProcedure,
                        static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH)))
                    && drain_storm::RegisterChildWindowClass(instance, L"DrainStormBoardWindow",
                        drain_storm::BoardWindowProcedure,
                        static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH))),
                "register production shell for hidden window integration");
        drain_storm::ApplicationState gui;
        gui.gameData_ = base; gui.demoMode_ = false; gui.soundEnabled_ = false;
        Require(gui.LoadKeyBindings(), "initialize production default bindings without a sidecar");
        gui.resourceDirectory_ = std::filesystem::path(argv[1]).parent_path();
        Require(gui.boardAtlas_.Load(gui.resourceDirectory_ / L"bitmap_BOARD_BITMAPS.bmp"),
                "load hidden shell artwork");
        const HWND window = drain_storm::CreateMainWindow(instance, SW_HIDE, gui);
        const HWND board = GetDlgItem(window, 2);
        Require(board != nullptr, "production main window creates board child");
        std::array<BYTE, 256> originalKeys{}, neutralKeys{};
        Require(GetKeyboardState(originalKeys.data()) && SetKeyboardState(neutralKeys.data()),
                "isolate test thread key modifiers");
        gui.drainAnimationBoard_ = gui.gameData_; gui.drainAnimationFrame_ = 0;
        SendMessageW(window, WM_KEYDOWN, gui.KeyBinding(2), 0);
        SendMessageW(board, WM_LBUTTONDOWN, 0, MAKELPARAM(12 * 12, 10 * 12));
        for (int tick = 0; tick < 4; ++tick) SendMessageW(board, WM_TIMER, 0x0d, 0);
        Require(gui.IsDrainAnimating() && gui.gameData_.turnCounter_ == 0,
                "keyboard and mouse actions remain deferred through first four drain frames");
        SendMessageW(board, WM_TIMER, 0x0d, 0);
        Require(!gui.IsDrainAnimating() && gui.gameData_.turnCounter_ == 2
                    && gui.PlayerColumn() == 12 && gui.PlayerRow() == 10,
                "fifth drain frame replays keyboard then mouse through native handlers");

        gui.gameData_ = base;
        gui.drainAnimationBoard_ = gui.gameData_; gui.drainAnimationFrame_ = 0;
        auto shiftKeys = neutralKeys; shiftKeys[VK_SHIFT] = 0x80;
        Require(SetKeyboardState(shiftKeys.data()), "set queued shot modifier");
        SendMessageW(window, WM_KEYDOWN, gui.KeyBinding(2), 0);
        Require(SetKeyboardState(neutralKeys.data()), "release modifier before drain completes");
        for (int tick = 0; tick < 5; ++tick) SendMessageW(board, WM_TIMER, 0x0d, 0);
        Require(gui.gameData_.turnCounter_ == 1 && gui.PlayerColumn() == 10
                    && !gui.gameData_.projectiles_.empty(),
                "deferred native key action retains captured Shift instead of becoming movement");

        gui.gameData_ = base; gui.gameData_.level_[11][10] = 0x27;
        gui.drainAnimationBoard_ = gui.gameData_; gui.drainAnimationFrame_ = 0;
        SendMessageW(window, WM_KEYDOWN, gui.KeyBinding(2), 0);
        SendMessageW(window, WM_KEYDOWN, gui.KeyBinding(8), 0);
        for (int tick = 0; tick < 5; ++tick) SendMessageW(board, WM_TIMER, 0x0d, 0);
        Require(gui.IsDrainAnimating() && gui.CurrentLevel() != 17,
                "replay stops when its first action starts another transition");
        const auto turnsAtArrival = gui.gameData_.turnCounter_;
        for (int tick = 0; tick < 5; ++tick) SendMessageW(board, WM_TIMER, 0x0d, 0);
        Require(!gui.IsDrainAnimating() && gui.gameData_.turnCounter_ == turnsAtArrival + 1,
                "remaining deferred action resumes after second transition");
        Require(SetKeyboardState(originalKeys.data()), "restore test thread keyboard state");
        DestroyWindow(window);
        MSG quit{};
        PeekMessageW(&quit, nullptr, WM_QUIT, WM_QUIT, PM_REMOVE);
    }
    std::cout << "Audit regressions passed.\n";
}
