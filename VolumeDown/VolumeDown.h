#pragma once
#ifndef VOLUMEDOWN_H
#define VOLUMEDOWN_H

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <initguid.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>


/// <summary>Classic ErrorExit funciton, but prints instead of displaying a MessageBox</summary>
/// <param name="lpszFunction">Function name to display</param>
/// <returns>Does not return</returns>
void ErrorExit(LPTSTR);


/// <summary>Entry point</summary>
/// <param name="argc"># of command-line arguments</param>
/// <param name="argv">Pointer to command-line arguments</param>
/// <returns>Exit code</returns>
int main(int argc, const char* argv[]);

#endif // VOLUMEDOWN_H
