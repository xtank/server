
#include <stdlib.h>
#include <libtaomee++/utils/strings.hpp>

#include <vector>

#include "player.h"
#include "xmlutils.h"
#include "global_data.h"
#include "xml_configs.h"
#include "map_conf.h"

// 加载地图配置信息
int load_map_config(xmlNodePtr root)
{
    g_map_conf_mgr.clear();
    xmlNodePtr cur = root->xmlChildrenNode;
    while (cur) {
        if (!xmlStrcmp(cur->name, (const xmlChar *)"map")) {
            map_conf_t map;
            DECODE_XML_PROP_UINT32(map.id, cur, "id");
            if (g_map_conf_mgr.is_map_conf_exist(map.id)) {
                ERROR_TLOG("map %u already exists", map.id);
                return -1;
            }

            xmlNodePtr team_cur = cur->xmlChildrenNode;
            while (team_cur) {
                map_team_conf_t team;
                if (!xmlStrcmp(team_cur->name, (const xmlChar *)"team")) {

                    char buf[256] = {0};
                    DECODE_XML_PROP_UINT32(team.id, team_cur, "id");
                    DECODE_XML_PROP_STR(buf, team_cur, "homePosition");

                    std::vector<std::string> str_list = split(buf, ',');
                    team.home_pos.x = atoi(str_list[0].c_str());
                    team.home_pos.y = atoi(str_list[1].c_str());

                    xmlNodePtr member_cur = team_cur->xmlChildrenNode;
                    while (member_cur) {
                        if (!xmlStrcmp(member_cur->name, (const xmlChar *)"member")) {

                            char buf[256] = {0};
                            DECODE_XML_PROP_STR(buf, member_cur, "bornPosition");
                            std::vector<std::string> str_list = split(buf, ',');

                            map_point_t point;
                            point.x = atoi(str_list[0].c_str());
                            point.y = atoi(str_list[1].c_str());

                            team.member_born_list.push_back(point);
                        }
                        member_cur++;
                    }
                    
                }
                //add team
                map.team_list.push_back(team);
                team_cur++;
            }

            //barrier
            if (!xmlStrcmp(cur->name, (const xmlChar *)"barriers")) {
                xmlNodePtr barrier_cur = cur->xmlChildrenNode;
                while (barrier_cur) {
                    if (!xmlStrcmp(barrier_cur->name, (const xmlChar *)"barrier")) {

                        uint32_t id = 0;
                        DECODE_XML_PROP_UINT32(id, barrier_cur, "id");
                        char buf[256] = {0};
                        DECODE_XML_PROP_STR(buf, barrier_cur, "reg");
                        std::vector<std::string> str_list = split(buf, ',');

                        map_point_t point;
                        point.x = atoi(str_list[0].c_str());
                        point.y = atoi(str_list[1].c_str());

                        //map.barrier_map.insert(std::pair<map_point_t, uint32_t>(point, id));
                        map.barrier_map[point.x][point.y] = id;
                    }
                    barrier_cur++;
                }
            }

            g_map_conf_mgr.add_map_conf(map);
        }
        
        cur = cur->next;
    }

    return 0;
}

