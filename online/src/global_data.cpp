
#include <map>

#include "global_data.h"
#include "player_manager.h"
#include "room_manager.h"
#include "server.h"
#include "service.h"
#include "map_conf.h"


PlayerManager* g_player_manager;

ProtoProcessor* g_proto_processor;

RoomManager* g_room_manager;
server_config_t g_server_config;


//dbproxy server
Service* g_dbproxy;

timer_head_t g_reconnect_timer;

map_conf_mgr_t g_map_conf_mgr;
