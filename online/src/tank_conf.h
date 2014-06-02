#ifndef __TANK_CONF_H__
#define __TANK_CONF_H__

#include "common.h"

struct tank_conf_t {
    uint32_t id;
    uint32_t hp;
    uint32_t atk;
    uint32_t def;
    uint32_t speed;
    uint32_t atk_speed;
    uint32_t atk_scope;
};

#endif
