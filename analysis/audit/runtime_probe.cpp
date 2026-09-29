#include <iostream>
#include "application_state.h"

int wmain(int argc, wchar_t** argv) {
    if (argc != 2) return 2;
    const std::filesystem::path assets(argv[1]);
    for (int skill = 1; skill <= 4; ++skill) {
        for (std::size_t level : {1, 2, 4, 5, 7, 8, 9, 10, 12}) {
            drain_storm::GameData board;
            if (!board.Load(assets, level, 1, skill)) return 2;
            const auto [chance, group] = board.StructureSpawnParameters();
            std::cout << "parameters class=" << level << " skill=" << skill
                << " chance=" << chance << " group=" << group
                << " break=" << board.ActorBreakDivisor();
            if (level == 12) {
                int centers = 0;
                for (int col = 3; col <= 36; col += 3)
                    for (int row = 2; row <= 29; row += 3)
                        centers += board.level_[col][row] == 0x18 || board.level_[col][row] == 0x19;
                std::cout << " true_centers=" << centers << " expected=0,1,100";
            }
            std::cout << '\n';
        }
    }
    for (unsigned boundary : {330u, 500u}) {
        drain_storm::GameData board;
        if (!board.Load(assets, 17, 1)) return 2;
        board.turnCounter_ = boundary;
        board.genericRuntimeFlags_ = boundary == 330 ? 3 : 7;
        board.AdvanceBoardRuntime(false, 0, 0, false);
        std::cout << "threshold turn=" << boundary << " flags=" << unsigned(board.genericRuntimeFlags_)
            << " expected=" << (boundary == 330 ? 3 : 7) << '\n';
    }
    drain_storm::GameData gauntlet;
    if (!gauntlet.Load(assets, 4, 1)) return 2;
    gauntlet.overlays_.clear();
    gauntlet.level_[33][5] = 0;
    gauntlet.levelFourSouthwestTriggered_ = true;
    gauntlet.levelFourCountdown_ = 0;
    gauntlet.playerColumn_ = gauntlet.playerRow_ = 1;
    gauntlet.randomState_ = 1;
    drain_storm::GameData oracle;
    oracle.randomState_ = 1;
    const int expectedColumn = oracle.NextRandom(24) + 10;
    const int expectedRow = oracle.NextRandom(7) + expectedColumn - 4;
    gauntlet.AdvanceBoardRuntime(false, 0, 0, false);
    if (gauntlet.overlays_.empty()) return 2;
    const auto& overlay = gauntlet.overlays_.front();
    std::cout << "gauntlet-hazard actual=" << overlay.column << ',' << overlay.row
        << " expected=" << expectedColumn << ',' << expectedRow << '\n';
    drain_storm::GameData hub;
    if (!hub.Load(assets, 3, 1)) return 2;
    hub.playerColumn_ = 38; hub.playerRow_ = 19;
    hub.turnCounter_ = 1; hub.actors_.clear(); hub.overlays_.clear();
    int closedBefore = 0;
    for (int col = 30; col <= 34; ++col)
        for (int row = 19; row <= 23; ++row) closedBefore += hub.level_[col][row] == 0x5f;
    hub.AdvanceBoardRuntime(true, 1, 0, false);
    int closedAfter = 0;
    for (int col = 30; col <= 34; ++col)
        for (int row = 19; row <= 23; ++row) closedAfter += hub.level_[col][row] == 0x5f;
    if (hub.actors_.empty()) return 2;
    std::cout << "hub-lever actors_at=" << hub.actors_.front().column << ',' << hub.actors_.front().row
        << " expected=32,21 barriers_before=" << closedBefore << " after=" << closedAfter
        << " expected_after=0\n";
    drain_storm::GameData alarm;
    if (!alarm.Load(assets, 8, 1)) return 2;
    // Isolate placement mechanics on open terrain, preserving the real runtime callback.
    for (auto& col : alarm.level_) col.fill(0);
    alarm.actors_.clear(); alarm.overlays_.clear();
    alarm.playerColumn_ = alarm.playerRow_ = 0;
    alarm.randomState_ = 1;
    oracle.randomState_ = 1;
    const int alarmColumn = oracle.NextRandom(36);
    const int alarmRow = oracle.NextRandom(32);
    alarm.AdvanceBoardRuntime(false, 0, 0, false);
    std::cout << "alarm expected_first_center=" << alarmColumn + 1 << ',' << alarmRow + 1
        << " actual_tile=" << alarm.level_[alarmColumn + 1][alarmRow + 1]
        << " expected_tile=24 overlays=" << alarm.overlays_.size() << " expected_at_least=9\n";
    drain_storm::GameData placement;
    if (!placement.Load(assets, 17, 1)) return 2;
    for (auto& col : placement.level_) col.fill(0);
    placement.playerColumn_ = placement.playerRow_ = 0;
    placement.randomState_ = 1;
    if (!placement.PlaceRandomGuardedStructure(false)) return 2;
    std::cout << "generic-placement original_center=7,5 actual_tile=" << placement.level_[7][5]
        << " expected=24\n";
    for (auto& col : placement.level_) col.fill(0);
    placement.randomState_ = 1; oracle.randomState_ = 1;
    const int hubCenterColumn = oracle.NextRandom(15) + 12;
    const int hubCenterRow = oracle.NextRandom(14) + 7;
    if (!placement.PlaceLevelThreeStructure(false)) return 2;
    std::cout << "hub-placement original_center=" << hubCenterColumn << ',' << hubCenterRow
        << " actual_tile=" << placement.level_[hubCenterColumn][hubCenterRow] << " expected=24\n";
    drain_storm::GameData fortress;
    if (!fortress.Load(assets, 10, 1)) return 2;
    int seals = 0, expectedActiveSeals = 0;
    for (int col = 0; col < 40; ++col) for (int row = 0; row < 36; ++row) {
        const auto tile = fortress.level_[col][row];
        seals += tile == 0x21;
        if (!(col >= 9 && col <= 12 && row >= 19 && row <= 22))
            expectedActiveSeals += tile == 0x20 || tile == 0x21;
    }
    fortress.level_[5][15] = 0;
    fortress.AdvanceBoardRuntime(false, 0, 0, false);
    int activeSeals = 0;
    for (const auto& col : fortress.level_) for (auto tile : col) activeSeals += tile == 0x20;
    std::cout << "fortress seals_before=" << seals << " active_seals_after=" << activeSeals
        << " expected=" << expectedActiveSeals << '\n';
    drain_storm::GameData alien;
    if (!alien.Load(assets, 16, 1)) return 2;
    alien.level_[7][29] = alien.level_[33][29] = alien.level_[33][7] = 0;
    alien.AdvanceBoardRuntime(false, 0, 0, false);
    for (const auto& key : alien.overlays_) if (key.type == 4)
        std::cout << "alien-core moon_key=" << key.column << ',' << key.row << " expected=20,18\n";
    for (unsigned seed = 1; seed <= 128; ++seed) {
        drain_storm::GameData mined;
        if (!mined.Load(assets, 16, 1)) return 2;
        mined.overlays_.clear(); mined.actors_.clear(); mined.projectiles_.clear();
        mined.overlays_.push_back({0x24, 5, 29});
        mined.randomState_ = seed;
        mined.AdvanceBoardRuntime(false, 0, 0, false);
        if (mined.level_[7][29] == 0) {
            std::cout << "mine-phase seed=" << seed << " chamber_active="
                << mined.levelSixteenChambersActive_[2] << " expected=1_this_turn\n";
            break;
        }
    }
    drain_storm::GameData statue;
    if (!statue.Load(assets, 7, 1)) return 2;
    statue.actors_.assign(300, {0, 1, 1, 0, 0, 1});
    statue.levelSevenSpawnDivisor_ = 25;
    for (unsigned seed = 1; seed <= 1000; ++seed) {
        oracle.randomState_ = seed;
        if (oracle.NextRandom(25) == 25) { statue.randomState_ = seed; break; }
    }
    statue.AdvanceBoardRuntime(false, 0, 0, false);
    std::cout << "statue portal_wave actors=" << statue.actors_.size() << " expected=300\n";
    // Isolate c9ce's region-boundary rule using identical actual-map inputs.
    // Removing external wall neighbors in the comparison copy emulates its
    // false result outside the active rectangle, without changing its interior.
    unsigned boundaryCases = 0, boundaryDifferences = 0;
    for (unsigned seed = 1; seed <= 128; ++seed) for (int skill = 1; skill <= 4; ++skill)
    for (const auto bounds : std::array<std::array<std::size_t, 4>, 4>{{
             {0,22,13,35}, {26,22,39,35}, {26,0,39,13}, {17,15,23,21}}}) {
        drain_storm::GameData native, bounded;
        if (!native.Load(assets, 16, seed, skill) || !bounded.Load(assets, 16, seed, skill)) return 2;
        const auto [left, top, right, bottom] = bounds;
        for (std::size_t x = 0; x < 40; ++x) for (std::size_t y = 0; y < 36; ++y) {
            if (x >= left && x <= right && y >= top && y <= bottom) continue;
            bounded.level_[x][y] = 0;
        }
        native.ActivateLevelSixteenRegion(left, top, right, bottom);
        bounded.ActivateLevelSixteenRegion(left, top, right, bottom);
        unsigned mismatches = 0;
        for (std::size_t x = left; x <= right; ++x) for (std::size_t y = top; y <= bottom; ++y) {
            if (native.level_[x][y] != bounded.level_[x][y]) {
                ++mismatches;
                std::cout << "alien-boundary cell=" << x << ',' << y << " native="
                    << native.level_[x][y] << " bounded=" << bounded.level_[x][y] << '\n';
            }
        }
        ++boundaryCases;
        boundaryDifferences += mismatches;
    }
    std::cout << "alien-boundary cases=" << boundaryCases << " mismatches="
        << boundaryDifferences << '\n';
    drain_storm::GameData switches;
    if (!switches.Load(assets, 5, 1)) return 2;
    switches.playerColumn_ = 37; switches.playerRow_ = 2;
    switches.AdvanceBoardRuntime(true, 1, 0, false);
    switches.level_[31][3] = 0; // Destroyed anchor must not reset the switch flag.
    switches.AdvanceBoardRuntime(true, 1, 0, false);
    std::cout << "switch-damaged-anchor tile=" << switches.level_[31][3]
        << " expected=56\n";
    drain_storm::GameData mechanism;
    if (!mechanism.Load(assets, 5, 1)) return 2;
    mechanism.levelFiveFirstProbeBroken_ = true;
    mechanism.playerColumn_ = mechanism.levelFiveProbeColumn_;
    mechanism.playerRow_ = mechanism.levelFiveProbeRow_;
    mechanism.AdvanceBoardRuntime(false, 0, 0, false);
    std::cout << "mechanism-wait transformed=" << mechanism.levelFiveProbeTransformed_
        << " expected=0\n";
    drain_storm::GameData plant;
    if (!plant.Load(assets, 1, 1, 1)) return 2;
    unsigned portalCaptains = 0, portalLieutenants = 0;
    for (const auto& actor : plant.actors_) {
        if ((actor.column == 33 && actor.row == 15) || (actor.column == 34 && actor.row == 20)) {
            portalCaptains += actor.type == 3;
            portalLieutenants += actor.type == 2;
        }
    }
    std::cout << "plant-initial-groups captains=" << portalCaptains << " expected_min=6 max=14"
        << " lieutenants=" << portalLieutenants << " expected=0\n";
    plant.turnCounter_ = 1;
    plant.AdvanceBoardRuntime(false, 0, 0, false);
    std::cout << "plant-closed-gates center=" << plant.level_[3][3] << " expected=31"
        << " spawn_chance=" << plant.StructureSpawnParameters().first << " expected=0\n";
    plant.playerColumn_ = 1; plant.playerRow_ = 19;
    plant.AdvanceBoardRuntime(true, 1, 0, false);
    std::cout << "plant-original-lever arena_flag=" << plant.levelOneArenaEntered_
        << " expected=1\n";
    drain_storm::GameData bossWave;
    if (!bossWave.Load(assets, 1, 1)) return 2;
    bossWave.actors_.clear(); bossWave.projectiles_.clear(); bossWave.overlays_.clear();
    bossWave.playerColumn_ = 1; bossWave.playerRow_ = 1;
    for (int x = 36; x <= 38; ++x) for (int y = 1; y <= 3; ++y)
        bossWave.level_[x][y] = static_cast<std::uint16_t>(0x87 + (y-1)*3 + x-36);
    for (unsigned seed = 1; seed < 10000; ++seed) {
        oracle.randomState_ = seed;
        if (oracle.NextRandom(50) == 50) { bossWave.randomState_ = seed; break; }
    }
    bossWave.AdvanceBoardRuntime(false, 0, 0, true);
    unsigned atCenter = 0, nonHunter = 0;
    for (const auto& actor : bossWave.actors_) {
        atCenter += actor.column == 37 && actor.row == 2;
        nonHunter += actor.behavior != 0;
    }
    std::cout << "boss-wave count=" << bossWave.actors_.size() << " expected=8 center="
        << atCenter << " expected=0 nonhunter=" << nonHunter << " expected=0\n";
    drain_storm::GameData sphere;
    if (!sphere.Load(assets, 1, 1)) return 2;
    sphere.actors_.clear(); sphere.projectiles_.clear(); sphere.overlays_.clear();
    if (!sphere.DamageBoardSquare(12, 12, 2)) return 2;
    for (auto& column : sphere.level_) column.fill(0);
    sphere.level_[2][2] = 0x20;
    sphere.turnCounter_ = 10;
    sphere.levelOneArenaEntered_ = true;
    for (unsigned seed = 1; seed < 10000; ++seed) {
        oracle.randomState_ = seed;
        if (oracle.NextRandom(100) <= 5) { sphere.randomState_ = seed; break; }
    }
    sphere.actors_.clear();
    const auto beforeSphereSpawn = sphere.randomState_;
    sphere.SpawnStructureActors();
    std::cout << "sphere-destroyed spawns=" << sphere.actors_.size() << " expected=0 rng_changed="
        << (sphere.randomState_ != beforeSphereSpawn) << " expected=0\n";
    for (std::size_t mazeClass : {13u,14u,15u}) {
        drain_storm::GameData maze;
        if (!maze.Load(assets, mazeClass, 1)) return 2;
        unsigned unrevealed = 0;
        for (int x = static_cast<int>(maze.playerColumn_)-1;
             x <= static_cast<int>(maze.playerColumn_)+1; ++x)
            for (int y = static_cast<int>(maze.playerRow_)-1;
                 y <= static_cast<int>(maze.playerRow_)+1; ++y)
                if (x >= 0 && x < 40 && y >= 0 && y < 36)
                    unrevealed += maze.level_[x][y] >= 0x2b && maze.level_[x][y] <= 0x35;
        std::cout << "maze-entry class=" << mazeClass << " hidden_neighbors=" << unrevealed
            << " expected=0\n";
    }
    return 0;
}
