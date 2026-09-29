#include <iostream>
#include "application_state.h"

int wmain(int argc, wchar_t** argv) {
    if (argc != 2) return 2;
    const std::filesystem::path assets(argv[1]);
    for (std::size_t room : {13, 14, 15}) {
        drain_storm::GameData board;
        if (!board.Load(assets, room, 1)) return 2;
        int signs = 0;
        for (const auto& column : board.level_)
            for (auto tile : column) signs += tile == 0x7e;
        std::wcout << L"maze room=" << room << L" signs=" << signs
            << L" text=" << board.SignpostText() << L" expected=The Labyrinth.\n";
    }
    drain_storm::GameData boss;
    if (!boss.Load(assets, 1, 1)) return 2;
    boss.actors_.clear(); boss.projectiles_.clear(); boss.overlays_.clear();
    for (int index = 0; index < 2; ++index) {
        const auto col = boss.levelOneTargetColumns_[index];
        const auto row = boss.levelOneTargetRows_[index];
        std::wcout << L"boss target=" << index << L" column=" << col << L" row=" << row
            << L" label=" << boss.IdentifySquare(col, row) << L'\n';
    }
    drain_storm::GameData casino;
    if (!casino.Load(assets, 2, 1)) return 2;
    casino.actors_.clear(); casino.projectiles_.clear();
    casino.playerColumn_ = 23; casino.playerRow_ = 26;
    casino.inventory_ = {{4, 2}}; casino.selectedItem_ = 0;
    casino.UseSelectedItem(1, 0);
    std::wcout << L"casino after-bet ticker=" << casino.CasinoAnnouncementText() << L'\n';
    drain_storm::BitmapSurface atlas;
    if (!atlas.Load(assets.parent_path() / L"bitmap_BOARD_BITMAPS.bmp")) return 2;
    HDC rendered = CreateCompatibleDC(nullptr), expected = CreateCompatibleDC(nullptr);
    BITMAPINFO info{};
    info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    info.bmiHeader.biWidth = 480; info.bmiHeader.biHeight = -432;
    info.bmiHeader.biPlanes = 1; info.bmiHeader.biBitCount = 32;
    info.bmiHeader.biCompression = BI_RGB;
    void* pixels{};
    HBITMAP actualBitmap = CreateDIBSection(rendered, &info, DIB_RGB_COLORS, &pixels, nullptr, 0);
    HBITMAP expectedBitmap = CreateDIBSection(expected, &info, DIB_RGB_COLORS, &pixels, nullptr, 0);
    if (!rendered || !expected || !actualBitmap || !expectedBitmap) return 3;
    const auto oldActual = SelectObject(rendered, actualBitmap);
    const auto oldExpected = SelectObject(expected, expectedBitmap);
    boss.playerColumn_ = 1; boss.playerRow_ = 1;
    boss.level_[10][10] = 0;
    boss.actors_ = {{2, 10, 10, 0, 0, 0}};
    boss.overlays_ = {{0x16, 10, 10}};
    boss.ArmTransientOverlayAnimation();
    boss.DrawLevel(rendered, atlas);
    const auto& effect = boss.sprites_[0x5b];
    atlas.DrawRegion(expected, 120, 120, effect.sourceColumn * 12,
        effect.sourceRow * 12, effect.widthInCells * 12, effect.heightInCells * 12);
    unsigned differences = 0;
    for (int y = 0; y < 12; ++y) for (int x = 0; x < 12; ++x)
        differences += GetPixel(rendered, 120+x, 120+y) != GetPixel(expected, 120+x, 120+y);
    std::cout << "effect-over-actor pixel_differences=" << differences << " expected=0\n";
    SelectObject(rendered, oldActual); SelectObject(expected, oldExpected);
    DeleteObject(actualBitmap); DeleteObject(expectedBitmap);
    DeleteDC(rendered); DeleteDC(expected);
    drain_storm::GameData cardBoard;
    if (!cardBoard.Load(assets, 2, 1)) return 2;
    const auto countCardMarkers = [&cardBoard]() {
        return std::count_if(cardBoard.overlays_.begin(), cardBoard.overlays_.end(),
            [](const auto& node) { return node.type == 0x1e; });
    };
    std::cout << "casino-initial native_card_draw_markers=" << countCardMarkers()
        << " original_backed_card_slots=5\n";
    cardBoard.casinoStatus_ = 1;
    cardBoard.playerColumn_ = cardBoard.playerRow_ = 26;
    cardBoard.AdvanceBoardRuntime(true, 1, 0, false);
    cardBoard.ArmTransientOverlayAnimation();
    const auto dealtMarkers = countCardMarkers();
    for (int frame = 0; frame < 6; ++frame) cardBoard.AdvanceAnimation();
    std::cout << "casino-deal markers_before=" << dealtMarkers
        << " markers_after_effect=" << countCardMarkers() << " expected_after=0\n";
    return 0;
}
