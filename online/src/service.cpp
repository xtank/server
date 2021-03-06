
#include <string>
#include <async_serv/net_if.h>

extern "C" {
#include <libtaomee/log.h>
#include <libtaomee/timer.h>
}
#include <libtaomee++/proto/proto_base.h>
#include <libtaomee++/proto/proto_util.h>

#include "service.h"
#include "player.h"
#include "proto.h"
#include "timer_procs.h"
#include "global_data.h"

#define CHECK_ONLINE(header) \
    do { \
        if (player && player->is_login == false \
            && player->wait_cmd != cli_cmd_cs_enter_srv) { \
            asynsvr_send_warning_msg("offline send to svr",  \
                    header->uid, header->cmd, 1, ""); \
            WARN_TLOG("offine player[%u] send to svr stack: '%s'", \
                    player->userid, stack_trace().c_str()); \
        } \
    } while (0)

Service::Service(const std::string& service_name)
{
    service_name_ = service_name;
    fd_ = -1;
    this->is_connect_by_name_ = true;
}

Service::Service(const std::string& service_name, const std::string& ipaddr, in_addr_t port)
{
    this->service_name_ = service_name;
    this->ipaddr_ = ipaddr;
    this->port_ = port;
    fd_ = -1;
    this->is_connect_by_name_ = false;
}

Service::~Service()
{
    if (fd_ >= 0) {
        close_svr(fd_);
    }
}

int Service::send_buf(player_t* player, userid_t userid, uint16_t cmd, 
        const char* body, int bodylen, uint32_t ret)
{
    if (fd_ < 0) {
        ERROR_TLOG("service '%s' not available", service_name_.c_str());    
        return -1;
    }



    db_proto_header_t* header = (db_proto_header_t* )send_buf_;

    if (!body || bodylen < 0) {
        bodylen = 0; 
    }

    header->len = sizeof(*header) + bodylen;
    header->seq = player ? player->fd : 0;
    header->cmd = cmd;
    header->ret = ret;
    header->uid = userid;

    if (bodylen) {
        if (sizeof(send_buf_) - sizeof(*header) < (uint32_t)bodylen) {
            asynsvr_send_warning_msg("ServiceSendBuf OutBound", 
                    header->uid, header->cmd, 1, "");
            return -1;
        }

        memcpy(send_buf_ + sizeof(*header), body, bodylen);
    }

    int err = net_send(fd_, send_buf_, header->len);

    if (err != 0) {
        ERROR_TLOG("net_send to service '%s' failed", 
                service_name_.c_str());
        return -1; 
    }
 
TRACE_TLOG("SEND BUF TO SERVICE [u:%u, cmd:%u, hex_cmd:0x%04x, service:%s, len(%u+%u)=%u]",
        player ?player->userid :0, cmd, cmd, service_name().c_str(),
        sizeof(*header), bodylen, header->len);

    //CHECK_ONLINE(header);
    if (player) {
        player->serv_cmd = cmd;
        player->wait_serv_cmd = cmd;
        //if (player->db_request_timer) {
            //WARN_TLOG("Player[%u] Add DB_Request_Timer[ReqCmd:%u] While Another Timer Already Added",
                    //player->userid, cmd);
        //} else {
            //player->db_request_timer = ADD_TIMER_EVENT_EX(player, 
                    //kTimerTypeCheckDbTimeout,
                    //(void*)(player->userid), 
                    //get_now_tv()->tv_sec + kTimerIntervalCheckDbTimeout);
            //player->temp_info.db_request_time = *get_now_tv();
        //}
    }

    return 0;
}

