
#ifndef USER_INFO_PROCS_H
#define USER_INFO_PROCS_H

#include "cmd_processor_interface.h"
#include "dbproto.base_info.pb.h"
#include "dbproto.login.pb.h"

class CreateRoleCmdProcessor : public CmdProcessorInterface
{
public:
    uint32_t process(
            userid_t userid, const std::string& req_body, std::string& ack_body);
private:
    dbproto::cs_create_role m_cs_create_role;
};

class GetLoginInfoCmdProcessor : public CmdProcessorInterface
{
public:
    uint32_t process(
            userid_t userid, const std::string& req_body, std::string& ack_body);
private:
    dbproto::sc_get_login_info m_sc_get_login_info;
};

#endif
