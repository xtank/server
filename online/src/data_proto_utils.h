/**
 * @file data_proto_utils.h
 * @brief 打包data.proto公用协议结构体工具
 * @author lemon lemon@taomee.com
 * @version 1.0.0
 * @date 2013-07-03
 */

#ifndef DATA_PROTO_UTILS_H
#define DATA_PROTO_UTILS_H

#include "common.h"

#include "proto/client/data.pb.h"

struct room_t;
struct player_t;


class DataProtoUtils
{
public:
    static int pack_room_data(const room_t* room,
            onlineproto::room_data_t* room_data);

     static int pack_player_data(const player_t* player,
            onlineproto::player_data_t* player_data);
    
};

#endif
