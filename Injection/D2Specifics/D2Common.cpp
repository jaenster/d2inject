//
// Created by jaenster on 03/04/2020.
//

#include <utility>
#include <map>
#include <vector>
#include <cmath>
#include "D2Common.h"
#include "../../Tools/StringManipulation.h"

AutomapLayer *InitAutomapLayer(DWORD levelno) {
    AutomapLayer2 *pLayer = D2COMMON_GetLayer(levelno);
    return D2CLIENT_InitAutomapLayer_I(pLayer->nLayerNo);
}

POINT ScreenToAutomap(int x, int y) {
    POINT result = {0, 0};
    x *= 32;
    y *= 32;
    result.x = ((x - y) / 2 / (*p_D2CLIENT_Divisor)) - (*p_D2CLIENT_Offset).x + 8;
    result.y = ((x + y) / 4 / (*p_D2CLIENT_Divisor)) - (*p_D2CLIENT_Offset).y - 8;

    if (D2CLIENT_GetAutomapSize()) {
        --result.x;
        result.y += 5;
    }
    return result;
}

void AddRoomData(Room2* room) {
    D2COMMON_AddRoomData(room->pLevel->pMisc->pAct, room->pLevel->dwLevelNo, room->dwPosX, room->dwPosY, room->pRoom1);
}

void RemoveRoomData(Room2* room) {
    D2COMMON_RemoveRoomData(room->pLevel->pMisc->pAct, room->pLevel->dwLevelNo, room->dwPosX, room->dwPosY, room->pRoom1);
}

UnitAny* GetPlayerUnit() {
    return D2CLIENT_GetPlayerUnit();
}

char* __stdcall GetLevelName(const Level* level) {
    return D2COMMON_GetLevelText(level->dwLevelNo)->szName;
}

char* __stdcall GetLevelIdName(DWORD level) {
    return D2COMMON_GetLevelText(level)->szName;
}

double GetDistance(int x1, int y1, int x2, int y2) {
    return std::hypot(std::fmax(x2, x1) - std::fmin(x2, x1), std::fmax(y2, y1) - std::fmin(y2, y1));
}

const char *GetUnitName(UnitAny *pUnit, char *szTmp, size_t bufSize) {
    if (!pUnit) {
        strcpy_s(szTmp, bufSize, "Unknown");
        return szTmp;
    }
    if (pUnit->dwType == 2) {
        wchar_t *wName = D2CLIENT_GetUnitName(pUnit);
        WideCharToMultiByte(CP_UTF8, 0, wName, -1, szTmp, bufSize, 0, 0);
        return szTmp;
    }
    if (pUnit->dwType == 1 && pUnit->pPlayerData) {
        //	return pUnit->pPlayerData->szName;
        strcpy_s(szTmp, bufSize, pUnit->pPlayerData->szName);
        return szTmp;
    }
    if (pUnit->dwType == 4) {
        wchar_t wBuffer[256] = L"";
        D2CLIENT_GetItemName(pUnit, wBuffer, _countof(wBuffer));
        char *szBuffer = UnicodeToAnsi(wBuffer);
        if (strchr(szBuffer, '\n'))
            *strchr(szBuffer, '\n') = 0x00;

        strcpy_s(szTmp, bufSize, szBuffer);
        delete[] szBuffer;
        return szTmp;
    }
    if (pUnit->dwType == UNIT_OBJECT || pUnit->dwType == UNIT_TILE) {
        if (pUnit->pObjectData && pUnit->pObjectData->pTxt) {
            strcpy_s(szTmp, bufSize, pUnit->pObjectData->pTxt->szName);
            return szTmp;
        }
    }
    strcpy_s(szTmp, bufSize, "Unknown");
    return szTmp;
}

DWORD __declspec(naked) __fastcall D2CLIENT_GetUnitName_STUB(DWORD UnitAny) {
    __asm
    {
    // mov eax, ecx
    jmp D2CLIENT_GetUnitName_I
    }
}

DWORD __declspec(naked) __fastcall D2CLIENT_GetUIVar_STUB(DWORD varno) {
    __asm
    {
    mov eax, ecx
    jmp D2CLIENT_GetUiVar_I
    }
}

void __declspec(naked) __fastcall D2CLIENT_SetSelectedUnit_STUB(DWORD UnitAny) {
    __asm
    {
    mov eax, ecx
    jmp D2CLIENT_SetSelectedUnit_I
    }
}
DWORD GetTileLevelNo(Room2 *lpRoom2, DWORD dwTileNo) {
    for (RoomTile *pRoomTile = lpRoom2->pRoomTiles; pRoomTile; pRoomTile = pRoomTile->pNext) {
        if (*(pRoomTile->nNum) == dwTileNo)
            return pRoomTile->pRoom2->pLevel->dwLevelNo;
    }

    return NULL;
}