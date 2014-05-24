#include "battle.h"
#include "player.h"
#include "room.h"


Battle::Battle(room_t* room) {

    for (uint32_t i = 0; i < room->player_vec->size(); i++) {
        player_t* player = room->player_vec->at(i);
        if (player == NULL) {
            continue;
        }

        battle_player_t battle_player;
        battle_player.userid = player->userid;

        //todo
        battle_player.hp = 100;
        battle_player.x = 0;
        battle_player.y = 0;
        battle_player.face = 0;
        battle_player.teamid = player->teamid;
        battle_player.tankid = player->tankid;

        if (player->teamid == 1) {
            team1.push_back(battle_player);
        } else {
            team2.push_back(battle_player);
        }
    }

    this->mapid_ = room->map_id;
    this->step_ = 0;
}
