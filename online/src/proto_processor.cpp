#include <libtaomee++/utils/strings.hpp>

#include "proto_processor.h"
#include "proto.h"
#include "global_data.h"
#include "player_manager.h"
#include "db/proto/db_errno.h"


ProtoProcessor::ProtoProcessor()
{
    memset(&default_player_, 0, sizeof(default_player_));
}

ProtoProcessor::~ProtoProcessor()
{
    cmd_processors_.clear();
}

int ProtoProcessor::register_command(
        uint32_t cmd,
        CmdProcessorInterface* processor)
{
    cmd_processors_[cmd] = processor; 

    return 0;
}

int ProtoProcessor::get_pkg_len(int fd, const void* avail_data, 
       int avail_len, int from)
{
    if (from == PROTO_FROM_CLIENT) {

        static char request[]  = "<policy-file-request/>";
        static char response[] = "<?xml version=\"1.0\"?>"
            "<cross-domain-policy>"
            "<allow-access-from domain=\"*\" to-ports=\"*\" />"
            "</cross-domain-policy>";

        if ((avail_len == sizeof(request)) && !memcmp(avail_data, request, sizeof(request))) {
            net_send(fd, response, sizeof(response));
            TRACE_TLOG("Policy Req [%s] Received, Rsp [%s] Sent", request, response);
            return 0;
        }

        if (avail_len < (int)sizeof(cli_proto_header_t)) {
            return 0; // continue to recv        
        } else {
            cli_proto_header_t* header = (cli_proto_header_t *)avail_data;

            if (header->len < (int)sizeof(cli_proto_header_t)) {
                ERROR_TLOG("too small pkg %u from client", header->len); 
                return -1;
            }

            if (header->len > 4096) {
                ERROR_TLOG("too large pkg %u from client", header->len); 
                return -1;
            }

            return header->len;
        }
    } else if (from == PROTO_FROM_SERV) {
    
        if (avail_len < (int)sizeof(db_proto_header_t)) {
            return 0; // continue to recv 
        } else {
            db_proto_header_t* header = (db_proto_header_t *)avail_data;

            if (header->len < (int)sizeof(db_proto_header_t)) {
                ERROR_TLOG("too small pkg %u from server", header->len); 
                return -1;
            }

            if (header->len > 1024 * 1024) {
                ERROR_TLOG("too large pkg %u from server", header->len); 
                return -1;
            }

            return header->len;
        }
    } else {

        ERROR_TLOG("get pkg len invalid from %d", from); 
        return -1;
    }
}

int ProtoProcessor::proc_pkg_from_client(void* data, int len,
        fdsession_t* fdsess, bool from_queue)
{
    cli_proto_header_t* header =reinterpret_cast<cli_proto_header_t *>(data);
    char* body = static_cast<char *>(data) + sizeof(cli_proto_header_t);
    int bodylen = header->len - sizeof(cli_proto_header_t);

    TRACE_TLOG("len = %u, seq = %u, cmd = %u ret= %u", 
            header->len, header->seq, header->cmd, header->ret);

    player_t* player = g_player_manager->get_player_by_fd(fdsess->fd);

    if (header->cmd != cli_cmd_cs_enter_srv) {

        if (!player) {
            ERROR_TLOG("MUST LOGIN FIRST");
            return -1;
        }
    } else {
        if (player) {
            KERROR_LOG(player->userid, "DOUBLE LOGIN"); 
            return -1;
        }
        player = &default_player_;
        player->userid = header->ret;
        player->fd = fdsess->fd;
        player->fdsession = fdsess;
        //player->seqno = header->seq;
    }

TRACE_TLOG("GET PKG FROM CLIENT [u:%u cmd:%d hexcmd:0x%04x]", player->userid, header->cmd, header->cmd);
DEBUG_TLOG("GET PKG FROM CLIENT [u:%u cmd:%d hexcmd:0x%04x]", player->userid, header->cmd, header->cmd);

    player->wait_cmd = header->cmd;
    player->seqno = header->seq;


    std::map<uint32_t, CmdProcessorInterface *>::iterator it;

    it = cmd_processors_.find(header->cmd);

    if (it == cmd_processors_.end()) {
        return send_err_to_player(player, player->wait_cmd, cli_err_cmd_not_find);
    }

    CmdProcessorInterface* processor = it->second;

    int ret = processor->proc_pkg_from_client(player, body, bodylen);

    if (ret != 0) {
        KERROR_LOG(player->userid, "PROC CLIENT PKG %u ERR: %d", header->cmd, ret); 
        return send_err_to_player(player, player->wait_cmd,
                cli_err_sys_err);
    }

    return 0;
}

