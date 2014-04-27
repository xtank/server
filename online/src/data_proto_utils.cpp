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
        room_player->set_userid(room->player_vec->at(j)->userid);
        room_player->set_status(room->player_vec->at(j)->status);
        room_player->set_teamid(room->player_vec->at(j)->teamid);
        room_player->set_name("");
    }

    return 0;
}

