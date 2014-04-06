
#include "room_utils.h"
#include "global_data.h"
#include "player.h"
#include "room.h"
#include "room_manager.h"
#include "player_manager.h"
#include "data_proto_utils.h"

#include "proto/client/cli_errno.h"
#include "proto/client/attr_type.h"

#include "proto/client/room.pb.h"
#include "proto/client/online.pb.h"
#include "proto/client/player.pb.h"

#include<vector>


void RoomUtils::send_room_update_msg(uint32_t roomid, uint32_t oper) {

    onlineproto::sc_notify_room_update sc_notify_room_update_;

    sc_notify_room_update_.set_oper(oper);
    room_t* room = g_room_manager->get_room_by_id(roomid);

    if (room == NULL) {
        return ;
    }

    onlineproto::room_data_t* room_data = sc_notify_room_update_.mutable_room();
    DataProtoUtils::pack_room_data(room, room_data);

    g_player_manager->send_msg_to_all_player(cli_cmd_cs_notify_room_update, sc_notify_room_update_);
}

void RoomUtils::send_player_update_msg(player_t* player) {

    if (player == NULL) {
        return ;
    }
    
    onlineproto::sc_notify_player_update sc_notify_player_update_;
    onlineproto::player_data_t* player_data = sc_notify_player_update_.mutable_player();

    player_data->set_userid(player->userid);
    player_data->set_status(player->status);

    g_player_manager->send_msg_to_all_player(cli_cmd_cs_notify_player_update, sc_notify_player_update_);
}
