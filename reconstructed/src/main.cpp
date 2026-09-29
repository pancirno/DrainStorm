#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>

#include <algorithm>
#include <cstdint>
#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "application_state.h"

namespace drain_storm {

constexpr wchar_t kWindowClassName[] = L"DrainStormWindow";
constexpr wchar_t kArsenalWindowClassName[] = L"DrainStormArsenalWindow";
constexpr wchar_t kBoardWindowClassName[] = L"DrainStormBoardWindow";
constexpr wchar_t kWindowTitle[] = L"Drain Storm";
constexpr int kOriginalWindowWidth = 0x27c;
constexpr int kOriginalWindowHeight = 0x1d4;
constexpr UINT kSystemCommandHelpAndInformation = 0xe998;
constexpr UINT kSystemCommandAbout = 0xe999;
constexpr UINT kCommandNewGame = 100;
constexpr UINT kCommandOpenGame = 101;
constexpr UINT kCommandSaveGame = 102;
constexpr UINT kCommandExit = 103;
constexpr UINT kCommandHighScores = 104;
constexpr UINT kCommandSound = 105;
constexpr UINT kCommandKeyboard = 106;
constexpr UINT kCommandQuitCurrent = 107;
constexpr int kArsenalWindowId = 1;
constexpr int kBoardWindowId = 2;
constexpr UINT_PTR kBoardAnimationTimer = 0x0b;
constexpr UINT_PTR kDrainAnimationTimer = 0x0d;
constexpr UINT_PTR kArsenalScrollTimer = 0xc6;
int gArsenalScrollDirection = 0;
bool gArsenalScrollRepeating = false;
UINT gPreviousGameplayKey = 0;
bool gCopyrightGestureArmed = false;
int gDemoPressedButton = -1;
int gDemoArmedButton = -1;
struct DeferredDrainInput {
    HWND target{};
    UINT message{};
    WPARAM wParam{};
    LPARAM lParam{};
    bool shiftHeld{};
};
std::vector<DeferredDrainInput> gDeferredDrainInputs;
bool gReplayingDrainInput = false;
bool gReplayShiftHeld = false;

void DeferDrainInput(HWND target, UINT message, WPARAM wParam, LPARAM lParam) {
    gDeferredDrainInputs.push_back({target, message, wParam, lParam,
        (GetKeyState(VK_SHIFT) & 0x8000) != 0});
}

void ReleaseDeferredDrainInputs(ApplicationState& state, HWND boardWindow) {
    std::vector<DeferredDrainInput> ready;
    ready.swap(gDeferredDrainInputs);
    for (std::size_t index = 0; index < ready.size(); ++index) {
        if (!IsWindow(boardWindow)) break;
        if (state.IsDrainAnimating()) {
            gDeferredDrainInputs.insert(gDeferredDrainInputs.begin(),
                ready.begin() + static_cast<std::ptrdiff_t>(index), ready.end());
            break;
        }
        const auto& input = ready[index];
        if (!IsWindow(input.target)) continue;
        gReplayingDrainInput = true;
        gReplayShiftHeld = input.shiftHeld;
        SendMessageW(input.target, input.message, input.wParam, input.lParam);
        gReplayingDrainInput = false;
    }
}
constexpr int kHighScoreNameEdit = 1001;
constexpr int kDifficultyBeginner = 1101;
constexpr int kDifficultyIntermediate = 1102;
constexpr int kDifficultyAdvanced = 1103;
constexpr int kDifficultyDrainDeath = 1104;
constexpr int kKeyboardList = 1201;
constexpr int kKeyboardChange = 1202;
constexpr int kKeyboardDefaults = 1203;

struct KeyboardDialogState {
    ApplicationState::KeyBindings bindings{};
    int waitingIndex{-1};
};

constexpr std::array<const wchar_t*, ApplicationState::kKeyBindingCount> kBindingNames = {
    L"Move up", L"Move down", L"Move right", L"Move left",
    L"Move up-right", L"Move down-right", L"Move up-left", L"Move down-left",
    L"Wait one turn", L"Previous arsenal item", L"Next arsenal item",
    L"Reread fortune", L"Previous message", L"Next message", L"Quit current game",
    L"Save game", L"Toggle sound", L"Use upward drain", L"Use downward drain"};

static_assert(sizeof(void*) == 8, "Drain Storm must be built for x64");

ApplicationState* GetApplicationState(HWND window) noexcept {
    return reinterpret_cast<ApplicationState*>(GetWindowLongPtrW(window, GWLP_USERDATA));
}

void AttachApplicationState(HWND window, LPARAM createParameter) noexcept {
    const auto* create = reinterpret_cast<const CREATESTRUCTW*>(createParameter);
    SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create->lpCreateParams));
}

int ArsenalScrollDirectionAt(HWND window, int x, int y) {
    RECT bounds{};
    GetClientRect(window, &bounds);
    const int right = bounds.right;
    if (y <= 198 || y >= 234 || x <= right - 26 || x >= right - 1) return 0;
    if (y < right - x + 185 || y < x - right + 211
        || y > x - right + 247 || y > right - x + 221) return 0;
    return y < 217 ? -1 : 1;
}

void AppendTemplateDword(std::vector<WORD>& words, DWORD value) {
    words.push_back(static_cast<WORD>(value));
    words.push_back(static_cast<WORD>(value >> 16u));
}

void AppendTemplateString(std::vector<WORD>& words, const wchar_t* value) {
    do {
        words.push_back(static_cast<WORD>(*value));
    } while (*value++ != L'\0');
}

void AlignDialogItem(std::vector<WORD>& words) {
    if ((words.size() & 1u) != 0) {
        words.push_back(0);
    }
}

void AppendDialogItem(std::vector<WORD>& words, DWORD style, short x, short y,
                      short width, short height, WORD id, WORD classAtom,
                      const wchar_t* title) {
    AlignDialogItem(words);
    AppendTemplateDword(words, style);
    AppendTemplateDword(words, 0);
    words.push_back(static_cast<WORD>(x));
    words.push_back(static_cast<WORD>(y));
    words.push_back(static_cast<WORD>(width));
    words.push_back(static_cast<WORD>(height));
    words.push_back(id);
    words.push_back(0xffff);
    words.push_back(classAtom);
    AppendTemplateString(words, title);
    words.push_back(0);
}

std::wstring KeyName(UINT key) {
    switch (key) {
    case VK_CLEAR: return L"Numpad 5";
    case VK_PRIOR: return L"Page Up";
    case VK_NEXT: return L"Page Down";
    }
    wchar_t name[64]{};
    const UINT scanCode = MapVirtualKeyW(key, MAPVK_VK_TO_VSC);
    if (GetKeyNameTextW(static_cast<LONG>(scanCode << 16u), name,
                        static_cast<int>(std::size(name))) > 0) return name;
    wchar_t fallback[16]{};
    wsprintfW(fallback, L"VK %02X", key);
    return fallback;
}

void RefreshKeyboardList(HWND dialog, const KeyboardDialogState& state, int selection) {
    HWND list = GetDlgItem(dialog, kKeyboardList);
    SendMessageW(list, LB_RESETCONTENT, 0, 0);
    for (std::size_t index = 0; index < state.bindings.size(); ++index) {
        std::wstring row = kBindingNames[index];
        row += L"\t";
        row += KeyName(state.bindings[index]);
        SendMessageW(list, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(row.c_str()));
    }
    SendMessageW(list, LB_SETCURSEL, static_cast<WPARAM>(std::max(selection, 0)), 0);
}

