#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "common.h"
#include "proto.h"
//#include <libtaomee/timer.h>
//#include <libtaomee/project/types.h>

#include <map>

enum room_player_status_t{
    kOutside = 0,
    kInsideFree = 1,
    kInsideReady = 2,
    kInsideBusy = 3,
};

struct player_t
{
    userid_t userid; // 米米号
    int fd; // 客户端fd
    uint32_t seqno; // 协议序列号
    struct fdsession* fdsession; // async_serv 和客户端通信session
    uint32_t wait_cmd; // 客户端请求命令号

    //room info
    uint32_t roomid;
    room_player_status_t status;

    /* 缓存session放在最后一个字段 */
    char session[4096];  // 缓存
};


// 发送二进制buf流到客户端
int send_buf_to_player(player_t* player, uint32_t cmd, const char* body, int bodylen);

// 发送protobuf到客户端
int send_msg_to_player(player_t* player, uint32_t cmd, const google::protobuf::Message& message);

// 发送错误码到客户端
int send_err_to_player(player_t* player, uint32_t cmd, const uint32_t ret);

// 初始化player内部相关内存
int init_player(player_t* player);

// 玩家离开服务器
int player_leave_server(player_t* player);

// 反初始化player内部相关内存
int uninit_player(player_t* player);

#endif
