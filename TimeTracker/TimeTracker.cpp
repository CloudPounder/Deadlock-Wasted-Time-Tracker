#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <objidl.h>
#include <dwmapi.h>
#include <windowsx.h>
#include <gdiplus.h>
#include <mmsystem.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>

#define MAINICON 101
#define SND_FEEDBACK 102
#define IMG_FLASH 103
#define IMG_FLASH_CLOSE 104
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "winmm.lib")

static const BYTE ClockIconBits[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xE0,0x1F,0xF8,0x3F,0xFC,0x7F,0xFE,0x7F,0xFE,
    0xFF,0xFF,0xFF,0xEF,0xFF,0xEF,0xFF,0xEF,0xFF,0x81,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0x7F,0xFE,0x7F,0xFE,0x3F,0xFC,0x1F,0xF8,0x07,0xE0,0x00,0x00,0x00,0x00
};

struct Theme {
    const wchar_t* name;
    COLORREF bg, surface, border;
    COLORREF textMain, textSub, textDisabled;
    COLORREF btn, btnHover, btnPress, btnDis;
    COLORREF btnAccent, btnAccentHover, btnAccentPress;
    COLORREF statusWork, statusBreak, dailyTotal, statusIdle;
    COLORREF titleBar, titleText, captionBtn, captionBtnHover, closeBtn, closeBtnHover;
    COLORREF optBtn, optBtnHover;
};

static const Theme Themes[] = {
    {L"Dark Purple",
     RGB(30,28,36),RGB(42,40,52),RGB(80,72,100),
     RGB(240,235,255),RGB(170,160,200),RGB(90,85,110),
     RGB(58,54,74),RGB(78,72,98),RGB(44,40,58),RGB(50,48,64),
     RGB(140,90,230),RGB(165,115,255),RGB(100,60,180),
     RGB(190,150,255),RGB(255,200,100),RGB(200,165,255),RGB(130,120,155),
     RGB(36,34,44),RGB(200,195,220),RGB(58,54,74),RGB(78,72,98),RGB(160,45,45),RGB(210,55,55),
     RGB(65,60,85),RGB(85,78,110)},
    {L"Midnight Blue",
     RGB(15,20,40),RGB(22,30,58),RGB(45,60,110),
     RGB(210,225,255),RGB(130,155,215),RGB(60,75,120),
     RGB(30,45,90),RGB(45,62,115),RGB(20,32,68),RGB(28,38,75),
     RGB(30,120,240),RGB(60,150,255),RGB(10,85,190),
     RGB(90,175,255),RGB(255,195,70),RGB(110,210,255),RGB(80,105,165),
     RGB(12,16,32),RGB(180,200,240),RGB(30,45,90),RGB(50,70,130),RGB(160,45,45),RGB(210,55,55),
     RGB(25,40,80),RGB(40,60,115)},
    {L"Forest Green",
     RGB(20,30,24),RGB(30,44,34),RGB(55,82,62),
     RGB(215,245,220),RGB(135,185,145),RGB(70,105,78),
     RGB(40,68,48),RGB(55,88,62),RGB(28,50,35),RGB(36,58,42),
     RGB(45,175,85),RGB(65,205,105),RGB(30,135,62),
     RGB(110,230,140),RGB(255,205,80),RGB(130,240,160),RGB(80,135,95),
     RGB(16,24,20),RGB(180,220,185),RGB(40,68,48),RGB(60,95,70),RGB(160,45,45),RGB(210,55,55),
     RGB(32,56,38),RGB(48,80,55)},
    {L"Crimson Dark",
     RGB(30,18,18),RGB(46,26,26),RGB(90,45,45),
     RGB(255,225,225),RGB(205,155,155),RGB(110,70,70),
     RGB(70,35,35),RGB(95,48,48),RGB(52,24,24),RGB(60,30,30),
     RGB(210,50,70),RGB(240,75,92),RGB(165,28,46),
     RGB(255,135,135),RGB(255,215,80),RGB(255,155,155),RGB(155,95,95),
     RGB(24,14,14),RGB(220,185,185),RGB(70,35,35),RGB(100,50,50),RGB(185,35,35),RGB(225,45,45),
     RGB(62,28,28),RGB(88,40,40)},
    {L"Light & Clean",
     RGB(245,245,250),RGB(255,255,255),RGB(190,190,210),
     RGB(25,25,45),RGB(95,95,125),RGB(175,175,195),
     RGB(215,215,230),RGB(230,230,245),RGB(200,200,220),RGB(210,210,228),
     RGB(100,75,210),RGB(125,100,235),RGB(75,55,180),
     RGB(95,75,210),RGB(195,115,0),RGB(75,155,255),RGB(135,135,168),
     RGB(235,235,245),RGB(40,40,60),RGB(215,215,230),RGB(195,195,215),RGB(185,45,45),RGB(220,55,55),
     RGB(220,218,235),RGB(205,202,225)},
    {L"Warm Amber",
     RGB(30,22,10),RGB(46,34,14),RGB(90,68,28),
     RGB(255,242,215),RGB(205,175,115),RGB(110,88,44),
     RGB(72,54,20),RGB(98,75,30),RGB(52,38,12),RGB(62,46,16),
     RGB(225,150,20),RGB(250,175,45),RGB(180,115,10),
     RGB(255,205,75),RGB(110,225,135),RGB(255,190,55),RGB(155,122,60),
     RGB(24,18,8),RGB(230,210,175),RGB(72,54,20),RGB(100,75,30),RGB(160,45,45),RGB(210,55,55),
     RGB(60,45,16),RGB(85,64,22)}
};
static const int ThemeCount = sizeof(Themes) / sizeof(Themes[0]);
static int ActiveTheme = 0;
static const int TitleBarHeight = 40;

struct Brushes {
    HBRUSH bg, surface, btn, btnHover, btnPress, btnDis, accent, accentH, accentP;
    HBRUSH titleBar, captionBtn, captionBtnHover, closeBtn, closeBtnHover, optBtn, optBtnHover;
    void Build(const Theme& t) {
        Destroy();
        bg = CreateSolidBrush(t.bg); surface = CreateSolidBrush(t.surface);
        btn = CreateSolidBrush(t.btn); btnHover = CreateSolidBrush(t.btnHover);
        btnPress = CreateSolidBrush(t.btnPress); btnDis = CreateSolidBrush(t.btnDis);
        accent = CreateSolidBrush(t.btnAccent); accentH = CreateSolidBrush(t.btnAccentHover);
        accentP = CreateSolidBrush(t.btnAccentPress);
        titleBar = CreateSolidBrush(t.titleBar);
        captionBtn = CreateSolidBrush(t.captionBtn);
        captionBtnHover = CreateSolidBrush(t.captionBtnHover);
        closeBtn = CreateSolidBrush(t.closeBtn);
        closeBtnHover = CreateSolidBrush(t.closeBtnHover);
        optBtn = CreateSolidBrush(t.optBtn);
        optBtnHover = CreateSolidBrush(t.optBtnHover);
    }
    void Destroy() {
        HBRUSH b[] = { bg,surface,btn,btnHover,btnPress,btnDis,accent,accentH,accentP,
                    titleBar,captionBtn,captionBtnHover,closeBtn,closeBtnHover,optBtn,optBtnHover };
        for (int i = 0;i < 16;i++) if (b[i]) DeleteObject(b[i]);
        bg = surface = btn = btnHover = btnPress = btnDis = accent = accentH = accentP =
            titleBar = captionBtn = captionBtnHover = closeBtn = closeBtnHover = optBtn = optBtnHover = NULL;
    }
};
static Brushes B;

enum AppState { IDLE, RUNNING, ON_BREAK };
static AppState State = IDLE;
static ULONGLONG Elapsed = 0, StartTick = 0;
static SYSTEMTIME SessionStart;
static const wchar_t* LogFile = L"time_log.txt";

