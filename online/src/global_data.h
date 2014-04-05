
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
// 玩家管理器
extern PlayerManager* g_player_manager;

// 命令处理器
extern ProtoProcessor* g_proto_processor;

// online服务相关配置 
//extern server_config_t g_server_config;

#endif
