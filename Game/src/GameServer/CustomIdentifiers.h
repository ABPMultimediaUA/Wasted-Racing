#pragma once
#include <raknet/MessageIdentifiers.h>

enum customMessages {
    ID_GAME_START = ID_USER_PACKET_ENUM,
    ID_GAME_ENDED,
    ID_CREATE_PLAYER,
    ID_CREATE_REMOTE_PLAYER
};