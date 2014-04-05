#include <assert.h>
#include <async_serv/net_if.h>
extern "C" {
#include <libtaomee/log.h>
#include <libtaomee/timer.h>
}
#include <libtaomee++/utils/strings.hpp>
#include <libtaomee++/proto/proto_base.h>
#include <libtaomee++/proto/proto_util.h>

#include <openssl/rc4.h>



#include "player.h"
static char send_buf[65536 * 32];

int send_buf_to_player(player_t* player, uint32_t cmd, const char* body, int bodylen)
{
    cli_proto_header_t* header = (cli_proto_header_t *)(send_buf);
    if (!body) {
        bodylen = 0; 
    } else {
        if (sizeof(send_buf) - sizeof(*header) < (uint32_t)bodylen) {
            asynsvr_send_warning_msg("PlayerSendBuf OutBound", 
                    player->userid, header->cmd, 1, "");
            return -1;
        }
        memcpy(send_buf + sizeof(*header), body, bodylen);
        //encrypt_msg((unsigned char *)send_buf + sizeof(*header), bodylen,
                //player->userid);
    }

    header->len = sizeof(*header) + bodylen;
    header->seq = 0;
    header->cmd = cmd;
    header->ret = 0;
    header->checksum = 0;

TRACE_TLOG("Send Buf To Player Ok: [u:%u cmd:%d hex_cmd:0X%04X len(%u+%u)=%u]",
            player ?player->userid :0, cmd, cmd, 
            sizeof(*header), bodylen, header->len);

    //header->checksum = Utils::fold_binary(
            //(const uint8_t *)(send_buf + sizeof(*header)),
            //bodylen);


    return send_pkg_to_client(player->fdsession, send_buf, header->len);
}

int send_msg_to_player(player_t* player, uint32_t cmd, const google::protobuf::Message& message)
{
    cli_proto_header_t* header = (cli_proto_header_t *)(send_buf);

    if (sizeof(send_buf) < sizeof(*header) + message.ByteSize()) {
        KERROR_LOG(player->userid, "send buf size %lu not enough proto %lu",
                sizeof(send_buf), sizeof(*header) + message.ByteSize());
        return -1;
    }

    message.SerializeToArray(send_buf + sizeof(*header), 
            sizeof(send_buf) - sizeof(*header));

    header->len = sizeof(*header) + message.ByteSize();
    header->seq = player->seqno;
    header->cmd = cmd;
    header->ret = 0;

DEBUG_TLOG("SEND PKG TO CLIENT [u:%u cmd:%d hexcmd:0x%04x]", player->userid, header->cmd, header->cmd);

    return send_pkg_to_client(player->fdsession, send_buf, header->len);
}

int send_err_to_player(player_t* player, uint32_t cmd, const uint32_t ret)
{
    cli_proto_header_t* header = (cli_proto_header_t *)send_buf;

    header->len = sizeof(*header);
    header->seq = player->seqno;
    header->cmd = cmd;
    header->ret = ret;
    header->checksum = 0;

TRACE_TLOG("Send Err To Player Ok: [u:%u cmd:%d hex_cmd:0X%04X err:%d len=%u]",
            player ?player->userid :0, cmd, cmd, ret, header->len);

    return send_pkg_to_client(player->fdsession, send_buf, header->len);
}

// 初始化player内部相关内存
int init_player(player_t* player)
{
    // 初始化地图链表
    return 0;
}

// 玩家离开服务器
int player_leave_server(player_t* player)
{
    return 0;
}

// 反初始化player内部相关内存
int uninit_player(player_t* player)
{
    return 0;
}


