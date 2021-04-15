#pragma once
#ifndef GETDOWN_H
#define GETDOWN_H

#pragma comment(lib, "Dwmapi.lib")
 
#include <Windows.h>
#include <dwmapi.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

/// <summary>Classic ErrorExit funciton, but prints instead of displaying a MessageBox</summary>
/// <param name="lpszFunction">Function name to display</param>
/// <returns>Does not return</returns>
void ErrorExit(LPTSTR);

/// <summary>Returns if the window is an "ALT-TAB"-able window (https://stackoverflow.com/a/7292674/10280970)</summary>
/// <note>I added the last conditional checking for cloaked window</note>
/// <param name="hwnd">Window handle to check</param>
/// <returns>TRUE if the Window is ALT-TAB-able, otherwise FALSE</returns>
BOOL IsAltTabWindow(HWND);

/// <summary>Callback for EnumWindows</summary>
/// <param name="hWindow">Window Handle received</param>
/// <param name="lparam">NULL</param>
/// <returns>Always returns TRUE to get the next Window</returns>
static BOOL CALLBACK enumWindowCB(HWND, LPARAM);

/// <summary>Entry point</summary>
/// <param name="argc"># of command-line arguments</param>
/// <param name="argv">Pointer to command-line arguments</param>
/// <returns>Exit code</returns>
int main(int argc, const char* argv[]);

#endif // GETDOWN_H
