#ifndef __BATTLE_H__
#define __BATTLE_H__

#include "common.h"
#include "proto.h"

#include <map>
#include <vector>

struct room_t;

struct battle_player_t
{
    userid_t userid; // 米米号

    uint32_t hp; //战斗血量
    uint32_t x;  //位置坐标
    uint32_t y; //位置坐标
    uint32_t face; //朝向
    uint32_t teamid; //队伍
};

class Battle {
public:
    Battle(room_t* room);

    uint32_t step() {return step_;};

    void set_step(uint32_t step) {
        step_ = step;
    }

    uint32_t mapid() {return mapid_;};

    void set_mapid(uint32_t mapid) {
        mapid_ = mapid;
    }

    uint32_t battleid() {return battleid_;};

    void set_battleid(uint32_t battleid) {
        battleid_ = battleid;
    }


private:
    std::vector<battle_player_t> team1; //队伍1
    std::vector<battle_player_t> team2; //队伍2
    uint32_t step_; //战斗过程  //0 战斗预备阶段 1战斗过程
    uint32_t mapid_; //战斗地图id
    uint32_t battleid_; //战斗id
};


#endif
