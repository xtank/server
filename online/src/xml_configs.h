
#ifndef XML_CONFIGS_H
#define XML_CONFIGS_H

#include "xmlutils.h"
#include "global_data.h"


// 加载地图配置
int load_map_config(xmlNodePtr root);

// 加载坦克
int load_tank_config(xmlNodePtr root);

#endif
