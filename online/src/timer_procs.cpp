
extern "C" {
#include <libtaomee/log.h>
#include <libtaomee/timer.h>
}
#include "timer_procs.h"
#include "service.h"
#include "global_data.h"
#include "player_manager.h"
#include "proto/client/cli_errno.h"
#include "global_data.h"

#define REGISTER_TIMER_TYPE(nbr_, cb_) \
    do { \
        if (register_timer_callback(nbr_, cb_) == -1) { \
            ERROR_LOG("register timer type error\t[%u]", nbr_); \
            return -1; \
        }\
    } while(0)

#define CHECK_TIMER(type) \
    player_t *player = 0; \
    do { \
        uint64_t tmp = (uint64_t)data; \
        uint32_t uid = (uint32_t)tmp; \
        player = g_player_manager->get_player_by_userid(uid); \
        if (!player) { \
            WARN_TLOG("Timer[type:%d] Callback But Player[%u] deleted", type, uid); \
            return 0; \
        } \
    } while(0)

int reconnect_service_timely(void* owner, void* data)
{
    Service* service = (Service *)data;
    if (service->connect() != 0) {
        ADD_TIMER_EVENT_EX(&g_reconnect_timer,
                kTimerTypeReconnectServiceTimely,
                service,
                get_now_tv()->tv_sec + kTimerIntervalReconnectServiceTimely);  
    }
    return 0;
}

/* 
 * @brief 添加定时器，30秒向switch发送心跳包
 */
//int send_switch_keep_alive_timely(void* owner, void* data)
//{
    //int ret = g_switch->send_buf(NULL, 0, svr_proto_os_keepalive_cmd, NULL, 0);
	//ADD_TIMER_EVENT_EX(&g_switch_keep_alive_timer, 
            //kTimerTypeSendSwitchKeepAlive, 0, 
            //get_now_tv()->tv_sec + kTimerIntervalSendSwitchKeepAlive);
    //INFO_LOG("send keep alive message to switch, ret = %d", ret);
	//return 0;
//}

//int sync_client_time_timely(void* owner, void* data)
//{
    //CHECK_TIMER(kTimerTypeSyncClientTimeTimely);
    //onlineproto::sc_keep_live sc_keep_live;
    //sc_keep_live.set_server_time(get_now_tv()->tv_sec);
    //send_msg_to_player(player, cli_cmd_cs_keep_live, sc_keep_live);
    //player->sync_client_time_timer = ADD_TIMER_EVENT_EX(
            //player,
            //kTimerTypeSyncClientTimeTimely,
            //(void*)(player->userid),
            //get_now_tv()->tv_sec + kTimerIntervalSyncClientTimeTimely);  
    //return 0;
//}

//int daily_operation(void *owner, void *data)
//{
    //CHECK_TIMER(kTimerTypeDailyOperation);
    ////到了下一天的整天做一些操作
    ////删除当前有效的物品
    ////重新定时
    //player->daily_op_timer = ADD_TIMER_EVENT_EX(
            //player,
            //kTimerTypeDailyOperation,
            //(void*)(player->userid),
            //TimeUtils::second_at_day_start(2) - 5); //下一天的晚上23点59分
    //// 清理每日属性
    //AttrUtils::clear_daily_attr_value(player, true);
    //// 清理每周属性
    //AttrUtils::clear_weekly_attr_value(player, true);
    //// 清理每月属性
    //AttrUtils::clear_monthly_attr_value(player, true);
    //// 重新设置登录时间
    //uint32_t last_login_time = TimeUtils::day_align_low(NOW() + 1);
    //attr_data_info_t attr = {
        //kDailyLastLoginTime, last_login_time
    //};
    //AttrUtils::set_attr_value(player, 1, &attr, false);
    ////下线
    //send_err_to_player(player, player->wait_cmd, cli_err_server_closed);
    //close_client_conn(player->fd);
    //return 0;
//}

//int sys_offline_noti(void *owner, void *data)
//{
    ////发送广播通知
    //static uint32_t cnt = 1;
    //static int next_sec = 0;

    //static char msg[256];
    //snprintf(msg, sizeof(msg), "各位亲爱的玩家,热血精灵派还有%d分钟就要关服了,请大家保存好游戏进度,及时下线",
            //11-cnt);

    //if (cnt <= 9) {
        //next_sec = TimeUtils::second_at_day_start(1) - 600 + 60 *cnt;
        //cnt ++;
    //} else {
        //cnt = 1;
        //next_sec = TimeUtils::second_at_day_start(2) - 600;
    //}

    //std::string noti_msg;
    //noti_msg.assign(msg);
    //McastUtils::notify(noti_msg);
    ////定时通知用户下线
    //ADD_TIMER_EVENT_EX(&g_sys_noti_timer,
            //kTimerTypeSysNotiOffline, 0,
            //next_sec);

    //return 0;
