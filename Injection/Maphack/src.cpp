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
    };
    MyData vault;


    void Init() {
        D2::GameLoop::hooks.push_back([]() {

//            std::cout << "lol here" << std::endl;

            UnitAny *player = D2CLIENT_GetPlayerUnit();
            if (player == nullptr) return;

            auto revealRoom = [](Room2 *room2, UnitAny *player) {
                bool beenAdded = false, beenInit = false;

                if (room2 == nullptr) return;

                // Need to init room1?
                if ((beenAdded = !room2->pLevel || !room2->pRoom1)) {
                    std::cout << "add room" << room2->pLevel->pMisc->pAct<< room2->pLevel->dwLevelNo<< room2->dwPosX<<
                            room2->dwPosY<< NULL << std::endl;
                    D2COMMON_AddRoomData(room2->pLevel->pMisc->pAct, room2->pLevel->dwLevelNo, room2->dwPosX,
                                         room2->dwPosY, NULL);

                    // Failed at initing it?
                    if (!room2->pRoom1) {
                        std::cout << "failed inititing" << std::endl;
                        return;
                    }
                }

                if (!(room2->pLevel
                      && room2->pLevel->dwLevelNo
                      && player->pPath
                      && player->pPath->pRoom1
                      && player->pPath->pRoom1->pRoom2
                      && player->pPath->pRoom1->pRoom2->pLevel
                      && player->pPath->pRoom1->pRoom2->pLevel->dwLevelNo == room2->pLevel->dwLevelNo)
                        ) {

                    std::cout << "need to init InitAutomapLayer" << std::endl;
                    // need to init it?
                    InitAutomapLayer(room2->pLevel->dwLevelNo);
                    beenInit = true;
                }

                if (beenAdded)
                    D2COMMON_RemoveRoomData(room2->pLevel->pMisc->pAct, room2->pLevel->dwLevelNo, room2->dwPosX,
                                            room2->dwPosY, NULL);

                if (beenInit) {
                    InitAutomapLayer(player->pPath->pRoom1->pRoom2->pLevel->dwLevelNo);
                }
                return;
            };

            // Add the current level to this vector
            std::vector<Level *> areas = {player->pPath->pRoom1->pRoom2->pLevel};

            for (int i = 0; i < areas.size(); i++) {
//                std::cout << "lol here " << i << std::endl;
                for (Room2 *room = areas[0]->pRoom2First; room; room = room->pRoom2Next) {
//                    std::cout << "lol here " << areas[0]->pRoom2First << std::endl;
                    revealRoom(room, player);
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