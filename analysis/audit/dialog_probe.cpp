// Native dialog-message diagnostic, never calls the game's entry point.
// The test dialog has no WS_VISIBLE style and is never shown. No game files.
#include <iostream>
#include "../../reconstructed/src/main.cpp"

int wmain() {
    using namespace drain_storm;
    std::vector<WORD> words;
    AppendTemplateDword(words, WS_POPUP | WS_SYSMENU);
    AppendTemplateDword(words, 0);
    words.push_back(2);
    words.insert(words.end(), {0,0,100,100,0,0,0});
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        0,0,30,10,2,0x0080,L"Register");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        0,15,30,10,100,0x0080,L"Close");
    HelpDialogState state{1,nullptr};
    HWND dialog = CreateDialogIndirectParamW(GetModuleHandleW(nullptr),
        reinterpret_cast<const DLGTEMPLATE*>(words.data()), nullptr,
        HelpDialogProcedure, reinterpret_cast<LPARAM>(&state));
    if (!dialog) return 2;
    const bool visible = IsWindowVisible(dialog) != FALSE;
    SendMessageW(dialog, WM_CLOSE, 0, 0);
    MSG message{};
    while (PeekMessageW(&message, dialog, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    std::cout << "help-close hidden=" << !visible << " page_after_close=" << state.page
        << " still_window=" << bool(IsWindow(dialog))
        << " original_close_calls_EndDialog=1\n";
    if (IsWindow(dialog)) DestroyWindow(dialog);
    return 0;
}