static HWND Wnd, LblTime, LblStatus, LblDaily;
static HWND BtnClock, BtnBreak, BtnCopy, BtnDelete;
static HWND BtnOptions, BtnExit, BtnFeedback;
static HWND LogBox, LogHeader;
static HFONT FTimer, FStatus, FBtn, FLog, FBold, FHead, FTitle, FCaption;

struct Session { std::wstring line, date; };
static std::vector<Session> AllSessions;

static bool HoverClock = false, HoverBreak = false, HoverCopy = false, HoverDelete = false;
static bool HoverMin = false, HoverClose = false, HoverOpt = false, HoverExit = false, HoverFeedback = false;
static bool TrackingMouse = false;
static RECT RectMin = { 0,0,0,0 }, RectClose = { 0,0,0,0 };

static ULONG_PTR GdipToken = 0;

static std::wstring FormatTime(ULONGLONG s) {
    std::wostringstream o;
    o << std::setw(2) << std::setfill(L'0') << (s / 3600) << L':' << std::setw(2) << (s % 3600) / 60 << L':' << std::setw(2) << s % 60;
    return o.str();
}
static std::wstring DateStr(const SYSTEMTIME& t) {
    std::wostringstream o;
    o << t.wYear << L'-' << std::setw(2) << std::setfill(L'0') << t.wMonth << L'-' << std::setw(2) << t.wDay;
    return o.str();
}
static std::wstring TimeStr(const SYSTEMTIME& t) {
    std::wostringstream o;
    o << std::setw(2) << std::setfill(L'0') << t.wHour << L':' << std::setw(2) << std::setfill(L'0') << t.wMinute << L':' << std::setw(2) << std::setfill(L'0') << t.wSecond;
    return o.str();
}
static std::wstring Today() { SYSTEMTIME t;GetLocalTime(&t);return DateStr(t); }
static ULONGLONG ParseDuration(const std::wstring& line) {
    size_t pos = line.rfind(L"DURATION ");
    if (pos == std::wstring::npos)return 0;
    std::wstring hms = line.substr(pos + 9);
    try { return std::stoi(hms.substr(0, 2)) * 3600 + std::stoi(hms.substr(3, 2)) * 60 + std::stoi(hms.substr(6, 2)); }
    catch (...) { return 0; }
}

static void LoadAllSessions() {
    AllSessions.clear();
    std::wifstream f(LogFile);std::wstring line;
    while (std::getline(f, line)) {
        if (line.size() < 10)continue;
        Session s;s.line = line;s.date = line.substr(0, 10);AllSessions.push_back(s);
    }
}
static void SaveAllSessions() {
    std::wofstream f(LogFile, std::ios::trunc);
    for (auto& s : AllSessions)f << s.line << L"\n";
}
static void AppendSession(const std::wstring& line) {
    std::wofstream f(LogFile, std::ios::app);
    if (f)f << line << L"\n";
    Session s;s.line = line;s.date = line.substr(0, 10);AllSessions.push_back(s);
}

static ULONGLONG GetElapsed() {
    if (State == RUNNING)return Elapsed + (GetTickCount64() / 1000 - StartTick);
    return Elapsed;
}
static void UpdateTime() { SetWindowTextW(LblTime, FormatTime(GetElapsed()).c_str()); }
static void UpdateLog() {
    std::wstring today = Today();std::wstring txt;ULONGLONG tot = 0;
    for (auto& s : AllSessions) { if (s.date == today) { txt += s.line + L"\r\n";tot += ParseDuration(s.line); } }
    if (txt.empty())txt = L"  No sessions logged today.";
    SetWindowTextW(LogBox, txt.c_str());
    SendMessageW(LogBox, EM_SETSEL, (WPARAM)-1, (LPARAM)-1);
    SendMessageW(LogBox, EM_SCROLLCARET, 0, 0);
    std::wostringstream d;d << L"Today's Total:  " << FormatTime(tot);
    SetWindowTextW(LblDaily, d.str().c_str());
}

static void RefreshButtons() {
    InvalidateRect(BtnClock, NULL, TRUE);InvalidateRect(BtnBreak, NULL, TRUE);
    InvalidateRect(BtnCopy, NULL, TRUE);InvalidateRect(BtnDelete, NULL, TRUE);
    InvalidateRect(BtnOptions, NULL, TRUE);InvalidateRect(BtnExit, NULL, TRUE);
    InvalidateRect(BtnFeedback, NULL, TRUE);
}
static void DoClockIn() {
    GetLocalTime(&SessionStart);State = RUNNING;StartTick = GetTickCount64() / 1000;
    SetTimer(Wnd, 1, 500, NULL);SetWindowTextW(BtnClock, L"Clock Out");
    SetWindowTextW(LblStatus, L"Working");EnableWindow(BtnBreak, TRUE);
    EnableWindow(BtnCopy, TRUE);RefreshButtons();
}
static void DoClockOut() {
    if (State == RUNNING)Elapsed += (GetTickCount64() / 1000 - StartTick);
    SYSTEMTIME end;GetLocalTime(&end);
    std::wostringstream line;
    line << DateStr(SessionStart) << L" | START " << TimeStr(SessionStart) << L" | END " << TimeStr(end) << L" | DURATION " << FormatTime(Elapsed);
    AppendSession(line.str());UpdateLog();KillTimer(Wnd, 1);
    State = IDLE;Elapsed = 0;
    SetWindowTextW(LblTime, L"00:00:00");SetWindowTextW(BtnClock, L"Clock In");
    SetWindowTextW(BtnBreak, L"Break");SetWindowTextW(LblStatus, L"Idle");
    EnableWindow(BtnBreak, FALSE);EnableWindow(BtnCopy, FALSE);RefreshButtons();
}
static void DoBreak() {
    Elapsed += (GetTickCount64() / 1000 - StartTick);State = ON_BREAK;KillTimer(Wnd, 1);
    SetWindowTextW(BtnBreak, L"Resume");SetWindowTextW(LblStatus, L"On Break");RefreshButtons();
}
static void DoResume() {
    StartTick = GetTickCount64() / 1000;State = RUNNING;SetTimer(Wnd, 1, 500, NULL);
    SetWindowTextW(BtnBreak, L"Break");SetWindowTextW(LblStatus, L"Working");RefreshButtons();
}
static void CopyToClipboard() {
    std::wstring text = FormatTime(GetElapsed());
    if (!OpenClipboard(Wnd))return;
    EmptyClipboard();
    size_t bytes = (text.size() + 1) * sizeof(wchar_t);
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, bytes);
    if (hMem) { void* p = GlobalLock(hMem);if (p) { memcpy(p, text.c_str(), bytes);GlobalUnlock(hMem);SetClipboardData(CF_UNICODETEXT, hMem); } else GlobalFree(hMem); }
    CloseClipboard();SetWindowTextW(LblStatus, L"Copied!");
}

static void FillRoundRect(HDC hdc, RECT r, int radius, HBRUSH br) {
    HBRUSH old = (HBRUSH)SelectObject(hdc, br);
    HPEN pen = CreatePen(PS_SOLID, 1, GetPixel(hdc, r.left, r.top));
    HPEN oldP = (HPEN)SelectObject(hdc, pen);
    RoundRect(hdc, r.left, r.top, r.right, r.bottom, radius, radius);
    SelectObject(hdc, old);SelectObject(hdc, oldP);DeleteObject(pen);
}

