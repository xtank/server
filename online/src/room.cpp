#include "room.h"


int init_room(room_t* room) {

    room->player_vec = new std::vector<player_t*>();

    return 0;
}

int uninit_room(room_t* room) {

    delete room->player_vec;
    room->player_vec = NULL;

    return 0;
}



