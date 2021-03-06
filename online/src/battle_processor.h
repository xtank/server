#ifndef BATTLE_PROCESSOR_H
#define BATTLE_PROCESSOR_H

#include "proto/client/room.pb.h"
#include "proto/client/online.pb.h"
#include "proto/client/player.pb.h"
#include "proto/client/battle.pb.h"
#include "cmd_processor_interface.h"

#include "global_data.h"
#include "player.h"
#include "proto.h"
#include "macro_utils.h"

class InsideStartCmdProcessor : public CmdProcessorInterface
{
public:

    int proc_pkg_from_client(player_t* player, const char* body, int bodylen);
    int proc_pkg_from_serv(player_t* player, const char* body, int bodylen);

private:
    onlineproto::cs_inside_start cli_in_;  
    onlineproto::sc_inside_start cli_out_;  
};

class BattleReadyCmdProcessor : public CmdProcessorInterface
{
public:

    int proc_pkg_from_client(player_t* player, const char* body, int bodylen);
    int proc_pkg_from_serv(player_t* player, const char* body, int bodylen);

private:
    onlineproto::cs_battle_ready cli_in_;  
    onlineproto::sc_battle_ready cli_out_;  
};

class TankMoveCmdProcessor : public CmdProcessorInterface
{
public:

    int proc_pkg_from_client(player_t* player, const char* body, int bodylen);
    int proc_pkg_from_serv(player_t* player, const char* body, int bodylen);

private:
    onlineproto::cs_tank_move cli_in_;  
    onlineproto::sc_tank_move cli_out_;  
};

class TankMoveStopCmdProcessor : public CmdProcessorInterface
{
public:

    int proc_pkg_from_client(player_t* player, const char* body, int bodylen);
    int proc_pkg_from_serv(player_t* player, const char* body, int bodylen);

private:
    onlineproto::cs_tank_move_stop cli_in_;  
    onlineproto::sc_tank_move_stop cli_out_;  
};

class TankFireCmdProcessor : public CmdProcessorInterface
{
public:

    int proc_pkg_from_client(player_t* player, const char* body, int bodylen);
    int proc_pkg_from_serv(player_t* player, const char* body, int bodylen);

private:
    onlineproto::cs_tank_fire cli_in_;  
    onlineproto::sc_tank_fire cli_out_;  
};

class TankHitCmdProcessor : public CmdProcessorInterface
{
public:

    int proc_pkg_from_client(player_t* player, const char* body, int bodylen);
    int proc_pkg_from_serv(player_t* player, const char* body, int bodylen);

private:
    onlineproto::cs_tank_hit cli_in_;  
    onlineproto::sc_tank_hit cli_out_;  
};

#endif
