#include <dbser/mysql_iface.h>
#include <dbser/db_macro.h>
#include "base_info_table.h"
#include "db_errno.h"
#include "utils.h"
#include "sql_utils.h"

BaseInfoTable::BaseInfoTable(mysql_interface* db) 
    : CtableRoute(db, "game_db", "base_info_table", "userid")
{
}

uint32_t BaseInfoTable::create_role(
        userid_t userid,
        const dbproto::cs_create_role& cs_create_role)
{
    char nick[32 + 1] = {0};
    char nick_mysql[2 * sizeof(nick) + 1] = {0};

    strncpy(nick, cs_create_role.nick().c_str(), sizeof(nick) - 1);
    set_mysql_string(nick_mysql, nick, strlen(nick));

    GEN_SQLSTR(this->sqlstr,
            " INSERT INTO %s "
            " (userid, nick, create_time) "
            " VALUES (%u, '%s', NOW()) ",
            this->get_table_name(userid),
            userid, 
            nick_mysql
            );

    return this->exec_insert_sql(this->sqlstr, db_err_role_already_exist);
}

uint32_t BaseInfoTable::get_info(
        userid_t userid,
        dbproto::base_info_t& base_info)
{
    GEN_SQLSTR(this->sqlstr,
            " SELECT nick"
            " FROM %s "
            " WHERE userid = %u ",
            this->get_table_name(userid),
            userid);

    STD_QUERY_ONE_BEGIN(this->sqlstr, db_err_user_not_find)
        base_info.set_nick(row[0]);
    STD_QUERY_ONE_END();
}