//}

//int check_db_timeout(void* owner, void* data)
//{
    //CHECK_TIMER(kTimerTypeCheckDbTimeout);
    //player->db_request_timer = NULL;
    //player->wait_serv_cmd = 0;
    //send_err_to_player(player, player->wait_cmd,
            //cli_err_sys_err);
    //return 0;
//}

//int clean_expired_items_timely(void* onwer, void* data)
//{
    //CHECK_TIMER(kTimerTypeCleanExpiredItemsTimely);
    //player->clean_expired_items_timer = NULL;
    //clean_expired_items(player);
    //player->clean_expired_items_timer = ADD_TIMER_EVENT_EX(
            //player,
            //kTimerTypeCleanExpiredItemsTimely,
            //(void*)(player->userid),
            //get_now_tv()->tv_sec + kTimerIntervalCleanExpiredItems);
    //return 0;
//}

//int check_escort_end_timely(void* owner, void* data)
//{
    //CHECK_TIMER(kTimerTypeCheckEscortEndTimely);
    //bool is_end = g_escort->check_escort_end(player);
    //if (!is_end) {
        //KERROR_LOG(player->userid, "escort_end operation fail");
    //} 

    //player->check_escort_end_timer = NULL;

    //return 0;
//}

//int refuse_invite_battle_timely(void* owner, void* data)
//{
    //CHECK_TIMER(kTimerTypeRefuseInviteBattleTimely);
    //InviteFightInfo* invitee_info = player->invited_by_other_info;
    //if (invitee_info == NULL) {
        //player->refuse_invite_battle_timer = NULL;
        //return 0;
    //}

    //if (invitee_info->other() == 0) {
        //player->refuse_invite_battle_timer = NULL;
        //return 0;
    //}

    ////向被邀请者发送取消
    //onlineproto::sc_cancel_fight_player sc_cancel_fight_player;
    //sc_cancel_fight_player.set_userid(invitee_info->other());
    //send_msg_to_player(player, cli_cmd_cs_cancel_fight_player, sc_cancel_fight_player);

    //player->refuse_invite_battle_timer = NULL;

    //return 0;
//}

//int pet_exercise_add_exp_timely(void* owner, void* data)
//{
    //CHECK_TIMER(kTimerTypePetExerciseAddExpTimely);
    //PetUtils::pet_exercise(player, kTimerIntervalPetExerciseAddExpTimely);
    //player->pet_exercise_add_exp_timer = ADD_TIMER_EVENT_EX(
            //player, 
            //kTimerTypePetExerciseAddExpTimely,
            //(void*)(player->userid), 
            //get_now_tv()->tv_sec + kTimerIntervalPetExerciseAddExpTimely);

    //return 0;
//}