static void DrawSoftBtn(HDC hdc, RECT r, HBRUSH fill, COLORREF borderCol, const wchar_t* text, HFONT font, COLORREF textCol, bool shadow = true) {
    if (shadow) {
        RECT sr = { r.left + 2,r.top + 3,r.right + 2,r.bottom + 3 };
        HBRUSH sb = CreateSolidBrush(RGB(0, 0, 0));
        HPEN sp = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HPEN op = (HPEN)SelectObject(hdc, sp);
        HBRUSH ob = (HBRUSH)SelectObject(hdc, sb);
        BLENDFUNCTION bf = { AC_SRC_OVER,0,40,0 };
        HBITMAP hbm = CreateCompatibleBitmap(hdc, sr.right - sr.left, sr.bottom - sr.top);
        HDC mdc = CreateCompatibleDC(hdc);
        HBITMAP old = (HBITMAP)SelectObject(mdc, hbm);
        RECT mr = { 0,0,sr.right - sr.left,sr.bottom - sr.top };
        FillRect(mdc, &mr, sb);
        AlphaBlend(hdc, sr.left, sr.top, sr.right - sr.left, sr.bottom - sr.top, mdc, 0, 0, mr.right, mr.bottom, bf);
        SelectObject(mdc, old);DeleteDC(mdc);DeleteObject(hbm);
        SelectObject(hdc, ob);SelectObject(hdc, op);DeleteObject(sb);DeleteObject(sp);
    }
    HPEN pen = CreatePen(PS_SOLID, 1, borderCol);
    HPEN oldP = (HPEN)SelectObject(hdc, pen);
    HBRUSH oldB = (HBRUSH)SelectObject(hdc, fill);
    RoundRect(hdc, r.left, r.top, r.right, r.bottom, 12, 12);
    SelectObject(hdc, oldB);SelectObject(hdc, oldP);DeleteObject(pen);
    HFONT oldF = (HFONT)SelectObject(hdc, font);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, textCol);
    DrawTextW(hdc, text, -1, const_cast<RECT*>(&r), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(hdc, oldF);
}
static void DrawBtn(DRAWITEMSTRUCT* d) {
    bool pressed = (d->itemState & ODS_SELECTED) != 0;
    bool dis = (d->itemState & ODS_DISABLED) != 0;
    bool hover = (d->hwndItem == BtnClock && HoverClock)
        || (d->hwndItem == BtnBreak && HoverBreak)
        || (d->hwndItem == BtnCopy && HoverCopy)
        || (d->hwndItem == BtnDelete && HoverDelete)
        || (d->hwndItem == BtnOptions && HoverOpt)
        || (d->hwndItem == BtnExit && HoverExit)
        || (d->hwndItem == BtnFeedback && HoverFeedback);
    bool accent = (d->hwndItem == BtnClock && State == IDLE);
    bool isDel = (d->hwndItem == BtnDelete);
    bool isExit = (d->hwndItem == BtnExit);
    bool isOpt = (d->hwndItem == BtnOptions);
    const Theme& t = Themes[ActiveTheme];

    HBRUSH fill;
    if (isExit) fill = dis ? B.btnDis : (pressed ? B.closeBtnHover : (hover ? B.closeBtnHover : B.closeBtn));
    else if (isDel) fill = dis ? B.btnDis : (pressed ? B.closeBtnHover : (hover ? B.closeBtnHover : B.closeBtn));
    else if (isOpt) fill = dis ? B.btnDis : (pressed ? B.optBtnHover : (hover ? B.optBtnHover : B.optBtn));
    else fill = dis ? B.btnDis : (pressed ? (accent ? B.accentP : B.btnPress) : (hover ? (accent ? B.accentH : B.btnHover) : (accent ? B.accent : B.btn)));

    RECT r = d->rcItem;

    FillRect(d->hDC, &r, fill);

    HPEN pen = CreatePen(PS_SOLID, 1, t.border);
    HPEN oldP = (HPEN)SelectObject(d->hDC, pen);
    HBRUSH oldB = (HBRUSH)SelectObject(d->hDC, GetStockObject(NULL_BRUSH));
    RoundRect(d->hDC, r.left, r.top, r.right - 1, r.bottom - 2, 12, 12);
    SelectObject(d->hDC, oldB);
    SelectObject(d->hDC, oldP);
    DeleteObject(pen);

    wchar_t buf[64]; GetWindowTextW(d->hwndItem, buf, 63);
    HFONT oldF = (HFONT)SelectObject(d->hDC, FBtn);
    SetBkMode(d->hDC, TRANSPARENT);
    COLORREF tc = dis ? t.textDisabled : RGB(255, 255, 255);
    if (!accent && !isDel && !isExit && !isOpt && !dis) tc = t.textMain;
    SetTextColor(d->hDC, tc);
    RECT tr = r; tr.bottom -= 2;
    DrawTextW(d->hDC, buf, -1, &tr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(d->hDC, oldF);

    if (d->itemState & ODS_FOCUS) {
        RECT fr = r; InflateRect(&fr, -6, -4);
        DrawFocusRect(d->hDC, &fr);
    }
}

static RECT RectOptBtn = { 0,0,0,0 }, RectExitBtn = { 0,0,0,0 };
static bool HoverTitleOpt = false, HoverTitleExit = false;

static void GetCaptionRects(int winW) {
    int btnW = 46;
    RectClose = { winW - btnW,0,winW,TitleBarHeight };
    RectMin = { winW - btnW * 2,0,winW - btnW,TitleBarHeight };
}

static void DrawCaption(HDC hdc, int w) {
    const Theme& t = Themes[ActiveTheme];
    RECT tb = { 0,0,w,TitleBarHeight };
    FillRect(hdc, &tb, B.titleBar);

    HPEN sep = CreatePen(PS_SOLID, 1, t.border);
    HPEN oldP = (HPEN)SelectObject(hdc, sep);
    MoveToEx(hdc, 0, TitleBarHeight - 1, 0);LineTo(hdc, w, TitleBarHeight - 1);
    SelectObject(hdc, oldP);DeleteObject(sep);

    HBRUSH oldBr = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    HPEN cp = CreatePen(PS_SOLID, 2, t.titleText);
    HPEN oldcp = (HPEN)SelectObject(hdc, cp);
    Ellipse(hdc, 8, 8, 32, 32);
    HPEN hp2 = CreatePen(PS_SOLID, 2, t.titleText);
    SelectObject(hdc, hp2);DeleteObject(cp);
    MoveToEx(hdc, 20, 10, 0);LineTo(hdc, 20, 20);
    MoveToEx(hdc, 20, 20, 0);LineTo(hdc, 27, 25);
    SelectObject(hdc, oldcp);DeleteObject(hp2);
    SelectObject(hdc, oldBr);

    HFONT oldF = (HFONT)SelectObject(hdc, FTitle);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, t.titleText);
    RECT tt = { 38,0,w - 100,TitleBarHeight };
    DrawTextW(hdc, L"Time Tracker", -1, &tt, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    FillRect(hdc, &RectMin, HoverMin ? B.captionBtnHover : B.captionBtn);
    SetTextColor(hdc, RGB(220, 220, 220));
    SelectObject(hdc, FCaption);
    DrawTextW(hdc, L"\u2013", -1, &RectMin, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    FillRect(hdc, &RectClose, HoverClose ? B.closeBtnHover : B.closeBtn);
    SetTextColor(hdc, RGB(255, 255, 255));
    DrawTextW(hdc, L"\u00D7", -1, &RectClose, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdc, oldF);
}

static WNDPROC OldBtnProc;
LRESULT CALLBACK BtnProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    if (m == WM_MOUSEMOVE) {
        bool ch = false;
        if (h == BtnClock && !HoverClock) { HoverClock = true;ch = true; }
        else if (h == BtnBreak && !HoverBreak) { HoverBreak = true;ch = true; }
        else if (h == BtnCopy && !HoverCopy) { HoverCopy = true;ch = true; }
        else if (h == BtnDelete && !HoverDelete) { HoverDelete = true;ch = true; }
        else if (h == BtnOptions && !HoverOpt) { HoverOpt = true;ch = true; }
        else if (h == BtnExit && !HoverExit) { HoverExit = true;ch = true; }
        else if (h == BtnFeedback && !HoverFeedback) { HoverFeedback = true;ch = true; }
        if (ch) { InvalidateRect(h, NULL, FALSE);TRACKMOUSEEVENT tme = { sizeof(tme),TME_LEAVE,h,0 };TrackMouseEvent(&tme); }
    }
    else if (m == WM_MOUSELEAVE) {
        if (h == BtnClock)HoverClock = false;
        else if (h == BtnBreak)HoverBreak = false;
        else if (h == BtnCopy)HoverCopy = false;
        else if (h == BtnDelete)HoverDelete = false;
        else if (h == BtnOptions)HoverOpt = false;
        else if (h == BtnExit)HoverExit = false;
        else if (h == BtnFeedback)HoverFeedback = false;
        InvalidateRect(h, NULL, FALSE);
    }
    return CallWindowProcW(OldBtnProc, h, m, w, l);
}

static WNDPROC OldLogProc;
LRESULT CALLBACK LogProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    if (m == WM_ERASEBKGND) { RECT rc;GetClientRect(h, &rc);FillRect((HDC)w, &rc, B.surface);return 1; }
    return CallWindowProcW(OldLogProc, h, m, w, l);
}

