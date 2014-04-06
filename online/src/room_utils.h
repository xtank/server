
#ifndef ROOM_UTILS_H
#define ROOM_UTILS_H

#include <libtaomee/project/types.h>
#include <google/protobuf/message.h>

struct player_t;

class RoomUtils {
public:
    static void send_room_update_msg(uint32_t roomid, uint32_t oper);

    static void send_player_update_msg(player_t* player);

    static void player_levave_room(player_t* player);
};

#endif