INT_PTR CALLBACK KeyboardDialogProcedure(HWND dialog, UINT message,
                                          WPARAM wParam, LPARAM lParam) {
    auto* state = reinterpret_cast<KeyboardDialogState*>(GetWindowLongPtrW(dialog, DWLP_USER));
    if (message == WM_INITDIALOG) {
        state = reinterpret_cast<KeyboardDialogState*>(lParam);
        SetWindowLongPtrW(dialog, DWLP_USER, lParam);
        RefreshKeyboardList(dialog, *state, 0);
        return TRUE;
    }
    if (message == WM_GETDLGCODE && state != nullptr && state->waitingIndex >= 0)
        return DLGC_WANTALLKEYS;
    if (message == WM_KEYDOWN && state != nullptr && state->waitingIndex >= 0) {
        if (wParam != VK_ESCAPE && wParam != VK_RETURN && wParam <= 0xff) {
            const int selection = state->waitingIndex;
            state->bindings[static_cast<std::size_t>(selection)] = static_cast<UINT>(wParam);
            state->waitingIndex = -1;
            SetDlgItemTextW(dialog, kKeyboardChange, L"Change...");
            RefreshKeyboardList(dialog, *state, selection);
        }
        return TRUE;
    }
    if (message == WM_COMMAND && state != nullptr) {
        switch (LOWORD(wParam)) {
        case kKeyboardChange: {
            const LRESULT selection = SendDlgItemMessageW(dialog, kKeyboardList,
                                                           LB_GETCURSEL, 0, 0);
            if (selection != LB_ERR) {
                state->waitingIndex = static_cast<int>(selection);
                SetDlgItemTextW(dialog, kKeyboardChange, L"Press a key...");
                SetFocus(dialog);
            }
            return TRUE;
        }
        case kKeyboardDefaults:
            state->bindings = {VK_UP, VK_DOWN, VK_RIGHT, VK_LEFT, VK_PRIOR, VK_NEXT,
                VK_HOME, VK_END, VK_CLEAR, 'A', 'Z', 'F', 'P', 'M', 'Q', 'S', 'T', 'U', 'D'};
            state->waitingIndex = -1;
            RefreshKeyboardList(dialog, *state, 0);
            return TRUE;
        case IDOK: EndDialog(dialog, IDOK); return TRUE;
        case IDCANCEL: EndDialog(dialog, IDCANCEL); return TRUE;
        }
    }
    return FALSE;
}

bool PromptForKeyboardBindings(HWND owner, ApplicationState& application) {
    std::vector<WORD> words;
    AppendTemplateDword(words, WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_MODALFRAME | DS_SETFONT);
    AppendTemplateDword(words, 0);
    words.push_back(6);
    words.insert(words.end(), {10, 10, 232, 176});
    words.push_back(0); words.push_back(0);
    AppendTemplateString(words, L"Keyboard Commands");
    words.push_back(9); AppendTemplateString(words, L"Segoe UI");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | LBS_NOTIFY,
                     8, 8, 216, 125, kKeyboardList, 0x0083, L"");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
                     8, 140, 58, 14, kKeyboardChange, 0x0080, L"Change...");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
                     70, 140, 52, 14, kKeyboardDefaults, 0x0080, L"Defaults");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
                     132, 140, 44, 14, IDOK, 0x0080, L"OK");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
                     180, 140, 44, 14, IDCANCEL, 0x0080, L"Cancel");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE, 8, 158, 216, 10, 0xffff, 0x0082,
                     L"Select a command, choose Change, then press its new key.");
    KeyboardDialogState state{application.KeyBindingTable()};
    const INT_PTR result = DialogBoxIndirectParamW(
        reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(owner, GWLP_HINSTANCE)),
        reinterpret_cast<const DLGTEMPLATE*>(words.data()), owner,
        KeyboardDialogProcedure, reinterpret_cast<LPARAM>(&state));
    return result != IDOK || application.SetKeyBindings(state.bindings);
}

INT_PTR CALLBACK HighScoreNameDialogProcedure(HWND dialog, UINT message,
                                               WPARAM wParam, LPARAM lParam) {
    if (message == WM_INITDIALOG) {
        SetWindowLongPtrW(dialog, DWLP_USER, lParam);
        HWND edit = GetDlgItem(dialog, kHighScoreNameEdit);
        // FUN_1008_4014 calls GetWindowText with nMaxCount 0x0d, retaining at
        // most twelve name characters plus the terminating null.
        SendMessageW(edit, EM_SETLIMITTEXT, 12, 0);
        SetFocus(edit);
        return FALSE;
    }
    if (message == WM_COMMAND) {
        if (LOWORD(wParam) == IDOK) {
            auto* result = reinterpret_cast<std::wstring*>(GetWindowLongPtrW(dialog, DWLP_USER));
            wchar_t name[32]{};
            GetDlgItemTextW(dialog, kHighScoreNameEdit, name, static_cast<int>(std::size(name)));
            *result = name;
            EndDialog(dialog, IDOK);
            return TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(dialog, IDCANCEL);
            return TRUE;
        }
    }
    return FALSE;
}

std::optional<std::wstring> PromptForHighScoreName(HWND owner) {
    std::vector<WORD> words;
    AppendTemplateDword(words, WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_MODALFRAME | DS_SETFONT);
    AppendTemplateDword(words, 0);
    words.push_back(3);
    words.insert(words.end(), {10, 10, 124, 84});
    words.push_back(0);
    words.push_back(0);
    AppendTemplateString(words, L"HIGH SCORE");
    words.push_back(8);
    AppendTemplateString(words, L"MS Sans Serif");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE, 17, 10, 78, 18, 0xffff, 0x0082,
                     L"Add your name to the High Score Table");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL,
                     17, 31, 90, 20, kHighScoreNameEdit, 0x0081, L"");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
                     37, 60, 50, 14, IDOK, 0x0080, L"OK");
    std::wstring name;
    const INT_PTR result = DialogBoxIndirectParamW(
        reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(owner, GWLP_HINSTANCE)),
        reinterpret_cast<const DLGTEMPLATE*>(words.data()), owner,
        HighScoreNameDialogProcedure, reinterpret_cast<LPARAM>(&name));
    if (result != IDOK) {
        return std::nullopt;
    }
    return name;
}

INT_PTR CALLBACK DifficultyDialogProcedure(HWND dialog, UINT message,
                                            WPARAM wParam, LPARAM lParam) {
    if (message == WM_INITDIALOG) {
        SetWindowLongPtrW(dialog, DWLP_USER, lParam);
        CheckRadioButton(dialog, kDifficultyBeginner, kDifficultyDrainDeath,
                         kDifficultyBeginner);
        SetFocus(GetDlgItem(dialog, IDOK));
        return FALSE;
    }
    if (message == WM_COMMAND) {
        if (LOWORD(wParam) == IDOK) {
            auto* result = reinterpret_cast<int*>(GetWindowLongPtrW(dialog, DWLP_USER));
            for (int difficulty = 1; difficulty <= 4; ++difficulty) {
                if (IsDlgButtonChecked(dialog, kDifficultyBeginner + difficulty - 1)
                    == BST_CHECKED) {
                    *result = difficulty;
                    break;
                }
            }
            EndDialog(dialog, IDOK);
            return TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(dialog, IDCANCEL);
            return TRUE;
        }
    }
    return FALSE;
}

