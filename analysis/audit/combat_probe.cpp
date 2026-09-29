#include <iostream>
#include "application_state.h"

int wmain(int argc, wchar_t** argv) {
    if (argc != 2) return 2;
    const std::filesystem::path assets(argv[1]);
    const auto prepare = [&assets](drain_storm::GameData& board, std::size_t level = 17) {
        if (!board.Load(assets, level, 1)) return false;
        for (auto& column : board.level_) column.fill(0);
        board.actors_.clear(); board.overlays_.clear(); board.projectiles_.clear();
        board.playerColumn_ = 1; board.playerRow_ = 1;
        return true;
    };
    drain_storm::GameData missile;
    drain_storm::GameData selfCell;
    if (!prepare(selfCell)) return 2;
    selfCell.actors_ = {{2,10,10,0,0,2}};
    // Original 91ec rejects the actor's own occupied cell. Native excludes
    // the current record from its scan. Select a legal zero-delta first roll.
    unsigned selfSeed = 0;
    for (unsigned seed=1; seed<10000; ++seed) {
        auto rng = selfCell; rng.randomState_ = seed;
        if (rng.NextRandom(3)==2 && rng.NextRandom(3)==2) { selfSeed=seed; break; }
    }
    if (!selfSeed) return 3;
    auto selfOracle = selfCell; selfOracle.randomState_ = selfSeed;
    int expectedX=10, expectedY=10;
    for (int attempt=0; attempt<8; ++attempt) {
        const int dx=selfOracle.NextRandom(3)-2, dy=selfOracle.NextRandom(3)-2;
        if (dx || dy) { expectedX+=dx; expectedY+=dy; break; }
    }
    selfCell.randomState_ = selfSeed;
    selfCell.AdvanceActors();
    std::cout << "actor-self-cell seed=" << selfSeed << " position="
        << selfCell.actors_[0].column << ',' << selfCell.actors_[0].row
        << " expected=" << expectedX << ',' << expectedY << " rng_matches="
        << (selfCell.randomState_==selfOracle.randomState_) << " expected=1\n";
    for (std::uint16_t weapon : {6, 7}) {
        drain_storm::GameData burst;
        if (!prepare(burst, 5)) return 2;
        burst.playerColumn_ = 20; burst.playerRow_ = 20;
        burst.inventory_ = {{weapon, 1}}; burst.selectedItem_ = 0;
        burst.UseSelectedItem(1, 0);
        if (burst.projectiles_.size() < 2) return 3;
        const auto& second = burst.projectiles_[1];
        std::cout << "eight-way weapon=" << weapon << " second_delta="
            << second.deltaColumn << ',' << second.deltaRow << " expected=-1,0\n";
    }
    if (!prepare(missile)) return 2;
    missile.actors_ = {{0, 11, 10, 0, 0, 1}, {0, 12, 10, 0, 0, 1}};
    missile.projectiles_ = {{2, 10, 10, 1, 0}};
    missile.AdvanceProjectiles();
    unsigned unmoved = 0;
    for (const auto& shot : missile.projectiles_) unmoved += shot.column == 11 && shot.row == 10;
    std::cout << "missile-chain surviving_actors=" << missile.actors_.size()
        << " fragments_at_impact=" << unmoved << " expected_actors=0 expected_unmoved=0\n";

    drain_storm::GameData mine;
    if (!prepare(mine)) return 2;
    mine.actors_ = {{0, 10, 10, 0, 0, 1}};
    mine.projectiles_ = {{1, 10, 10, 0, 0}};
    mine.AdvanceProjectiles();
    std::cout << "stationary-projectile surviving_actors=" << mine.actors_.size()
        << " shots=" << mine.projectiles_.size() << " expected_both=0\n";

    drain_storm::GameData smart;
    if (!prepare(smart)) return 2;
    smart.actors_ = {{3, 10, 10, 0, 0, 1}, {3, 30, 10, 0, 0, 1}};
    smart.inventory_ = {{13, 1}}; smart.selectedItem_ = 0; smart.randomState_ = 1;
    auto oracle = smart;
    const int expectedRight = oracle.NextRandom(7), expectedLeft = oracle.NextRandom(7);
    smart.UseSelectedItem(1, 0);
    unsigned left = 0, right = 0;
    for (const auto& actor : smart.actors_) (actor.column < 20 ? left : right)++;
    std::cout << "smart-bomb left=" << left << " right=" << right
        << " expected_left=" << expectedLeft << " expected_right=" << expectedRight << '\n';
    std::cout << "captain-split nonhunter_replacements="
        << std::count_if(smart.actors_.begin(), smart.actors_.end(),
            [](const auto& actor) { return actor.type == 2 && actor.behavior != 0; })
        << " expected=0\n";

    drain_storm::GameData casino;
    if (!prepare(casino, 2)) return 2;
    casino.playerColumn_ = 20; casino.playerRow_ = 4; casino.level_[21][4] = 3;
    const auto before = casino.turnCounter_;
    const bool accepted = casino.MovePlayer(1, -1);
    std::cout << "casino-deflection accepted=" << accepted
        << " turns=" << casino.turnCounter_ - before << " expected_accepted=1 expected_turns=1\n";
    drain_storm::GameData chest;
    if (!prepare(chest)) return 2;
    chest.playerColumn_ = 9; chest.playerRow_ = 10;
    chest.level_[10][10] = 0x7f; chest.level_[11][10] = 0x80;
    chest.level_[10][11] = 0x81; chest.level_[11][11] = 0x82;
    chest.inventory_ = {{19, 1}}; chest.selectedItem_ = 0; chest.randomState_ = 1;
    chest.UseSelectedItem(1, 0);
    for (const auto& effect : chest.overlays_) {
        if (effect.type >= 0x19 && effect.type <= 0x1c)
            std::cout << "chest-effect column=" << effect.column << " row=" << effect.row
                << " type=" << effect.type << '\n';
    }

    drain_storm::GameData radio;
    if (!prepare(radio)) return 2;
    radio.actors_ = {{2, 11, 9, 0, 0, 1}, {2, 9, 11, 0, 0, 1}};
    for (unsigned seed = 1; seed < 10000; ++seed) {
        auto oracle = radio; oracle.randomState_ = seed;
        if (oracle.NextRandom(10) == 2 && oracle.NextRandom(10) != 2) {
            radio.randomState_ = seed;
            break;
        }
    }
    radio.ApplyRadioExplosion(10, 10);
    for (const auto& overlay : radio.overlays_) {
        if (overlay.type == 1)
            std::cout << "radio-drop column=" << overlay.column << " row=" << overlay.row
                << " expected_column=11 expected_row=9\n";
    }
    drain_storm::GameData moon;
    if (!moon.Load(assets, 6, 1)) return 2;
    moon.playerColumn_ = 31; moon.playerRow_ = 10;
    moon.ResolveOverlaysAtPlayer();
    std::cout << "moon-stack first_is_extra_life="
        << (!moon.pendingMessages_.empty() && moon.pendingMessages_[0] == L"You gained an extra life!")
        << " expected=1\n";

    drain_storm::GameData mines;
    if (!prepare(mines)) return 2;
    mines.actors_ = {{3, 10, 10, 0, 0, 1}};
    mines.overlays_ = {{0x24, 10, 10}, {0x24, 11, 10}};
    mines.randomState_ = 1;
    mines.ResolveTriggeredMines();
    std::cout << "overlapping-mines surviving_actors=" << mines.actors_.size() << " expected=0\n";
    drain_storm::GameData bubble;
    if (!prepare(bubble)) return 2;
    bubble.level_[10][10] = 0x23;
    bubble.actors_ = {{0, 10, 10, 0, 0, 1}};
    bubble.projectiles_ = {{0, 9, 10, 1, 0}};
    bubble.AdvanceProjectiles();
    unsigned bubbleEffects = 0;
    for (const auto& overlay : bubble.overlays_) bubbleEffects += overlay.type == 0x16;
    std::cout << "bubble-nonempty actors=" << bubble.actors_.size()
        << " effects=" << bubbleEffects << " expected_effects=1 pop="
        << (bubble.pendingSound_ == drain_storm::GameplaySound::Pop)
        << " expected_pop=1 tile=" << bubble.level_[10][10] << " expected_tile=35\n";
    drain_storm::GameData unshielded;
    if (!prepare(unshielded, 1)) return 2;
    unshielded.levelOneArenaEntered_ = true;
    unshielded.actors_ = {{2, 12, 12, 0, 0, 1}};
    unshielded.inventory_ = {{13, 1}}; unshielded.selectedItem_ = 0;
    unshielded.turnCounter_ = 3;
    for (unsigned seed = 1; seed < 100; ++seed) {
        auto oracle = unshielded; oracle.randomState_ = seed;
        if (oracle.NextRandom(2) == 1) { unshielded.randomState_ = seed; break; }
    }
    unshielded.UseSelectedItem(1, 0);
    std::cout << "smart-bomb-after-lever actors=" << unshielded.actors_.size()
        << " expected=0\n";
    for (const auto [terrain, type] : {std::pair{8, 0}, {0x23, 2}, {0, 2}}) {
        drain_storm::GameData entry;
        if (!prepare(entry)) return 2;
        entry.actors_ = {{static_cast<std::uint16_t>(type), 10, 10, 1, 0, 3}};
        entry.level_[11][10] = static_cast<std::uint16_t>(terrain);
        if (type == 2) entry.projectiles_ = {{0, 11, 10, 0, 0}};
        for (unsigned seed = 1; seed < 100; ++seed) {
            auto oracle = entry; oracle.randomState_ = seed;
            if (oracle.NextRandom(2) == 2) { entry.randomState_ = seed; break; }
        }
        entry.AdvanceActors();
        std::cout << "actor-entry tile=" << terrain << " type=" << type
            << " column=" << entry.actors_.front().column
            << " expected_column=" << (type == 0 ? 11 : 10)
            << " remaining_shots=" << entry.projectiles_.size() << '\n';
    }
    drain_storm::GameData bossRemains;
    if (!prepare(bossRemains, 1)) return 2;
    bossRemains.levelOneTargetColumns_ = {10,13};
    bossRemains.levelOneTargetRows_ = {10,10};
    bossRemains.levelOneTargetDefeated_ = {0,1};
    bossRemains.levelOneTargetHits_ = {0,10};
    for (unsigned x=0; x<3; ++x) for (unsigned y=0; y<3; ++y) {
        bossRemains.level_[10+x][10+y] = static_cast<std::uint16_t>(0x87+x+y*3);
        bossRemains.level_[13+x][10+y] = static_cast<std::uint16_t>(0x99+x+y*3);
    }
    bossRemains.playerColumn_ = 20; bossRemains.playerRow_ = 11;
    for (unsigned seed=1; seed<10000; ++seed) {
        oracle.randomState_ = seed;
        if (oracle.NextRandom(50) != 50 && oracle.NextRandom(3) == 1) {
            bossRemains.randomState_ = seed; break;
        }
    }
    bossRemains.AdvanceBoardRuntime(true, 1, 0, true);
    std::cout << "boss-overlap-remains column=" << bossRemains.levelOneTargetColumns_[0]
        << " expected=10 row=" << bossRemains.levelOneTargetRows_[0] << " expected=10\n";
    return 0;
}