void ProtoProcessor::proc_pkg_from_serv(int fd, void* data, int len)
{
    db_proto_header_t* header = static_cast<db_proto_header_t *>(data);
    const char* body = (char *)data + sizeof(*header);
    int bodylen = len - sizeof(*header);
   

    player_t* player;
    
    player = g_player_manager->get_player_by_fd(header->seq);

    if (!player) return;  

    bool need_match_wait_cmd = true;
    bool use_header_cmd = false;
    //switch (header->cmd) {
        //case home_cmd_notify_entered:
        //case home_cmd_notify_free_home:
        //case home_cmd_notify_visited:
        //case home_cmd_notify_walk:
        //case home_cmd_notify_come:
        //case home_cmd_notify_go:
        //case home_cmd_notify_pet_change:
            //need_match_wait_cmd = false;
            //use_header_cmd = true;
            //break;
////        case cache_cmd_ol_req_users_info:
////            need_match_wait_cmd = true;
////            use_header_cmd = true;
////            break;
        //default :
            //break;
    //}

    if (need_match_wait_cmd && header->cmd != player->wait_serv_cmd) {
        ERROR_TLOG("RECV NO MATCH PKG FROM SERVER [u:%u cmd:0x%04X wait_cmd:0x%04x ret:%d]", 
                player->userid, header->cmd, player->wait_serv_cmd, header->ret);
        return ; 
    }


    // 如果需要等待服务器cmd 且cmd相等 清除等待的命令号
    //if (need_match_wait_cmd) { //可以将等待的定时器删除
        //player->wait_serv_cmd = 0;
        //if (player->db_request_timer) {
            //REMOVE_TIMER(player->db_request_timer); 
            //player->db_request_timer = NULL;
        //}
    //}

    std::map<uint32_t, CmdProcessorInterface *>::iterator it;

    if (use_header_cmd) {
        it = cmd_processors_.find(header->cmd);
    } else {
        it = cmd_processors_.find(player->wait_cmd);
    }

    if (it == cmd_processors_.end()) {
        ERROR_TLOG("SERVER RET CMD NOT FOUND[u:%u cmd:0x%04X ret:%d]", 
                player->userid, header->cmd, header->ret);  
        send_err_to_player(player, player->wait_cmd, cli_err_sys_err);
        return ;
    }

    CmdProcessorInterface* processor = it->second;

    //temp_info_t* temp_info = &player->temp_info;
    //struct timeval db_response_time = *get_now_tv();
    //struct timeval db_diff_time = {0};
    //timersub(&db_response_time, &(temp_info->db_request_time), &db_diff_time);
//TRACE_TLOG("GET PKG FROM SERVER [u:%u cmd:0x%04X ret:%d len:%u cli-cmd:%d cost: %d.%06d]", 
            //player->userid, header->cmd, header->ret, header->len, player->wait_cmd,
            //db_diff_time.tv_sec, db_diff_time.tv_usec);

    if (header->ret == 0) {
        int ret = processor->proc_pkg_from_serv(player, body, bodylen);
        if (ret != 0) {
            KERROR_LOG(player->userid, "PROC SERVER CMD ERR [cmd:0x%04x ret:%d]", header->cmd, ret); 
            send_err_to_player(player, player->wait_cmd, ret);
        }
    } else {
        
        //if (header->ret == db_err_sys_err
                //|| header->ret == 2002 [> sql有误<] 
                //|| header->ret == 1017 [> 系统超时<] 
                //|| header->ret == family_err_sys_err
                //|| header->ret == btl_err_sys_err) {
            //asynsvr_send_warning_msg("SysErr", 
                    //header->uid, header->cmd, 1, "");
        //}

        KERROR_LOG(player->userid, "SERVER RET CMD ERR[cmd:0x%04X ret:%d]", 
                header->cmd, header->ret);
        uint32_t cli_errno = processor->proc_errno_from_serv(player, header->ret);
        //if (header->ret == 2004) {
            //cli_errno = cli_err_user_not_found;
        //}
        if (cli_errno) {
            send_err_to_player(player, player->wait_cmd, cli_errno);
        }
    }

}

CmdProcessorInterface* ProtoProcessor::get_processor(uint32_t cmd)
{
    std::map<uint32_t, CmdProcessorInterface *>::iterator it;

    it = cmd_processors_.find(cmd);

    if (it == cmd_processors_.end()) {
        return NULL;
    } else {
        return it->second; 
    }
}