std::optional<int> PromptForDifficulty(HWND owner) {
    std::vector<WORD> words;
    AppendTemplateDword(words, WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_MODALFRAME | DS_SETFONT);
    AppendTemplateDword(words, 0);
    words.push_back(6);
    words.insert(words.end(), {10, 10, 156, 138});
    words.push_back(0);
    words.push_back(0);
    AppendTemplateString(words, L"New Game");
    words.push_back(8);
    AppendTemplateString(words, L"MS Sans Serif");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP | BS_AUTORADIOBUTTON,
                     14, 28, 127, 13, kDifficultyBeginner, 0x0080, L"Beginner");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON,
                     14, 47, 127, 13, kDifficultyIntermediate, 0x0080, L"Intermediate");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON,
                     14, 66, 127, 13, kDifficultyAdvanced, 0x0080, L"Advanced");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON,
                     14, 85, 127, 13, kDifficultyDrainDeath, 0x0080,
                     L"Drain Death Guaranteed");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
                     52, 115, 50, 14, IDOK, 0x0080, L"Begin");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                     6, 13, 142, 92, 0xffff, 0x0080, L"Select Skill Level");
    int difficulty = 1;
    const INT_PTR result = DialogBoxIndirectParamW(
        reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(owner, GWLP_HINSTANCE)),
        reinterpret_cast<const DLGTEMPLATE*>(words.data()), owner,
        DifficultyDialogProcedure, reinterpret_cast<LPARAM>(&difficulty));
    // FUN_1008_3fd6 ignores DialogBox's result and returns its Skill global,
    // initialized to Beginner. Closing the dialog therefore starts Skill 1.
    static_cast<void>(result);
    return difficulty;
}

void ShowPendingGameplayMessage(HWND owner, ApplicationState& state) {
    state.TakeGameplayMessage();
    InvalidateRect(GetDlgItem(owner, kArsenalWindowId), nullptr, FALSE);
    const std::wstring lifecycleMessage = state.TakeLifecycleMessage();
    if (!lifecycleMessage.empty())
        MessageBoxW(owner, lifecycleMessage.c_str(), kWindowTitle, MB_OK | MB_ICONINFORMATION);
    if (state.HasPendingHighScore()) {
        if (const std::optional<std::wstring> name = PromptForHighScoreName(owner)) {
            if (!state.SubmitPendingHighScore(*name)) {
                MessageBoxW(owner, L"Unable to save the high-score table.", kWindowTitle,
                            MB_OK | MB_ICONERROR);
            }
        } else if (!state.SubmitPendingHighScore(L"")) {
            MessageBoxW(owner, L"Unable to save the high-score table.", kWindowTitle,
                        MB_OK | MB_ICONERROR);
        }
    }
}

void RefreshGameWindows(HWND window) {
    InvalidateRect(GetDlgItem(window, kBoardWindowId), nullptr, FALSE);
    InvalidateRect(GetDlgItem(window, kArsenalWindowId), nullptr, FALSE);
}

struct HelpDialogState {
    int page{1};
    ApplicationState* application{};
    std::uint32_t animationFrame{};
};


INT_PTR CALLBACK HelpDialogProcedure(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam) {
    auto* state = reinterpret_cast<HelpDialogState*>(GetWindowLongPtrW(dialog, DWLP_USER));
    if (message == WM_INITDIALOG) {
        state = reinterpret_cast<HelpDialogState*>(lParam);
        SetWindowLongPtrW(dialog, DWLP_USER, lParam);
        RECT bounds{};
        GetWindowRect(dialog, &bounds);
        // Reserve the recovered 444x348 page canvas below the controls.
        // Window dimensions include non-client chrome; do not squeeze the
        // illustration area by treating an outer size as a client size.
        RECT desiredClient{0, 0, 488, 466};
        AdjustWindowRectEx(&desiredClient, static_cast<DWORD>(GetWindowLongPtrW(dialog, GWL_STYLE)),
            FALSE, static_cast<DWORD>(GetWindowLongPtrW(dialog, GWL_EXSTYLE)));
        SetWindowPos(dialog, nullptr, bounds.left, bounds.top,
                     desiredClient.right - desiredClient.left, desiredClient.bottom - desiredClient.top,
                     SWP_NOACTIVATE | SWP_NOZORDER);
        const struct { int id, x, y, width, height; } controls[] = {
            {100, 360, 54, 96, 18}, {2, 240, 54, 108, 18},
            {10, 360, 6, 96, 18}, {11, 240, 6, 108, 18},
            {8, 240, 30, 108, 18}, {9, 360, 30, 96, 18},
            {3, 72, 36, 24, 24}, {4, 108, 36, 24, 24},
            {5, 144, 36, 24, 24}, {6, 180, 36, 24, 24},
            {7, 12, 12, 30, 30}};
        for (const auto& control : controls) {
            MoveWindow(GetDlgItem(dialog, control.id), control.x, control.y,
                       control.width, control.height, TRUE);
        }
        HWND owner = GetParent(dialog);
        HICON icon = reinterpret_cast<HICON>(
            SendMessageW(owner, WM_GETICON, ICON_BIG, 0));
        if (icon == nullptr) {
            icon = reinterpret_cast<HICON>(GetClassLongPtrW(owner, GCLP_HICON));
        }
        SendDlgItemMessageW(dialog, 7, STM_SETICON,
                            reinterpret_cast<WPARAM>(icon), 0);
        SetFocus(GetDlgItem(dialog, 100));
        SetTimer(dialog, 1, 200, nullptr);
        return FALSE;
    }
    if (message == WM_DESTROY) {
        KillTimer(dialog, 1);
        return TRUE;
    }
    if (message == WM_CLOSE) {
        EndDialog(dialog, 0);
        return TRUE;
    }
    if (message == WM_TIMER && wParam == 1 && state != nullptr) {
        ++state->animationFrame;
        if (state->page == 2 || state->page == 4 || state->page == 5
            || state->page == 6 || state->page == 11 || state->page == 12) {
            RECT client{};
            GetClientRect(dialog, &client);
            const RECT pageBounds{22, 98, client.right - 22, client.bottom - 20};
            InvalidateRect(dialog, &pageBounds, FALSE);
        }
        return TRUE;
    }
    if (message == WM_COMMAND && state != nullptr) {
        int page = state->page;
        switch (LOWORD(wParam)) {
        case 100: EndDialog(dialog, 0); return TRUE;
        case 2: page = 11; break;
        case 3: page = 1; break;
        case 4: page = page - 1; break;
        case 5: page = page + 1; break;
        case 6: page = 12; break;
        case 8: page = 10; break;
        case 9: page = 4; break;
        case 10: page = 6; break;
        case 11: page = 8; break;
        default: return FALSE;
        }
        state->page = std::clamp(page, 1, 12);
        InvalidateRect(dialog, nullptr, TRUE);
        return TRUE;
    }
    if (message == WM_PAINT && state != nullptr) {
        PAINTSTRUCT paint{};
        HDC paintDC = BeginPaint(dialog, &paint);
        RECT client{};
        GetClientRect(dialog, &client);
        // Compose each help animation frame offscreen so its background erase
        // and atlas/text drawing never become separate visible frames.
        HDC buffer = CreateCompatibleDC(paintDC);
        HBITMAP bitmap = buffer ? CreateCompatibleBitmap(paintDC, client.right, client.bottom) : nullptr;
        HGDIOBJ oldBitmap = bitmap ? SelectObject(buffer, bitmap) : nullptr;
        HDC dc = bitmap ? buffer : paintDC;
        const RECT labelBounds{365, 76, client.right - 8, 90};
        FillRect(dc, &labelBounds, GetSysColorBrush(COLOR_BTNFACE));
        RECT pageBounds{12, 90, client.right - 12, client.bottom - 12};
        FillRect(dc, &pageBounds, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
        FrameRect(dc, &pageBounds, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
        InflateRect(&pageBounds, -10, -8);
        SetBkMode(dc, TRANSPARENT);
        const std::wstring text = state->application ? state->application->HelpPageText(state->page) : L"";
        if (state->application == nullptr || !state->application->DrawHelpPage(dc, state->page, pageBounds, state->animationFrame, text)) {
            DrawTextW(dc, text.c_str(), static_cast<int>(text.size()), &pageBounds,
                      DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);
        }
        wchar_t pageLabel[32]{};
        wsprintfW(pageLabel, L"Page %d of 12", state->page);
        const auto labelColor = GetTextColor(dc);
        SetTextColor(dc, RGB(255, 255, 255));
        TextOutW(dc, 364, 75, pageLabel, lstrlenW(pageLabel));
        SetTextColor(dc, RGB(0, 0, 0));
        TextOutW(dc, 366, 77, pageLabel, lstrlenW(pageLabel));
        SetTextColor(dc, labelColor);
        TextOutW(dc, 365, 76, pageLabel, lstrlenW(pageLabel));
        if (bitmap) {
            // Leave child controls to the dialog manager; only these two
            // regions belong to our renderer (also clipped by BeginPaint).
            BitBlt(paintDC, 12, 90, client.right - 24, client.bottom - 102,
                buffer, 12, 90, SRCCOPY);
            BitBlt(paintDC, labelBounds.left, labelBounds.top,
                labelBounds.right - labelBounds.left, labelBounds.bottom - labelBounds.top,
                buffer, labelBounds.left, labelBounds.top, SRCCOPY);
            SelectObject(buffer, oldBitmap);
            DeleteObject(bitmap);
        }
        if (buffer) DeleteDC(buffer);
        EndPaint(dialog, &paint);
        return TRUE;
    }
    return FALSE;
}

void ShowHelpAndInformation(HWND owner, int initialPage = 1) {
    std::vector<WORD> words;
    AppendTemplateDword(words, WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_MODALFRAME | DS_SETFONT);
    AppendTemplateDword(words, 0);
    words.push_back(11);
    words.insert(words.end(), {10, 10, 188, 98});
    words.push_back(0);
    words.push_back(0);
    AppendTemplateString(words, L"Help and Information");
    words.push_back(8);
    AppendTemplateString(words, L"MS Sans Serif");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
                     27, 35, 15, 10, 3, 0x0080, L"<<");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
                     60, 36, 14, 13, 4, 0x0080, L"<");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
                     90, 43, 11, 9, 5, 0x0080, L">");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
                     120, 45, 17, 12, 6, 0x0080, L">>");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
                     132, 23, 50, 14, 11, 0x0080, L"Laws && Tips");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
                     132, 23, 50, 14, 10, 0x0080, L"Commands");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
                     132, 23, 50, 14, 8, 0x0080, L"High Scores");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
                     132, 23, 50, 14, 9, 0x0080, L"Elements");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
                     132, 23, 50, 14, 2, 0x0080, L"Register");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | SS_ICON,
                     20, 20, 30, 30, 7, 0x0082, L"");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
                     132, 6, 50, 14, 100, 0x0080, L"Close");
    HelpDialogState state{std::clamp(initialPage, 1, 12), GetApplicationState(owner)};
    DialogBoxIndirectParamW(
        reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(owner, GWLP_HINSTANCE)),
        reinterpret_cast<const DLGTEMPLATE*>(words.data()), owner,
        HelpDialogProcedure, reinterpret_cast<LPARAM>(&state));
}