//int check_world_boss_start_or_over_timely(void* owner, void* data)
//{
    //DEBUG_LOG("check world boss");

    //const tm* tm = get_now_tm();
    //struct tm tm_new;
    //time_t time_new = get_now_tv()->tv_sec + 3 * 60;
    //localtime_r(&time_new, &tm_new);

    //int now = tm->tm_hour * 100 + tm->tm_min;
    //int now_after_3min = tm_new.tm_hour * 100 + tm_new.tm_min;
    //uint32_t maxhp = g_module_mgr.get_module_conf_uint32_def(
            //module_type_world_boss, "boss_maxhp", 0);

    //std::string start_time_str = g_module_mgr.get_module_conf_string_def(
            //module_type_world_boss, "module_start_tm", "00:00");
    //int start_time = WorldBoss::parse_time_str(start_time_str);

    //// 还有3分钟开始清理数据
    //if (now_after_3min == start_time) {
        //DEBUG_LOG("set world boss1 start");
        //g_world_boss->set_maxhp(maxhp);
        //g_world_boss->restart();  
    //}

    //std::string start_time_str1 = g_module_mgr.get_module_conf_string_def(
            //module_type_world_boss, "module_start_tm1", "00:00");
    //int start_time1 = WorldBoss::parse_time_str(start_time_str1);

    //// 还有3分钟开始清理数据
    //if (now_after_3min == start_time1) {
        //DEBUG_LOG("set world boss2 start");
        //g_world_boss->set_maxhp(maxhp);
        //g_world_boss->restart(); 
    //}

    //std::string end_time_str = g_module_mgr.get_module_conf_string_def(
            //module_type_world_boss, "module_end_tm", "00:00");
    //int end_time = WorldBoss::parse_time_str(end_time_str);

    //if (now == end_time) {
        //DEBUG_LOG("set world boss1 finish");
        //g_world_boss->game_over(); 
    //}

    //std::string end_time_str1 = g_module_mgr.get_module_conf_string_def(
            //module_type_world_boss, "module_end_tm1", "00:00");
    //int end_time1 = WorldBoss::parse_time_str(end_time_str1);

    //if (now == end_time1) {
        //DEBUG_LOG("set world boss2 finish");
        //g_world_boss->game_over(); 
    //}

    //ADD_TIMER_EVENT_EX(&g_world_boss_timer,
            //kTimerTypeWorldBossTimely,
            //NULL,
            //get_now_tv()->tv_sec + kTimerIntervalWorldBossTimely);  

    //if (WorldBoss::in_activity_time() && !g_world_boss->is_over()) {

        //// 血量少于10%通知
        //if (g_world_boss->hp() < g_world_boss->maxhp() * 10 / 100) {
            //onlineproto::sc_notice sc_notice;  
            //sc_notice.set_type(onlineproto::sc_notice::NOTICE_TYPE_WORLD_BOSS_10_PERCENT);
            //g_player_manager->send_msg_to_all_player(cli_cmd_cs_notice, sc_notice);
        //} else if (g_world_boss->hp() < g_world_boss->maxhp() * 50 / 100) {
        //// 血量少于50%通知
            //onlineproto::sc_notice sc_notice; 
            //sc_notice.set_type(onlineproto::sc_notice::NOTICE_TYPE_WORLD_BOSS_50_PERCENT);
            //g_player_manager->send_msg_to_all_player(cli_cmd_cs_notice, sc_notice);
        //}
    //}

    //return 0;
//}

//int check_db_bean_return_timely(void *owner, void *data)
//{
    //CHECK_TIMER(kTimerTypeCheckDbBeanTimely);
    //player->check_bean_return = NULL;
    //StatInfo stat_info;
    //stat_info.add_info("proc_bean_op_time_out", "online");
    //stat_info.add_op(StatInfo::op_sum, "proc_bean_op_time_out");
    //g_stat_logger->log("后台监控", "精灵豆操作超时", "", "", stat_info);

    //send_err_to_player(player, player->wait_cmd, cli_err_sys_busy);
    //return 0;
//}

////雪球大战开始/结束
//int snowball_check_start_end(void *owner, void *data)
//{
    //const tm *now = get_now_tm();
    //uint32_t cur_hm = now->tm_hour * 100 + now->tm_min;

    //string start_tm1 = g_module_mgr.get_module_conf_string_def(
            //module_type_snowball, "module_start_tm1", "12:30");
    //string end_tm1 = g_module_mgr.get_module_conf_string_def(
            //module_type_snowball, "module_end_tm1", "13:00");
    //uint32_t start_tm1_hm = TimeUtils::parse_hour_min_str(start_tm1);
    //uint32_t end_tm1_hm = TimeUtils::parse_hour_min_str(end_tm1);

    //string start_tm2 = g_module_mgr.get_module_conf_string_def(
            //module_type_snowball, "module_start_tm2", "19:30");
    //string end_tm2 = g_module_mgr.get_module_conf_string_def(
            //module_type_snowball, "module_end_tm2", "20:00");

    //if (TimeUtils::time_to_date(NOW()) == 20131224) {
        //start_tm2.assign("20:00");
        //end_tm2.assign("20:30");
    //}

    //uint32_t start_tm2_hm = TimeUtils::parse_hour_min_str(start_tm2);
    //uint32_t end_tm2_hm = TimeUtils::parse_hour_min_str(end_tm2);
    
    ////雪球大战不开启 
    //if (start_tm1_hm == end_tm1_hm 
        //&& end_tm1_hm == start_tm2_hm
        //&& start_tm2_hm == end_tm2_hm) {
        //return 0;
    //}

    //assert(start_tm1_hm < end_tm1_hm);
    //assert(end_tm1_hm < start_tm2_hm);
    //assert(start_tm2_hm < end_tm2_hm);

    //uint32_t secs_offset = 0;
    //if ((cur_hm >= start_tm1_hm && cur_hm < end_tm1_hm)
            //|| (cur_hm >= start_tm2_hm && cur_hm < end_tm2_hm)) {
        ////游戏开始 增加结束定时器
        //int ret = g_snowball_mgr.snowball_battle_start();
        //assert(ret != -1);
        //if (cur_hm < end_tm1_hm) {
            //secs_offset = TimeUtils::second_to_hm(end_tm1_hm);
        //} else {
            //secs_offset = TimeUtils::second_to_hm(end_tm2_hm);
        //}
        //ADD_TIMER_EVENT_EX(&g_snowball_timer, 
                //KTimerTypeSnowballControl,
                //NULL,
                //NOW() + secs_offset); 
        //return 0;
    //} else if (cur_hm == end_tm1_hm || cur_hm == end_tm2_hm) {
        //g_snowball_mgr.snowball_battle_end();
    //}
    ////增加游戏开始定时器
    //if (cur_hm >= end_tm1_hm && cur_hm < start_tm2_hm) {
        //secs_offset = TimeUtils::second_to_hm(start_tm2_hm);
    //} else {
        //secs_offset = TimeUtils::second_to_hm(start_tm1_hm);
    //}
    //DEBUG_TLOG("snowball secs_offset:%u", secs_offset);
    //ADD_TIMER_EVENT_EX(&g_snowball_timer,
            //KTimerTypeSnowballControl,
            //NULL,
            //NOW() + secs_offset);
    //return 0;
