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
    
private:

    uint32_t battle_idx_;
    std::map<int, Battle *> battle_list_;
};

#endif