INT_PTR CALLBACK AboutDialogProcedure(HWND dialog, UINT message, WPARAM wParam, LPARAM) {
    if (message == WM_COMMAND && LOWORD(wParam) == IDOK) {
        EndDialog(dialog, IDOK);
        return TRUE;
    }
    if (message == WM_CLOSE) {
        EndDialog(dialog, 0);
        return TRUE;
    }
    if (message == WM_PAINT) {
        PAINTSTRUCT paint{};
        HDC dc = BeginPaint(dialog, &paint);
        SetBkMode(dc, TRANSPARENT);

        HFONT titleFont = CreateFontW(-24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                      CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                      DEFAULT_PITCH | FF_SWISS, L"Arial");
        HFONT previousFont = titleFont != nullptr
            ? static_cast<HFONT>(SelectObject(dc, titleFont)) : nullptr;
        TextOutW(dc, 10, 8, L"DRAIN STORM (TM)", 16);
        if (previousFont != nullptr) {
            SelectObject(dc, previousFont);
        }
        if (titleFont != nullptr) {
            DeleteObject(titleFont);
        }

        TextOutW(dc, 12, 38, L"Copyright © 1997 Dave Horner", 28);
        TextOutW(dc, 12, 54, L"All Rights Reserved.", 20);
        TextOutW(dc, 12, 78, L"Designed and developed by Dave Horner", 37);
        EndPaint(dialog, &paint);
        return TRUE;
    }
    return FALSE;
}

void ShowAbout(HWND owner) {
    std::vector<WORD> words;
    AppendTemplateDword(words, WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_MODALFRAME | DS_SETFONT);
    AppendTemplateDword(words, 0);
    words.push_back(1);
    words.insert(words.end(), {10, 10, 209, 67});
    words.push_back(0);
    words.push_back(0);
    AppendTemplateString(words, L"About");
    words.push_back(8);
    AppendTemplateString(words, L"MS Sans Serif");
    AppendDialogItem(words, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
                     146, 41, 41, 17, IDOK, 0x0080, L"OK");
    DialogBoxIndirectParamW(
        reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(owner, GWLP_HINSTANCE)),
        reinterpret_cast<const DLGTEMPLATE*>(words.data()), owner,
        AboutDialogProcedure, 0);
}

LRESULT CALLBACK ArsenalWindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_NCCREATE:
        AttachApplicationState(window, lParam);
        return TRUE;

    case WM_COMMAND:
        SetFocus(window);
        return 0;

    case WM_LBUTTONDOWN:
        SetFocus(window);
        if (ApplicationState* state = GetApplicationState(window)) {
            const int scrollDirection = ArsenalScrollDirectionAt(
                window, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            if (scrollDirection != 0) {
                gArsenalScrollDirection = scrollDirection;
                gArsenalScrollRepeating = false;
                state->ScrollVisibleArsenal(scrollDirection < 0);
                SetCapture(window);
                SetTimer(window, kArsenalScrollTimer, 500, nullptr);
                InvalidateRect(window, nullptr, FALSE);
                return 0;
            }
            if (state->SelectVisibleArsenalItem(GET_Y_LPARAM(lParam))) {
                InvalidateRect(window, nullptr, FALSE);
            }
        }
        return 0;

    case WM_TIMER:
        if (wParam == kArsenalScrollTimer && gArsenalScrollDirection != 0) {
            if (!gArsenalScrollRepeating) {
                gArsenalScrollRepeating = true;
                SetTimer(window, kArsenalScrollTimer, 100, nullptr);
            }
            if (ApplicationState* state = GetApplicationState(window)) {
                if (state->ScrollVisibleArsenal(gArsenalScrollDirection < 0))
                    InvalidateRect(window, nullptr, FALSE);
            }
            return 0;
        }
        return DefWindowProcW(window, message, wParam, lParam);

    case WM_LBUTTONUP:
        if (gArsenalScrollDirection != 0) {
            KillTimer(window, kArsenalScrollTimer);
            gArsenalScrollDirection = 0;
            gArsenalScrollRepeating = false;
            if (GetCapture() == window) ReleaseCapture();
            InvalidateRect(window, nullptr, FALSE);
        }
        return 0;

    case WM_CAPTURECHANGED:
        KillTimer(window, kArsenalScrollTimer);
        gArsenalScrollDirection = 0;
        gArsenalScrollRepeating = false;
        InvalidateRect(window, nullptr, FALSE);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT paint{};
        HDC deviceContext = BeginPaint(window, &paint);
        RECT bounds{};
        GetClientRect(window, &bounds);
        FillRect(deviceContext, &bounds, static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH)));

        // FUN_1008_481e divides the vertical arsenal strip at 47, 85, 193, and 421 pixels.
        constexpr int divisions[] = {47, 85, 193, 421};
        for (const int y : divisions) {
            MoveToEx(deviceContext, 0, y, nullptr);
            LineTo(deviceContext, bounds.right, y);
        }
        SetBkMode(deviceContext, TRANSPARENT);
        if (ApplicationState* state = GetApplicationState(window)) {
            state->DrawArsenal(deviceContext, bounds.right, gArsenalScrollDirection);
            state->DrawMessagePanel(deviceContext, bounds.right);
        }
        EndPaint(window, &paint);
        return 0;
    }

    default:
        return DefWindowProcW(window, message, wParam, lParam);
    }
}

