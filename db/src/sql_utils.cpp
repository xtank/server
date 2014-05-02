
#include <map>
#include <vector>
#include <google/protobuf/descriptor.h>
#include <mysql/mysql.h>

extern "C" {
#include <libtaomee/log.h>
}

#include "sql_utils.h"

const std::string& to_str(uint32_t value, std::string& str)
{
    char num_buf[20]; 

    sprintf(num_buf, "%u", value);

    str.clear();
    str.assign(num_buf);

    return str;
}

std::string to_escape_string(MYSQL* mysql, const std::string& src)
{
    static char dest[65536];

    if (sizeof(dest) < src.size() * 2 + 1) {
        return ""; 
    }

    int dest_len = mysql_real_escape_string(mysql, dest, src.c_str(), src.size());

    return std::string(dest, dest_len);
}

int SQLUtils::gen_replace_sql_from_proto(
        userid_t userid,
        const char* table_name,
        const google::protobuf::Message& message,
        std::string& sql, 
        MYSQL* mysql)
{
    sql.clear();

    const google::protobuf::Reflection * msg_reflect = message.GetReflection();
    std::vector<const google::protobuf::FieldDescriptor *> fields;
    msg_reflect->ListFields(message, &fields);

    if (fields.size() == 0) {
        return 0;
    }

    std::string uid_str;
    to_str(userid, uid_str);

    sql.append("INSERT INTO "); 
    sql.append(table_name);
    sql.append(" (userid");

    std::vector<std::string> keys;
    std::vector<std::string> vals;

    std::vector<const google::protobuf::FieldDescriptor *>::iterator it;

    for (it = fields.begin(); it != fields.end(); it++) {

        const std::string& key = (*it)->name();
        std::string value;
        uint32_t n = 0;

        switch ((*it)->type()) {
            case google::protobuf::FieldDescriptor::TYPE_INT32:
                n = msg_reflect->GetInt32(message, *it);
                to_str(n, value);
                break;
            case google::protobuf::FieldDescriptor::TYPE_UINT32:
                n = msg_reflect->GetUInt32(message, *it);
                to_str(n, value);
                break;	
            case google::protobuf::FieldDescriptor::TYPE_STRING:
                value.append("'");
                value.append(to_escape_string(mysql, msg_reflect->GetString(message, *it)));
                value.append("'");
                break;
            case google::protobuf::FieldDescriptor::TYPE_BOOL:
                n = msg_reflect->GetBool(message, *it);
                to_str(n, value);
                break;
            default:
                ERROR_LOG("unsupport field type %u",
                        (*it)->type());
                return -1;
        }

        keys.push_back(key);
        vals.push_back(value);
    }

    for (uint32_t i = 0; i < keys.size(); i++) {

        sql.append(" , ");
        sql.append(keys[i]);
    }
    sql.append(") VALUES (");
    sql.append(uid_str);

    for (uint32_t i = 0; i < vals.size(); i++) {
    
        sql.append(" , ");
        sql.append(vals[i]);
    }
    sql.append(") ON DUPLICATE KEY UPDATE ");

    for (uint32_t i = 0; i < keys.size(); i++) {

        if (keys[i] == "userid") {
            continue; 
        }

        if (i != 0) {
            sql.append(" , "); 
        }
        sql.append(keys[i] + " = " + vals[i]); 
    }

    return 0;
}

int SQLUtils::gen_update_sql_from_proto(
        userid_t userid,
        const char* table_name, 
        const google::protobuf::Message& message,
        std::string& sql,
        MYSQL* mysql)
{
    sql.clear();

    const google::protobuf::Reflection * msg_reflect = message.GetReflection();
    std::vector<const google::protobuf::FieldDescriptor *> fields;
    msg_reflect->ListFields(message, &fields);

    if (fields.size() == 0) {
        return 0;
    }

    std::string uid_str;
    to_str(userid, uid_str);

    sql.append("UPDATE "); 
    sql.append(table_name);
    sql.append(" SET ");

    std::vector<const google::protobuf::FieldDescriptor *>::iterator it;

    int count = 0;
    for (it = fields.begin(); it != fields.end(); it++, count++) {

        const std::string& key = (*it)->name();
        std::string value;
        uint32_t n = 0;

        if (count != 0) {
            sql.append(" , ");  
        }

        switch ((*it)->type()) {
            case google::protobuf::FieldDescriptor::TYPE_INT32:
                n = msg_reflect->GetInt32(message, *it);
                to_str(n, value);
                break;
            case google::protobuf::FieldDescriptor::TYPE_UINT32:
                n = msg_reflect->GetUInt32(message, *it);
                to_str(n, value);
                break;	
            case google::protobuf::FieldDescriptor::TYPE_STRING:
                value.append("'");
                value.append(to_escape_string(mysql, msg_reflect->GetString(message, *it)));
                value.append("'");
                break;
            case google::protobuf::FieldDescriptor::TYPE_BOOL:
                n = msg_reflect->GetBool(message, *it);
                to_str(n, value);
                break;
            default:
                ERROR_LOG("unsupport field type %u",
                        (*it)->type());
                return -1;
        }

        sql.append(key + " = " + value);
    }

    sql.append(" WHERE userid = ");
    sql.append(uid_str);

    return 0;
}

int SQLUtils::gen_insert_sql_from_proto(
        userid_t userid,
        const char* table_name,
        const google::protobuf::Message& message,
        std::string& sql,
        MYSQL* mysql)
{
    sql.clear();

    const google::protobuf::Reflection * msg_reflect = message.GetReflection();
    std::vector<const google::protobuf::FieldDescriptor *> fields;
    msg_reflect->ListFields(message, &fields);

    if (fields.size() == 0) {
        return 0;
    }

    std::string uid_str;
    to_str(userid, uid_str);

    sql.append("INSERT INTO "); 
    sql.append(table_name);
    sql.append(" (userid");

    std::vector<std::string> keys;
    std::vector<std::string> vals;

    std::vector<const google::protobuf::FieldDescriptor *>::iterator it;

    for (it = fields.begin(); it != fields.end(); it++) {

        const std::string& key = (*it)->name();
        std::string value;
        uint32_t n = 0;

        switch ((*it)->type()) {
            case google::protobuf::FieldDescriptor::TYPE_INT32:
                n = msg_reflect->GetInt32(message, *it);
                to_str(n, value);
                break;
            case google::protobuf::FieldDescriptor::TYPE_UINT32:
                n = msg_reflect->GetUInt32(message, *it);
                to_str(n, value);
                break;	
            case google::protobuf::FieldDescriptor::TYPE_STRING:
                value.append("'");
                value.append(to_escape_string(mysql, msg_reflect->GetString(message, *it)));
                value.append("'");
                break;
            case google::protobuf::FieldDescriptor::TYPE_BOOL:
                value = msg_reflect->GetBool(message, *it);
                to_str(n, value);
                break;
            default:
                ERROR_LOG("unsupport field type %u",
                        (*it)->type());
                return -1;
        }

        keys.push_back(key);
        vals.push_back(value);
    }

    for (uint32_t i = 0; i < keys.size(); i++) {

        sql.append(" , ");
        sql.append(keys[i]);
    }
    sql.append(") VALUES (");
    sql.append(uid_str);

    for (uint32_t i = 0; i < vals.size(); i++) {
    
        sql.append(" , ");
        sql.append(vals[i]);
    }
    sql.append(")");

    return 0;
}
