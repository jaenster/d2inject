#pragma once

#include "..\Map.h"

#include <list>
#include <map>

#include "../../Injection/D2Specifics/D2Structs.h"
#include "../../Injection/D2Specifics/D2Ptrs.h"
#include "Types.h"

namespace Mapping {

class LevelMap;

typedef std::map<DWORD, LevelMap*> LevelMapList;
class LevelMap : public Map {
  public:
    enum CollisionFlag {
        None = 0x0000,
        BlockWalk = 0x0001,
        BlockLineOfSight = 0x0002,
        Wall = 0x0004,
        BlockPlayer = 0x0008,
        AlternateTile = 0x0010,
        Blank = 0x0020,
        Missile = 0x0040,
        Player = 0x0080,
        NPCLocation = 0x0100,
        Item = 0x0200,
        Object = 0x0400,
        ClosedDoor = 0x0800,
        NPCCollision = 0x1000,
        FriendlyNPC = 0x2000,
        Unknown = 0x4000,
        DeadBody = 0x8000, // also portal
        ThickenedWall = 0xfefe,
        Avoid = 0xffff
    };

  private:
    static LevelMapList cache;

    Act* act;
    const Level* level;
    int width, height;
    int posX, posY;
    Matrix<CollisionFlag> mapPoints;
    RoomList addedRooms;

    Level* cachedLevel;
    CRITICAL_SECTION* lock;

    void Build(void);

    static inline int GetLevelNo(Room2* room, DWORD tile) {
        return GetTileLevelNo(room, tile);
    }

    void AddRoom(Room2* const room, RoomList& rooms, UnitAny* player);
    void AddCollisionMap(Room1* pRoom1);
    void SetCollisionData(int x, int y, int value);
    bool IsGap(int x, int y, bool abs) const;
    void FillGaps(void);
    void ShrinkMap(void);
    void ThickenWalls(void);

    bool RoomSpaceIsWalkable(Room1* pRoom1, const Point& point, bool abs) const;
    bool ValueIsWalkable(const WORD* value) const;
    bool ValueHasFlag(int flag, const WORD* value) const;

    bool EdgeIsWalkable(const Point& edgePoint, const Point& offsetPoint, Room1* pRoom1Adjecent, bool abs) const;

    void FindRoomTileExits(Room2* room, ExitArray& exits) const;
    void FindRoomLinkageExits(ExitArray& exits, RoomList& added) const;
    Point GetEdgeCenterPoint(const Point& currentPoint, const Point& edgeDirection) const;
    bool ExitExists(Point loc, ExitArray& exits) const;
    bool ExitExists(DWORD dwLevelNo, ExitArray& exits) const;
    bool isPointInRoom(Room2* room, const Point& pt) const;
    bool isPointInLevel(Level* level, const Point& pt) const;
    WORD getCollFromRoom(Room2* room, const Point& pt) const;

    void DrillExits();

    LevelMap(const Level* level);
    ~LevelMap(void);

  public:
    static LevelMap* GetMap(Level* level);
    static void ClearCache(void);
    void Dump(const char* file, const PointList& points) const;
    void CleanUp(void) const;
    Point AbsToRelative(const Point& point) const;
    Point RelativeToAbs(const Point& point) const;

    inline int GetWidth(void) const {
        return width;
    }
    inline int GetHeight(void) const {
        return height;
    }
    inline int GetPosX(void) const {
        return posX;
    }
    inline int GetPosY(void) const {
        return posY;
    }
    /* If these are backwards, it's not me. */
    inline int GetMaxX(void) const {
        return posX + height;
    }
    inline int GetMaxY(void) const {
        return posY + width;
    }

    int GetMapData(const Point& point, bool abs = true) const;
    bool IsValidPoint(const Point& point, bool abs = true) const;

    void GetExits(ExitArray&) const;

    bool SpaceHasFlag(int flag, const Point& point, bool abs = true) const;
    bool PathHasFlag(int flag, const PointList& points, bool abs = true) const;

    bool SpaceIsWalkable(const Point& point, bool abs = true) const;
    bool SpaceIsWalkableForExit(const Point& point, bool abs = true) const;
    bool SpaceHasLineOfSight(const Point& point, bool abs = true) const;
    bool SpaceIsInvalid(const Point& point, bool abs = true) const;
    bool SpaceIsThickenedWall(const Point& point, bool abs = true) const;

    bool PathIsWalkable(const PointList& points, bool abs = true) const;
    bool PathHasLineOfSight(const PointList& points, bool abs = true) const;
};

} // namespace Mapping
