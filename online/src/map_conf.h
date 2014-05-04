#ifndef __MAP_CONF_H__
#define __MAP_CONF_H__

#include "common.h"

enum barrier_block_t {
    kBarrierUnBlock = 0,
    kBarrierBlock = 1,
};

struct map_point_t {
    uint32_t x;
    uint32_t y;
};

struct barrier_conf_t {
    uint32_t id;
    barrier_block_t block;
    std::vector<map_point_t> occupy_list;
    uint32_t hp;
};

struct map_team_conf_t {
    uint32_t id;
    map_point_t home_pos;
    std::vector<map_point_t> member_born_list;
};

struct map_conf_t {
    uint32_t id;
    std::vector<map_team_conf_t> team_list;
    //std::map<map_point_t, uint32_t> barrier_map;
    uint32_t barrier_map[1000][1000];
};


class map_conf_mgr_t {
public:
    map_conf_mgr_t() {
        clear();
    }
    ~map_conf_mgr_t() {
        clear();
    }
    inline void clear() {
        map_conf_map_.clear();
    }
    inline const std::map<uint32_t, map_conf_t> &const_map_conf_map() const {
        return map_conf_map_;
    }
    inline bool is_map_conf_exist(uint32_t map_id) {
        if (map_conf_map_.count(map_id) > 0) return true;
        return false;
    }
    inline bool add_map_conf(const map_conf_t &map) {
        if (is_map_conf_exist(map.id)) return false;
        map_conf_map_[map.id] = map; return true;
    }
    inline const map_conf_t *find_map_conf(uint32_t map_id) {
        if (!is_map_conf_exist(map_id)) return 0;
        return &((map_conf_map_.find(map_id))->second);
    }

private:
    std::map<uint32_t, map_conf_t> map_conf_map_;
};

extern map_conf_mgr_t g_map_conf_mgr;
#endif //__MAP_CONF_H__

