
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

    room_t* alloc_room(player_t* player, const std::string& name, uint32_t map_id);

    void get_room_list(std::vector<room_t*>& room_list);

    uint32_t enter_room(player_t* player, uint32_t room_id);

    private:

    uint32_t room_idx_;
    std::map<int, room_t *> room_list_;
};

#endif
