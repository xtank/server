
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
#include<algorithm>


void RoomUtils::send_room_update_msg(uint32_t roomid, uint32_t oper) {

    onlineproto::sc_notify_room_update sc_notify_room_update_;

    sc_notify_room_update_.set_oper(oper);
    room_t* room = g_room_manager->get_room_by_id(roomid);

    if (room == NULL && oper != 2) {
        return ;
    }

    if (room != NULL) {
        onlineproto::room_data_t* room_data = sc_notify_room_update_.mutable_room();
        DataProtoUtils::pack_room_data(room, room_data);
    }

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


void RoomUtils::player_levave_room(player_t* player) {

    if (player == NULL) {
        return;
    }

    if (player->status == kOutside) {
        return; 
    }


    uint32_t room_id = player->roomid;
    room_t* room = g_room_manager->get_room_by_id(room_id);
    if (room == NULL) {
        return ;
    }

    if (room->player_vec->size() == 1) {

        player->roomid = 0;
        player->status = kOutside;
        player->teamid = 0;

        send_player_update_msg(player);
        send_room_update_msg(room_id, kDelRoom);
        g_room_manager->dealloc_room(room);
    }
    else if (player->userid == room->owner_id) {
        for (uint32_t i = 0; i < room->player_vec->size(); i++) {
            if (room->player_vec->at(i) != NULL) {
                room->player_vec->at(i)->roomid = 0;
                room->player_vec->at(i)->status = kOutside;
                room->player_vec->at(i)->teamid = 0;

                send_player_update_msg(room->player_vec->at(i));
            }

            send_room_update_msg(room_id, kDelRoom);
            g_room_manager->dealloc_room(room);
        }
    }
    else {
        player->roomid = 0;
        player->status = kOutside;
        player->teamid = 0;
        send_player_update_msg(player);

        std::vector<player_t*>::iterator iter = find(room->player_vec->begin(), room->player_vec->end(), player);

        if (iter != room->player_vec->end()) {
            room->player_vec->erase(iter);
        }

        send_room_update_msg(room_id, kUpdateRoom);
    }
}


uint32_t RoomUtils::select_team_id(room_t* room) {

    if (room == NULL) {
        return 0;
    }
    //team 1 num
    uint32_t team1_num = 0;
    uint32_t team2_num = 0;
    for (uint32_t j = 0; j < room->player_vec->size(); j++) {
        if (room->player_vec->at(j)->teamid == 1) {
            team1_num ++;
        } else {
            team2_num ++;
        }
    }

    if (team1_num < team2_num) {
        return 1;
    } else {
        return 2;
    }

    return 0;
}