LRESULT CALLBACK BoardWindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_NCCREATE:
        AttachApplicationState(window, lParam);
        return TRUE;

    case WM_CREATE:
        if (ApplicationState* state = GetApplicationState(window)) {
            state->MarkEntireBoardDirty();
        }
        SetTimer(window, kBoardAnimationTimer, 200, nullptr);
        SetTimer(window, kDrainAnimationTimer, 50, nullptr);
        return 0;

    case WM_DESTROY:
        KillTimer(window, kBoardAnimationTimer);
        KillTimer(window, kDrainAnimationTimer);
        return 0;

    case WM_TIMER:
        if (wParam == kDrainAnimationTimer) {
            if (!IsWindowEnabled(GetParent(window))) return 0;
            if (ApplicationState* state = GetApplicationState(window);
                state != nullptr && state->AdvanceDrainAnimation()) {
                InvalidateRect(window, nullptr, FALSE);
                if (!state->IsDrainAnimating()) {
                    // WM_PAINT is normally lower priority than posted input.
                    // Show the new board before replaying actions queued by
                    // the original blocking five-frame drain transition.
                    UpdateWindow(window);
                    ReleaseDeferredDrainInputs(*state, window);
                }
            }
            return 0;
        }
        if (wParam == kBoardAnimationTimer) {
            // Original modal notices suspend the attract sequence as well.
            if (!IsWindowEnabled(GetParent(window))) return 0;
            // Keep the clicked demo panel in place until mouse release.
            if (gDemoArmedButton >= 0) return 0;
            if (ApplicationState* state = GetApplicationState(window)) {
                state->AdvanceAnimation();
                if (state->HasPendingLifecycleMessage())
                    ShowPendingGameplayMessage(GetParent(window), *state);
            }
            InvalidateRect(window, nullptr, FALSE);
            return 0;
        }
        break;

    case WM_ERASEBKGND:
        if (ApplicationState* state = GetApplicationState(window)) {
            state->MarkEntireBoardDirty();
        }
        return 1;

    case WM_PAINT: {
        PAINTSTRUCT paint{};
        HDC deviceContext = BeginPaint(window, &paint);
        RECT bounds{};
        GetClientRect(window, &bounds);

        // Compose a complete frame off-screen.  The original game drew through a persistent
        // board DC; painting the black background directly to the window exposed that
        // intermediate frame on modern composited desktops and caused a visible flash.
        HDC frameContext = CreateCompatibleDC(deviceContext);
        HBITMAP frameBitmap = CreateCompatibleBitmap(
            deviceContext, bounds.right - bounds.left, bounds.bottom - bounds.top);
        if (frameContext != nullptr && frameBitmap != nullptr) {
            HGDIOBJ previousBitmap = SelectObject(frameContext, frameBitmap);
            FillRect(frameContext, &bounds, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
            if (ApplicationState* state = GetApplicationState(window)) {
                state->DrawBoard(frameContext, gDemoPressedButton);
            }
            BitBlt(deviceContext, 0, 0, bounds.right, bounds.bottom,
                   frameContext, 0, 0, SRCCOPY);
            SelectObject(frameContext, previousBitmap);
        } else {
            FillRect(deviceContext, &bounds, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
            if (ApplicationState* state = GetApplicationState(window)) {
                state->DrawBoard(deviceContext, gDemoPressedButton);
            }
        }
        if (frameBitmap != nullptr) {
            DeleteObject(frameBitmap);
        }
        if (frameContext != nullptr) {
            DeleteDC(frameContext);
        }
        EndPaint(window, &paint);
        return 0;
    }

    case WM_LBUTTONDOWN: {
        SetFocus(GetParent(window));
        if (ApplicationState* state = GetApplicationState(window)) {
            if (state->IsDrainAnimating()) {
                DeferDrainInput(window, message, wParam, lParam);
                return 0;
            }
            if (state->IsDemoMode()) {
                gDemoPressedButton = state->DemoButtonAt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                gDemoArmedButton = gDemoPressedButton;
                if (gDemoPressedButton >= 0) SetCapture(window);
                InvalidateRect(window, nullptr, FALSE);
                return 0;
            }
            const int clickedColumn = GET_X_LPARAM(lParam) / GameData::kCellSize;
            const int clickedRow = GET_Y_LPARAM(lParam) / GameData::kCellSize;
            if ((wParam & MK_SHIFT) == 0) {
                if (state->MovePlayerToward(clickedColumn, clickedRow)) {
                    InvalidateRect(window, nullptr, FALSE);
                    InvalidateRect(GetDlgItem(GetParent(window), kArsenalWindowId), nullptr, FALSE);
                    ShowPendingGameplayMessage(GetParent(window), *state);
                }
            } else {
                if (state->UseSelectedItemToward(clickedColumn, clickedRow)) {
                    InvalidateRect(window, nullptr, FALSE);
                    InvalidateRect(GetDlgItem(GetParent(window), kArsenalWindowId), nullptr, FALSE);
                }
                // FUN_1008_1fe2 reports rejected placements after their world turn.
                ShowPendingGameplayMessage(GetParent(window), *state);
            }
        }
        return 0;
    }
    case WM_LBUTTONUP: {
        const int pressed = gDemoArmedButton;
        gDemoArmedButton = -1;
        gDemoPressedButton = -1;
        if (GetCapture() == window) ReleaseCapture();
        InvalidateRect(window, nullptr, FALSE);
        if (ApplicationState* state = GetApplicationState(window);
            state != nullptr && pressed >= 0
            && state->DemoButtonAt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)) == pressed) {
            if (pressed == 2) ShowHelpAndInformation(GetParent(window));
            else SendMessageW(GetParent(window), WM_COMMAND,
                pressed == 0 ? kCommandNewGame : kCommandOpenGame, 0);
        }
        return 0;
    }
    case WM_CAPTURECHANGED:
        gDemoPressedButton = -1;
        gDemoArmedButton = -1;
        InvalidateRect(window, nullptr, FALSE);
        return 0;
    case WM_MOUSEMOVE:
        if (gDemoArmedButton >= 0 && GetCapture() == window) {
            if (ApplicationState* state = GetApplicationState(window)) {
                const int hit = state->DemoButtonAt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                const int pressed = hit == gDemoArmedButton ? hit : -1;
                if (pressed != gDemoPressedButton) {
                    gDemoPressedButton = pressed;
                    InvalidateRect(window, nullptr, FALSE);
                }
            }
        }
        return 0;
    case WM_RBUTTONDOWN: {
        SetFocus(GetParent(window));
        if (ApplicationState* state = GetApplicationState(window)) {
            if (state->IsDrainAnimating()) {
                DeferDrainInput(window, message, wParam, lParam);
                return 0;
            }
            const std::wstring description = state->IdentifySquare(
                GET_X_LPARAM(lParam) / GameData::kCellSize,
                GET_Y_LPARAM(lParam) / GameData::kCellSize);
            if (!description.empty())
                InvalidateRect(GetDlgItem(GetParent(window), kArsenalWindowId), nullptr, FALSE);
        }
        return 0;
    }

    default:
        break;
    }
    return DefWindowProcW(window, message, wParam, lParam);
}

bool AppendOriginalSystemMenuItems(HWND window) {
    HMENU systemMenu = GetSystemMenu(window, FALSE);
    if (systemMenu == nullptr) {
        return false;
    }

    // Reconstructed from FUN_1008_6e4c and strings at segment offsets 0x7498 and 0x74ac.
    return AppendMenuW(systemMenu, MF_SEPARATOR, 0, nullptr) != FALSE
        && AppendMenuW(systemMenu, MF_STRING, kSystemCommandHelpAndInformation,
                       L"&Help && Info...") != FALSE
        && AppendMenuW(systemMenu, MF_SEPARATOR, 0, nullptr) != FALSE
        && AppendMenuW(systemMenu, MF_STRING, kSystemCommandAbout, L"&About...") != FALSE;
}

