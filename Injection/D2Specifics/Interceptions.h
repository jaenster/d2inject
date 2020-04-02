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

 Naming convention:

 D2::Intercepts::PatchName::Override <-- function that replaces code at existing d2 code
 D2::Intercepts::PatchName::Outside <-- function that is called by Override
 D2::Intercepts::PatchName::Handler <-- function that is calls the hook's

 Outside can be skipped if overrider is in the position to directly make a call

 */
namespace D2 {
    namespace Intercepts {
        namespace GameLoop {
            std::vector<void (*)(void)> hooks = {};

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
        namespace GameDrawOOG {

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

        namespace GameDraw {
            std::vector<void (*)(void)> hooks = {};

            void Handler(void) {
                for (auto func: hooks) func();
            }

            void Override(void) {
                Handler();
            }
        }

        namespace CongratsScreen {
            std::vector<void (*)(void)> hooks = {
                    []() { // Fix max diff flag
                        if (D2CLIENT_GetDifficulty() == 1 && *p_D2CLIENT_ExpCharFlag) {
                            BnetData* pData = *p_D2LAUNCH_BnData;
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
        namespace GameLeave {
            std::vector<void (*)(void)> hooks = {
                    []() { // Fix max diff flag

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
    }
}

#endif //D21_14DCLEANINJECT_INTERCEPTIONS_H
