#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

extern "C" {
#include <libtaomee/crypt/qdes.h>
}
#include <libtaomee++/utils/strings.hpp>
#include <libtaomee++/utils/strings.hpp>
#include <libtaomee++/random/random.hpp> 
#include "cmd_procs.h"
#include "db_errno.h"
#include "base_info_table.h"
#include "server.h"

uint32_t CreateRoleCmdProcessor::process(
        userid_t userid, const std::string& req_body, std::string& ack_body)
{
    m_cs_create_role.Clear();    

    if (!m_cs_create_role.ParseFromString(req_body)) {
        return db_err_proto_format_err;
    }

    uint32_t ret = 0;

    ret = g_base_info_table->create_role(userid, m_cs_create_role);

    if (ret != db_err_succ) {
        return ret; 
    }

    return db_err_succ; 
}

uint32_t GetLoginInfoCmdProcessor::process(
        userid_t userid, const std::string& req_body, std::string& ack_body)
{
    m_sc_get_login_info.Clear(); 

    // 获取base_info
    dbproto::base_info_t* base_info = m_sc_get_login_info.mutable_base_info();
    uint32_t ret = g_base_info_table->get_info(userid, *base_info);
    if (ret) {
        return ret; 
    }

    m_sc_get_login_info.SerializeToString(&ack_body);

    return db_err_succ;
}


