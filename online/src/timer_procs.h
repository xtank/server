#ifndef TIMER_PROCS_H
#define TIMER_PROCS_H

enum timer_type_t
{
    kTimerTypeReconnectServiceTimely = 1, // 重连service服务器
    kTimerTypeSendSwitchKeepAlive = 2, // 保持和switch服务心跳
    //kTimerTypeReconnectServerTimely = 3, // 重连server服务器
    kTimerTypeSyncClientTimeTimely = 4, // 同步客户端时间
    kTimerTypeSyncMailTimely = 5,      // 通知新邮件
    kTimerTypeCheckDbTimeout = 6, // db超时
    kTimerTypeDailyOperation = 8,      // 每日操作
    kTimerTypeSaveCurBattleValueTimely = 9, //定时保存用户当前战斗力
    kTimerTypeCleanExpiredItemsTimely = 10, // 定期清理用户过期道具
    kTimerTypeCheckEscortEndTimely = 11, //定时检查运宝是否已经结束
    kTimerTypeRefuseInviteBattleTimely = 12, //自动拒绝对战邀请
    kTimerTypeSysNotiOffline = 13, //系统定时在23:50分发布服务器即将关闭的通知
    kTimerTypePetExerciseAddExpTimely = 14, // 每过一段时间增加精灵锻炼的经验
    kTimerTypeWorldBossTimely = 15, // 世界boss活动检测
    kTimerTypeCheckDbBeanTimely = 16, //检测精灵豆DB请求是否正常返回

    KTimerTypeSnowballControl = 17, //控制雪球大战开始和结束的定时器
    KTimerTypeSnowballBornProtect = 18, //雪球大战出生保护定时器
    KTimerTypeSnowballBombClick = 19, //雪球大战炸弹倒计时定时器
    KTimerTypeSnowballDeadAlive = 20, //雪球大战死亡后复活计时器

    KTimerTypePetHotSpring = 21, //精灵泡温泉
    KTimerTypeLuckyBag = 22, //福袋大作战
    KTimerTypeCollegePK = 23, //学院对抗
    kTimerTypeAlive = 24, // 手机客户端生存时间
};

enum timer_interval_t
{
    kTimerIntervalReconnectServiceTimely = 1, // 重连service服务器间隔
    kTimerIntervalSendSwitchKeepAlive = 30, // 保持和switch服务心跳
    //kTimerIntervalReconnectServerTimely = 1, // 重连server服务器间隔
    kTimerIntervalSyncClientTimeTimely = 30, // 同步客户端时间
    kTimerIntervalSyncMailTimeTimely = 300, // 通知新邮件时间
    kTimerIntervalSaveCurBattleValueTimely = 30,//定时保存用户当前战斗力
    kTimerIntervalCheckDbTimeout = 6, // db超时通知  
    kTimerIntervalCleanExpiredItems = 10 * 60, // 定期清理用户过期道具
//    kTimerIntervalCheckEscortEnd = 4, //定期检查运宝是否结束，不需要每秒检查一次吧大概
    kTimerIntervalCheckEscortEndTimely = 4, //定期检查运宝是否结束，不需要每秒检查一次吧大概
    kTimerIntervalRefuseInviteBattleTimely = 60, //自动拒绝对战邀请
    kTimerIntervalPetExerciseAddExpTimely = 60, // 每过一段时间增加精灵锻炼的经验
    kTimerIntervalWorldBossTimely = 60, // 世界boss活动检测
    kTimerIntervalDbBeanTimely = 3, //精灵豆超时时间

    KTimerIntervalSnowballBornProtect = 3, //雪球大战出生保护时间
    KTimerIntervalSnowballDeadAlive = 30, //雪球大战死亡等待复活的时间
    KTimerIntervalSnowballBombClick = 5, //雪球大战炸弹的时长
    KTimerIntervalLuckyBag = 60, //福袋大作战
    kTimerIntervalAlive = 4 * 60 * 60, // 手机客户端生存时间
    KTimerIntervalCollegePK = 60, //学院对抗
};

int send_switch_keep_alive_timely(void* owner, void* data);

int register_timers();

#endif
