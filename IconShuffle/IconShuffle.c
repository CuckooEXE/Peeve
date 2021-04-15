/// Program: IconShuffle - Shuffle Desktop icons randomly
/// Author: Axel Persinger
/// License: MIT License
#include "IconShuffle.h"


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


HWND GetDesktopHandle()
{
    // https://stackoverflow.com/a/5691808/10280970
    HWND hShellWnd = GetShellWindow();
    if (hShellWnd == NULL) {
        ErrorExit((LPTSTR)_T("hShellWnd"));
    }
    HWND hDefView = FindWindowEx(hShellWnd, NULL, _T("SHELLDLL_DefView"), NULL);
    if (hDefView == NULL) {
        ErrorExit((LPTSTR)_T("FindWindowEx"));
    }
    HWND folderView = FindWindowEx(hDefView, NULL, _T("SysListView32"), NULL);
    if (folderView == NULL) {
        ErrorExit((LPTSTR)_T("FindWindowEx"));
    }

    return folderView;
}


void Shuffle(POINT* allCoords, int nCoords) {
    POINT tmpCoord;
    int randIdx;

    // Seed random
    srand(GetTickCount());

    for (int i = nCoords - 1; i > 0; i--) {
        randIdx = rand() % (i + 1);

        // Swap i and randIdx
        memcpy(&tmpCoord, &allCoords[i], sizeof(POINT));
        memcpy(&allCoords[i], &allCoords[randIdx], sizeof(POINT));
        memcpy(&allCoords[randIdx], &tmpCoord, sizeof(POINT));
    }
}



int main(int argc, const char* argv[])
{
    int iCode;
    int nIcons;
    unsigned long pidDesktop;
    bool bSuccess;
    TCHAR iconName[MAX_PATH];
    TCHAR* _iconName;
    HANDLE hProcDesktop;
    HWND hwDesktop;
    LVITEM lvi;
    LVITEM* _lvi;
    POINT iconCoords;
    POINT* _iconCoords;
    POINT* allCoords;
    LPARAM lparamCoords;


    // Get Desktop Information
    hwDesktop = GetDesktopHandle();
    GetWindowThreadProcessId(hwDesktop, &pidDesktop);
    hProcDesktop = OpenProcess(
        PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION,
        FALSE,
        pidDesktop
    );
    if (hProcDesktop == NULL) {
        ErrorExit((LPTSTR)_T("OpenProcess"));
    }

    // Get number of icons
    nIcons = (int)SendMessage(hwDesktop, LVM_GETITEMCOUNT, 0, 0);
    // _tprintf(_T("# of Icons: %d\n"), nIcons);

    allCoords = (POINT*)malloc(nIcons * sizeof(POINT));
    if (allCoords == NULL) {
        ErrorExit((LPTSTR)_T("malloc"));
    }

    // Allocate memory buffers in the target process
    _lvi = (LVITEM*)VirtualAllocEx(
        hProcDesktop,
        NULL,
        sizeof(LVITEM),
        MEM_COMMIT,
        PAGE_READWRITE
    );
    _iconCoords = (POINT*)VirtualAllocEx(
        hProcDesktop,
        NULL,
        sizeof(POINT),
        MEM_COMMIT,
        PAGE_READWRITE
    );
    _iconName = (TCHAR*)VirtualAllocEx(
        hProcDesktop,
        NULL,
        MAX_PATH,
        MEM_COMMIT,
        PAGE_READWRITE
    );
    if (_iconName == NULL || _lvi == NULL || _iconCoords == NULL) {
        ErrorExit((LPTSTR)_T("VirtualAllocEx"));
    }
    lvi.cchTextMax = MAX_PATH;

    for (int i = 0; i < nIcons; i++) {
        lvi.iSubItem = 0;
        lvi.pszText = _iconName;

        // Read iconName
        iCode = WriteProcessMemory(hProcDesktop, _lvi, &lvi, sizeof(LVITEM), NULL);
        if (iCode == 0) {
            ErrorExit((LPTSTR)_T("WriteProcessMemory"));
        }
        SendMessage(hwDesktop, LVM_GETITEMTEXT, (WPARAM)i, (LPARAM)_lvi);
        iCode = ReadProcessMemory(hProcDesktop, _iconName, iconName, MAX_PATH, NULL);
        if (iCode == 0) {
            ErrorExit((LPTSTR)_T("ReadProcessMemory"));
        }

        // Read iconCoords
        bSuccess = SendMessage(hwDesktop, LVM_GETITEMPOSITION, i, (LPARAM)_iconCoords);
        if (!bSuccess) {
            ErrorExit((LPTSTR)_T("SendMessage"));
        }
        iCode = ReadProcessMemory(hProcDesktop, _iconCoords, &iconCoords, sizeof(POINT), NULL);
        if (iCode == 0) {
            ErrorExit((LPTSTR)_T("ReadProcessMemory"));
        }

        // _tprintf(_T("%s - x: %ld\ty: %ld\n"), iconName, iconCoords.x, iconCoords.y);

        // Copy the coords to our own structure
        memcpy(&allCoords[i], &iconCoords, sizeof(POINT));
    }

    // Shuffle the coordinates
    Shuffle(allCoords, nIcons);
    for (int i = 0; i < nIcons; i++) {
        // Write new iconCoords
        lparamCoords = MAKELPARAM(allCoords[i].x, allCoords[i].y);
        bSuccess = SendMessage(hwDesktop, LVM_SETITEMPOSITION, i, lparamCoords);
        if (!bSuccess) {
            ErrorExit((LPTSTR)_T("SendMessage"));
        }
    }

    // Cleanup
    iCode = VirtualFreeEx(hProcDesktop, _lvi, 0, MEM_RELEASE);
    if (iCode == 0) {
        ErrorExit((LPTSTR)_T("VirtualFreeEx1"));
    }
    iCode = VirtualFreeEx(hProcDesktop, _iconName, 0, MEM_RELEASE);
    if (iCode == 0) {
        ErrorExit((LPTSTR)_T("VirtualFreeEx"));
    }
    free(allCoords);

    return 0;
}