#include "data_proto_utils.h"
#include "room.h"
#include "player.h"
#include "battle.h"

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

int DataProtoUtils::pack_battle_data(Battle* battle,
        onlineproto::battle_data_t* battle_data)
{
    battle_data->set_mapid(battle->mapid());
    onlineproto::battle_team_data_t* team1_data = battle_data->mutable_team1();
    onlineproto::battle_team_data_t* team2_data = battle_data->mutable_team2();

    team1_data->set_teamid(1);
    team2_data->set_teamid(2);

    const std::vector<battle_player_t>* team1 = battle->get_team1();
    const std::vector<battle_player_t>* team2 = battle->get_team2();
    if (team1 == NULL || team2 == NULL) {
        return 0;
    }

    for (uint32_t i = 0; i < team1->size(); i++) {
        onlineproto::battle_member_data_t* member_data1 = team1_data->add_member_list();
        member_data1->set_userid(team1->at(i).userid);
        member_data1->set_tankid(team1->at(i).tankid);
        member_data1->set_x(team1->at(i).x);
        member_data1->set_y(team1->at(i).y);
        member_data1->set_dir(team1->at(i).face);
    }

    for (uint32_t i = 0; i < team2->size(); i++) {
        onlineproto::battle_member_data_t* member_data2 = team2_data->add_member_list();
        member_data2->set_userid(team2->at(i).userid);
        member_data2->set_tankid(team2->at(i).tankid);
        member_data2->set_x(team2->at(i).x);
        member_data2->set_y(team2->at(i).y);
        member_data2->set_dir(team2->at(i).face);
    }

    return 0;
}

