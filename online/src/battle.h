#ifndef __BATTLE_H__
#define __BATTLE_H__

#include "common.h"
#include "proto.h"

#include <map>
#include <vector>

struct room_t;

enum battle_player_status_t
{
    kInit = 0, //初始状态
    kResourceReady = 1, //客户端资源加载完毕
    kBattle = 2, //战斗状态
};

struct battle_player_t
{
    userid_t userid; // 米米号

    uint32_t hp; //战斗血量
    uint32_t x;  //位置坐标
    uint32_t y; //位置坐标
    uint32_t face; //朝向
    uint32_t teamid; //队伍
    uint32_t tankid; //坦克id
    battle_player_status_t status; //0 最初 1客户端资源已加载 2战斗开始
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

    const std::vector<battle_player_t>* get_team1() {
        return &team1;
    }
   
    const std::vector<battle_player_t>* get_team2() {
        return &team2;
    }
 
    battle_player_t* get_battle_player_by_id(uint32_t userid) {

        for (uint32_t i = 0; i < team1.size(); i++) {
            if (team1[i].userid == userid) {
                return &team1[i];
            }
        }

        for (uint32_t i = 0; i < team2.size(); i++) {
            if (team2[i].userid == userid) {
                return &team2[i];
            }
        }

        return NULL;
    }

private:
    std::vector<battle_player_t> team1; //队伍1
    std::vector<battle_player_t> team2; //队伍2

    uint32_t step_; //战斗过程  //0 战斗预备阶段 1战斗过程
    uint32_t mapid_; //战斗地图id
    uint32_t battleid_; //战斗id
};


#endif
