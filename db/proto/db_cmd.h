
#ifndef DB_CMD_H
#define DB_CMD_H

enum db_cmd_t
{
    db_cmd_base_cmd_begin = 0x8200,
    //////////////////////////////////// game_db_00 ~ 99 号段 ////////////////////////////////
    /* 基础信息命令号段 0x8200 ~ 0x83FF */
    db_cmd_create_role = 0x8300,
    db_cmd_get_login_info = 0x8301,
};


#endif
