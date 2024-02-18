#include "stdafx.hpp"
#include "core/init.hpp"

//future exit scam ;)
BOOL APIENTRY DllMain(HMODULE hmod, DWORD reason, LPVOID reserve) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
       // DisableThreadLibraryCalls(hmod);
        if (HANDLE hThread = CreateThread(nullptr, NULL, Core::Init::Start, hmod, NULL, nullptr)) {
            CloseHandle(hThread);
        }
        break;
     
    }
    return TRUE;
}