struct DlgSt { int* sel;HWND list;HFONT font;HBRUSH bg; };
static const int DLG_LIST_ID = 100;

static INT_PTR CALLBACK ThemeDlgProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    DlgSt* d = (DlgSt*)GetWindowLongPtrW(h, GWLP_USERDATA);
    if (m == WM_INITDIALOG) {
        d = new DlgSt();
        SetWindowLongPtrW(h, GWLP_USERDATA, (LONG_PTR)d);
        d->sel = (int*)l;
        BOOL dark = TRUE;DwmSetWindowAttribute(h, 20, &dark, sizeof(dark));
        SetWindowTextW(h, L"Choose Theme");
        d->font = CreateFontW(15, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, L"Segoe UI");
        d->bg = CreateSolidBrush(Themes[ActiveTheme].bg);
        HWND lbl = CreateWindowExW(0, L"STATIC", L"Select a color theme:", WS_CHILD | WS_VISIBLE | SS_LEFT, 16, 12, 340, 22, h, 0, 0, 0);
        SendMessageW(lbl, WM_SETFONT, (WPARAM)d->font, 0);
        d->list = CreateWindowExW(WS_EX_CLIENTEDGE, L"LISTBOX", 0, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT, 16, 40, 340, 52 * ThemeCount, h, (HMENU)(INT_PTR)DLG_LIST_ID, 0, 0);
        SendMessageW(d->list, LB_SETITEMHEIGHT, 0, 52);
        for (int i = 0;i < ThemeCount;i++)SendMessageW(d->list, LB_ADDSTRING, 0, (LPARAM)Themes[i].name);
        SendMessageW(d->list, LB_SETCURSEL, *(d->sel), 0);
        int y = 48 + 52 * ThemeCount;
        HWND hOk = CreateWindowExW(0, L"BUTTON", L"Apply", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 196, y, 76, 28, h, (HMENU)(INT_PTR)IDOK, 0, 0);
        HWND hCan = CreateWindowExW(0, L"BUTTON", L"Cancel", WS_CHILD | WS_VISIBLE, 280, y, 76, 28, h, (HMENU)(INT_PTR)IDCANCEL, 0, 0);
        SendMessageW(hOk, WM_SETFONT, (WPARAM)d->font, 0);SendMessageW(hCan, WM_SETFONT, (WPARAM)d->font, 0);
        return 1;
    }
    if (m == WM_MEASUREITEM) { ((MEASUREITEMSTRUCT*)l)->itemHeight = 52;return TRUE; }
    if (m == WM_DRAWITEM && ((DRAWITEMSTRUCT*)l)->CtlID == DLG_LIST_ID) {
        DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*)l;if (dis->itemID == UINT(-1))return 0;
        const Theme& t = Themes[dis->itemID];bool sel = (dis->itemState & ODS_SELECTED) != 0;
        HBRUSH rb = CreateSolidBrush(sel ? t.btnAccent : t.bg);FillRect(dis->hDC, &dis->rcItem, rb);DeleteObject(rb);
        COLORREF sw[] = { t.bg,t.btnAccent,t.statusWork,t.statusBreak,t.dailyTotal };
        int x = dis->rcItem.left + 10;
        for (int i = 0;i < 5;i++) { RECT sr = { x,dis->rcItem.top + 13,x + 26,dis->rcItem.top + 39 };HBRUSH sb = CreateSolidBrush(sw[i]);FillRect(dis->hDC, &sr, sb);DeleteObject(sb);x += 30; }
        HFONT hf = CreateFontW(14, 0, 0, 0, sel ? FW_BOLD : FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        HFONT of = (HFONT)SelectObject(dis->hDC, hf);
        SetTextColor(dis->hDC, sel ? RGB(255, 255, 255) : t.textMain);SetBkMode(dis->hDC, TRANSPARENT);
        TextOutW(dis->hDC, dis->rcItem.left + 170, dis->rcItem.top + 18, t.name, (int)wcslen(t.name));
        SelectObject(dis->hDC, of);DeleteObject(hf);
        HPEN hp = CreatePen(PS_SOLID, 1, RGB(55, 55, 55));HPEN op = (HPEN)SelectObject(dis->hDC, hp);
        MoveToEx(dis->hDC, dis->rcItem.left, dis->rcItem.bottom - 1, 0);LineTo(dis->hDC, dis->rcItem.right, dis->rcItem.bottom - 1);
        SelectObject(dis->hDC, op);DeleteObject(hp);return 1;
    }
    if (m == WM_COMMAND) {
        if (LOWORD(w) == IDOK) { *(d->sel) = (int)SendMessageW(d->list, LB_GETCURSEL, 0, 0);EndDialog(h, IDOK); }
        else if (LOWORD(w) == IDCANCEL)EndDialog(h, IDCANCEL);
        return 1;
    }
    if (m == WM_CTLCOLORSTATIC) { SetBkMode((HDC)w, TRANSPARENT);SetTextColor((HDC)w, Themes[ActiveTheme].textMain);return(INT_PTR)d->bg; }
    if (m == WM_ERASEBKGND) { RECT rc;GetClientRect(h, &rc);FillRect((HDC)w, &rc, d->bg);return 1; }
    if (m == WM_DESTROY) { DeleteObject(d->font);DeleteObject(d->bg);delete d; }
    return 0;
}
static void ShowThemeDlg() {
    int sel = ActiveTheme;
    struct DlgBuf { DLGTEMPLATE t;WORD m, c, tit; };
    DlgBuf buf = {};buf.t.style = WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_MODALFRAME | DS_CENTER;
    buf.t.cx = 372;buf.t.cy = (short)((48 + 52 * ThemeCount + 44) / 2);
    HINSTANCE hi = (HINSTANCE)GetWindowLongPtrW(Wnd, GWLP_HINSTANCE);
    if (DialogBoxIndirectParamW(hi, (DLGTEMPLATE*)&buf, Wnd, ThemeDlgProc, (LPARAM)&sel) == IDOK && sel != ActiveTheme) {
        ActiveTheme = sel;B.Build(Themes[ActiveTheme]);InvalidateRect(Wnd, NULL, TRUE);UpdateWindow(Wnd);
    }
}

