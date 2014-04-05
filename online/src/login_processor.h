#ifndef LOGIN_PROCESSOR_H
#define LOGIN_PROCESSOR_H

#include "proto/client/login.pb.h"
#include "proto/client/online.pb.h"
#include "cmd_processor_interface.h"

#include "global_data.h"
#include "player.h"
#include "proto.h"
#include "macro_utils.h"

class LoginCmdProcessor : public CmdProcessorInterface
{
public:

    int proc_pkg_from_client(player_t* player, const char* body, int bodylen);
    int proc_pkg_from_serv(player_t* player, const char* body, int bodylen);

private:
    onlineproto::cs_enter_server cs_enter_server_;  
    onlineproto::sc_enter_server sc_enter_server_;
};

class KeepLiveCmdProcessor : public CmdProcessorInterface
{
public:

    int proc_pkg_from_client(player_t* player, const char* body, int bodylen);
    int proc_pkg_from_serv(player_t* player, const char* body, int bodylen);

private:
    onlineproto::cs_keep_live cs_keep_live_;  
    onlineproto::sc_keep_live sc_keep_live_;
};

#endif
