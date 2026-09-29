#include <iostream>
#include "application_state.h"

int wmain(int argc, wchar_t** argv) {
    if (argc != 2) return 2;
    const std::filesystem::path assets(argv[1]);
    const auto prepare = [&](drain_storm::GameData& board) {
        if (!board.Load(assets, 2, 1)) return false;
        board.actors_.clear(); board.projectiles_.clear(); board.overlays_.clear();
        board.inventory_.clear(); board.casinoStatus_ = 2;
        board.casinoBetCount_ = 1; board.casinoBetItemType_ = 4;
        board.casinoWildRank_ = 0; board.casinoHeld_.fill(1);
        board.playerColumn_ = board.playerRow_ = 26;
        board.turnCounter_ = 1; board.score_ = 0;
        board.casinoSuits_ = {1, 2, 3, 4, 1};
        return true;
    };
    for (int hand = 0; hand < 2; ++hand) {
        drain_storm::GameData board;
        if (!prepare(board)) return 2;
        board.casinoRanks_ = hand == 0 ? std::array<std::uint16_t, 5>{2, 2, 4, 8, 10}
            : std::array<std::uint16_t, 5>{1, 10, 11, 12, 13};
        board.AdvanceBoardRuntime(true, 1, 0, false);
        std::cout << "poker " << (hand == 0 ? "low-pair" : "ace-high-straight")
            << " payout=" << (board.inventory_.empty() ? 0 : board.inventory_.front().quantity)
            << " expected=" << (hand == 0 ? 0 : 8) << '\n';
    }
    for (bool fourWilds : {false, true}) {
        drain_storm::GameData board;
        if (!prepare(board)) return 2;
        board.casinoWildRank_ = 2;
        board.casinoRanks_ = fourWilds ? std::array<std::uint16_t, 5>{2, 2, 2, 2, 3}
            : std::array<std::uint16_t, 5>{1, 2, 10, 11, 12};
        // Four deuces have distinct suits; a sole non-wild card is a flush
        // under both evaluators. The other hand has mixed non-wild suits.
        board.AdvanceBoardRuntime(true, 1, 0, false);
        std::cout << "poker " << (fourWilds ? "four-wild-original-quirk" : "wild-ace-high")
            << " payout=" << (board.inventory_.empty() ? 0 : board.inventory_.front().quantity)
            << " expected=" << (fourWilds ? 20 : 4) << '\n';
    }
    for (int bonus = 0; bonus < 3; ++bonus) {
        drain_storm::GameData board;
        if (!prepare(board)) return 2;
        board.casinoRanks_ = {1, 10, 11, 12, 13}; board.casinoSuits_.fill(1);
        board.casinoBonusStage_ = bonus;
        board.AdvanceBoardRuntime(true, 1, 0, false);
        if (bonus < 2) std::cout << "bonus stage=" << bonus << " original_door_tile="
            << board.level_[bonus == 0 ? 13 : 35][19] << " expected=92\n";
        else {
            int blocked = 0;
            for (int col : {7, 8, 9, 32, 33, 34})
                for (int row = 2; row <= 4; ++row) blocked += board.level_[col][row] != 0;
            std::cout << "bonus final blocked_original_cells=" << blocked << " expected=0\n";
        }
    }
    for (bool late : {false, true}) {
        drain_storm::GameData board;
        if (!prepare(board)) return 2;
        board.casinoRanks_ = {1, 3, 6, 8, 10}; board.casinoSuits_.fill(1);
        board.score_ = late ? 0 : 2000; board.turnCounter_ = late ? 1601 : 1;
        board.AdvanceBoardRuntime(true, 1, 0, false);
        std::cout << "retaliation late=" << late << " mode=" << board.levelTwoSequenceMode_
            << " expected=" << (late ? 3 : 2) << '\n';
    }
    for (bool right : {false, true}) {
        drain_storm::GameData machine;
        if (!prepare(machine)) return 2;
        machine.casinoStatus_ = 0;
        machine.levelTwoSequenceMode_ = 1;
        machine.levelTwoSequenceState_ = right ? 104 : 4;
        machine.AdvanceBoardRuntime(false, 0, 0, false);
        std::cout << "retaliation restored_gate right=" << right << " tile="
            << machine.level_[right ? 36 : 4][right ? 30 : 31]
            << " expected=" << (right ? 63 : 62) << '\n';
    }
    {
        drain_storm::GameData machine;
        if (!prepare(machine)) return 2;
        machine.casinoRanks_ = {1, 3, 6, 8, 10}; machine.casinoSuits_.fill(1);
        machine.levelTwoSequenceMode_ = 1; machine.levelTwoSequenceState_ = 2;
        machine.AdvanceBoardRuntime(true, 1, 0, false);
        std::cout << "retaliation active_win mode=" << machine.levelTwoSequenceMode_
            << " state=" << machine.levelTwoSequenceState_ << " expected_mode=1 expected_state=4\n";
    }
    drain_storm::GameData board;
    if (!board.Load(assets, 2, 1)) return 2;
    board.actors_.clear(); board.level_[11][0] = 0; board.turnCounter_ = 1;
    board.AdvanceBoardRuntime(false, 0, 0, false);
    std::cout << "casino-chest-defense actors=" << board.actors_.size() << " expected_min=3\n";
    board.level_[4][33] = 0; board.turnCounter_ = 200;
    board.AdvanceBoardRuntime(false, 0, 0, false);
    std::cout << "casino-rebuild tile=" << board.level_[4][33] << " expected=30\n";
    return 0;
}