struct DelDlgState { HWND list;HFONT font;HBRUSH bg; };
static INT_PTR CALLBACK DelDlgProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    DelDlgState* d = (DelDlgState*)GetWindowLongPtrW(h, GWLP_USERDATA);
    if (m == WM_INITDIALOG) {
        d = new DelDlgState();SetWindowLongPtrW(h, GWLP_USERDATA, (LONG_PTR)d);
        BOOL dark = TRUE;DwmSetWindowAttribute(h, 20, &dark, sizeof(dark));
        SetWindowTextW(h, L"Delete Sessions");
        d->font = CreateFontW(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, L"Consolas");
        d->bg = CreateSolidBrush(Themes[ActiveTheme].bg);
        HFONT lf = CreateFontW(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, L"Segoe UI");
        HWND lbl = CreateWindowExW(0, L"STATIC", L"Select sessions to delete (Ctrl+click for multiple):", WS_CHILD | WS_VISIBLE | SS_LEFT, 12, 10, 560, 18, h, 0, 0, 0);
        SendMessageW(lbl, WM_SETFONT, (WPARAM)lf, 0);
        d->list = CreateWindowExW(WS_EX_CLIENTEDGE, L"LISTBOX", 0, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | LBS_EXTENDEDSEL | LBS_NOINTEGRALHEIGHT, 12, 34, 560, 300, h, (HMENU)(INT_PTR)200, 0, 0);
        SendMessageW(d->list, WM_SETFONT, (WPARAM)d->font, 0);
        for (int i = (int)AllSessions.size() - 1;i >= 0;i--)SendMessageW(d->list, LB_ADDSTRING, 0, (LPARAM)AllSessions[i].line.c_str());
        HWND hDel = CreateWindowExW(0, L"BUTTON", L"Delete", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 12, 342, 100, 28, h, (HMENU)(INT_PTR)IDOK, 0, 0);
        HWND hCan = CreateWindowExW(0, L"BUTTON", L"Cancel", WS_CHILD | WS_VISIBLE, 120, 342, 80, 28, h, (HMENU)(INT_PTR)IDCANCEL, 0, 0);
        SendMessageW(hDel, WM_SETFONT, (WPARAM)lf, 0);SendMessageW(hCan, WM_SETFONT, (WPARAM)lf, 0);
        return 1;
    }
    if (m == WM_COMMAND) {
        if (LOWORD(w) == IDOK) {
            int cnt = (int)SendMessageW(d->list, LB_GETSELCOUNT, 0, 0);
            if (cnt <= 0) { EndDialog(h, IDCANCEL);return 1; }
            std::vector<int> sel(cnt);
            SendMessageW(d->list, LB_GETSELITEMS, cnt, (LPARAM)sel.data());
            std::wostringstream msg;msg << L"Delete " << cnt << L" session(s)?";
            if (MessageBoxW(h, msg.str().c_str(), L"Confirm", MB_YESNO | MB_ICONWARNING) != IDYES)return 1;
            std::vector<int> toRemove;
            for (int idx : sel)toRemove.push_back((int)AllSessions.size() - 1 - idx);
            std::sort(toRemove.begin(), toRemove.end(), std::greater<int>());
            for (int idx : toRemove)AllSessions.erase(AllSessions.begin() + idx);
            SaveAllSessions();EndDialog(h, IDOK);
        }
        else if (LOWORD(w) == IDCANCEL)EndDialog(h, IDCANCEL);
        return 1;
    }
    if (m == WM_CTLCOLORSTATIC) { SetBkMode((HDC)w, TRANSPARENT);SetTextColor((HDC)w, Themes[ActiveTheme].textMain);return(INT_PTR)d->bg; }
    if (m == WM_CTLCOLORLISTBOX) { SetTextColor((HDC)w, Themes[ActiveTheme].textMain);SetBkColor((HDC)w, Themes[ActiveTheme].surface);return(LRESULT)B.surface; }
    if (m == WM_ERASEBKGND) { RECT rc;GetClientRect(h, &rc);FillRect((HDC)w, &rc, d->bg);return 1; }
    if (m == WM_DESTROY) { DeleteObject(d->font);DeleteObject(d->bg);delete d; }
    return 0;
}
static void ShowDeleteDlg() {
    struct DlgBuf { DLGTEMPLATE t;WORD m, c, tit; };
    DlgBuf buf = {};buf.t.style = WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_MODALFRAME | DS_CENTER;
    buf.t.cx = 300;buf.t.cy = 220;
    HINSTANCE hi = (HINSTANCE)GetWindowLongPtrW(Wnd, GWLP_HINSTANCE);
    if (DialogBoxIndirectParamW(hi, (DLGTEMPLATE*)&buf, Wnd, DelDlgProc, 0) == IDOK) { UpdateLog();InvalidateRect(Wnd, NULL, TRUE); }
}

static void ShowOptionsDlg() {
    int choice = MessageBoxW(Wnd,
        L"1. Change Theme\n\nClick Yes to change theme, No to cancel.",
        L"Options", MB_YESNO | MB_ICONINFORMATION);
    if (choice == IDYES)ShowThemeDlg();
}

struct SpinAnim {
    bool active = false;
    bool reversing = false;
    HWND ghost = NULL, main = NULL;
    Gdiplus::Bitmap* shot = NULL;
    HBITMAP shotDib = NULL;
    RECT startRect = {};
    POINT target = {};
    ULONGLONG t0 = 0;
    DWORD duration = 350;
};
static SpinAnim Anim;

static const wchar_t GhostClass[] = L"TimeTrackerGhost";

static void CaptureWindowShot(HWND hwnd) {
    RECT rc; GetWindowRect(hwnd, &rc);
    int w = rc.right - rc.left, h = rc.bottom - rc.top;

    BITMAPINFO bi = {};
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = w;
    bi.bmiHeader.biHeight = -h;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;

    void* bits = nullptr;
    HDC wndDC = GetDC(hwnd);
    HBITMAP dib = CreateDIBSection(wndDC, &bi, DIB_RGB_COLORS, &bits, NULL, 0);
    if (dib && bits) {
        HDC mem = CreateCompatibleDC(wndDC);
        HBITMAP old = (HBITMAP)SelectObject(mem, dib);
        PrintWindow(hwnd, mem, PW_RENDERFULLCONTENT);
        SelectObject(mem, old);
        DeleteDC(mem);
        delete Anim.shot;
        if (Anim.shotDib) DeleteObject(Anim.shotDib);
        Anim.shotDib = dib;
        Anim.shot = new Gdiplus::Bitmap(w, h, w * 4, PixelFormat32bppRGB, (BYTE*)bits);
    }
    else {
        if (dib) DeleteObject(dib);
    }
    ReleaseDC(hwnd, wndDC);
}

static void EndSpinAnim() {
    KillTimer(Anim.ghost, 2);
    ShowWindow(Anim.main, SW_MINIMIZE);
    DestroyWindow(Anim.ghost);
    Anim.ghost = NULL;
    Anim.active = false;
}

static void EndRestoreAnim() {
    KillTimer(Anim.ghost, 2);
    ShowWindow(Anim.main, SW_SHOWNORMAL);
    SetForegroundWindow(Anim.main);
    DestroyWindow(Anim.ghost);
    Anim.ghost = NULL;
    delete Anim.shot; Anim.shot = NULL;
    if (Anim.shotDib) { DeleteObject(Anim.shotDib); Anim.shotDib = NULL; }
    Anim.active = false;
}

