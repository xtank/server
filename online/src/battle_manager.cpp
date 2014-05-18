
#include <stdlib.h>
#include <google/protobuf/message.h>

extern "C" {
#include <libtaomee/log.h>
#include <libtaomee/random/random.h>
}

#include "room.h"
#include "player.h"
#include "battle.h"
#include "battle_manager.h"


BattleManager::BattleManager()
{
    battle_idx_ = 0;
}

BattleManager::~BattleManager()
{

}

Battle* BattleManager::alloc_battle(room_t* room)
{
    Battle* battle = new Battle(room);

    if (battle == NULL) {
        KERROR_LOG(0, "out of memory");
        return NULL; 
    }

    battle_list_[battle_idx_] = battle;
    battle->set_battleid(battle_idx_);
    battle->set_mapid(room->map_id);
    battle->set_step(0);

    battle_idx_++;

    return battle;
}

void BattleManager::dealloc_battle(Battle* battle)
{
    std::map<int, Battle*>::iterator it1;

    it1 = battle_list_.find(battle->battleid());

    if (it1 != battle_list_.end()) {
        battle_list_.erase(it1); 
    }

    delete battle;
}


