//
// Created by jaenster on 04/04/2020.
//

#include "src.h"

namespace Testing {

    DWORD myOffset(int Offset) {
        HMODULE hMod = GetModuleHandle(NULL);

        if (Offset < 0)
            return (DWORD) GetProcAddress(hMod, (LPCSTR) (-Offset));

        return ((DWORD) hMod) + Offset;
    }

    void Init() {
        BYTE buf[16];
        ::ReadProcessMemory(GetCurrentProcess(), (void *) myOffset(0x2194A0), buf, 16, NULL);
        std::cout << hexStr(buf, 16) << std::endl;
    }
}