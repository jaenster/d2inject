//
// Created by jaenster on 03/04/2020.
//

#pragma once

#include <vector>
#include "../D2Specifics/D2Ptrs.h"
#include "../D2Specifics/Events.h"
#include "../D2Specifics/D2Common.h"

namespace Maphack {
    struct Exit_t {
        Room2 *pRoom2;
        bool bStairs;
    };
    struct MyData {
        bool loadedAreas[137 /*UT + 1*/] = {};
        std::vector<Level *> visibleAreas;
        std::vector<Exit_t> exits;
    };
    MyData vault;

    void RevealRoom(Room2 *pRoom2) {
        bool bAdded = false;
        bool bInit = false;

        Level *level = pRoom2->pLevel;
        DWORD dwLevelNo = level->dwLevelNo;
        // Make sure we have the room.

        // Check if we have Room1(Needed in order to reveal)
        if (!(pRoom2->pLevel && pRoom2->pRoom1)) {
            D2COMMON_AddRoomData(pRoom2->pLevel->pMisc->pAct, pRoom2->pLevel->dwLevelNo, pRoom2->dwPosX,
                                 pRoom2->dwPosY, NULL);
            bAdded = true;
        }
        if (!(pRoom2->pRoom1)) { // second check added to see if we DID indeed init the room!
            return;
        }

        // If we are somewhere other then the given area, init automap layer to be drawn to.
        if (!(pRoom2->pLevel && pRoom2->pLevel->dwLevelNo && level->dwLevelNo == pRoom2->pLevel->dwLevelNo)) {
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

        return;
    };

    void Init() {

        D2::GameLoop::hooks.push_back([]() {
        });

        D2::GameDraw::hooks.push_back([]() {
            for(int i =0;i<vault.exits.size();i++) {
//                std::cout << vault.exits[i].pRoom2 << "test" << std::endl;
            }
            UnitAny *player = D2CLIENT_GetPlayerUnit();

            POINT myCoords = ScreenToAutomap(D2CLIENT_GetUnitX(player),D2CLIENT_GetUnitY(player));
            for(auto exit: vault.exits) {
                int x= exit.pRoom2->dwPosX * 5 + exit.pRoom2->dwSizeX * 5, y= exit.pRoom2->dwPosY * 5 + exit.pRoom2->dwSizeY * 5;
                POINT myPoint = ScreenToAutomap(x,y);

                std::cout << myPoint.x <<"\t" << myPoint.y <<"\t"   << "\t" << myCoords.x << "\t" << myCoords.y << "\t" << 1 << "\t" << -1 << std::endl;
                D2GFX_DrawLine(myPoint.x , myPoint.y , myCoords.x, myCoords.y, 6, -1);
            }
        });

        D2::GameJoin::hooks.push_back([]() {
            // On game join, we need to reset the loadedAreas

            for (int i = 0; i < 137; i++) { // Town area's get loaded automatically
                Maphack::vault.loadedAreas[i] = i == 1 || i == 40 || i == 75 || i == 103 || i == 109;
            }
        });

        D2::AreaChange::hooks.push_back([](int area) {
            std::cout << "changed area -> " << area << std::endl;

            // Remove all exists from list
            std::cout << "vault size" << std::endl;
            while(!Maphack::vault.exits.empty()) {
                std::cout << "LOL -> " <<Maphack::vault.exits.size() << " some size " << Maphack::vault.exits[Maphack::vault.exits.size()-1].bStairs << "here lol" << std::endl;
                Maphack::vault.exits.pop_back();
            }
            std::cout << std::endl;

            std::cout << "start process" << area << std::endl;

            // We need to wait a little but until the area is fully
            D2::Timer::add((1000 / 25) * 4, [](DWORD __unused) {
//                std::cout << "called after 250 ms" << std::endl;
                UnitAny *player = D2CLIENT_GetPlayerUnit();
                if (player == nullptr) return;
//            if (player->pPath->dwLevel)


                // Add the current level to this vector
                std::vector<Level *> areas = {player->pPath->pRoom1->pRoom2->pLevel};
                std::vector<Level *> avoid;

                // Check the neighbouring area's of this level
                std::vector<Room2 *> AddedRoomData;
                for (Room2 *room = player->pPath->pRoom1->pRoom2->pLevel->pRoom2First; room; room = room->pRoom2Next) {
                    // init the room if needed
                    if (!room->pRoom1) {
                        D2COMMON_AddRoomData(room->pLevel->pMisc->pAct, room->pLevel->dwLevelNo, room->dwPosX,
                                             room->dwPosY, room->pRoom1);
                        AddedRoomData.push_back(room);
                    }

                    if (room->pLevel->dwLevelNo != areas[0]->dwLevelNo) continue;

                    // Find exit that go via stairs (not via portals!)
                    for (PresetUnit *preset = room->pPreset; preset; preset = preset->pPresetNext)
                        if (preset->dwType == 5 /*TILE*/) {

                            Level *levelFrom = nullptr;

                            // get the level of the otherside
                            for (RoomTile *pRoomTile = room->pRoomTiles; pRoomTile; pRoomTile = pRoomTile->pNext) {
                                if (*(pRoomTile->nNum) == preset->dwTxtFileNo)
                                    levelFrom = pRoomTile->pRoom2->pLevel;
                            }


                            if (levelFrom && levelFrom->dwLevelNo) {
                                bool found = false;
                                for (auto &area : avoid) found = found || area->dwLevelNo == levelFrom->dwLevelNo;
                                if (!found) {
//                                    std::cout << "this area connects to, via stairs. Add on the bad list: "<< levelFrom->dwLevelNo << std::endl;

                                    // we cant render that
                                    avoid.push_back(levelFrom);
                                    Maphack::vault.exits.push_back({room,true});
                                }
                            }
                        }

                    // find neighbouring areas
                    Room2 **RoomNear = room->pRoom2Near;
                    for (DWORD i = 0; i < room->dwRoomsNear; i++) {
                        if (!RoomNear[i]->pRoom1) {
                            D2COMMON_AddRoomData(RoomNear[i]->pLevel->pMisc->pAct, RoomNear[i]->pLevel->dwLevelNo,
                                                 RoomNear[i]->dwPosX,
                                                 RoomNear[i]->dwPosY, RoomNear[i]->pRoom1);
                            AddedRoomData.push_back(RoomNear[i]);
                        }


                        if (RoomNear[i]->pLevel && RoomNear[i]->pLevel->dwLevelNo != room->pLevel->dwLevelNo) {

                            // check if this area, need to take with stairs?
                            bool found = false;
                            for (int j = 0; j < avoid.size() && !found; j++) found = found || avoid[j]->dwLevelNo ==
                                                                                              RoomNear[i]->pLevel->dwLevelNo;
                            if (found) continue;

                            // If we found a neighbouring room that isn't ours, add that to the list of area's to reveal
                            for (int j = 0; j < areas.size() && !found; j++) found = found || areas[j]->dwLevelNo ==
                                                                                              RoomNear[i]->pLevel->dwLevelNo;
                            if (!found) {
//                                std::cout << "this area connects to: " << RoomNear[i]->pLevel->dwLevelNo << std::endl;
                                areas.push_back(RoomNear[i]->pLevel);

                                Maphack::vault.exits.push_back({RoomNear[i],false});
                            }
                        }
                    }
                }

                // clean up init'd area's
                for (auto &addedRoom : AddedRoomData) {
                    D2COMMON_RemoveRoomData(addedRoom->pLevel->pMisc->pAct, addedRoom->pLevel->dwLevelNo,
                                            addedRoom->dwPosX, addedRoom->dwPosY,
                                            addedRoom->pRoom1);
                }

                Maphack::vault.visibleAreas = areas;
                int who = 0;
                for (int i = 0; i < areas.size(); i++) {
                    if (!Maphack::vault.loadedAreas[areas[i]->dwLevelNo]) {
                        auto area = (DWORD) areas[i];
                        D2::Timer::add([](DWORD area) {
                            auto *level = (Level *) area;
//                            std::cout << "here" << level->dwLevelNo << std::endl;
                            if (!Maphack::vault.loadedAreas[level->dwLevelNo]) {
                                Maphack::vault.loadedAreas[level->dwLevelNo] = true;

//                                std::cout << "maphacking area " << level->dwLevelNo << std::endl;
                                for (Room2 *room = level->pRoom2First; room; room = room->pRoom2Next) {
                                    Maphack::RevealRoom(room);
                                }
                            }
                        }, (1000 / 25) /*every frame 1*/ * (who++), area);
                    }
                }
            });
        });
    }
}