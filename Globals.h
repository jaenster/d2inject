//
// Created by jaenster on 04/04/2020.
//

#pragma  once
#ifndef D2INJECT_GLOBALS_H
#define D2INJECT_GLOBALS_H

#include <windows.h>

struct Globals_h {
    long SectionCount;

//    CRITICAL_SECTION cEventSection;
//    CRITICAL_SECTION cRoomSection;
//    CRITICAL_SECTION cMiscSection;
//    CRITICAL_SECTION cScreenhookSection;
//    CRITICAL_SECTION cPrintSection;
//    CRITICAL_SECTION cTextHookSection;
//    CRITICAL_SECTION cImageHookSection;
//    CRITICAL_SECTION cBoxHookSection;
//    CRITICAL_SECTION cFrameHookSection;
//    CRITICAL_SECTION cLineHookSection;
//    CRITICAL_SECTION cFlushCacheSection;
//    CRITICAL_SECTION cConsoleSection;
    CRITICAL_SECTION cGameLoopSection;
//    CRITICAL_SECTION cUnitListSection;
//    CRITICAL_SECTION cFileSection;
    bool bGameLoopEntered;
};
extern Globals_h global;

#endif //D2INJECT_GLOBALS_H
