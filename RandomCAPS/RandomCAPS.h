#pragma once
#ifndef RANDOMCAPS_H
#define RANDOMCAPS_H

// WINVER must be set to let Windows.h know to turn on certain functionality (SendInput in this case)
#define WINVER 0x0500
#define KEYEVENTF_KEYDOWN 0x0

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>


/// <summary>Classic ErrorExit funciton, but prints instead of displaying a MessageBox</summary>
/// <param name="lpszFunction">Function name to display</param>
/// <returns>Does not return</returns>
void ErrorExit(LPTSTR);


/// <summary>Entry point</summary>
/// <param name="argc"># of command-line arguments</param>
/// <param name="argv">Pointer to command-line arguments</param>
/// <returns>Exit code</returns>
int main(int argc, const char* argv[]);

#endif // RANDOMCAPS_H
