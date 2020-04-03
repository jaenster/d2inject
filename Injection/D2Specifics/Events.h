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


namespace D2 {
    extern bool inGame;
    namespace GameJoin {
        extern std::vector<void (*)(void)> hooks;
    }
    namespace GameLoop { // PATCH
        extern std::vector<void (*)(void)> hooks ;

        void Override(void);
    }
    namespace GameDrawOOG { // PATCH

        extern std::vector<void (*)(void)> hooks;

        void Override(void);
    }

    namespace GameDraw { // PATCH
        extern std::vector<void (*)(void)> hooks;

        void Override(void);
    }

    namespace CongratsScreen { // PATCH
        extern std::vector<void (*)(void)> hooks;

        void  Override(void);
    }
    namespace GameLeave { // PATCH
        extern std::vector<void (*)(void)> hooks;

        void  Override(void);
    }
}

#endif //D21_14DCLEANINJECT_INTERCEPTIONS_H
