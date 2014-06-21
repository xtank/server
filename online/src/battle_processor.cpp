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

    //将房间状态设为开始
    room->status = kRoomBusy;
    RoomUtils::send_room_update_msg(roomid, kUpdateRoom);

    //战斗管理类 创建战斗
    Battle* battle = g_battle_manager->alloc_battle(room);
    if (battle == NULL) {
        return send_err_to_player(player, player->wait_cmd, cli_err_sys_err);
    }

    for (uint32_t i = 0; i < room->player_vec->size(); i++) {
        player_t* player = room->player_vec->at(i);
        player->status = kInsideBusy;
        player->battleid = battle->battleid();
        RoomUtils::send_player_update_msg(player);
    }


    onlineproto::battle_data_t* battle_data = cli_out_.mutable_battle_data();
    DataProtoUtils::pack_battle_data(battle, battle_data);

    g_room_manager->send_msg_to_room_player(player->wait_cmd, player->roomid, cli_out_);

    return 0;
}

int InsideStartCmdProcessor::proc_pkg_from_serv(
        player_t* player, const char* body, int bodylen)
{
    return 0;
}

int BattleReadyCmdProcessor::proc_pkg_from_client(
        player_t* player, const char* body, int bodylen)
{
    cli_in_.Clear();
    cli_out_.Clear();

    if (parse_message(body, bodylen, &cli_in_)) {
        return send_err_to_player(player, 
                player->wait_cmd, cli_err_proto_format_err);
    }

    Battle* battle = g_battle_manager->get_battle_by_id(player->battleid);
    if (battle == NULL) {
        return send_err_to_player(player, player->wait_cmd, cli_err_sys_err);
    }

    battle_player_t* battle_player = battle->get_battle_player_by_id(player->userid);
    if (battle_player == NULL) {
        return send_err_to_player(player, player->wait_cmd, cli_err_sys_err);
    }

    battle_player->status = kResourceReady;

    //若所有玩家加载完成 进入战斗
    bool team1_ready = true;
    bool team2_ready = true;

    const std::vector<battle_player_t>* team1 = battle->get_team1();
    const std::vector<battle_player_t>* team2 = battle->get_team2();
    if (team1 == NULL || team2 == NULL) {
        return send_err_to_player(player, player->wait_cmd, cli_err_sys_err);
    }

    for (uint32_t i = 0; i < team1->size(); i++) {
        if (team1->at(i).status != kResourceReady) {
            team1_ready = false;
            break;
        }
    }

    for (uint32_t i = 0; i < team2->size(); i++) {
        if (team2->at(i).status != kResourceReady) {
            team2_ready = false;
            break;
        }
    }

    send_msg_to_player(player, player->wait_cmd, cli_out_);

    if (team1_ready && team2_ready) {
        onlineproto::sc_notify_battle_start msg;
        g_room_manager->send_msg_to_room_player(cli_cmd_cs_notify_battle_start, player->roomid, msg);
    }

    return 0;
}

int BattleReadyCmdProcessor::proc_pkg_from_serv(
        player_t* player, const char* body, int bodylen)
{
    return 0;
}

int TankMoveCmdProcessor::proc_pkg_from_client(
        player_t* player, const char* body, int bodylen)
{
    cli_in_.Clear();
    cli_out_.Clear();

    if (parse_message(body, bodylen, &cli_in_)) {
        return send_err_to_player(player, 
                player->wait_cmd, cli_err_proto_format_err);
    }


    uint32_t start_x = cli_in_.start_x();
    uint32_t start_y = cli_in_.start_y();
    uint32_t start_time = cli_in_.start_time();
    uint32_t dir = cli_in_.dir();


    cli_out_.set_start_x(start_x);
    cli_out_.set_start_y(start_y);
    cli_out_.set_start_time(start_time);
    cli_out_.set_dir(dir);
    cli_out_.set_userid(player->userid);
    
    //向房间内广播移动协议
    g_room_manager->send_msg_to_room_player(player->wait_cmd, player->roomid, cli_out_);

    return 0;
}

int TankMoveCmdProcessor::proc_pkg_from_serv(
        player_t* player, const char* body, int bodylen)
{
    return 0;
}

