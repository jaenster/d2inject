#include "D2Ptrs.h"
#include "Events.h"

namespace D2 {
    extern bool inGame = false;
    extern int area = 0;
    namespace GameJoin {
        std::vector<void (*)(void)> hooks = {

        };

        void Handler(void) {
            for (auto func: hooks) func();
        }
    }
    namespace AreaChange {
        std::vector<void (*)(int)> hooks;

        void Handler(int areaId) {
            for (auto func: hooks) func(areaId);
        }

        void Check(void) {
            UnitAny *player = D2CLIENT_GetPlayerUnit();
            if (player == nullptr) return;

            int current = player->pPath->pRoom1->pRoom2->pLevel->dwLevelNo;
            if (D2::area != current) {
                D2::area = current;
                Handler(current);
            }
        }
    }
    namespace GameLoop { // PATCH
        std::vector<void (*)(void)> hooks = {
                []() {
                    // this is the sleep function inGame, so that means we are in game
                    // if we wherent in game yet, this is the time to load call GameJoin
                    if (!D2::inGame) {
                        D2::inGame = true;
                        D2::area = 0;
                        D2::GameJoin::Handler();
                    }
                    // check the area change
                    D2::AreaChange::Check();
                },
        };

        void Handler(void) {
            for (auto func: hooks) func();
        }


        // Called @ Override
        void __declspec(naked) OutsideFunc(void) {
            __asm {
            jmp Handler

            //Something we override @ sleepy patch
            mov    eax, ds:0x7a0610
            }
        }

        void __declspec(naked) Override(void) {
            __asm {
            // this function is outside of this injected code
            jmp OutsideFunc

            // to cancel out for the mov    eax,ds:0x7a0610
            nop
            nop
            }
        }
    }
    namespace GameDrawOOG { // PATCH

        std::vector<void (*)(void)> hooks = {
                []() { // We still need to draw the original sprite right
                    D2WIN_DrawSprites();
                }
        };

        void Handler(void) {
            for (auto func: hooks) func();
        }

        void Override(void) {
            Handler();
        }
    }

    namespace GameDraw { // PATCH
        std::vector<void (*)(void)> hooks = {};

        void Handler(void) {
            for (auto func: hooks) func();
        }

        void Override(void) {
            Handler();
        }
    }

    namespace CongratsScreen { // PATCH
        std::vector<void (*)(void)> hooks = {
                []() { // Fix max diff flag
                    if (D2CLIENT_GetDifficulty() == 1 && *p_D2CLIENT_ExpCharFlag) {
                        BnetData *pData = *p_D2LAUNCH_BnData;
                        if (pData)
                            pData->nMaxDiff = 10;
                    }
                }
        };

        void Handler(void) {
            for (auto func: hooks) func();
        }

        void __declspec(naked) Override(void) {
            __asm
            {
            call D2CLIENT_CongratsScreen_I
            pushad
            call Handler
            popad
            retn
            }
        }
    }
    namespace GameLeave { // PATCH
        std::vector<void (*)(void)> hooks = {
            []() {
                // Once we left the game, we arent in one
                D2::inGame = false;
            }
        };

        void Handler(void) {
            for (auto func: hooks) func();
        }

        void __declspec(naked) Override(void) {
            __asm
            {
            call Handler
            jmp D2CLIENT_GameLeave_I
            }
        }
    }
}
