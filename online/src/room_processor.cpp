
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

    RoomUtils::send_room_update_msg(roomid, 0);

    RoomUtils::send_player_update_msg(player);

    return send_msg_to_player(player, player->wait_cmd, cli_out_);
}

int EnterRoomCmdProcessor::proc_pkg_from_serv(
        player_t* player, const char* body, int bodylen)
{
    return 0;
}


