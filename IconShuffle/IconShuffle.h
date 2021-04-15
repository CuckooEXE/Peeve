#pragma once
#ifndef ICONSHUFFLE_H
#define ICONSHUFFLE_H

#include <Windows.h>
#include <Commctrl.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <stdbool.h>

/// <summary>Returns the Desktop Window Handle (https://stackoverflow.com/a/5691808/10280970)</summary>
/// <returns>Window Handle for the Desktop</returns>
HWND GetDesktopHandle();

/// <summary>Classic ErrorExit funciton, but prints instead of displaying a MessageBox</summary>
/// <param name="lpszFunction">Function name to display</param>
/// <returns>Does not return</returns>
void ErrorExit(LPTSTR);

/// <summary>Fisher-Yates Shuffle for POINT structs</summary>
/// <param name="allCoords">Pointer to an array of POINT structs to shuffle</param>
/// <param name="nCoords">Number of coordinates in the struct</param>
/// <returns></returns>
void Shuffle(POINT*, int);

/// <summary>Entry point</summary>
/// <param name="argc"># of command-line arguments</param>
/// <param name="argv">Pointer to command-line arguments</param>
/// <returns>Exit code</returns>
int main(int argc, const char* argv[]);

#endif // ICONSHUFFLE_H

