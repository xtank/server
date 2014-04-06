#include <async_serv/service.h>
#include "global_data.h"
#include "player_manager.h"
#include "room_manager.h"
#include "proto_processor.h"
#include "login_processor.h"
#include "common.h"

extern "C" {
#include <libtaomee/log.h>
#include <libtaomee/conf_parser/config.h>
#include <libtaomee/timer.h>
}
#include <libtaomee++/utils/strings.hpp>

static int init_processors();

extern "C" int  init_service(int isparent)
{
    g_proto_processor = new ProtoProcessor();

	if (!isparent) {

        setup_timer();
        init_processors();
        g_player_manager = new PlayerManager(); 
        g_room_manager = new RoomManager();
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
    //g_proto_processor->proc_pkg_from_serv(fd, data, len);
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
    return 0;
}

//const char *gen_full_path(const char *base_path, const char *file_name)
//{
    //static char full_path[1024];
    //snprintf(full_path, sizeof(full_path), "%s/%s", base_path, file_name);
    //return full_path;
//}

bool load_configs()
{
    //bool succ = (1
            //&& load_xmlconf(gen_full_path(g_server_config.conf_path, "titles.xml"), load_title_config) == 0
           //);

    //return succ;
    return 0;
}

int init_connections()
{
    return 0;
}

int start_function_timers()
{
    return 0;
}
