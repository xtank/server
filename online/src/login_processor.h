#ifndef LOGIN_PROCESSOR_H
#define LOGIN_PROCESSOR_H

#include "proto/client/login.pb.h"
#include "proto/client/online.pb.h"
#include "cmd_processor_interface.h"

#include "global_data.h"
#include "player.h"
#include "proto.h"
#include "macro_utils.h"
#include "db/proto/dbproto.base_info.pb.h"
#include "db/proto/dbproto.login.pb.h"
#include "db/proto/db_cmd.h"

class LoginCmdProcessor : public CmdProcessorInterface
{
public:

    int proc_pkg_from_client(player_t* player, const char* body, int bodylen);
    int proc_pkg_from_serv(player_t* player, const char* body, int bodylen);

private:
    onlineproto::cs_enter_server cs_enter_server_;  
    onlineproto::sc_enter_server sc_enter_server_;

    dbproto::cs_get_login_info db_cs_get_login_info_;  
    dbproto::sc_get_login_info db_sc_get_login_info_;  
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

class CreateRoleCmdProcessor : public CmdProcessorInterface
{
public:

    int proc_pkg_from_client(player_t* player, const char* body, int bodylen);
    int proc_pkg_from_serv(player_t* player, const char* body, int bodylen);

private:
    onlineproto::cs_create_role cs_create_role_;  
    onlineproto::sc_create_role sc_create_role_;  

    dbproto::cs_create_role db_cs_create_role_;  
    dbproto::sc_create_role db_sc_create_role_;  
};

#endif