HMENU CreateMainMenu() {
    HMENU menu = CreateMenu();
    HMENU fileMenu = CreatePopupMenu();
    HMENU optionsMenu = CreatePopupMenu();
    if (menu == nullptr || fileMenu == nullptr || optionsMenu == nullptr
        || !AppendMenuW(fileMenu, MF_STRING, kCommandNewGame, L"&New Game\tCtrl+N")
        || !AppendMenuW(fileMenu, MF_SEPARATOR, 0, nullptr)
        || !AppendMenuW(fileMenu, MF_STRING, kCommandOpenGame, L"&Open Game...\tCtrl+O")
        || !AppendMenuW(fileMenu, MF_STRING, kCommandSaveGame, L"&Save Game...\tCtrl+S")
        || !AppendMenuW(fileMenu, MF_SEPARATOR, 0, nullptr)
        || !AppendMenuW(fileMenu, MF_STRING, kCommandHighScores, L"&High Scores...")
        || !AppendMenuW(fileMenu, MF_SEPARATOR, 0, nullptr)
        || !AppendMenuW(fileMenu, MF_STRING, kCommandExit, L"E&xit")
        || !AppendMenuW(menu, MF_POPUP, reinterpret_cast<UINT_PTR>(fileMenu), L"&File")
        || !AppendMenuW(optionsMenu, MF_STRING | MF_CHECKED, kCommandSound, L"&Sound")
        || !AppendMenuW(optionsMenu, MF_STRING, kCommandKeyboard, L"&Keyboard Commands...")
        || !AppendMenuW(menu, MF_POPUP, reinterpret_cast<UINT_PTR>(optionsMenu), L"&Options")) {
        if (fileMenu != nullptr) {
            DestroyMenu(fileMenu);
        }
        if (menu != nullptr) {
            DestroyMenu(menu);
        }
        return nullptr;
    }
    return menu;
}

bool ChooseGameFile(HWND owner, bool save, std::filesystem::path& result) {
    wchar_t path[MAX_PATH]{};
    constexpr wchar_t filter[] = L"Drain Storm Saved Game (*.VKW)\0*.VKW\0All Files (*.*)\0*.*\0\0";
    OPENFILENAMEW dialog{};
    dialog.lStructSize = sizeof(dialog);
    dialog.hwndOwner = owner;
    dialog.lpstrFilter = filter;
    dialog.lpstrFile = path;
    dialog.nMaxFile = MAX_PATH;
    dialog.lpstrDefExt = L"VKW";
    dialog.Flags = OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR
        | (save ? OFN_OVERWRITEPROMPT : OFN_FILEMUSTEXIST);
    const BOOL accepted = save ? GetSaveFileNameW(&dialog) : GetOpenFileNameW(&dialog);
    if (!accepted) {
        const DWORD error = CommDlgExtendedError();
        if (error != 0) {
            const auto notice = L"Unable to open the game-file dialog (error "
                + std::to_wstring(error) + L").";
            MessageBoxW(owner, notice.c_str(), kWindowTitle, MB_OK | MB_ICONERROR);
        }
        return false;
    }
    result = path;
    return true;
}

bool LayoutChildWindows(HWND window, bool create, ApplicationState& state) {
    RECT client{};
    if (!GetClientRect(window, &client)) {
        return false;
    }

    // Reconstructed from FUN_1008_6e4c. The board is 40x36 12-pixel cells plus a one-pixel
    // border on each edge; the arsenal uses the same height and is 120 pixels wide.
    const int boardWidth = 2 * (GetSystemMetrics(SM_CXBORDER) + 240);
    const int childHeight = 2 * (GetSystemMetrics(SM_CYBORDER) + 216);
    constexpr int arsenalWidth = 120;
    const int y = ((client.bottom - client.top) - childHeight) / 2;
    constexpr int arsenalX = 12;
    const int boardX = client.right - boardWidth - 6;

    if (create) {
        HWND arsenal = CreateWindowExW(0, kArsenalWindowClassName, nullptr,
                                       WS_CHILD | WS_VISIBLE, arsenalX, y, arsenalWidth,
                                       childHeight, window,
                                       reinterpret_cast<HMENU>(static_cast<INT_PTR>(kArsenalWindowId)),
                                       reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(window, GWLP_HINSTANCE)),
                                       &state);
        HWND board = CreateWindowExW(0, kBoardWindowClassName, nullptr,
                                     WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPCHILDREN,
                                     boardX, y, boardWidth, childHeight, window,
                                     reinterpret_cast<HMENU>(static_cast<INT_PTR>(kBoardWindowId)),
                                     reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(window, GWLP_HINSTANCE)),
                                     &state);
        return arsenal != nullptr && board != nullptr;
    }

    const BOOL arsenalMoved = MoveWindow(GetDlgItem(window, kArsenalWindowId), arsenalX, y,
                                          arsenalWidth, childHeight, TRUE);
    const BOOL boardMoved = MoveWindow(GetDlgItem(window, kBoardWindowId), boardX, y,
                                       boardWidth, childHeight, TRUE);
    return arsenalMoved != FALSE && boardMoved != FALSE;
}

