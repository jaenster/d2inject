//
// Created by jaenster on 03/04/2020.
//

#pragma once

#include <vector>
#include "../D2Specifics/D2Ptrs.h"
#include "../D2Specifics/Events.h"
#include "../D2Specifics/D2Common.h"

namespace Maphack {

    struct MyData {
        bool loadedAreas[137 /*UT + 1*/] = {};
//        int neighbours[137][7] = {
//                //0; doesnt have neighbours as it doesnt exists
//                {},
//                // 1; has
//                {2}
//        };
        long areaChangeTick;
        int lastArea;
    };
    MyData vault;


    void Init() {

        D2::GameLoop::hooks.push_back([]() {

//            std::cout << "lol here" << std::endl;

            UnitAny *player = D2CLIENT_GetPlayerUnit();
            if (player == nullptr) return;
//            if (player->pPath->dwLevel)

            auto revealRoom = [](Room2 *pRoom2) {
                bool bAdded = false;
                bool bInit = false;

                DWORD dwLevelNo = D2CLIENT_GetPlayerUnit()->pPath->pRoom1->pRoom2->pLevel->dwLevelNo;
                // Make sure we have the room.
                if (!pRoom2)
                    return false;

                UnitAny *player = D2CLIENT_GetPlayerUnit();
                // Check if we have Room1(Needed in order to reveal)
                if (!(pRoom2 && pRoom2->pLevel && pRoom2->pRoom1)) {
                    D2COMMON_AddRoomData(pRoom2->pLevel->pMisc->pAct, pRoom2->pLevel->dwLevelNo, pRoom2->dwPosX,
                                         pRoom2->dwPosY, NULL);
                    bAdded = true;
                }
                if (!(pRoom2 && pRoom2->pRoom1)) { // second check added to see if we DID indeed init the room!
                    return false;
                }

                // If we are somewhere other then the given area, init automap layer to be drawn to.
                if (!(pRoom2 && pRoom2->pLevel && pRoom2->pLevel->dwLevelNo && player->pPath && player->pPath->pRoom1 &&
                      player->pPath->pRoom1->pRoom2 &&
                      player->pPath->pRoom1->pRoom2->pLevel &&
                      player->pPath->pRoom1->pRoom2->pLevel->dwLevelNo == pRoom2->pLevel->dwLevelNo)) {
                    InitAutomapLayer(pRoom2->pLevel->dwLevelNo);
                    bInit = true;
                }

                // Reveal this room!
                D2CLIENT_RevealAutomapRoom(pRoom2->pRoom1, TRUE, (*p_D2CLIENT_AutomapLayer));

                // Remove room data if we have added.
                if (bAdded)
                    D2COMMON_RemoveRoomData(pRoom2->pLevel->pMisc->pAct, pRoom2->pLevel->dwLevelNo, pRoom2->dwPosX,
                                            pRoom2->dwPosY, NULL);

                if (bInit)
                    InitAutomapLayer(dwLevelNo);

                return true;
            };

            // Add the current level to this vector
            std::vector<Level *> areas = {player->pPath->pRoom1->pRoom2->pLevel};

            for (int i = 0; i < areas.size(); i++) {
                if (!vault.loadedAreas[areas[0]->dwLevelNo]) {
                    vault.loadedAreas[areas[0]->dwLevelNo] = true;
//                std::cout << "lol here " << i << std::endl;
                    for (Room2 *room = areas[0]->pRoom2First; room; room = room->pRoom2Next) {
//                    std::cout << "lol here " << areas[0]->pRoom2First << std::endl;
                        revealRoom(room);
                    }
                }

            }

        });

        D2::GameJoin::hooks.push_back([]() {
            // On game join, we need to reset the loadedAreas

            for (int i = 0; i < 137; i++) { // Town area's get loaded automatically
                vault.loadedAreas[i] = i == 1 || i == 40 || i == 75 || i == 103 || i == 109;
            }
        });
    }
}