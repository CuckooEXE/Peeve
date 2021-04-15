/// Program: VolumeDown - Lower the volume slowly
/// Author: Axel Persinger
/// License: MIT License
/// 
/// TODO: Figure out how to convert to C, mainly just have to find out how to call CoCreateInstance _without_ __uuidof
#include "VolumeDown.h"


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

int GetSystemVolume()
{
    HRESULT hresult;
    float fVolume;
    IMMDeviceEnumerator* deviceEnumerator;
    IMMDevice* defaultDevice;
    IAudioEndpointVolume* endpointVolume;

    fVolume = 0.0;
    deviceEnumerator = NULL;
    defaultDevice = NULL;
    endpointVolume = NULL;


    hresult = CoInitialize(NULL);
    if (hresult != S_OK) {
        ErrorExit((LPTSTR)_T("CoInitialize"));
    }

    hresult = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
    if (hresult != S_OK) {
        ErrorExit((LPTSTR)_T("CoCreateInstance"));
    }

    hresult = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
    if (hresult != S_OK) {
        ErrorExit((LPTSTR)_T("GetDefaultAudioEndpoint"));
    }
    deviceEnumerator->Release();
    deviceEnumerator = NULL;

    hresult = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&endpointVolume);
    if (hresult != S_OK) {
        ErrorExit((LPTSTR)_T("Activate"));
    }
    defaultDevice->Release();
    defaultDevice = NULL;

    hresult = endpointVolume->GetMasterVolumeLevelScalar(&fVolume);
    if (hresult != S_OK) {
        ErrorExit((LPTSTR)_T("GetMasterVolumeLevel"));
    }
    endpointVolume->Release();
    CoUninitialize();

    return (int)(fVolume * 100);
}


void SetSystemVolume(float volume)
{
    HRESULT hresult;
    IMMDeviceEnumerator* deviceEnumerator;
    IMMDevice* defaultDevice;
    IAudioEndpointVolume* endpointVolume;

    deviceEnumerator = NULL;
    defaultDevice = NULL;
    endpointVolume = NULL;


    hresult = CoInitialize(NULL);
    if (hresult != S_OK) {
        ErrorExit((LPTSTR)_T("CoInitialize"));
    }

    hresult = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
    if (hresult != S_OK) {
        ErrorExit((LPTSTR)_T("CoCreateInstance"));
    }

    hresult = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
    if (hresult != S_OK) {
        ErrorExit((LPTSTR)_T("GetDefaultAudioEndpoint"));
    }
    deviceEnumerator->Release();
    deviceEnumerator = NULL;

    hresult = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&endpointVolume);
    if (hresult != S_OK) {
        ErrorExit((LPTSTR)_T("Activate"));
    }
    defaultDevice->Release();
    defaultDevice = NULL;

    hresult = endpointVolume->SetMasterVolumeLevelScalar(volume, NULL);
    if (hresult != S_OK) {
        ErrorExit((LPTSTR)_T("SetMasterVolumeLevelScalar"));
    }
    endpointVolume->Release();
    CoUninitialize();
}


int main(int argc, const char* argv[])
{
    float fVolume;
    int nSteps;
    float step;
    nSteps = 10;

    fVolume = (float)GetSystemVolume() / 100;
    _tprintf(_T("Volume Level = %f\n"), fVolume);

    step = fVolume / nSteps;
    for (int i = 0; i <= nSteps; i++) {
        fVolume -= (i * step);
        SetSystemVolume(fVolume);
        _tprintf(_T("Volume Level = %d\n"), GetSystemVolume());
    }

    _tprintf(_T("Final Volume Level = %d\n"), GetSystemVolume());
    return 0;
}