int Service::send_msg(player_t* player, userid_t userid, uint16_t cmd,
        const google::protobuf::Message& message, uint32_t ret)
{
    if (fd_ < 0) {
        ERROR_TLOG("service '%s' not available", service_name_.c_str());    
        return -1;
    }

    db_proto_header_t* header = (db_proto_header_t* )send_buf_;

    header->len = sizeof(*header) + message.ByteSize();
    header->seq = player ? player->fd : 0;
    header->cmd = cmd;
    header->ret = ret;
    header->uid = userid;

    if (header->len > sizeof(send_buf_)) {
        ERROR_TLOG("too large pkg size %u cmd %u send to '%s'",
                header->len, cmd, service_name_.c_str()); 
        return -1;
    }

    if (!message.SerializePartialToArray(
            send_buf_ + sizeof(*header), 
            sizeof(send_buf_) - sizeof(*header))) {
        ERROR_TLOG("serialize message 0x'%04x' failed", cmd);
        return -1;
    }

    int err = net_send(fd_, send_buf_, header->len);

    if (err < 0) {
        ERROR_TLOG("net_send to service '%s' failed", 
                service_name_.c_str());
        return -1; 
    }

TRACE_TLOG("Send Msg To Service Ok: [u:%u cmd:%u hex_cmd:0X%04X service:%s len(%u+%u)=%u]\nmsg:%s\n[%s]",
            player ?player->userid :0, cmd, cmd, service_name().c_str(),
            sizeof(*header), message.ByteSize(), header->len,
            message.GetTypeName().c_str(), message.Utf8DebugString().c_str());
 
    //CHECK_ONLINE(header);
    if (player) {
        player->serv_cmd = cmd;
        player->wait_serv_cmd = cmd;
        //if (player->db_request_timer) {
            //WARN_TLOG("Player[%u] Add DB_Request_Timer[ReqCmd:%u] While Another Timer Already Added",
                    //player->userid, cmd);
        //} else {
            //player->db_request_timer = ADD_TIMER_EVENT_EX(player, 
                    //kTimerTypeCheckDbTimeout,
                    //(void*)(player->userid), 
                    //get_now_tv()->tv_sec + kTimerIntervalCheckDbTimeout);
            //player->temp_info.db_request_time = *get_now_tv();
        //}
    }

    //log
    //if ((cmd >= db_cmd_base_cmd_begin && cmd <= db_cmd_base_cmd_end)
            //|| (cmd >= db_cmd_common_cmd_begin && cmd <= db_cmd_common_cmd_end)
            //|| (cmd >= db_cmd_log_cmd_begin && cmd <= db_cmd_log_cmd_end)) {
        //StatInfo stat_info;
        //stat_info.add_info("db_called", "count");
        //stat_info.add_op(StatInfo::op_sum, "db_called");
        //g_stat_logger->log("后台监控", "调用db次数", "", "", stat_info);
    //}

    return 0;
}

int Service::send_cmsg(player_t* player, userid_t userid, 
        uint16_t cmd, Cmessage *msg, uint32_t ret)
{
    if (fd_ < 0) {
        ERROR_TLOG("service '%s' not available", service_name_.c_str());    
        return -1;
    }

	db_proto_header_t header;

	header.len = sizeof(header);
	header.seq = player ? player->fd : 0;
	header.cmd = cmd;
	header.ret = ret;
	header.uid = userid;

    //CHECK_ONLINE((&header));
    if (player) {
        player->serv_cmd = cmd;
        player->wait_serv_cmd = cmd;
        //if (player->db_request_timer) {
            //WARN_TLOG("Player[%u] Add DB_Request_Timer[ReqCmd:%u] While Another Timer Already Added",
                    //player->userid, cmd);
        //} else {
            //player->db_request_timer = ADD_TIMER_EVENT_EX(player, 
                    //kTimerTypeCheckDbTimeout,
                    //(void*)(player->userid), 
                    //get_now_tv()->tv_sec + kTimerIntervalCheckDbTimeout);
            //player->temp_info.db_request_time = *get_now_tv();
        //}
    }

TRACE_TLOG("Send CMsg To Service Ok: [u:%u cmd:%u, cmd:0X%04X service:%s len(%u+?)=?]",
            player ?player->userid :0, cmd, cmd, service_name().c_str(), header.len);

	return net_send_msg(fd_, (char *)&header, msg);
}

int Service::close()
{
    fd_ = -1;

    return 0;
}

void Service::on_connect_callback(int fd, void* args)
{
    Service* service = (Service *)args;

    DEBUG_LOG("on connect callback");
    KERROR_LOG(0, "on connect callback"); 

    if (fd > 0) {
        DEBUG_LOG("connect to service %s OK",
                service->service_name().c_str());
        service->set_fd(fd);

        // 如果是switch服务 注册online到switch
        //if (service == g_switch) {
            //svr_proto_os_register_online_info();
        //} else if (service == g_home_svr) {
            //同步online玩家
        //    sync_online_players_to_home_svr();
        //}
    } else {
        ERROR_TLOG("connect to service %s Failed", 
                service->service_name().c_str());

        KERROR_LOG(0, "connect to service %s Failed", 
                service->service_name().c_str());


        ADD_TIMER_EVENT_EX(&g_reconnect_timer, 
                kTimerTypeReconnectServiceTimely, 
                service, 
                get_now_tv()->tv_sec + kTimerIntervalReconnectServiceTimely);

        asynsvr_send_warning_msg("ConnErr", 0, 0, 0, service->service_name().c_str());
    }
}

int Service::connect()
{
    if (is_connect_by_name_) {
        return asyn_connect_to_service(service_name_.c_str(), 0,
                65536, Service::on_connect_callback, this);    
    } else {
        return asyn_connect_to_svr(ipaddr_.c_str(), port_, 65536, Service::on_connect_callback, this);
    }
}

