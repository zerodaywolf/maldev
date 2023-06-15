#include <windows.h>

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD Reason, LPVOID lpvReserved) {

    switch (Reason)
    {
    case DLL_PROCESS_ATTACH:
        MessageBoxW(NULL, L"hello", L"Who goes there", MB_ICONQUESTION | MB_OK);
        break;
    }
    return TRUE;
}
