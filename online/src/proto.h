
#ifndef PROTO_H
#define PROTO_H

struct cli_proto_header_t
{
    uint32_t len;
    uint32_t seq;
    uint32_t cmd;
    uint32_t ret;
    uint32_t checksum;
} __attribute__((packed));

struct banben_cli_proto_header_t
{
    uint32_t len;
    uint32_t seq;
    uint32_t cmd;
    uint32_t ret;
} __attribute__((packed));


struct db_proto_header_t
{
    uint32_t len;   
    uint32_t seq;
    uint16_t cmd;
    uint32_t ret;
    uint32_t uid;
} __attribute__((packed));

enum {
    db_cmd_login_with_verify_code = 0xA039,
    db_cmd_check_session = 0xA03A,
    db_cmd_is_active = 0xA029,
    db_cmd_add_session = 0xA122,
    db_cmd_user_logout = 0xA125,
    db_err_check_session_failed = 4431,
};

struct db_is_active_req_t
{
    uint8_t gameid;
} __attribute__((packed));

struct db_is_active_ack_t
{
    uint8_t is_active;
} __attribute__((packed));

struct db_add_session_req_t
{
    uint32_t gameid; 
    uint32_t ip;
} __attribute__((packed));

struct db_add_session_ack_t
{
    char session[16];
} __attribute__((packed));

struct db_check_session_req_t
{
    uint32_t from_game;
    char session[16];
    uint32_t del_flag;
    uint32_t to_game;
    uint32_t ip;
    uint16_t region;
    uint8_t enter_game;
    char tad[128];
} __attribute__((packed));

struct db_user_logout_req_t
{
    uint32_t gameid; 
    uint32_t login_time;
    uint32_t logout_time;
} __attribute__((packed));

struct db_login_with_verify_code_req_t
{
    char email[64];
    char passwd_md5_two[16];
    uint16_t verifyid;
    uint16_t region;
    uint16_t gameid;
    uint32_t ip;
    char verify_session[16];
    char verify_code[6];
    uint16_t login_channel;
    char login_promot_tag[128];
} __attribute__((packed));

#define DB_LOGIN_ACK_FLAG_SUCC 0
#define DB_LOGIN_ACK_FLAG_WRONG_PASSWD_TOO_MUCH 1
#define DB_LOGIN_ACK_FLAG_LOGIN_IN_DIFF_CITY 2
#define DB_LOGIN_ACK_FLAG_LOGIN_IN_DIFF_CITY_TOO_MUCH 3
#define DB_LOGIN_ACK_FLAG_WRONG_VERIFY_CODE 4
#define DB_LOGIN_ACK_FLAG_ACCOUNT_UNSAFE 5

struct db_login_with_verify_code_ack_t
{
    uint32_t flag; 
} __attribute__((packed));

#endif
