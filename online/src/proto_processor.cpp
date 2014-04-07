#include <libtaomee++/utils/strings.hpp>

#include "proto_processor.h"
#include "proto.h"
#include "global_data.h"
#include "player_manager.h"


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
            TRACE_LOG("Policy Req [%s] Received, Rsp [%s] Sent", request, response);
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

    TRACE_LOG("len = %u, seq = %u, cmd = %u ret= %u", 
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