LRESULT CALLBACK MainWindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_NCCREATE: {
        AttachApplicationState(window, lParam);
        return TRUE;
    }

    case WM_CREATE:
        // Original handler: FUN_1008_6e4c.
        if (!AppendOriginalSystemMenuItems(window)) {
            return -1;
        }
        if (ApplicationState* state = GetApplicationState(window)) {
            if (!LayoutChildWindows(window, true, *state)) {
                return -1;
            }
        }
        return 0;

    case WM_SIZE:
        if (ApplicationState* state = GetApplicationState(window)) {
            LayoutChildWindows(window, false, *state);
        }
        return 0;

    case WM_COMMAND:
        if (LOWORD(wParam) == kCommandNewGame || LOWORD(wParam) == kCommandOpenGame
            || LOWORD(wParam) == kCommandQuitCurrent)
            gDeferredDrainInputs.clear();
        if (LOWORD(wParam) == kCommandSound) {
            if (ApplicationState* state = GetApplicationState(window)) {
                const bool enabled = state->ToggleSound();
                CheckMenuItem(GetMenu(window), kCommandSound,
                              MF_BYCOMMAND | (enabled ? MF_CHECKED : MF_UNCHECKED));
            }
            return 0;
        }
        if (LOWORD(wParam) == kCommandHighScores) {
            ShowHelpAndInformation(window, 10);
            return 0;
        }
        if (LOWORD(wParam) == kCommandKeyboard) {
            if (ApplicationState* state = GetApplicationState(window);
                state != nullptr && !PromptForKeyboardBindings(window, *state)) {
                MessageBoxW(window, L"Unable to save keyboard commands.", kWindowTitle,
                            MB_OK | MB_ICONERROR);
            }
            return 0;
        }
        if (LOWORD(wParam) == kCommandNewGame) {
            ApplicationState* state = GetApplicationState(window);
            const bool confirmed = state != nullptr && (state->IsDemoMode()
                || MessageBoxW(window, L"Abandon the current game and begin a new one?",
                               kWindowTitle, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES);
            if (confirmed) {
                if (const std::optional<int> difficulty = PromptForDifficulty(window);
                    difficulty.has_value()) {
                    if (state->NewGame(*difficulty)) {
                        RefreshGameWindows(window);
                    } else {
                        MessageBoxW(window, L"Unable to start a new game.", kWindowTitle,
                                    MB_OK | MB_ICONERROR);
                    }
                }
            }
            return 0;
        }
        if (LOWORD(wParam) == kCommandQuitCurrent) {
            if (ApplicationState* state = GetApplicationState(window)) {
                const int choice = MessageBoxW(window, L"Do you want to save the current game?",
                    kWindowTitle, MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON1);
                if (choice == IDCANCEL) return 0;
                if (choice == IDYES) {
                    std::filesystem::path path;
                    if (ChooseGameFile(window, true, path) && !state->SaveGame(path)) {
                        MessageBoxW(window, L"Unable to save this game.", kWindowTitle,
                                    MB_OK | MB_ICONERROR);
                    }
                }
                if (state->QuitCurrentGame(choice == IDNO)) {
                    RefreshGameWindows(window);
                    ShowPendingGameplayMessage(window, *state);
                } else {
                    MessageBoxW(window, L"Unable to restart the game.", kWindowTitle,
                                MB_OK | MB_ICONERROR);
                }
            }
            return 0;
        }
        if (LOWORD(wParam) == kCommandExit) {
            DestroyWindow(window);
            return 0;
        }
        if (LOWORD(wParam) == kCommandOpenGame || LOWORD(wParam) == kCommandSaveGame) {
            if (ApplicationState* state = GetApplicationState(window)) {
                const bool save = LOWORD(wParam) == kCommandSaveGame;
                std::filesystem::path path;
                if (ChooseGameFile(window, save, path)) {
                    const bool succeeded = save ? state->SaveGame(path) : state->LoadGame(path);
                    if (!succeeded) {
                        MessageBoxW(window,
                                    save ? L"Unable to save this game."
                                         : L"This is not a valid Drain Storm saved game.",
                                    kWindowTitle, MB_OK | MB_ICONERROR);
                    } else if (!save) {
                        RefreshGameWindows(window);
                    }
                }
            }
            return 0;
        }
        break;

    case WM_MOVE:
    case WM_ACTIVATEAPP:
        // Original handler: FUN_1008_a5fa.
        if (ApplicationState* state = GetApplicationState(window)) {
            state->MarkEntireBoardDirty();
        }
        return 0;

    case WM_KEYDOWN: {
        ApplicationState* state = GetApplicationState(window);
        if (state != nullptr && state->IsDrainAnimating()) {
            DeferDrainInput(window, message, wParam, lParam);
            return 0;
        }
        // FUN_1008_6f46 permits N, L, and VK_F1 on attract boards.
        if (state != nullptr && state->IsDemoMode()) {
            if (wParam == 'N' || wParam == 'L') {
                SendMessageW(window, WM_COMMAND,
                             wParam == 'N' ? kCommandNewGame : kCommandOpenGame, 0);
            } else if (wParam == VK_F1) {
                ShowHelpAndInformation(window, 1);
            }
            return 0;
        }
        if (state != nullptr) {
            if (gPreviousGameplayKey != state->KeyBinding(4))
                gCopyrightGestureArmed = false;
            gPreviousGameplayKey = static_cast<UINT>(wParam);
            if (wParam == state->KeyBinding(4)
                && state->PlayerColumn() == 39 && state->PlayerRow() == 0
                && (gReplayingDrainInput ? gReplayShiftHeld
                    : (GetKeyState(VK_SHIFT) & 0x8000) != 0)) {
                gCopyrightGestureArmed = true;
            }
        }
        if ((GetKeyState(VK_CONTROL) & 0x8000) != 0) {
            UINT command = 0;
            if (wParam == 'N') command = kCommandNewGame;
            if (wParam == 'O') command = kCommandOpenGame;
            if (wParam == 'S') command = kCommandSaveGame;
            if (command != 0) {
                SendMessageW(window, WM_COMMAND, command, 0);
                return 0;
            }
        }
        // 1008:6f46 tests configurable movement before command bindings and
        // only then the literal N/L/C/F1 shortcuts. Duplicate bindings are
        // legal, so this order is observable even without rebinding UI.
        if (state != nullptr) {
            int deltaColumn = 0;
            int deltaRow = 0;
            if (wParam == state->KeyBinding(0)) deltaRow = -1;
            else if (wParam == state->KeyBinding(1)) deltaRow = 1;
            else if (wParam == state->KeyBinding(2)) deltaColumn = 1;
            else if (wParam == state->KeyBinding(3)) deltaColumn = -1;
            else if (wParam == state->KeyBinding(4)) { deltaColumn = 1; deltaRow = -1; }
            else if (wParam == state->KeyBinding(5)) { deltaColumn = 1; deltaRow = 1; }
            else if (wParam == state->KeyBinding(6)) { deltaColumn = -1; deltaRow = -1; }
            else if (wParam == state->KeyBinding(7)) { deltaColumn = -1; deltaRow = 1; }
            const bool directionAccepted = deltaColumn != 0 || deltaRow != 0
                || wParam == state->KeyBinding(8);
            if (directionAccepted) {
                const bool shiftHeld = gReplayingDrainInput ? gReplayShiftHeld
                    : (GetKeyState(VK_SHIFT) & 0x8000) != 0;
                const bool changed = shiftHeld
                    ? state->UseSelectedItem(deltaColumn, deltaRow)
                    : state->MovePlayer(deltaColumn, deltaRow);
                if (changed) {
                    InvalidateRect(GetDlgItem(window, kBoardWindowId), nullptr, FALSE);
                    InvalidateRect(GetDlgItem(window, kArsenalWindowId), nullptr, FALSE);
                }
                if (changed || shiftHeld) ShowPendingGameplayMessage(window, *state);
                return 0;
            }
        }
        if (state != nullptr && wParam == state->KeyBinding(16)) {
            SendMessageW(window, WM_COMMAND, kCommandSound, 0);
            state->PostGameplayNotice(state->SoundEnabled() ? L"Sound On" : L"Sound Off");
            InvalidateRect(GetDlgItem(window, kArsenalWindowId), nullptr, FALSE);
            return 0;
        }
        if (state != nullptr && wParam == state->KeyBinding(11)) {
            if (state->RecallDiscoveredLaw()) {
                ShowPendingGameplayMessage(window, *state);
            }
            return 0;
        }
        if (state != nullptr && (wParam == state->KeyBinding(12)
                                || wParam == state->KeyBinding(13))) {
            (void)state->NavigateMessageHistory(wParam == state->KeyBinding(12));
            InvalidateRect(GetDlgItem(window, kArsenalWindowId), nullptr, FALSE);
            return 0;
        }
        if (state != nullptr && (wParam == state->KeyBinding(9)
                                || wParam == state->KeyBinding(10))) {
            const bool changed = wParam == state->KeyBinding(9)
                ? state->SelectPreviousItem() : state->SelectNextItem();
            if (changed) {
                InvalidateRect(GetDlgItem(window, kArsenalWindowId), nullptr, FALSE);
            }
            return 0;
        }
        if (state != nullptr && (wParam == state->KeyBinding(17)
                                || wParam == state->KeyBinding(18))) {
            if (wParam == state->KeyBinding(18) && gCopyrightGestureArmed) {
                state->PostGameplayNotice(L"(c) Copyright 1997 by\nDave Horner");
                InvalidateRect(GetDlgItem(window, kArsenalWindowId), nullptr, FALSE);
            }
            if (state->ActivateTransporter(wParam == state->KeyBinding(17))) {
                RefreshGameWindows(window);
                ShowPendingGameplayMessage(window, *state);
            }
            return 0;
        }
        if (state != nullptr && wParam == state->KeyBinding(15)) {
            SendMessageW(window, WM_COMMAND, kCommandSaveGame, 0);
            return 0;
        }
        if (state != nullptr && wParam == state->KeyBinding(14)) {
            SendMessageW(window, WM_COMMAND, kCommandQuitCurrent, 0);
            return 0;
        }
        if (wParam == 'N' || wParam == 'L') {
            MessageBoxW(window,
                        wParam == 'N'
                            ? L"You must quit this game before starting a new game."
                            : L"You must quit this game before loading a new game.",
                        kWindowTitle, MB_OK | MB_ICONINFORMATION);
            return 0;
        }
        if (wParam == 'C' || wParam == VK_F1) {
            ShowHelpAndInformation(window, wParam == VK_F1 ? 1 : 6);
            return 0;
        }
        return 0;
    }

    case WM_SYSKEYDOWN:
        // FUN_1008_73ce consumes Alt+Up/Alt+Down to move the arsenal selection.
        if (ApplicationState* state = GetApplicationState(window);
            state != nullptr && state->IsDrainAnimating()) {
            DeferDrainInput(window, message, wParam, lParam);
            return 0;
        }
        if ((lParam & (1LL << 29)) != 0) {
            if (ApplicationState* state = GetApplicationState(window)) {
                bool changed = false;
                if (wParam == VK_UP) {
                    changed = state->SelectPreviousItem();
                } else if (wParam == VK_DOWN) {
                    changed = state->SelectNextItem();
                }
                if (changed) {
                    InvalidateRect(GetDlgItem(window, kArsenalWindowId), nullptr, FALSE);
                    return 0;
                }
            }
        }
        break;

    case WM_SYSCOMMAND:
        // The original routes these to FUN_1008_2dfc (help) and FUN_1008_3f18 (about).
        // MainWindowProcedure compares the custom command words directly.
        // Applying the usual SC_* low-nibble mask changes both original IDs
        // and makes the appended menu entries unreachable.
        if (static_cast<UINT>(wParam) == kSystemCommandHelpAndInformation) {
            ShowHelpAndInformation(window);
            return 0;
        }
        if (static_cast<UINT>(wParam) == kSystemCommandAbout) {
            ShowAbout(window);
            return 0;
        }
        break;

    case WM_ERASEBKGND:
        return 1;

    case WM_PAINT: {
        PAINTSTRUCT paint{};
        HDC deviceContext = BeginPaint(window, &paint);
        RECT arsenal{}, board{}, client{};
        GetClientRect(window, &client);
        GetWindowRect(GetDlgItem(window, kArsenalWindowId), &arsenal);
        GetWindowRect(GetDlgItem(window, kBoardWindowId), &board);
        MapWindowPoints(nullptr, window, reinterpret_cast<POINT*>(&arsenal), 2);
        MapWindowPoints(nullptr, window, reinterpret_cast<POINT*>(&board), 2);
        ApplicationState::DrawControlSurroundings(deviceContext, client, arsenal, board);
        EndPaint(window, &paint);
        return 0;
    }

    case WM_DESTROY:
        gDeferredDrainInputs.clear();
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProcW(window, message, wParam, lParam);
    }

    return DefWindowProcW(window, message, wParam, lParam);
}

