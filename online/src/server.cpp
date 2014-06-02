#include <async_serv/service.h>
#include "global_data.h"
#include "player_manager.h"
#include "room_manager.h"
#include "proto_processor.h"
#include "login_processor.h"
#include "battle_processor.h"
#include "room_processor.h"
#include "battle_manager.h"
#include "common.h"
#include "server.h"
#include "service.h"
#include "timer_procs.h"
#include "room_utils.h"
//#include "xmlutils.h"
#include "xml_configs.h"

extern "C" {
#include <libtaomee/log.h>
#include <libtaomee/conf_parser/config.h>
#include <libtaomee/timer.h>
}
#include <libtaomee++/utils/strings.hpp>

#define STRCPY_SAFE(buf, str) \
    do { \
        int buflen = sizeof(buf); \
        buf[buflen - 1] = '\0';\
        strncpy(buf, str, buflen - 1); \
    } while (0);


#define CONFIG_READ_INTVAL(data, name) \
    do { \
        int ret = -1; \
        ret = config_get_intval(#name, ret); \
        if (ret == -1) { \
            ERROR_LOG("not find config '%s'", #name); \
            return -1; \
        } \
        data.name = ret; \
    } while (0);

#define CONFIG_READ_STRVAL(data, name) \
    do { \
        const char *conf_str = NULL; \
        conf_str = config_get_strval(#name); \
        if (conf_str == NULL) { \
            ERROR_LOG("not find config '%s'", #name); \
            return -1; \
        } \
        STRCPY_SAFE(data.name, conf_str); \
    } while (0);

static int init_processors();
static int init_connections();
static int start_function_timers();
static bool load_configs();

extern "C" int  init_service(int isparent)
{
    g_proto_processor = new ProtoProcessor();

	if (!isparent) {

        SET_LOG_LEVEL((tlog_lvl_t)/*tlog_lvl_trace*/config_get_intval("log_level", 6));
        SET_TIME_SLICE_SECS(86400);

        srand(time(NULL));
        srandom(time(NULL));

        init_processors();
        g_player_manager = new PlayerManager(); 
        g_room_manager = new RoomManager();
        g_battle_manager = new BattleManager();

        INIT_LIST_HEAD(&g_reconnect_timer.timer_list);
        setup_timer();
        register_timers();

        memset(&g_server_config, 0, sizeof(g_server_config));
        CONFIG_READ_STRVAL(g_server_config, dbproxy_name);
        CONFIG_READ_STRVAL(g_server_config, conf_path);

        // 初始化网络连接
        if (init_connections() != 0) {
            KERROR_LOG(0, "init server connections failed"); 
            return -1;
        }

        // 加载配置文件
        if (!load_configs()) {
            return -1;
        }

        // 启动业务逻辑定时器 
        if (start_function_timers() != 0) {
            KERROR_LOG(0, "start function timers failed"); 
            return -1;
        }

	}

	return 0;
}

/**
* @brief AsyncServer框架要求实现的接口之一。 
*
*/
extern "C" int  fini_service(int isparent)
{
    delete g_proto_processor;

	if (!isparent) {

	}

	return 0;
}

/**
* @brief AsyncServer框架要求实现的接口之一。 
*
*/
extern "C" void proc_events()
{
    handle_timer();
}

/**
* @brief AsyncServer框架要求实现的接口之一。 
*
*/
extern "C" int get_pkg_len(int fd, const void* avail_data, int avail_len, int isparent)
{
    if (isparent) {
        return g_proto_processor->get_pkg_len(fd, avail_data, avail_len, PROTO_FROM_CLIENT); 
    } else {
        return g_proto_processor->get_pkg_len(fd, avail_data, avail_len, PROTO_FROM_SERV); 
    }
}

/**
* @brief AsyncServer框架要求实现的接口之一。 
*
*/
extern "C" int  proc_pkg_from_client(void* data, int len, fdsession_t* fdsess)
{
    return g_proto_processor->proc_pkg_from_client(data, len, fdsess, false);
}

/**
* @brief AsyncServer框架要求实现的接口之一。 
*
*/
extern "C" void proc_pkg_from_serv(int fd, void* data, int len)
{
    g_proto_processor->proc_pkg_from_serv(fd, data, len);
}

/**
* @brief AsyncServer框架要求实现的接口之一。 
*
*/
extern "C" void on_client_conn_closed(int fd)
{
    player_t* player = g_player_manager->get_player_by_fd(fd);                                                                                          
    if (player) {
        player_leave_server(player);
        uninit_player(player);
        g_player_manager->dealloc_player(player);
        RoomUtils::player_levave_room(player);
    }
}

extern "C" int before_reload(int isparent)
{
    return 0;
}

extern "C" int reload_global_data()
{
    return 0;
}


/**
* @brief AsyncServer框架要求实现的接口之一。 
*
*/
extern "C" void proc_mcast_pkg(const void* data, int len)
{
}


/**
* @brief AsyncServer框架要求实现的接口之一。 
*
*/
extern "C" void on_fd_closed(int fd)
{
}

int init_processors()
{
    g_proto_processor->register_command(cli_cmd_cs_enter_srv, new LoginCmdProcessor());
    g_proto_processor->register_command(cli_cmd_cs_keep_live, new KeepLiveCmdProcessor());
    g_proto_processor->register_command(cli_cmd_cs_create_role, new KeepLiveCmdProcessor());

    //ROOM
    g_proto_processor->register_command(cli_cmd_cs_get_room_list, new GetRoomListCmdProcessor());
    g_proto_processor->register_command(cli_cmd_cs_enter_room, new EnterRoomCmdProcessor());
    g_proto_processor->register_command(cli_cmd_cs_leave_room, new LeaveRoomCmdProcessor());
    g_proto_processor->register_command(cli_cmd_cs_inside_ready, new InsideReadyCmdProcessor());
    g_proto_processor->register_command(cli_cmd_cs_create_room, new CreateRoomCmdProcessor());
    g_proto_processor->register_command(cli_cmd_cs_cancel_inside_ready, new CancelInsideReadyCmdProcessor());

    g_proto_processor->register_command(cli_cmd_cs_select_tank, new SelectTankCmdProcessor());
    g_proto_processor->register_command(cli_cmd_cs_select_team, new SelectTeamCmdProcessor());


    g_proto_processor->register_command(cli_cmd_cs_inside_start, new InsideStartCmdProcessor());
    g_proto_processor->register_command(cli_cmd_cs_battle_ready, new BattleReadyCmdProcessor());

    return 0;
}

const char *gen_full_path(const char *base_path, const char *file_name)
{
    static char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", base_path, file_name);
    return full_path;
}

bool load_configs()
{
    bool succ = (1
            && load_xmlconf(gen_full_path(g_server_config.conf_path, "tank.xml"), load_tank_config) == 0
           );

    return succ;
}

int init_connections()
{
    // 初始化dbproxy
    g_dbproxy = new Service(std::string(g_server_config.dbproxy_name));
    ADD_TIMER_EVENT_EX(&g_reconnect_timer, 
            kTimerTypeReconnectServiceTimely, 
            g_dbproxy,
            get_now_tv()->tv_sec + kTimerIntervalReconnectServiceTimely); 

    return 0;
}

int start_function_timers()
{
    return 0;
}
