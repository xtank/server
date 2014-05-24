extern "C" {
#include <libtaomee/timer.h>
}
#include <libtaomee++/utils/strings.hpp>
#include <async_serv/net_if.h>

#include "battle_processor.h"
#include "global_data.h"
#include "player.h"
#include "room.h"
#include "battle.h"
#include "room_manager.h"
#include "battle_manager.h"
#include "player_manager.h"
#include "data_proto_utils.h"
#include "room_utils.h"

#include "proto/client/cli_errno.h"
#include "proto/client/attr_type.h"

#include<vector>

//=======================================================================
int InsideStartCmdProcessor::proc_pkg_from_client(
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

    //将room当中成员设为开始状态
    uint32_t roomid = player->roomid;
    room_t* room = g_room_manager->get_room_by_id(roomid);
    if (room == NULL) {
        return send_err_to_player(player, player->wait_cmd, cli_err_sys_err);
    }

    for (uint32_t i = 0; i < room->player_vec->size(); i++) {
        player_t* player = room->player_vec->at(i);
        player->status = kInsideBusy;
        RoomUtils::send_player_update_msg(player);
    }

    //将房间状态设为开始
    room->status = kRoomBusy;
    RoomUtils::send_room_update_msg(roomid, kUpdateRoom);

    //战斗管理类 创建战斗
    Battle* battle = g_battle_manager->alloc_battle(room);
    if (battle == NULL) {
        return send_err_to_player(player, player->wait_cmd, cli_err_sys_err);
    }

    onlineproto::battle_data_t* battle_data = cli_out_.mutable_battle_data();
    DataProtoUtils::pack_battle_data(battle, battle_data);

    return send_msg_to_player(player, player->wait_cmd, cli_out_);
}

int InsideStartCmdProcessor::proc_pkg_from_serv(
        player_t* player, const char* body, int bodylen)
{
    return 0;
}



