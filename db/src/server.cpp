#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtaomee++/utils/strings.hpp>
#include <dbser/benchapi.h>
#include <dbser/mysql_iface.h>
#include <dbser/db_macro.h>

#include "proto_header.h"
#include "base_info_table.h"

#include "server.h"
#include "cmd_procs.h"
#include "db_cmd.h"
#include "proto_processor.h"
#include "base_info_table.h"
ProtoProcessor* processor;
BaseInfoTable* g_base_info_table;

mysql_interface* g_db = NULL;
server_config_t g_config;
//static char hex_buf[65536];

void pb_log_handler(google::protobuf::LogLevel level,
        const char *filename, int line, const std::string &message)
{
    static const char *level_names[] = {"INFO", "WARNING", "ERROR", "FATAL" };
    ERROR_LOG("[%s %s:%d] %s",
            level_names[level], filename, line, message.c_str());
    DEBUG_LOG("[%s %s:%d] %s",
            level_names[level], filename, line, message.c_str());
}

#define CONFIG_READ_INTVAL(data, name) \
    do { \
        int ret = -1; \
        ret = config_get_intval(#name, ret); \
        if (ret == -1) { \
            ERROR_LOG("not find config '%s'", #name); \
            return -1; \
        } \
        data.name = ret; \
        DEBUG_LOG("set config '%s' = %d", \
#name, data.name); \
    } while (0);

#define CONFIG_READ_STRVAL(data, name) \
    do { \
        const char *conf_str = NULL; \
        conf_str = config_get_strval(#name); \
        if (conf_str == NULL) { \
            ERROR_LOG("not find config '%s'", #name); \
            return -1; \
        } \
        strncpy(data.name, conf_str, sizeof(data.name) - 1); \
        DEBUG_LOG("set config '%s' = '%s'",  \
#name, data.name); \
    } while (0);

int init_tables(mysql_interface* db)
{
    if (db == NULL) {
        return -1;    
    }

    g_base_info_table = new BaseInfoTable(db);
    return 0;
}

int init_cmd_procs()
{
    processor->register_cmd(db_cmd_create_role, new CreateRoleCmdProcessor());
    processor->register_cmd(db_cmd_get_login_info, new GetLoginInfoCmdProcessor());

    return 0;
}

int handle_init(
        int argc, 
        char **argv, 
        int proc_type)
{
#ifdef ENABLE_TRACE_LOG
#ifdef USE_TLOG
    SET_LOG_LEVEL(tlog_lvl_trace);
    SET_TIME_SLICE_SECS(86400);
#endif
#endif

    if (proc_type == PROC_WORK) {

        SetLogHandler(pb_log_handler);

        processor = new ProtoProcessor();

        CONFIG_READ_STRVAL(g_config, mysql_host);
        CONFIG_READ_STRVAL(g_config, mysql_user);
        CONFIG_READ_STRVAL(g_config, mysql_passwd);
        CONFIG_READ_STRVAL(g_config, mysql_charset);
        CONFIG_READ_INTVAL(g_config, mysql_port);

        // 初始化db连接
        g_db = new mysql_interface(
                g_config.mysql_host,
                g_config.mysql_user,
                g_config.mysql_passwd, 
                g_config.mysql_port,
                NULL, 
                g_config.mysql_charset);

        if (!g_db) {
            ERROR_LOG("init db failed"); 
            return -1;
        }

        g_db->set_is_log_debug_sql(1);
        g_db->set_is_only_exec_select_sql(0);

        //char sql_buf[512];
        int ret = 0;
        //int affected_rows = 0;

        // 设置字符集
        ret = mysql_set_character_set(&g_db->handle, g_config.mysql_charset);
        if (ret != 0) {
            ERROR_LOG("ste charset '%s' failed", g_config.mysql_charset); 
            return -1;
        }

        // 设置默认是自动提交事务
        //ret = mysql_autocommit(&g_db->handle, 1);
        //if (ret != 0) {
            //ERROR_LOG("set autocommit true failed\n"); 
            //return -1;
        //}

        // 初始化表
        ret = init_tables(g_db);
        if (ret != 0) {
            ERROR_LOG("init tables failed"); 
            return -1;
        }

        // 初始化协议函数
        ret = init_cmd_procs();
        if (ret != 0) {
            ERROR_LOG("init cmd procs failed");        
            return -1;
        }

        return 0;
    } else if (proc_type == PROC_CONN) {
        processor = new ProtoProcessor();
        return 0;
    }

    return 0;
}

int handle_input(
        const char* recv_buf, 
        int recv_len, 
        const skinfo_t* skinfo)
{
    return processor->get_pkg_len(recv_buf, recv_len);
}

int handle_process(
        char *recv, 
        int recv_len, 
        char** send_buf, 
        int* send_len, 
        const skinfo_t*)
{
    //bin2hex(hex_buf, recv, recv_len, sizeof(hex_buf));
    //DEBUG_LOG("I %d[%s]", recv_len, hex_buf);

    int ret = processor->process(recv, recv_len, send_buf, send_len);

    //bin2hex(hex_buf, *send_buf, *send_len, sizeof(hex_buf));
    //DEBUG_LOG("O[%s]", hex_buf);

    return ret;
}

void handle_fini(
        int proc_type)
{
    if (proc_type == PROC_WORK) {
        delete processor; 
    } else if (proc_type == PROC_CONN) {
        delete processor; 
    }
}

int handle_filter_key(const char *buf, int len, uint32_t *key)
{
    if (config_get_intval("dont_use_filter_key", 0) == 1) {
        return -1;
    }
    blitz_proto_header_t *header = (blitz_proto_header_t *)buf;
    *key = header->uid;
    return 0;
}
