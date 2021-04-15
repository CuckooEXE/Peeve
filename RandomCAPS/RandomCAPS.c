/// Program: RandomCAPS - Send a Virtual Keystroke for the CAPSLOCK button
/// Author: Axel Persinger
/// License: MIT License

#include "RandomCAPS.h"


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


int main(int argc, const char* argv[])
{
    // https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-input -> https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-keybdinput
    INPUT stInput;
    short sCapsLockState;

    // Check if CAPSLOCK is already on. Don't do anything, but it's nice to show how to check
    sCapsLockState = GetKeyState(VK_CAPITAL);
    if ((sCapsLockState & 0x0001) != 0) {
        _tprintf(_T("CAPSLOCK is ON... Turning OFF.\n"));
    }
    else {
        _tprintf(_T("CAPSLOCK is OFF... Turning ON.\n"));
    }

    // Build Input structure, say it's type of Keyboard, specifcy the VirtualKey is CAPSLOCK
    stInput.type = INPUT_KEYBOARD;
    stInput.ki.wScan = 0x0;
    stInput.ki.time = 0x0;
    stInput.ki.wVk = VK_CAPITAL; // https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    stInput.ki.dwExtraInfo = 0;

    // Keydown
    stInput.ki.dwFlags = KEYEVENTF_KEYDOWN;
    if (SendInput(1, &stInput, sizeof(INPUT)) != 1) { 
        ErrorExit((LPTSTR)_T("SendInput"));
    }

    // Keyup
    stInput.ki.dwFlags = KEYEVENTF_KEYUP;
    if (SendInput(1, &stInput, sizeof(INPUT)) != 1) {
        ErrorExit((LPTSTR)_T("SendInput"));
    }

    return 0;
}
