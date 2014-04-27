
#include <stdlib.h>
#include <google/protobuf/message.h>

extern "C" {
#include <libtaomee/log.h>
#include <libtaomee/random/random.h>
}

#include "room.h"
#include "room_utils.h"
#include "player.h"
#include "room_manager.h"


RoomManager::RoomManager()
{
    room_idx_ = 0;
}

RoomManager::~RoomManager()
{

}

room_t* RoomManager::get_room_by_id(int id)
{
    std::map<int, room_t*>::iterator it;

    it = room_list_.find(id);

    if (it == room_list_.end()) {
        return NULL; 
    } else {
        return it->second; 
    }
}

room_t* RoomManager::alloc_room(player_t* player, const std::string& name, uint32_t mapid)
{
    room_t* room = (room_t *)calloc(1, sizeof(room_t));

    if (room == NULL) {
        KERROR_LOG(0, "out of memory");
        return NULL; 
    }

    init_room(room);

    room_idx_++;
    room->room_id = room_idx_;
    //room->name = name;
    strcpy(room->name, name.c_str());
    room->owner_id = player->userid;
    room->map_id = mapid;

    room->player_vec->push_back(player);


    room_list_[room_idx_] = room;

    player->roomid = room_idx_;
    player->status = kInsideFree;
    player->teamid = RoomUtils::select_team_id(room);

    return room;
}

void RoomManager::get_room_list(std::vector<room_t*>& room_list)
{
    std::map<int, room_t *> ::iterator ptr; 

    room_list.clear();
    for (ptr = room_list_.begin(); ptr != room_list_.end(); ptr++) {
            room_list.push_back(ptr->second);
    }
}

uint32_t RoomManager::enter_room(player_t* player, uint32_t room_id) {
    room_t* room = get_room_by_id(room_id);
    if (room == NULL) {
        return cli_err_room_not_exist;
    }

    if (player->status != kOutside) {
        return cli_err_player_already_in_other_room;
    }

    player->status = kInsideFree;
    player->roomid = room_id;
    player->teamid = RoomUtils::select_team_id(room);

    room->player_vec->push_back(player);

    return 0;
}

void RoomManager::dealloc_room(room_t* room)
{
    std::map<int, room_t*>::iterator it1;

    it1 = room_list_.find(room->room_id);

    if (it1 != room_list_.end()) {
        room_list_.erase(it1); 
    }

    uninit_room(room);

    delete room;
}



