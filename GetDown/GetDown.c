/// Program: GetDown - Minimize all open Widnows
/// Author: Axel Persinger
/// License: MIT License

#include "GetDown.h"


void ErrorExit(LPTSTR lpszFunction)
{
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL
    );

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);

    _tprintf((TCHAR*)lpDisplayBuf);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw);
}


BOOL IsAltTabWindow(HWND hwnd)
{
    BOOL bWindowCloaked;
    TITLEBARINFO ti;
    HWND hwndTry, hwndWalk = NULL;

    if (!IsWindowVisible(hwnd))
        return FALSE;

    hwndTry = GetAncestor(hwnd, GA_ROOTOWNER);
    while (hwndTry != hwndWalk)
    {
        hwndWalk = hwndTry;
        hwndTry = GetLastActivePopup(hwndWalk);
        if (IsWindowVisible(hwndTry))
            break;
    }
    if (hwndWalk != hwnd)
        return FALSE;

    // the following removes some task tray programs and "Program Manager"
    ti.cbSize = sizeof(ti);
    GetTitleBarInfo(hwnd, &ti);
    if (ti.rgstate[0] & STATE_SYSTEM_INVISIBLE)
        return FALSE;

    // Tool windows should not be displayed either, these do not appear in the
    // task bar.
    if (GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW)
        return FALSE;

    // Check if the Window is Cloaked
    DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &bWindowCloaked, sizeof(BOOL));
    if (bWindowCloaked) 
        return FALSE;

    return TRUE;
}

static BOOL CALLBACK enumWindowCB(HWND hWindow, LPARAM lparam)
{
    if (!IsAltTabWindow(hWindow)) { // Window isn't visible, skip and move on
        return TRUE;
    }

    int WindowsTextLength;
    TCHAR* WindowsText;

    WindowsTextLength = GetWindowTextLength(hWindow);
    WindowsText = (TCHAR*) malloc((WindowsTextLength+1) * sizeof(TCHAR));
    GetWindowText(hWindow, WindowsText, WindowsTextLength+1);
    _tprintf(_T("Window Title: %s\n"), WindowsText);

    ShowWindow(hWindow, SW_MINIMIZE);
    // Always return TRUE to get the next window
    return TRUE;
}

int main(int argc, const char* argv[])
{
    if (!EnumWindows(enumWindowCB, (LPARAM)NULL)) {
        ErrorExit((LPTSTR)_T("EnumWindows"));
    }

    return 0;
}
