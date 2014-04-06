#ifndef LOGIN_PROCESSOR_H
#define LOGIN_PROCESSOR_H

#include "proto/client/room.pb.h"
#include "proto/client/online.pb.h"
#include "proto/client/player.pb.h"
#include "cmd_processor_interface.h"

#include "global_data.h"
#include "player.h"
#include "proto.h"
#include "macro_utils.h"

class GetRoomListCmdProcessor : public CmdProcessorInterface
{
public:

    int proc_pkg_from_client(player_t* player, const char* body, int bodylen);
    int proc_pkg_from_serv(player_t* player, const char* body, int bodylen);

private:
    onlineproto::cs_get_room_list cli_in_;  
    onlineproto::sc_get_room_list cli_out_;  
};

class EnterRoomCmdProcessor : public CmdProcessorInterface
{
public:

    int proc_pkg_from_client(player_t* player, const char* body, int bodylen);
    int proc_pkg_from_serv(player_t* player, const char* body, int bodylen);

private:
    onlineproto::cs_enter_room cli_in_;  
    onlineproto::sc_enter_room cli_out_;  

};

#endif
