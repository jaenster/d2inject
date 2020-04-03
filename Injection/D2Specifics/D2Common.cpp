//
// Created by jaenster on 03/04/2020.
//

#include "D2Common.h"
AutomapLayer *InitAutomapLayer(DWORD levelno) {
    AutomapLayer2 *pLayer = D2COMMON_GetLayer(levelno);
    return D2CLIENT_InitAutomapLayer_I(pLayer->nLayerNo);
}