static LRESULT CALLBACK GhostProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    if (m == WM_TIMER && w == 2) {
        double t = (double)(GetTickCount64() - Anim.t0) / Anim.duration;
        if (t >= 1.0) {
            if (Anim.reversing) EndRestoreAnim();
            else EndSpinAnim();
            return 0;
        }

        double e = t * t * (3.0 - 2.0 * t);
        double scale, angle;
        if (Anim.reversing) {
            scale = e;
            angle = 360.0 * (1.0 - e);
        }
        else {
            scale = 1.0 - e;
            angle = 360.0 * e;
        }

        int sw = Anim.startRect.right - Anim.startRect.left;
        int sh = Anim.startRect.bottom - Anim.startRect.top;

        double cx, cy;
        if (Anim.reversing) {
            cx = Anim.target.x * (1 - e) + (Anim.startRect.left + sw / 2.0) * e;
            cy = Anim.target.y * (1 - e) + (Anim.startRect.top + sh / 2.0) * e;
        }
        else {
            cx = (Anim.startRect.left + sw / 2.0) * (1 - e) + Anim.target.x * e;
            cy = (Anim.startRect.top + sh / 2.0) * (1 - e) + Anim.target.y * e;
        }

        int side = (int)(sqrt((double)sw * sw + (double)sh * sh) * scale) + 4;
        if (side < 2) side = 2;

        Gdiplus::Bitmap frame(side, side, PixelFormat32bppPARGB);
        Gdiplus::Graphics g(&frame);
        g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBilinear);
        g.TranslateTransform((Gdiplus::REAL)(side / 2.0), (Gdiplus::REAL)(side / 2.0));
        g.RotateTransform((Gdiplus::REAL)angle);
        g.ScaleTransform((Gdiplus::REAL)scale, (Gdiplus::REAL)scale);
        g.DrawImage(Anim.shot,
            (Gdiplus::REAL)(-sw / 2.0), (Gdiplus::REAL)(-sh / 2.0),
            (Gdiplus::REAL)sw, (Gdiplus::REAL)sh);

        HBITMAP hb = NULL;
        frame.GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &hb);
        if (!hb) return 0;

        HDC screen = GetDC(NULL);
        HDC memDC = CreateCompatibleDC(screen);
        HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, hb);

        POINT pt = { (int)cx - side / 2, (int)cy - side / 2 };
        POINT src = { 0, 0 };
        SIZE  sz = { side, side };
        BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
        UpdateLayeredWindow(h, screen, &pt, &sz, memDC, &src, 0, &bf, ULW_ALPHA);

        SelectObject(memDC, oldBmp);
        DeleteDC(memDC);
        ReleaseDC(NULL, screen);
        DeleteObject(hb);
        return 0;
    }
    return DefWindowProcW(h, m, w, l);
}

static void PlayMinimizeSpin(HWND hwnd) {
    ANIMATIONINFO ai = { sizeof(ANIMATIONINFO) };
    SystemParametersInfoW(SPI_GETANIMATION, sizeof(ai), &ai, 0);
    if (ai.iMinAnimate == 0) { ShowWindow(hwnd, SW_MINIMIZE); return; }

    CaptureWindowShot(hwnd);
    if (!Anim.shot) { ShowWindow(hwnd, SW_MINIMIZE); return; }

    GetWindowRect(hwnd, &Anim.startRect);
    Anim.main = hwnd;
    Anim.reversing = false;

    HMONITOR mon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi = { sizeof(mi) };
    GetMonitorInfoW(mon, &mi);
    Anim.target.x = (Anim.startRect.left + Anim.startRect.right) / 2;
    Anim.target.y = mi.rcWork.bottom + 20;

    static bool registered = false;
    if (!registered) {
        WNDCLASSEXW gc = { sizeof(gc) };
        gc.lpfnWndProc = GhostProc;
        gc.hInstance = (HINSTANCE)GetWindowLongPtrW(hwnd, GWLP_HINSTANCE);
        gc.lpszClassName = GhostClass;
        RegisterClassExW(&gc);
        registered = true;
    }

    Anim.ghost = CreateWindowExW(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TRANSPARENT,
        GhostClass, L"", WS_POPUP,
        Anim.startRect.left, Anim.startRect.top,
        Anim.startRect.right - Anim.startRect.left,
        Anim.startRect.bottom - Anim.startRect.top,
        NULL, NULL, (HINSTANCE)GetWindowLongPtrW(hwnd, GWLP_HINSTANCE), 0);

    ShowWindow(hwnd, SW_HIDE);
    ShowWindow(Anim.ghost, SW_SHOWNOACTIVATE);
    Anim.active = true;
    Anim.t0 = GetTickCount64();
    SetTimer(Anim.ghost, 2, 10, NULL);
}

static void PlayRestoreSpin(HWND hwnd) {
    if (!Anim.shot || Anim.active) { ShowWindow(hwnd, SW_RESTORE); return; }

    Anim.reversing = true;
    Anim.main = hwnd;
    Anim.t0 = GetTickCount64();

    Anim.ghost = CreateWindowExW(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TRANSPARENT,
        GhostClass, L"", WS_POPUP,
        Anim.startRect.left, Anim.startRect.top,
        Anim.startRect.right - Anim.startRect.left,
        Anim.startRect.bottom - Anim.startRect.top,
        NULL, NULL, (HINSTANCE)GetWindowLongPtrW(hwnd, GWLP_HINSTANCE), 0);

    ShowWindow(Anim.ghost, SW_SHOWNOACTIVATE);
    Anim.active = true;
    SetTimer(Anim.ghost, 2, 10, NULL);
}


static const wchar_t FlashClass[] = L"TimeTrackerFlash";

static IStream* ResourceToStream(UINT id, const wchar_t* type) {
    HMODULE hMod = GetModuleHandleW(NULL);
    HRSRC hrsrc = FindResourceW(hMod, MAKEINTRESOURCEW(id), type);
    if (!hrsrc) return NULL;
    HGLOBAL hres = LoadResource(hMod, hrsrc);
    if (!hres) return NULL;
    const void* data = LockResource(hres);
    DWORD size = SizeofResource(hMod, hrsrc);
    if (!data || !size) return NULL;
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);
    if (!hMem) return NULL;
    void* pMem = GlobalLock(hMem);
    if (!pMem) { GlobalFree(hMem); return NULL; }
    memcpy(pMem, data, size);
    GlobalUnlock(hMem);
    IStream* stream = NULL;
    if (FAILED(CreateStreamOnHGlobal(hMem, TRUE, &stream))) { GlobalFree(hMem); return NULL; }
    return stream;
}

struct FlashState {
    UINT resId = IMG_FLASH;
    Gdiplus::Bitmap* img = NULL;
    IStream* strm = NULL;
    HDC memDC = NULL;
    HBITMAP hbmp = NULL, hbmpOld = NULL;
    BYTE* bits = NULL;
    int vw = 0, vh = 0;
    BYTE alpha = 255;
    int hold = 13;
};

