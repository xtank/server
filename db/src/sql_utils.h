
#ifndef SQL_UTILS_H
#define SQL_UTILS_H

#include <string>
#include <google/protobuf/message.h>
#include <libtaomee/project/types.h>

class SQLUtils
{
public:
    static int gen_replace_sql_from_proto(
            userid_t userid,
            const char* table_name,
            const google::protobuf::Message& message,
            std::string& sql,
            MYSQL* mysql);

    static int gen_update_sql_from_proto(
            userid_t userid,
            const char* table_name, 
            const google::protobuf::Message& message,
            std::string& sql,
            MYSQL* mysql);

    static int gen_insert_sql_from_proto(
            userid_t userid,
            const char* table_name,
            const google::protobuf::Message& message,
            std::string& sql,
            MYSQL* mysql);

};

#endif
