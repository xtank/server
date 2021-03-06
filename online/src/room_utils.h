
#ifndef ROOM_UTILS_H
#define ROOM_UTILS_H

#include <libtaomee/project/types.h>
#include <google/protobuf/message.h>

struct player_t;
struct room_t;

class RoomUtils {
public:
    static void send_room_update_msg(uint32_t roomid, uint32_t oper);

    static void send_player_update_msg(player_t* player);

    static void send_player_update_msg_in_room(player_t* player);

    static void player_levave_room(player_t* player);

    static uint32_t select_team_id(room_t* room);

    static uint32_t select_seat_id(room_t* room, uint32_t teamid);
};

#endif
