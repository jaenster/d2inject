//
// Created by jaenster on 02/04/2020.
//

#ifndef INJECTION_MEMORYMANIPULATION_H
#define INJECTION_MEMORYMANIPULATION_H

#include <minwindef.h>
#include <memoryapi.h>
#include <vector>
#include INJECTION_TARGET

#define INST_NOP 0x90
#define INST_RET 0xC3
#define INST_INT3 0xCC
#define INST_CALL 0xE8
#define INST_JMP 0xE9
struct PatchStructure {
    void (*pFunc)(DWORD, DWORD, DWORD);
    DWORD dwAddr;
    DWORD dwFunc;
    DWORD dwLen;
    BYTE* bOldCode;
};

BOOL WriteBytes(void* pAddr, void* pData, DWORD dwLen) {
    DWORD dwOld;

    if (!VirtualProtect(pAddr, dwLen, PAGE_READWRITE, &dwOld))
        return FALSE;

    ::memcpy(pAddr, pData, dwLen);
    return VirtualProtect(pAddr, dwLen, dwOld, &dwOld);
}

void InterceptLocalCode(BYTE bInst, DWORD pAddr, DWORD pFunc, DWORD dwLen) {
    BYTE* bCode = new BYTE[dwLen];
    ::memset(bCode, INST_NOP, dwLen);
    DWORD dwFunc = pFunc - (pAddr + 5);

    bCode[0] = bInst;
    *(DWORD*)&bCode[1] = dwFunc;
    WriteBytes((void*)pAddr, bCode, dwLen);

    delete[] bCode;
}

void PatchCall(DWORD dwAddr, DWORD dwFunc, DWORD dwLen) {
    InterceptLocalCode(INST_CALL, dwAddr, dwFunc, dwLen);
}

void PatchJmp(DWORD dwAddr, DWORD dwFunc, DWORD dwLen) {
    InterceptLocalCode(INST_JMP, dwAddr, dwFunc, dwLen);
}

void PatchBytes(DWORD dwAddr, DWORD dwValue, DWORD dwLen) {
    BYTE* bCode = new BYTE[dwLen];
    ::memset(bCode, (BYTE)dwValue, dwLen);

    WriteBytes((LPVOID)dwAddr, bCode, dwLen);

    delete[] bCode;
}

void InstallPatches(std::vector<PatchStructure> Patches) {
    for (int x = 0; x < Patches.size(); x++) {
        Patches[x].bOldCode = new BYTE[Patches[x].dwLen];

        ::ReadProcessMemory(GetCurrentProcess(), (void*)Patches[x].dwAddr, Patches[x].bOldCode, Patches[x].dwLen, NULL);
        Patches[x].pFunc(Patches[x].dwAddr, Patches[x].dwFunc, Patches[x].dwLen);
    }
}


void RemovePatches(std::vector<PatchStructure> Patches) {
    for (int x = 0; x < Patches.size(); x++) {
        WriteBytes((void*)Patches[x].dwAddr, Patches[x].bOldCode, Patches[x].dwLen);
        delete[] Patches[x].bOldCode;
    }
}
//
//void DefineOffsets() {
//    DWORD** p = (DWORD**)d2ptrs_list;
//    do {
//        **p = GetDllOffset(**p);
//    } while (ptrdiff_t(++p) < ((ptrdiff_t)d2ptrs_list) + sizeof(d2ptrs_list));
//}

DWORD GetDllOffset(char *DllName, int Offset) {
    HMODULE hMod = GetModuleHandle(NULL);

    if (Offset < 0)
        return (DWORD)GetProcAddress(hMod, (LPCSTR)(-Offset));

    return ((DWORD)hMod) + Offset;
}

DWORD GetDllOffset(int num) {
    static char* dlls[] = {"D2Client.DLL", "D2Common.DLL", "D2Gfx.DLL",    "D2Lang.DLL", "D2Win.DLL", "D2Net.DLL",  "D2Game.DLL",
                           "D2Launch.DLL", "Fog.DLL",      "BNClient.DLL", "Storm.DLL",  "D2Cmp.DLL", "D2Multi.DLL"};
    if ((num & 0xff) > 12)
        return 0;
    return GetDllOffset(dlls[num & 0xff], num >> 8);
}


#endif //INJECTION_MEMORYMANIPULATION_H
