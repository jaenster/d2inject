//
// Created by jaenster on 04/04/2020.
//

#include "src.h"
#include "../D2Specifics/Events.h"

namespace Testing {

    DWORD myOffset(int Offset) {
        HMODULE hMod = GetModuleHandle(NULL);

        if (Offset < 0)
            return (DWORD) GetProcAddress(hMod, (LPCSTR) (-Offset));

        return ((DWORD) hMod) + Offset;
    }

    void Init() {
        BYTE buf[200];
        ::ReadProcessMemory(GetCurrentProcess(), (void *) myOffset(0x2194A0), buf, 200, NULL);
        std::cout << hexStr(buf, 200) << std::endl;

        BYTE buf2[16];
        ::ReadProcessMemory(GetCurrentProcess(), (void *) (DWORD) D2::ActChange::Override_I, buf2, 16, NULL);
        std::cout << hexStr(buf2, 16) << std::endl;
    }
}