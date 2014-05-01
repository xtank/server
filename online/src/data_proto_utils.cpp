#include "data_proto_utils.h"
#include "room.h"
#include "player.h"

int DataProtoUtils::pack_room_data(const room_t* room,
        onlineproto::room_data_t* room_data)
{
    room_data->set_id(room->room_id);
    room_data->set_ownerid(room->owner_id);
    room_data->set_mapid(room->map_id);
    room_data->set_status(room->status);
    room_data->set_name(room->name);

    for (uint32_t j = 0; j < room->player_vec->size(); j++) {
        onlineproto::player_data_t* room_player = room_data->add_playlist();
        //room_player->set_userid(room->player_vec->at(j)->userid);
        //room_player->set_status(room->player_vec->at(j)->status);
        //room_player->set_teamid(room->player_vec->at(j)->teamid);
        //room_player->set_name("");

        pack_player_data(room->player_vec->at(j), room_player);
    }

    return 0;
}

int DataProtoUtils::pack_player_data(const player_t* player,
        onlineproto::player_data_t* player_data)
{
    player_data->set_userid(player->userid);
    player_data->set_status(player->status);
    player_data->set_teamid(player->teamid);
    player_data->set_tankid(player->tankid);
    player_data->set_name("");
    player_data->set_seatid(player->seatid);

    return 0;
}

