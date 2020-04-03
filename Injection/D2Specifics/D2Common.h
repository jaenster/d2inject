//
// Created by jaenster on 03/04/2020.
//

#ifndef D2INJECT_D2COMMON_H
#define D2INJECT_D2COMMON_H

#include "D2Structs.h"
#include "D2Ptrs.h"
#include "D2Constants.h"

AutomapLayer *InitAutomapLayer(DWORD levelno) ;
POINT ScreenToAutomap(int x, int y);

void AddRoomData(Room2* room);
void RemoveRoomData(Room2* room);

double GetDistance(int x1, int y1,int x2, int y2);

UnitAny* GetPlayerUnit();

char* __stdcall GetLevelName(const Level* level);
char* __stdcall GetLevelIdName(DWORD level);
const char* GetUnitName(UnitAny* pUnit, char* szBuf, size_t bufSize);

DWORD __fastcall D2CLIENT_GetUnitName_STUB(DWORD UnitAny);
DWORD __fastcall D2CLIENT_GetUIVar_STUB(DWORD varno);
void __fastcall D2CLIENT_SetSelectedUnit_STUB(DWORD UnitAny);

DWORD GetTileLevelNo(Room2 *lpRoom2, DWORD dwTileNo);
#endif //D2INJECT_D2COMMON_H
