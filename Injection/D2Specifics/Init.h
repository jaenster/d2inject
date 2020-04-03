//
// Created by jaenster on 02/04/2020.
//
#pragma once
#include <iostream>
#define _DEFINE_VARS
#define __D2PTRS_ONCE_
#include "D2Ptrs.h"
#undef __D2PTRS_H__
#define __D2PTRS_LIST_

#include "D2Common.h"
#include "../MemoryManipulation.h"
// Some trickery to load D2Ptr's correctly, yeey legacy code


#include "D2Ptrs.h"


#include "Events.h"
#include "Patches.h"
#include "../maphack/src.h"

void DefineOffsets() {
    DWORD** p = (DWORD**)d2ptrs_list;
    do {
        **p = GetDllOffset(**p);
    } while (ptrdiff_t(++p) < ((ptrdiff_t)d2ptrs_list) + sizeof(d2ptrs_list));
}


void Startup() {
    DefineOffsets();
    // We just entered the DLL.. Now what?
    InstallPatches(D2::Patches);


    // Load the maphack
    Maphack::Init();

    //////////
    // just some proof of concepts
    //////////

#ifdef DEBUG_CODE
    D2::GameDrawOOG::hooks.push_back([]() {
        D2WIN_DrawText(L"What else is new",10,10,4,10);
//        std::cout << "oog drawing" << std::endl;
    });

    D2::GameDraw::hooks.push_back([]() {
        D2WIN_DrawText(L"What else is new",10,10,4,10);
//        std::cout << "in-game drawing" << std::endl;
    });

    D2::GameLoop::hooks.push_back([]() {
//        std::cout << "Game sleep" << std::endl;
    });

    D2::GameLeave::hooks.push_back([]() {
        std::cout << "left game" << std::endl;
    });

    D2::GameJoin::hooks.push_back([]() {
        std::cout << "join game" << std::endl;
    });
#endif
}

void Shutdown() {
//    RemovePatches(&D2::Patches);
}