//}

////雪球炸弹爆炸
//int snowball_bomb_blow(void *owner, void *data)
//{
    //snowball_bomb_t *bb = (snowball_bomb_t*)owner;
    //uint64_t tmp = (uint64_t)data;
    //uint32_t battle_field_id = (uint32_t)tmp;
    //g_snowball_mgr.blow_bomb(battle_field_id, bb->bomb_id);
    //return 0;
//}

////雪球大战玩家从死亡变成复活
//int snowball_player_back(void *owner, void *data)
//{
    //CHECK_TIMER(KTimerTypeSnowballDeadAlive);
    //player->snowball_dead_alive_timer = NULL;
    //g_snowball_mgr.player_status_change(player, SNOWBALL_P_STATUS_BORN);
    //return 0;
//}

////雪球大战玩家出生无敌状态消失
//int snowball_player_born_protector_gone(void *owner, void *data)
//{
    //CHECK_TIMER(KTimerTypeSnowballBornProtect);
    //player->snowball_born_protect_timer = NULL;
    //g_snowball_mgr.player_status_change(player, SNOWBALL_P_STATUS_LIVING);
    //return 0;
//}

////精灵泡温泉定时器监听
//int pet_hotspring_bath_listen(void *owner, void *data)
//{
    //CHECK_TIMER(KTimerTypePetHotSpring);
    ////通知泡温泉结束
    //proc_pet_hotspring_end(player);
    //player->listen_hotspring_bath = NULL;
    //return 0;
//}

////福怪大作战 检查怪物刷新
//int check_refresh_lucky_bag_refresh_mapid(void *owner, void *data)
//{
    //DEBUG_LOG("check lucky_bag");
    ////随机地图id
    //const uint32_t mapid_list[26] = {
        //1001,
        //1002,
        //1003,
        //1004,
        //1005,
        //1006,
        //1007,
        //1008,
        //1009,
        //1010,
        //1011,
        //1012,
        //1013,
        //1014,
        //1015,
        //1022,
        //1023,
        //1024,
        //1025,
        //1026,
        //1027,
        //1028,
        //1029,
        //1031,
        //1,
        //2
    //};

    ////10点到21点整点检查
    //const tm* tm = get_now_tm();
    //struct tm tm_new;
    //time_t time_new = get_now_tv()->tv_sec;
    //localtime_r(&time_new, &tm_new);

    //uint32_t now = tm->tm_hour * 100 + tm->tm_min;
    //for (uint32_t i = 10; i <= 21; i++) {
        //uint32_t refresh_time = i * 100;

        ////随机一个地图 通知全服
        //if (now == refresh_time) {
            //g_lucky_bag_monster_map_id = mapid_list[taomee::ranged_random(0, 25)];
            //onlineproto::sc_notice sc_notice;  
            //sc_notice.set_type(onlineproto::sc_notice::NOTICE_TYPE_LUCKY_BAG);
            //sc_notice.set_content("");
            //sc_notice.set_args(g_lucky_bag_monster_map_id);
            //g_player_manager->send_msg_to_all_player(cli_cmd_cs_notice, sc_notice);

        //}
    //}

    //ADD_TIMER_EVENT_EX(&g_lucky_bag_timer,
            //KTimerTypeLuckyBag,
            //NULL,
            //get_now_tv()->tv_sec + KTimerIntervalLuckyBag);  

    //return 0;
