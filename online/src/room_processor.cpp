
extern "C" {
#include <libtaomee/timer.h>
}
#include <libtaomee++/utils/strings.hpp>
#include <async_serv/net_if.h>

#include "room_processor.h"
#include "global_data.h"
#include "player.h"
#include "room.h"
#include "room_manager.h"
#include "player_manager.h"
#include "data_proto_utils.h"
#include "room_utils.h"

#include "proto/client/cli_errno.h"
#include "proto/client/attr_type.h"

#include<vector>

//=======================================================================
int GetRoomListCmdProcessor::proc_pkg_from_client(
        player_t* player, const char* body, int bodylen)
{
    cli_in_.Clear();
    cli_out_.Clear();

    if (parse_message(body, bodylen, &cli_in_)) {
        return send_err_to_player(player, 
                player->wait_cmd, cli_err_proto_format_err);
    }

    std::vector<room_t*> room_list;
    g_room_manager->get_room_list(room_list);

    for (uint32_t i = 0; i < room_list.size(); i++) {
        onlineproto::room_data_t* room = cli_out_.add_room_list();
        DataProtoUtils::pack_room_data(room_list[i], room);
    }

    return send_msg_to_player(player, player->wait_cmd, cli_out_);
}

int GetRoomListCmdProcessor::proc_pkg_from_serv(
        player_t* player, const char* body, int bodylen)
{
    return 0;
}

int EnterRoomCmdProcessor::proc_pkg_from_client(
        player_t* player, const char* body, int bodylen)
{
    cli_in_.Clear();
    cli_out_.Clear();

    if (parse_message(body, bodylen, &cli_in_)) {
        return send_err_to_player(player, 
                player->wait_cmd, cli_err_proto_format_err);
    }

    uint32_t roomid = cli_in_.room_id();
       
    uint32_t ret = g_room_manager->enter_room(player, roomid);
    if (ret) {
        return send_err_to_player(player, player->wait_cmd, ret);
    }
    room_t* room = g_room_manager->get_room_by_id(roomid);
    if (room == NULL) {
         return send_err_to_player(player, 
                player->wait_cmd,  cli_err_room_not_exist);
    }

    //onlineproto::room_data_t* room_data = cli_out_.mutable_room();
    //DataProtoUtils::pack_room_data(room, room_data);
    cli_out_.set_roomid(roomid);

    RoomUtils::send_room_update_msg(roomid, kUpdateRoom);

    RoomUtils::send_player_update_msg(player);

    return send_msg_to_player(player, player->wait_cmd, cli_out_);
}

int EnterRoomCmdProcessor::proc_pkg_from_serv(
        player_t* player, const char* body, int bodylen)
{
    return 0;
}

int LeaveRoomCmdProcessor::proc_pkg_from_client(
        player_t* player, const char* body, int bodylen)
{
    cli_in_.Clear();
    cli_out_.Clear();

    if (parse_message(body, bodylen, &cli_in_)) {
        return send_err_to_player(player, 
                player->wait_cmd, cli_err_proto_format_err);
    }

    if (player->status == kOutside) {
        return send_msg_to_player(player, player->wait_cmd, cli_out_);
    }

    RoomUtils::player_levave_room(player);
       
    return send_msg_to_player(player, player->wait_cmd, cli_out_);
}

int LeaveRoomCmdProcessor::proc_pkg_from_serv(
        player_t* player, const char* body, int bodylen)
{
    return 0;
}

int InsideReadyCmdProcessor::proc_pkg_from_client(
        player_t* player, const char* body, int bodylen)
{
    cli_in_.Clear();
    cli_out_.Clear();

    if (parse_message(body, bodylen, &cli_in_)) {
        return send_err_to_player(player, 
                player->wait_cmd, cli_err_proto_format_err);
    }

    if (player->status != kInsideFree) {
        return send_err_to_player(player, player->wait_cmd, cli_err_sys_err);
    }

    player->status = kInsideReady;

    //RoomUtils::send_room_update_msg(player->roomid, kUpdateRoom);
    RoomUtils::send_player_update_msg(player);
      
    return send_msg_to_player(player, player->wait_cmd, cli_out_);
}

int InsideReadyCmdProcessor::proc_pkg_from_serv(
        player_t* player, const char* body, int bodylen)
{
    return 0;
}

int CreateRoomCmdProcessor::proc_pkg_from_client(
        player_t* player, const char* body, int bodylen)
{
    cli_in_.Clear();
    cli_out_.Clear();

    if (parse_message(body, bodylen, &cli_in_)) {
        return send_err_to_player(player, 
                player->wait_cmd, cli_err_proto_format_err);
    }

    uint32_t mapid = cli_in_.map_id();
    std::string name = cli_in_.name();

    room_t* room = g_room_manager->alloc_room(player, name, mapid);
    if (room == NULL) {
        return send_err_to_player(player, player->wait_cmd, cli_err_create_room_failed);
    }

    //onlineproto::room_data_t* room_data = cli_out_.mutable_room();
    //DataProtoUtils::pack_room_data(room, room_data);

    cli_out_.set_roomid(room->room_id);

    RoomUtils::send_room_update_msg(player->roomid, kAddRoom);
    RoomUtils::send_player_update_msg(player);
      
    return send_msg_to_player(player, player->wait_cmd, cli_out_);
}

int CreateRoomCmdProcessor::proc_pkg_from_serv(
        player_t* player, const char* body, int bodylen)
{
    return 0;
}

int CancelInsideReadyCmdProcessor::proc_pkg_from_client(
        player_t* player, const char* body, int bodylen)
{
    cli_in_.Clear();
    cli_out_.Clear();

    if (parse_message(body, bodylen, &cli_in_)) {
        return send_err_to_player(player, 
                player->wait_cmd, cli_err_proto_format_err);
    }

    if (player->status != kInsideReady) {
        return send_err_to_player(player, player->wait_cmd, cli_err_sys_err);
    }

    player->status = kInsideFree;

    //RoomUtils::send_room_update_msg(player->roomid, kUpdateRoom);
    RoomUtils::send_player_update_msg(player);
      
    return send_msg_to_player(player, player->wait_cmd, cli_out_);
}

int CancelInsideReadyCmdProcessor::proc_pkg_from_serv(
        player_t* player, const char* body, int bodylen)
{
    return 0;
}


