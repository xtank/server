
#ifndef CMD_PROCESSOR_INTERFACE_H
#define CMD_PROCESSOR_INTERFACE_H

extern "C" {
#include <libtaomee/log.h>
}
#include <google/protobuf/message.h>
#include <string>
#include <stdint.h>
#include <libtaomee++/proto/Ccmd_map.h>

struct player_t;

class CmdProcessorInterface
{
public:
    virtual ~CmdProcessorInterface() { }

public:

    /**
     * @brief  proc_pkg_from_client 响应client请求包
     *
     * @param player 客户端缓存
     * @param body 请求包体
     * @param bodylen 请求包体长度
     *
     * @return 0 处理成功 -1 处理失败
     */
    virtual int proc_pkg_from_client(
            player_t* player,
            const char* body,
            int bodylen) = 0;

    /**
     * @brief  proc_pkg_from_serv 响应dbserver返回包
     *
     * @param player 客户端缓存
     * @param body 返回包体
     * @param bodylen 返回包体长度
     *
     * @return 0 处理成功 -1 处理失败
     */
    virtual int proc_pkg_from_serv(
            player_t* player,
            const char* body,
            int bodylen) = 0;

};

inline int parse_message(
        const char* body, int bodylen, 
        google::protobuf::Message* message)
{
    message->Clear();

    std::string name = message->GetTypeName();
    if (!message->ParseFromArray(body, bodylen)) {
        std::string errstr = message->InitializationErrorString();
        ERROR_LOG("PARSE MSG '%s' failed, err = '%s'", 
                name.c_str(), errstr.c_str());
        return -1; 
    }

    std::string debug_str = message->Utf8DebugString();
    TRACE_TLOG("PARSE MSG:'%s' ok\nMSG:\n[%s]", 
            name.c_str(), debug_str.c_str());

    DEBUG_TLOG("PARSE MSG:'%s' ok\nMSG:\n[%s]", 
            name.c_str(), debug_str.c_str());


    return 0;
}

#endif
