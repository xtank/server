
#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include <map>
#include <vector>
#include <string>

#include <libtaomee/project/types.h>
#include <google/protobuf/message.h>

struct room_t;

class RoomManager
{
public:
    RoomManager();
    ~RoomManager();

    room_t* get_room_by_id(int id);

    room_t* alloc_room(player_t* player, const std::string& name);

    private:

    uint32_t room_idx;
    std::map<int, room_t *> rooms_list_; // fd映射player
};

#endif
