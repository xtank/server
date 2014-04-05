
#include <stdlib.h>
#include <google/protobuf/message.h>

extern "C" {
#include <libtaomee/log.h>
#include <libtaomee/random/random.h>
}

#include "player.h"
#include "player_manager.h"


PlayerManager::PlayerManager()
{

}

PlayerManager::~PlayerManager()
{

}

player_t* PlayerManager::get_player_by_fd(int fd)
{
    std::map<int, player_t*>::iterator it;

    it = fd_users_.find(fd);

    if (it == fd_users_.end()) {
        return NULL; 
    } else {
        return it->second; 
    }
}

player_t* PlayerManager::get_player_by_userid(userid_t userid)
{
    std::map<userid_t, player_t*>::iterator it;

    it = id_users_.find(userid);

    if (it == id_users_.end()) {
        return NULL; 
    } else {
        return it->second;   
    }
}

player_t* PlayerManager::alloc_player(int fd, userid_t userid)
{
    player_t* player = (player_t *)calloc(1, sizeof(player_t));

    if (player == NULL) {
        KERROR_LOG(0, "out of memory");
        return NULL; 
    }

    player->fd = fd;
    player->userid = userid;

    fd_users_[fd] = player;
    id_users_[userid] = player;
    
    return player;
}

void PlayerManager::dealloc_player(player_t* player)
{
    std::map<int, player_t*>::iterator it1;
    std::map<userid_t, player_t*>::iterator it2;

    it1 = fd_users_.find(player->fd);

    if (it1 != fd_users_.end()) {
        fd_users_.erase(it1); 
        assert(player == it1->second);
    }

    it2 = id_users_.find(player->userid);

    if (it2 != id_users_.end()) {
        id_users_.erase(it2); 
        assert(player == it2->second);
    }

    delete player;
}

void PlayerManager::get_userid_list(std::vector<userid_t>& userid_list)
{
    id_users_ptr_t ptr;

    userid_list.clear();

    for (ptr = id_users_.begin(); ptr != id_users_.end(); ptr++) {
        userid_list.push_back(ptr->first);
    }
}

void PlayerManager::get_player_list(std::vector<player_t*>& player_list)
{
    id_users_ptr_t ptr;

    player_list.clear();

    for (ptr = id_users_.begin(); ptr != id_users_.end(); ptr++) {
        //if (ptr->second->is_login) {
            player_list.push_back(ptr->second);
        //}
    }
}

void PlayerManager::send_msg_to_all_player(uint32_t cmd, const google::protobuf::Message& message)
{
    std::vector<player_t*> player_list;

    get_player_list(player_list);

    for (uint32_t i = 0; i < player_list.size(); i++) {
        player_t* player = player_list[i]; 
        send_msg_to_player(player, cmd, message);
    }
}

void PlayerManager::send_msg_to_player_in_special_map(uint32_t cmd, uint32_t map_id, google::protobuf::Message& message)
{
    std::vector<player_t*> player_list;

    get_player_list(player_list);

    for (uint32_t i = 0; i < player_list.size(); i++) {
        player_t* player = player_list[i]; 
        //if (player->cur_map_id == map_id) {
            send_msg_to_player(player, cmd, message);
        //}
    }
}

void PlayerManager::earse_player_from_fd_users(int fd)
{
    std::map<int, player_t*>::iterator it1;
    it1 = fd_users_.find(fd);
    if (it1 != fd_users_.end()) {
        it1->second->fd = 0;
        fd_users_.erase(it1); 
    }
}

void PlayerManager::add_player_to_fd_users(player_t *p, int fd)
{
    fd_users_[fd] = p;
}
