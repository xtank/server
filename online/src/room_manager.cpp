
#include <stdlib.h>
#include <google/protobuf/message.h>

extern "C" {
#include <libtaomee/log.h>
#include <libtaomee/random/random.h>
}

#include "room.h"
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

    room_idx_++;
    room->room_id = room_idx_;
    room->name = name;
    room->owner_id = player->userid;
    room->map_id = mapid;

    room->player_vec.push_back(player);


    room_list_[room_idx_] = room;
    
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
    room->player_vec.push_back(player);

    return 0;
}


