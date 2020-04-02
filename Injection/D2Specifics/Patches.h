//
// Created by jaenster on 02/04/2020.
//

#ifndef INJECTION_PATCHES
#define INJECTION_PATCHES

#include "../MemoryManipulation.h"
#include "Interceptions.h"
#include <vector>

// Load the Interceptions
#include "Interceptions.h"

#include <vector>
namespace D2 {

    DWORD off(int Offset) {
        HMODULE hMod = GetModuleHandle(NULL);

        if (Offset < 0)
            return (DWORD)GetProcAddress(hMod, (LPCSTR)(-Offset));

        return ((DWORD)hMod) + Offset;
    }

    std::vector<PatchStructure> Patches = {
            // GameDraw intercepts
            {PatchCall, off(0xF9A0D), (DWORD)D2::Intercepts::GameDrawOOG::Override, 5},
            {PatchJmp, off(0x53B30), (DWORD)D2::Intercepts::GameDraw::Override, 5},

            // Congrats screen, its hookable, yet i dont see why you want to.
            {PatchCall, off(0x4EBEF), (DWORD)D2::Intercepts::CongratsScreen::Override, 5},

            {PatchCall,off(0x4F57C),(DWORD)D2::Intercepts::GameLeave::Override,5},

            /*         Original data

    0:  75 17                   jne    0x19
    2:  a1 10 06 7a 00          mov    eax,ds:0x7a0610
    7:  83 f8 06                cmp    eax,0x6
    a:  74 0d                   je     0x19
    c:  83 f8 08                cmp    eax,0x8

         Replaced with. XX XX XX XX = Address Loc of GameLoop_Intercept
    0:  75 XX XX XX XX          call GameLoop_Intercept // overrides jne & eax,ds:0x7a0610
    5:  90                      nop  // overrides -> eax,ds:0x7a0610
    6:  90                      nop  // overrides -> eax,ds:0x7a0610
    7:  83 f8 06                cmp    eax,0x6
    a:  74 0d                   je     0x19
    c:  83 f8 08                cmp    eax,0x8
         */
            {PatchCall, off(0x51C31), (DWORD) D2::Intercepts::GameLoop::Override,5},
    };

}
#endif //INJECTION_PATCHES
