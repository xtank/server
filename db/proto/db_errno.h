
#ifndef DB_ERRNO_H
#define DB_ERRNO_H

enum db_errno_t 
{
    db_err_succ = 0, // 操作成功
    db_err_record_not_found = 2000, //查询的记录集没有找到
    db_err_proto_format_err = 2001, // 包体格式错误
    db_err_cmd_not_find = 2002, // 命令号不存在
    db_err_role_already_exist = 2003, // 角色已经存在
    db_err_user_not_find = 2004, // 米米号不存在
    db_err_gen_sql_from_proto_failed = 2005, // 生成sql错误
    db_err_user_not_own_pet= 2006, //用户无此精灵
    db_err_tactic_del_pet = 2007, //tactic中del pet err
    db_err_attr_del = 2008,//attr表删除操作失败
    db_err_attr_set = 2009,//attr表保存操作失败
    db_err_attr_get = 2010, //无法从attr表中得到数据
    db_err_user_not_own_rune= 2011, //用户无此符文
    db_err_family_not_find = 2012, // 家族不存在
    db_err_sys_err = 2013, // 系统错误
    db_err_invalid_replay_key = 2014, // 无效的replay key
    db_err_friend_add_attention = 2015, //friend表加关注操作失败
    db_err_friend_set = 2016, //修改好友状态失败
    db_err_friend_remove_attention = 2017, //friend表取消关注失败
    db_err_friend_set_recent_list = 2018, //修改最近联系人列表失败
    db_err_family_already_exist = 2020, // 家族已经存在
    db_err_friend_reduplicate_add = 2021,//重复添加好友
    db_err_friend_reduplicate_remove_attention = 2022, //取消不存在之好友
    db_err_friend_reduplicate_set_black = 2023,//重复加屏蔽
    db_err_friend_reduplicate_remove_black = 2024, //重复取消屏蔽
    db_err_nick_not_exist = 2025, //nick不存在于nick-table中
    db_err_nick_already_exist = 2026, //nick已经存在
    db_err_money_lack = 2027, //钱不够
    db_err_attr_init = 2028, //创建用户时初始化attr错误
    db_err_escort_start = 2029, //开始运宝错误，未分配航道
    db_err_escort_delete = 2030, //删除运宝错误
    db_err_escort_end = 2031, //运宝无法结束
    db_err_escort_init_channel = 2032, //用户获得航道错误
    db_err_escort_get_login_info = 2033, //登录时获取运宝信息错误
    db_err_escort_rob = 2034, //用户被拦截次数已满
    db_err_global_attr_not_enough = 2035, // 全服属性不足

    db_err_buff_reach_max = 2100, //buff达到最大
    db_err_already_register_family_battle = 2101, // 已经注册过家族战
    db_err_family_not_register_battle = 2102, // 家族没有报名家族战

    //农场错误码
    db_err_farm_field_id_reach_max = 2200, //地块ID超过最大值
    db_err_farm_field_id_exist = 2201, //地块已被开垦
    db_err_farm_field_id_not_exist = 2202, //地块未被开垦
    db_err_farm_steal_not_enough = 2203, //不够偷
    db_err_farm_field_have_plant = 2204, //地上有作物生长, 不可种植
    db_err_farm_field_no_plant = 2205, //地上没有作物 不可收获
    db_err_farm_steal_already = 2206, //你已偷过该作物 不可再偷
    db_err_farm_steal_reach_max = 2207, //不可以再偷这个庄稼了
    db_err_farm_field_not_ripe = 2208, //作物未成熟

    db_err_farm_god_tree_energy_full = 2209, //神树能量已满
    db_err_farm_god_tree_already_charged_today = 2210, //你今天已经为该神树冲过能了

    db_err_buff_data_err = 2211, //db buff数据出错
};

#endif
