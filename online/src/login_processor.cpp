
extern "C" {
#include <libtaomee/timer.h>
}
#include <libtaomee++/utils/strings.hpp>
#include <async_serv/net_if.h>

#include "login_processor.h"
#include "global_data.h"
#include "player.h"
#include "proto.h"
#include "player_manager.h"

#include "proto/client/cli_errno.h"
#include "proto/client/attr_type.h"
//=======================================================================
int LoginCmdProcessor::proc_pkg_from_client(
        player_t* player_temp, const char* body, int bodylen)
{
    cs_enter_server_.Clear();
    sc_enter_server_.Clear();

    if (parse_message(body, bodylen, &cs_enter_server_)) {
        return send_err_to_player(player_temp, 
                player_temp->wait_cmd, cli_err_proto_format_err);
    }

    player_temp->userid = cs_enter_server_.userid();

     // 查找该米米号是否在服务器中，关闭连接
    player_t* exist_player = g_player_manager->get_player_by_userid(player_temp->userid);

    if (exist_player) {
        close_client_conn(exist_player->fdsession->fd);
    }

    player_t* player_new = g_player_manager->alloc_player(
            player_temp->fd, player_temp->userid);

    if (!player_new) {
        KERROR_LOG(player_temp->userid, "Alloc memory for player %u failed",
                player_temp->userid);
        return send_err_to_player(player_temp, player_temp->wait_cmd, cli_err_sys_err);
    }

    init_player(player_new);

    player_new->userid = player_temp->userid;
    player_new->fd = player_temp->fd;
    player_new->fdsession = player_temp->fdsession;
    player_new->wait_cmd = player_temp->wait_cmd;
    player_new->seqno = player_temp->seqno;

    sc_enter_server_.set_userid(player_new->userid);
   
    return send_msg_to_player(player_new, player_new->wait_cmd, sc_enter_server_);
}

int LoginCmdProcessor::proc_pkg_from_serv(
        player_t* player, const char* body, int bodylen)
{
    return 0;
}

int KeepLiveCmdProcessor::proc_pkg_from_client(
        player_t* player, const char* body, int bodylen)
{
    cs_keep_live_.Clear();
    sc_keep_live_.Clear();

    if (parse_message(body, bodylen, &cs_keep_live_)) {
        return send_err_to_player(player, 
                player->wait_cmd, cli_err_proto_format_err);
    }

    uint32_t time = cs_keep_live_.time();

    sc_keep_live_.set_time(time);
    sc_keep_live_.set_server_time(NOW());
   
    return send_msg_to_player(player, player->wait_cmd, sc_keep_live_);
}

int KeepLiveCmdProcessor::proc_pkg_from_serv(
        player_t* player, const char* body, int bodylen)
{
    return 0;
}


