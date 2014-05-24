#ifndef BATTLE_MANAGER_H
#define BATTLE_MANAGER_H

#include <map>
#include <vector>
#include <string>

#include <libtaomee/project/types.h>
#include <google/protobuf/message.h>

struct room_t;
class Battle;

class BattleManager
{
public:
    BattleManager();
    ~BattleManager();

    Battle* alloc_battle(room_t* room);
    void dealloc_battle(Battle* battle);
    Battle* get_battle_by_id(uint32_t battleid) {
        if (battle_list_.count(battleid) == 0) {
            return NULL;
        }
        return battle_list_[battleid];
    }
    
private:

    uint32_t battle_idx_;
    std::map<int, Battle *> battle_list_;
};

#endif
