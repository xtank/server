
#include <stdlib.h>
#include <google/protobuf/message.h>

extern "C" {
#include <libtaomee/log.h>
#include <libtaomee/random/random.h>
}

#include "room.h"
#include "room_manager.h"


RoomManager::RoomManager()
{
    room_idx = 0;
}

RoomManager::~RoomManager()
{

}

room_t* RoomManager::get_room_by_id(int id)
{
    std::map<int, room_t*>::iterator it;

    it = rooms_list_.find(id);

    if (it == rooms_list_.end()) {
        return NULL; 
    } else {
        return it->second; 
    }
}

room_t* RoomManager::alloc_room(player_t* player, const std::string& name)
{
    room_t* room = (room_t *)calloc(1, sizeof(room_t));

    if (room == NULL) {
        KERROR_LOG(0, "out of memory");
        return NULL; 
    }

    room_idx++;
    room->room_id = room_idx;
    room->name = name;

    room->player_vec.push_back(player);


    rooms_list_[room_idx] = room;
    
    return room;
}

