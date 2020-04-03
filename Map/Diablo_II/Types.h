#pragma once
#include "..\Map.h"

#include <list>
#include <map>

#include "../../Injection/D2Specifics/D2Structs.h"
#include "../../Injection/D2Specifics/D2Ptrs.h"
//#include "../../D2Helpers.h"
namespace Mapping {
    enum ExitType {
        Linkage = 1, Tile = 2
    };

    struct Exit {
    public:
        DWORD Target;
        Point Position;
        ExitType Type;
        DWORD TileId;

        Exit(Point location, DWORD levelId, ExitType type, DWORD tileId) : Target(levelId), Position(location),
                                                                           Type(type), TileId(tileId) {
        }
    };
    typedef std::vector<Exit> ExitArray;
    typedef std::list<Room2*> RoomList;
    typedef std::list<Level*> LevelList;
    static RoomList RoomsAdded;
    static RoomList roomCache;
    static LevelList levelCache;
}