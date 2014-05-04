
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

    KERROR_LOG(0 ,"reconnect_service timely");
    return 0;
}

int register_timers()
{
    REGISTER_TIMER_TYPE(kTimerTypeReconnectServiceTimely, reconnect_service_timely);

    return 0;
}
