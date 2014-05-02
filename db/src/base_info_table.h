
#ifndef BASE_INFO_TABLE_H
#define BASE_INFO_TABLE_H

extern "C" {
#include <libtaomee/project/types.h>
}
#include <dbser/mysql_iface.h>
#include <dbser/CtableRoute10x10.h>
#include "dbproto.base_info.pb.h"
#include "dbproto.data.pb.h"
#include "db_errno.h"
 
class BaseInfoTable : public CtableRoute
{
public:
    BaseInfoTable(mysql_interface* db);

    uint32_t create_role(
            userid_t userid,
            const dbproto::cs_create_role& cs_create_role);

    uint32_t get_info(
            userid_t userid,
            dbproto::base_info_t& base_info);

};


extern BaseInfoTable* g_base_info_table;

#endif

