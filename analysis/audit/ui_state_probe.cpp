#include <iostream>
#include "application_state.h"

int wmain(int argc, wchar_t** argv) {
    if (argc != 2) return 2;
    drain_storm::GameData board;
    if (!board.Load(argv[1], 17, 1)) return 2;
    board.inventory_ = {{0,1},{2,1},{3,1},{1,1},{4,1},{5,1},{6,1},{7,1},{8,1},{9,1},{10,1},{11,1}};
    board.selectedItem_ = 1;
    board.firstVisibleItem_ = 1;
    board.ConsumeSelectedItem();
    std::cout << "arsenal-consume-visible-fallback selection=" << board.selectedItem_
        << " first_visible=" << board.firstVisibleItem_ << " expected=1\n";
    if (!board.Load(argv[1], 17, 1)) return 2;
    for (int y=240; y<=420; ++y) {
        board.firstVisibleItem_ = 0;
        board.selectedItem_ = 9;
        const int expected = y >= 384 ? 4 : ((y-240)%36 == 35 ? 9 : (y-240)/36);
        board.SelectVisibleArsenalItem(y);
        if (board.selectedItem_ != expected)
            std::cout << "arsenal-hit y=" << y << " selection=" << board.selectedItem_
                << " expected=" << expected << '\n';
    }
    board.selectedItem_ = 0;
    board.ScrollVisibleArsenal(false);
    board.SelectPreviousItem();
    std::cout << "arsenal-first-boundary first_visible=" << board.firstVisibleItem_
        << " expected=0\n";
    board.selectedItem_ = board.inventory_.size()-1;
    board.firstVisibleItem_ = 0;
    board.SelectNextItem();
    std::cout << "arsenal-last-boundary first_visible=" << board.firstVisibleItem_
        << " expected=" << board.selectedItem_-4 << '\n';
    board.discoveredLawOrder_ = {0,1,2};
    board.discoveredLaws_.fill(false);
    for (unsigned id : board.discoveredLawOrder_) board.discoveredLaws_[id] = true;
    board.lawRecallCursor_ = 2;
    board.inventory_ = {{15,2}};
    board.selectedItem_ = 0;
    board.firstVisibleItem_ = 0;
    for (auto& column : board.level_) column.fill(0);
    board.actors_.clear(); board.projectiles_.clear(); board.overlays_.clear();
    drain_storm::GameData oracle;
    for (unsigned seed=1; seed<10000; ++seed) {
        oracle.randomState_ = seed;
        if (oracle.NextRandom(47)>3) { board.randomState_=seed; break; }
    }
    if (!board.UseSelectedItem(1,0)) return 2;
    std::cout << "fortune-new-discovery count=" << board.discoveredLawOrder_.size()
        << " recall_cursor=" << board.lawRecallCursor_ << " expected=2\n";
    board.pendingMessages_.clear();
    board.RecallDiscoveredLaw();
    std::cout << "fortune-recall cursor=" << board.lawRecallCursor_
        << " expected=1 numbered_prefix="
        << (!board.pendingMessages_.empty() && board.pendingMessages_.back().starts_with(L"#"))
        << " expected=1\n";
    drain_storm::ApplicationState state;
    state.resourceDirectory_ = std::filesystem::path(argv[1]).parent_path();
    state.soundEnabled_ = false;
    if (!state.gameData_.Load(argv[1],17,1)
        || !state.gameData_.PositionPlayerAtTile(0x27)
        || !state.ActivateTransporter(false)
        || !state.drainAnimationBoard_) return 2;
    const auto turnBefore = state.gameData_.turnCounter_;
    const bool acted = state.MovePlayer(0,0);
    // Direct API intentionally bypasses the production window-handler guard.
    // This is an API observation, NOT a reachable hidden-input gameplay defect.
    std::cout << "drain-direct-api-bypassing-window-guard accepted=" << acted
        << " destination_turn_delta=" << state.gameData_.turnCounter_-turnBefore
        << " source_snapshot_still_visible=" << bool(state.drainAnimationBoard_)
        << " production_window_handlers_discard_input=1\n";
    if (!board.Load(argv[1],2,1)) return 2;
    int tableSquares=0, wrongTableLabels=0;
    for (std::size_t x=0;x<40;++x) for (std::size_t y=0;y<36;++y) {
        if (board.level_[x][y]!=2 || (x==board.playerColumn_ && y==board.playerRow_)) continue;
        ++tableSquares;
        if (board.IdentifySquare(x,y)!=L"Poker Table.") ++wrongTableLabels;
    }
    std::cout << "casino-special-identification squares=" << tableSquares
        << " wrong_labels=" << wrongTableLabels << " expected=0\n";
    board.level_[0][0]=0;
    board.actors_.clear(); board.projectiles_.clear(); board.overlays_={{0x14,0,0}};
    std::cout << "effect-identification is_treasure="
        << (board.IdentifySquare(0,0)==L"Treasure") << " expected=0\n";
    return 0;
}
