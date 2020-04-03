#include "DLL.h"

BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID lpReserved) {
    switch (dwReason) {
        case DLL_PROCESS_ATTACH: {

#ifdef DEBUG_CODE
            // Open a console, if on a debugging defined
            AllocConsole();
                    FILE* fDummy;
                    freopen_s(&fDummy, "CONIN$", "r", stdin);
                    freopen_s(&fDummy, "CONOUT$", "w", stderr);
                    freopen_s(&fDummy, "CONOUT$", "w", stdout);
#endif

            DisableThreadLibraryCalls(hDll);
            Startup();
        }
            break;
        case DLL_PROCESS_DETACH:
            Shutdown();
            break;
    }

    return TRUE;
}
