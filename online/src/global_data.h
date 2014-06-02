
#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

#include <map>
#include <stdint.h>
#include <vector>
#include <set>
#include <string>

extern "C" {
#include <libtaomee/list.h>
#include <libtaomee/project/types.h>
}
class ProtoProcessor;
class PlayerManager;
class RoomManager;
class BattleManager;
class Service;
class map_conf_mgr_t; 
struct server_config_t;
struct tank_conf_t;

struct timer_head_t{
    list_head_t timer_list;
};

// 玩家管理器
extern PlayerManager* g_player_manager;

// 命令处理器
extern ProtoProcessor* g_proto_processor;

// 房间管理器
extern RoomManager* g_room_manager;

// 战斗管理器
extern BattleManager* g_battle_manager;

//switch server
//extern Service* g_switch;

extern Service* g_dbproxy;

// online服务相关配置 
extern server_config_t g_server_config;
extern timer_head_t g_reconnect_timer;

extern map_conf_mgr_t g_map_conf_mgr;

extern std::map<uint32_t, tank_conf_t>  g_tank_conf;

#endif