//}

//int alive_timeout(void* owner, void* data)
//{
    //CHECK_TIMER(kTimerTypeAlive);

    //DEBUG_LOG("player %u alive timeout", player->userid);

    //if (player->fd) {
        //close_client_conn(player->userid); 
    //}

////    player_leave_server(player);
////    uninit_player(player);
////    g_player_manager->dealloc_player(player); 

    //player->alive_timer = NULL;

    //return 0;
//}

//int check_refresh_college_pk_win(void *owner, void *data)
//{
    //DEBUG_LOG("check college_pk");

    ////每晚八点 决出胜负
    //const tm* tm = get_now_tm();
    //struct tm tm_new;
    //time_t time_new = get_now_tv()->tv_sec;
    //localtime_r(&time_new, &tm_new);

    //uint32_t now = tm->tm_hour * 100 + tm->tm_min;
    //uint32_t refresh_time = 2000;

    //dbproto::cs_college_pk_result db_cs_college_pk_result;
    ////通知db计算 结果值
    //if (now == refresh_time) {
        //g_dbproxy->send_msg(NULL, 0, db_cmd_college_pk_result, db_cs_college_pk_result);
    //}

    //ADD_TIMER_EVENT_EX(&g_college_pk_timer,
            //KTimerTypeCollegePK,
            //NULL,
            //get_now_tv()->tv_sec + KTimerIntervalCollegePK);  

    //return 0;
//}

int register_timers()
{
    REGISTER_TIMER_TYPE(kTimerTypeReconnectServiceTimely, reconnect_service_timely);
    //REGISTER_TIMER_TYPE(kTimerTypeSendSwitchKeepAlive, send_switch_keep_alive_timely);
    //REGISTER_TIMER_TYPE(kTimerTypeSyncClientTimeTimely, sync_client_time_timely);
    //REGISTER_TIMER_TYPE(kTimerTypeSyncMailTimely, sync_mail_timely);
    //REGISTER_TIMER_TYPE(kTimerTypeSaveCurBattleValueTimely, save_cur_battle_value_timely);
    //REGISTER_TIMER_TYPE(kTimerTypeDailyOperation, daily_operation);
    //REGISTER_TIMER_TYPE(kTimerTypeCheckDbTimeout, check_db_timeout);
    //REGISTER_TIMER_TYPE(kTimerTypeCleanExpiredItemsTimely, clean_expired_items_timely);
    //REGISTER_TIMER_TYPE(kTimerTypeCheckEscortEndTimely, check_escort_end_timely);
    //REGISTER_TIMER_TYPE(kTimerTypeRefuseInviteBattleTimely, refuse_invite_battle_timely);
    //REGISTER_TIMER_TYPE(kTimerTypePetExerciseAddExpTimely, pet_exercise_add_exp_timely);
    //REGISTER_TIMER_TYPE(kTimerTypeSysNotiOffline, sys_offline_noti);
    //REGISTER_TIMER_TYPE(kTimerTypeWorldBossTimely, check_world_boss_start_or_over_timely);
    //REGISTER_TIMER_TYPE(kTimerTypeCheckDbBeanTimely, check_db_bean_return_timely);
    //REGISTER_TIMER_TYPE(KTimerTypeSnowballControl, snowball_check_start_end);
    //REGISTER_TIMER_TYPE(KTimerTypeSnowballBornProtect, snowball_player_born_protector_gone);
    //REGISTER_TIMER_TYPE(KTimerTypeSnowballBombClick, snowball_bomb_blow);
    //REGISTER_TIMER_TYPE(KTimerTypeSnowballDeadAlive, snowball_player_back);
    //REGISTER_TIMER_TYPE(KTimerTypePetHotSpring, pet_hotspring_bath_listen);
    //REGISTER_TIMER_TYPE(KTimerTypeLuckyBag, check_refresh_lucky_bag_refresh_mapid);
    //REGISTER_TIMER_TYPE(kTimerTypeAlive, alive_timeout);
    //REGISTER_TIMER_TYPE(KTimerTypeCollegePK, check_refresh_college_pk_win);

    return 0;
}