static LRESULT CALLBACK FlashProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    FlashState* s = (FlashState*)GetWindowLongPtrW(h, GWLP_USERDATA);
    switch (m) {
    case WM_NCCREATE:
        s = new FlashState();
        s->resId = (UINT)(UINT_PTR)((CREATESTRUCTW*)l)->lpCreateParams;
        SetWindowLongPtrW(h, GWLP_USERDATA, (LONG_PTR)s);
        return DefWindowProcW(h, m, w, l);
    case WM_CREATE:
        s->strm = ResourceToStream(s->resId, (const wchar_t*)RT_RCDATA);
        if (s->strm) s->img = Gdiplus::Bitmap::FromStream(s->strm);
        if (!s->img) { DestroyWindow(h); return -1; }
        s->vw = GetSystemMetrics(SM_CXVIRTUALSCREEN);
        s->vh = GetSystemMetrics(SM_CYVIRTUALSCREEN);
        s->alpha = 255;
        s->hold = 13;
        {
            BITMAPINFO bi = {};
            bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bi.bmiHeader.biWidth = s->vw;
            bi.bmiHeader.biHeight = -s->vh;
            bi.bmiHeader.biPlanes = 1;
            bi.bmiHeader.biBitCount = 32;
            bi.bmiHeader.biCompression = BI_RGB;
            s->memDC = CreateCompatibleDC(NULL);
            s->hbmp = CreateDIBSection(NULL, &bi, DIB_RGB_COLORS, (void**)&s->bits, NULL, 0);
            if (!s->hbmp) { DestroyWindow(h); return -1; }
            s->hbmpOld = (HBITMAP)SelectObject(s->memDC, s->hbmp);
            Gdiplus::Bitmap canvas(s->vw, s->vh, s->vw * 4, PixelFormat32bppPARGB, s->bits);
            Gdiplus::Graphics g(&canvas);
            g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBilinear);
            memset(s->bits, 0, (size_t)s->vw * s->vh * 4);
            g.DrawImage(s->img, 0, 0, s->vw, s->vh);
        }
        {
            POINT ptSrc = { 0, 0 };
            SIZE size = { s->vw, s->vh };
            POINT ptDst = { GetSystemMetrics(SM_XVIRTUALSCREEN), GetSystemMetrics(SM_YVIRTUALSCREEN) };
            BLENDFUNCTION bf = { AC_SRC_OVER, 0, s->alpha, AC_SRC_ALPHA };
            UpdateLayeredWindow(h, NULL, &ptDst, &size, s->memDC, &ptSrc, 0, &bf, ULW_ALPHA);
        }
        ShowWindow(h, SW_SHOWNOACTIVATE);
        SetTimer(h, 3, 15, NULL);
        return 0;
    case WM_TIMER:
        if (w != 3) break;
        if (s->hold > 0) s->hold--;
        else s->alpha = (s->alpha > 12) ? (BYTE)(s->alpha - 12) : 0;
        {
            POINT ptSrc = { 0, 0 };
            SIZE size = { s->vw, s->vh };
            POINT ptDst = { GetSystemMetrics(SM_XVIRTUALSCREEN), GetSystemMetrics(SM_YVIRTUALSCREEN) };
            BLENDFUNCTION bf = { AC_SRC_OVER, 0, s->alpha, AC_SRC_ALPHA };
            UpdateLayeredWindow(h, NULL, &ptDst, &size, s->memDC, &ptSrc, 0, &bf, ULW_ALPHA);
        }
        if (s->alpha == 0) DestroyWindow(h);
        return 0;
    case WM_DESTROY:
        KillTimer(h, 3);
        if (s) {
            if (s->hbmpOld) SelectObject(s->memDC, s->hbmpOld);
            if (s->hbmp) DeleteObject(s->hbmp);
            if (s->memDC) DeleteDC(s->memDC);
            delete s->img;
            if (s->strm) s->strm->Release();
            delete s;
            SetWindowLongPtrW(h, GWLP_USERDATA, 0);
        }
        return 0;
    }
    return DefWindowProcW(h, m, w, l);
}

static void StartFlashbang(HWND owner, UINT resId) {
    static bool registered = false;
    if (!registered) {
        WNDCLASSEXW fc = { sizeof(fc) };
        fc.lpfnWndProc = FlashProc;
        fc.hInstance = (HINSTANCE)GetWindowLongPtrW(owner, GWLP_HINSTANCE);
        fc.lpszClassName = FlashClass;
        RegisterClassExW(&fc);
        registered = true;
    }
    int vx = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int vy = GetSystemMetrics(SM_YVIRTUALSCREEN);
    int vw = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int vh = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    CreateWindowExW(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
        FlashClass, L"", WS_POPUP,
        vx, vy, vw, vh, NULL, NULL,
        (HINSTANCE)GetWindowLongPtrW(owner, GWLP_HINSTANCE), (LPVOID)(UINT_PTR)resId);
}

