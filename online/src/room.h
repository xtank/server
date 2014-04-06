#ifndef __ROOM_H__
#define __ROOM_H__

#include "common.h"
#include "proto.h"
//#include <libtaomee/timer.h>
//#include <libtaomee/project/types.h>

#include <map>
#include <vector>
#include <string>

enum room_status_t{
    kRoomFree = 0,
    kRoomFull = 1,
    kRoomReady = 2, 
    kRoomBusy = 3, 
};

enum room_oper_t{
    kUpdateRoom = 0,
    kAddRoom = 1,
    kDelRoom = 2
};

struct player_t;

struct room_t
{
    uint32_t room_id;
    userid_t owner_id;
    uint32_t map_id;
    room_status_t status;
    std::string name;
    std::vector<player_t*> player_vec;
};

#endif
