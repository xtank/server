
#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include <map>
#include <vector>

#include <libtaomee/project/types.h>
#include <google/protobuf/message.h>

struct player_t;

class PlayerManager
{
public:
    PlayerManager();
    ~PlayerManager();

    player_t* get_player_by_fd(int fd);

    player_t* get_player_by_userid(userid_t userid);

    player_t* alloc_player(int fd, userid_t userid);

    void dealloc_player(player_t* player);

    void get_userid_list(std::vector<userid_t>& userid_list);

    void get_player_list(std::vector<player_t*>& player_list);

    void send_msg_to_all_player(uint32_t cmd, const google::protobuf::Message& message);

    void send_msg_to_player_in_special_map(uint32_t cmd, uint32_t map_id, google::protobuf::Message& message);
    
    void earse_player_from_fd_users(int fd);

    void add_player_to_fd_users(player_t *p, int fd);

private:

    typedef std::map<userid_t, player_t *>::iterator id_users_ptr_t;

    std::map<int, player_t *> fd_users_; // fd映射player
    std::map<userid_t, player_t *> id_users_; // 米米号映射player
};

#endif