static LRESULT CALLBACK MainWndProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {

    case WM_NCHITTEST: {
        LRESULT hit = DefWindowProcW(h, m, w, l);
        if (hit == HTCLIENT) {
            POINT pt;pt.x = GET_X_LPARAM(l);pt.y = GET_Y_LPARAM(l);ScreenToClient(h, &pt);
            if (PtInRect(&RectClose, pt))return HTCLOSE;
            if (PtInRect(&RectMin, pt))return HTMINBUTTON;
            if (pt.y < TitleBarHeight)return HTCAPTION;
        }
        return hit;
    }
    case WM_NCMOUSEMOVE: {
        if (!TrackingMouse) { TRACKMOUSEEVENT tme = { sizeof(tme),TME_LEAVE,h,0 };TrackMouseEvent(&tme);TrackingMouse = true; }
        POINT pt;pt.x = GET_X_LPARAM(l);pt.y = GET_Y_LPARAM(l);ScreenToClient(h, &pt);
        bool om = HoverMin, oc = HoverClose;
        HoverMin = PtInRect(&RectMin, pt) != 0;
        HoverClose = PtInRect(&RectClose, pt) != 0;
        if (HoverMin != om || HoverClose != oc)InvalidateRect(h, NULL, FALSE);
        break;
    }
    case WM_NCMOUSELEAVE: { HoverMin = HoverClose = false;TrackingMouse = false;InvalidateRect(h, NULL, FALSE);break; }
    case WM_NCLBUTTONDOWN: {
        if (w == HTCLOSE) { PostMessageW(h, WM_CLOSE, 0, 0);return 0; }
        if (w == HTMINBUTTON) { PlayMinimizeSpin(h);return 0; }
        break;
    }
    case WM_SYSCOMMAND:
        if ((w & 0xFFF0) == SC_MINIMIZE) {
            PlayMinimizeSpin(h);
            return 0;
        }
        if ((w & 0xFFF0) == SC_RESTORE && IsIconic(h)) {
            PlayRestoreSpin(h);
            return 0;
        }
        break;
    case WM_CREATE: {
        Wnd = h;
        BOOL dark = TRUE;
        DwmSetWindowAttribute(h, 20, &dark, sizeof(dark));
        DwmSetWindowAttribute(h, 22, &dark, sizeof(dark));
        BOOL noTransitions = TRUE;
        DwmSetWindowAttribute(h, DWMWA_TRANSITIONS_FORCEDISABLED, &noTransitions, sizeof(noTransitions));

        FTimer = CreateFontW(64, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"Segoe UI");
        FStatus = CreateFontW(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, L"Segoe UI");
        FBtn = CreateFontW(15, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"Segoe UI");
        FLog = CreateFontW(13, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, L"Consolas");
        FBold = CreateFontW(17, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"Segoe UI");
        FHead = CreateFontW(11, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"Segoe UI");
        FTitle = CreateFontW(14, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"Segoe UI");
        FCaption = CreateFontW(18, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, L"Segoe UI");

        int Y = TitleBarHeight;

        LblTime = CreateWindowExW(0, L"STATIC", L"00:00:00", WS_CHILD | WS_VISIBLE | SS_CENTER, 0, Y + 20, 480, 68, h, 0, 0, 0);
        LblStatus = CreateWindowExW(0, L"STATIC", L"Idle", WS_CHILD | WS_VISIBLE | SS_CENTER, 0, Y + 96, 480, 22, h, 0, 0, 0);
        BtnClock = CreateWindowExW(0, L"BUTTON", L"Clock In", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 42, Y + 128, 190, 46, h, (HMENU)(INT_PTR)101, 0, 0);
        BtnBreak = CreateWindowExW(0, L"BUTTON", L"Break", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 248, Y + 128, 190, 46, h, (HMENU)(INT_PTR)102, 0, 0);
        BtnCopy = CreateWindowExW(0, L"BUTTON", L"Copy Time", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 42, Y + 186, 190, 36, h, (HMENU)(INT_PTR)103, 0, 0);
        BtnDelete = CreateWindowExW(0, L"BUTTON", L"Delete Sessions", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 248, Y + 186, 190, 36, h, (HMENU)(INT_PTR)104, 0, 0);
        BtnOptions = CreateWindowExW(0, L"BUTTON", L"Options", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 42, Y + 234, 130, 30, h, (HMENU)(INT_PTR)105, 0, 0);
        BtnExit = CreateWindowExW(0, L"BUTTON", L"Exit", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 308, Y + 234, 130, 30, h, (HMENU)(INT_PTR)106, 0, 0);
        BtnFeedback = CreateWindowExW(0, L"BUTTON", L"Feedback", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 182, Y + 234, 116, 30, h, (HMENU)(INT_PTR)107, 0, 0);
        LblDaily = CreateWindowExW(0, L"STATIC", L"Today's Total:  00:00:00", WS_CHILD | WS_VISIBLE | SS_CENTER, 0, Y + 276, 480, 22, h, 0, 0, 0);
        LogHeader = CreateWindowExW(0, L"STATIC", L"SESSION LOG", WS_CHILD | WS_VISIBLE | SS_CENTER, 20, Y + 302, 440, 18, h, 0, 0, 0);
        CreateWindowExW(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ, 20, Y + 323, 440, 2, h, 0, 0, 0);
        LogBox = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL, 20, Y + 330, 440, 148, h, 0, 0, 0);

        SendMessageW(LblTime, WM_SETFONT, (WPARAM)FTimer, 0);
        SendMessageW(LblStatus, WM_SETFONT, (WPARAM)FStatus, 0);
        SendMessageW(BtnClock, WM_SETFONT, (WPARAM)FBtn, 0);
        SendMessageW(BtnBreak, WM_SETFONT, (WPARAM)FBtn, 0);
        SendMessageW(BtnCopy, WM_SETFONT, (WPARAM)FBtn, 0);
        SendMessageW(BtnDelete, WM_SETFONT, (WPARAM)FBtn, 0);
        SendMessageW(BtnOptions, WM_SETFONT, (WPARAM)FBtn, 0);
        SendMessageW(BtnExit, WM_SETFONT, (WPARAM)FBtn, 0);
        SendMessageW(BtnFeedback, WM_SETFONT, (WPARAM)FBtn, 0);
        SendMessageW(LblDaily, WM_SETFONT, (WPARAM)FBold, 0);
        SendMessageW(LogHeader, WM_SETFONT, (WPARAM)FHead, 0);
        SendMessageW(LogBox, WM_SETFONT, (WPARAM)FLog, 0);

        EnableWindow(BtnBreak, FALSE);
        EnableWindow(BtnCopy, FALSE);

        OldBtnProc = (WNDPROC)SetWindowLongPtrW(BtnClock, GWLP_WNDPROC, (LONG_PTR)BtnProc);
        SetWindowLongPtrW(BtnBreak, GWLP_WNDPROC, (LONG_PTR)BtnProc);
        SetWindowLongPtrW(BtnCopy, GWLP_WNDPROC, (LONG_PTR)BtnProc);
        SetWindowLongPtrW(BtnDelete, GWLP_WNDPROC, (LONG_PTR)BtnProc);
        SetWindowLongPtrW(BtnOptions, GWLP_WNDPROC, (LONG_PTR)BtnProc);
        SetWindowLongPtrW(BtnExit, GWLP_WNDPROC, (LONG_PTR)BtnProc);
        SetWindowLongPtrW(BtnFeedback, GWLP_WNDPROC, (LONG_PTR)BtnProc);
        OldLogProc = (WNDPROC)SetWindowLongPtrW(LogBox, GWLP_WNDPROC, (LONG_PTR)LogProc);

        LoadAllSessions();
        UpdateLog();
        break;
    }
    case WM_CLOSE:
        StartFlashbang(h, IMG_FLASH_CLOSE);
        SetTimer(h, 4, 600, NULL);
        return 0;
    case WM_COMMAND:
        if (LOWORD(w) == 101) { if (State == IDLE)DoClockIn();else if (MessageBoxW(h, L"Clock out and save?", L"Clock Out", MB_YESNO | MB_ICONQUESTION) == IDYES)DoClockOut(); }
        else if (LOWORD(w) == 102) { if (State == RUNNING)DoBreak();else if (State == ON_BREAK)DoResume(); }
        else if (LOWORD(w) == 103)CopyToClipboard();
        else if (LOWORD(w) == 104)ShowDeleteDlg();
        else if (LOWORD(w) == 105)ShowOptionsDlg();
        else if (LOWORD(w) == 106) { if (MessageBoxW(h, L"Exit Time Tracker?", L"Exit", MB_YESNO | MB_ICONQUESTION) == IDYES)PostMessageW(h, WM_CLOSE, 0, 0); }
        else if (LOWORD(w) == 107) { PlaySoundW(MAKEINTRESOURCEW(SND_FEEDBACK), GetModuleHandleW(NULL), SND_RESOURCE | SND_ASYNC | SND_NODEFAULT); StartFlashbang(h, IMG_FLASH); }
        break;
    case WM_TIMER:
        if (w == 4) { KillTimer(h, 4);DestroyWindow(h);return 0; }
        UpdateTime();break;
    case WM_DRAWITEM:DrawBtn((DRAWITEMSTRUCT*)l);return 1;
    case WM_ERASEBKGND: { RECT rc;GetClientRect(h, &rc);FillRect((HDC)w, &rc, B.bg);return 1; }
    case WM_PAINT: {
        PAINTSTRUCT ps;BeginPaint(h, &ps);
        RECT rc;GetClientRect(h, &rc);
        GetCaptionRects(rc.right);
        DrawCaption(ps.hdc, rc.right);
        int Y = TitleBarHeight;
        HPEN pen = CreatePen(PS_SOLID, 1, Themes[ActiveTheme].border);
        HPEN old = (HPEN)SelectObject(ps.hdc, pen);
        MoveToEx(ps.hdc, 20, Y + 122, 0);LineTo(ps.hdc, 460, Y + 122);
        MoveToEx(ps.hdc, 20, Y + 228, 0);LineTo(ps.hdc, 460, Y + 228);
        SelectObject(ps.hdc, old);DeleteObject(pen);
        EndPaint(h, &ps);break;
    }
    case WM_CTLCOLORSTATIC: {
        HDC hdc = (HDC)w;HWND ctrl = (HWND)l;
        const Theme& t = Themes[ActiveTheme];
        SetBkMode(hdc, TRANSPARENT);
        if (ctrl == LblTime)SetTextColor(hdc, t.textMain);
        else if (ctrl == LblDaily)SetTextColor(hdc, t.dailyTotal);
        else if (ctrl == LblStatus) {
            COLORREF c = t.statusIdle;
            if (State == RUNNING)c = t.statusWork;
            if (State == ON_BREAK)c = t.statusBreak;
            SetTextColor(hdc, c);
        }
        else if (ctrl == LogBox) { SetTextColor(hdc, t.textSub);SetBkColor(hdc, t.surface);return(LRESULT)B.surface; }
        else SetTextColor(hdc, t.textSub);
        return(LRESULT)B.bg;
    }
    case WM_SIZE: { RECT rc;GetClientRect(h, &rc);GetCaptionRects(rc.right);InvalidateRect(h, NULL, TRUE);break; }
    case WM_DESTROY:
        KillTimer(h, 1);B.Destroy();
        DeleteObject(FTimer);DeleteObject(FStatus);DeleteObject(FBtn);DeleteObject(FLog);
        DeleteObject(FBold);DeleteObject(FHead);DeleteObject(FTitle);DeleteObject(FCaption);
        PostQuitMessage(0);break;
    }
    return DefWindowProcW(h, m, w, l);
}

int WINAPI WinMain(
    _In_ HINSTANCE hInst,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    Gdiplus::GdiplusStartupInput gsi;
    Gdiplus::GdiplusStartup(&GdipToken, &gsi, NULL);

    B.Build(Themes[0]);
    WNDCLASSEXW wc = { sizeof(wc) };
    wc.lpfnWndProc = MainWndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"TimeTracker";
    wc.hCursor = LoadCursorW(0, IDC_ARROW);
    wc.hIcon = LoadIconW((HINSTANCE)GetModuleHandleW(NULL), MAKEINTRESOURCE(MAINICON));
    wc.style = CS_DBLCLKS;
    RegisterClassExW(&wc);

    HWND win = CreateWindowExW(
        WS_EX_COMPOSITED | WS_EX_APPWINDOW,
        L"TimeTracker", L"Time Tracker",
        WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 480, 520 + TitleBarHeight,
        0, 0, hInst, 0);

    ShowWindow(win, SW_SHOWNORMAL);
    UpdateWindow(win);

    MSG msg;
    while (GetMessageW(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    Gdiplus::GdiplusShutdown(GdipToken);
    return 0;
}