HICON LoadOriginalMainIcon(const std::filesystem::path& executablePath) {
    const std::filesystem::path iconPath = executablePath.parent_path().parent_path()
        / L"assets" / L"original" / L"icon_1.bin";
    std::ifstream stream(iconPath, std::ios::binary);
    if (!stream) return nullptr;
    const std::vector<BYTE> image{std::istreambuf_iterator<char>(stream),
                                  std::istreambuf_iterator<char>()};
    if (image.empty() || image.size() > MAXDWORD) return nullptr;
    // MAINICON's extracted group record points at RT_ICON id 1. The payload is
    // already in the DIB-plus-mask form expected by CreateIconFromResourceEx.
    return CreateIconFromResourceEx(const_cast<PBYTE>(image.data()),
                                    static_cast<DWORD>(image.size()), TRUE, 0x00030000,
                                    0, 0, LR_DEFAULTCOLOR);
}

ATOM RegisterMainWindowClass(HINSTANCE instance, HICON icon) {
    WNDCLASSEXW windowClass{};
    windowClass.cbSize = sizeof(windowClass);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = MainWindowProcedure;
    windowClass.hInstance = instance;
    windowClass.hIcon = icon;
    windowClass.hIconSm = icon;
    windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    windowClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH));
    windowClass.lpszClassName = kWindowClassName;
    return RegisterClassExW(&windowClass);
}

ATOM RegisterChildWindowClass(HINSTANCE instance, const wchar_t* name, WNDPROC procedure,
                              HBRUSH background) {
    WNDCLASSEXW windowClass{};
    windowClass.cbSize = sizeof(windowClass);
    windowClass.lpfnWndProc = procedure;
    windowClass.hInstance = instance;
    windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    windowClass.hbrBackground = background;
    windowClass.lpszClassName = name;
    return RegisterClassExW(&windowClass);
}

std::filesystem::path GetExecutablePath() {
    std::wstring buffer(32768, L'\0');
    const DWORD length = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
    if (length == 0 || length == buffer.size()) {
        throw std::runtime_error("GetModuleFileNameW failed");
    }
    buffer.resize(length);
    return buffer;
}

HWND CreateMainWindow(HINSTANCE instance, int showCommand, ApplicationState& state) {
    RECT rectangle{0, 0, kOriginalWindowWidth, kOriginalWindowHeight};
    constexpr DWORD style = WS_OVERLAPPEDWINDOW;
    if (!AdjustWindowRectEx(&rectangle, style, TRUE, 0)) {
        throw std::runtime_error("AdjustWindowRectEx failed");
    }

    HMENU menu = CreateMainMenu();
    if (menu == nullptr) {
        throw std::runtime_error("CreateMenu failed");
    }
    HWND window = CreateWindowExW(
        0,
        kWindowClassName,
        kWindowTitle,
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rectangle.right - rectangle.left,
        rectangle.bottom - rectangle.top,
        nullptr,
        menu,
        instance,
        &state);
    if (window == nullptr) {
        DestroyMenu(menu);
        throw std::runtime_error("CreateWindowExW failed");
    }

    ShowWindow(window, showCommand);
    UpdateWindow(window);
    return window;
}

int RunMessageLoop() {
    MSG message{};
    while (true) {
        const BOOL result = GetMessageW(&message, nullptr, 0, 0);
        if (result == 0) {
            return static_cast<int>(message.wParam);
        }
        if (result == -1) {
            throw std::runtime_error("GetMessageW failed");
        }
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
}

} // namespace drain_storm

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int showCommand) {
    try {
        const std::filesystem::path executablePath = drain_storm::GetExecutablePath();
        drain_storm::ApplicationState state;
        if (!state.LoadOriginalResources(executablePath)) {
            throw std::runtime_error("Could not load the original BOARD_BITMAPS resource");
        }
        HICON mainIcon = drain_storm::LoadOriginalMainIcon(executablePath);
        if (drain_storm::RegisterMainWindowClass(instance, mainIcon) == 0) {
            throw std::runtime_error("RegisterClassExW failed");
        }
        if (drain_storm::RegisterChildWindowClass(
                instance, drain_storm::kArsenalWindowClassName,
                drain_storm::ArsenalWindowProcedure,
                static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH))) == 0) {
            throw std::runtime_error("Arsenal window registration failed");
        }
        if (drain_storm::RegisterChildWindowClass(
                instance, drain_storm::kBoardWindowClassName,
                drain_storm::BoardWindowProcedure,
                static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH))) == 0) {
            throw std::runtime_error("Board window registration failed");
        }
        const HWND mainWindow = drain_storm::CreateMainWindow(instance, showCommand, state);
        if (state.HasPendingLifecycleMessage())
            drain_storm::ShowPendingGameplayMessage(mainWindow, state);
        const int result = drain_storm::RunMessageLoop();
        if (mainIcon != nullptr) DestroyIcon(mainIcon);
        return result;
    }
    catch (const std::exception& exception) {
        MessageBoxA(nullptr, exception.what(), "Drain Storm", MB_OK | MB_ICONERROR);
        return EXIT_FAILURE;
    }
}