int TankMoveStopCmdProcessor::proc_pkg_from_client(
        player_t* player, const char* body, int bodylen)
{
    cli_in_.Clear();
    cli_out_.Clear();

    if (parse_message(body, bodylen, &cli_in_)) {
        return send_err_to_player(player, 
                player->wait_cmd, cli_err_proto_format_err);
    }

    uint32_t stop_x = cli_in_.stop_x();
    uint32_t stop_y = cli_in_.stop_y();
   
    cli_out_.set_userid(player->userid);
    cli_out_.set_stop_x(stop_x);
    cli_out_.set_stop_y(stop_y);


    //向房间内广播移动协议
    g_room_manager->send_msg_to_room_player(player->wait_cmd, player->roomid, cli_out_);

    return 0;
}

int TankMoveStopCmdProcessor::proc_pkg_from_serv(
        player_t* player, const char* body, int bodylen)
{
    return 0;
}


int TankFireCmdProcessor::proc_pkg_from_client(
        player_t* player, const char* body, int bodylen)
{
    cli_in_.Clear();
    cli_out_.Clear();

    if (parse_message(body, bodylen, &cli_in_)) {
        return send_err_to_player(player, 
                player->wait_cmd, cli_err_proto_format_err);
    }

    Battle* battle = g_battle_manager->get_battle_by_id(player->battleid);
    if (battle == NULL) {
        return send_err_to_player(player, player->wait_cmd, cli_err_sys_err);
    }

    battle_player_t* battle_player = battle->get_battle_player_by_id(player->userid);
    if (battle_player == NULL) {
        return send_err_to_player(player, player->wait_cmd, cli_err_sys_err);
    }

    cli_out_.set_atkuserid(cli_in_.atkuserid());
    cli_out_.set_atk_x(cli_in_.atk_x());
    cli_out_.set_atk_y(cli_in_.atk_y());
    cli_out_.set_fire_time(cli_in_.fire_time());
    cli_out_.set_fire_dir(cli_in_.fire_dir());
    cli_out_.set_fire_id(cli_in_.fire_id());
   

    //向房间内广播移动协议
    g_room_manager->send_msg_to_room_player(player->wait_cmd, player->roomid, cli_out_);

    return 0;
}

int TankFireCmdProcessor::proc_pkg_from_serv(
        player_t* player, const char* body, int bodylen)
{
    return 0;
}

int TankHitCmdProcessor::proc_pkg_from_client(
        player_t* player, const char* body, int bodylen)
{
    cli_in_.Clear();
    cli_out_.Clear();

    if (parse_message(body, bodylen, &cli_in_)) {
        return send_err_to_player(player, 
                player->wait_cmd, cli_err_proto_format_err);
    }

    uint32_t atkUserId = cli_in_.atkuserid();
    uint32_t defUserId = cli_in_.defuserid();

    if (player->userid != defUserId) {
        return send_err_to_player(player, 
                player->wait_cmd, cli_err_sys_err);
    }

    Battle* battle = g_battle_manager->get_battle_by_id(player->battleid);
    if (battle == NULL) {
        return send_err_to_player(player, player->wait_cmd, cli_err_sys_err);
    }

    battle_player_t* atk_battle_player = battle->get_battle_player_by_id(atkUserId);
    if (atk_battle_player == NULL) {
        return send_err_to_player(player, player->wait_cmd, cli_err_sys_err);
    }

    battle_player_t* def_battle_player = battle->get_battle_player_by_id(defUserId);
    if (def_battle_player == NULL) {
        return send_err_to_player(player, player->wait_cmd, cli_err_sys_err);
    }

    uint32_t atk = atk_battle_player->attack;

    def_battle_player->hp = (def_battle_player->hp - atk < 0) ? 0:def_battle_player->hp - atk;

    cli_out_.set_atkuserid(cli_in_.atkuserid());
    cli_out_.set_atk_x(cli_in_.atk_x());
    cli_out_.set_atk_y(cli_in_.atk_y());
    cli_out_.set_fire_id(cli_in_.fire_id());
    cli_out_.set_defuserid(cli_in_.defuserid());
    cli_out_.set_def_x(cli_in_.def_x());
    cli_out_.set_def_y(cli_in_.def_y());
    cli_out_.set_hp(atk);

    //向房间内广播移动协议
    g_room_manager->send_msg_to_room_player(player->wait_cmd, player->roomid, cli_out_);

    return 0;
}

int TankHitCmdProcessor::proc_pkg_from_serv(
        player_t* player, const char* body, int bodylen)
{
    return 0;
}





