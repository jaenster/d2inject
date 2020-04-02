//
// Created by jaenster on 02/04/2020.
//

#ifndef D21_14DCLEANINJECT_INTERCEPTIONS_H
#define D21_14DCLEANINJECT_INTERCEPTIONS_H

////////////////////////////////////////////////////
// Game loop patch
///////////////////////////////////////////////////
#include "Windows.h"


#include <iostream>
#include <vector>

/*

 Naming convention for patches

 D2::PatchName::Override <-- function that replaces code at existing d2 code
 D2::PatchName::Outside <-- function that is called by Override
 D2::PatchName::Handler <-- function that is calls the hook's

 Outside can be skipped if overrider is in the position to directly make a call

 */
namespace D2 {
    bool inGame = false;
    namespace GameJoin {
        std::vector<void (*)(void)> hooks = {

        };

        void Handler(void) {
            for (auto func: hooks) func();
        }
    }
    namespace GameLoop { // PATCH
        std::vector<void (*)(void)> hooks = {
                []() {
                    // this is the sleep function inGame, so that means we are in game
                    // if we wherent in game yet, this is the time to load call GameJoin
                    if (!D2::inGame) {
                        D2::inGame = true;
                        D2::GameJoin::Handler();
                    }

                }
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

#endif //D21_14DCLEANINJECT_INTERCEPTIONS_H
