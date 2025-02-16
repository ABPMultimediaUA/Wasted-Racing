#pragma once
#include <raknet/MessageIdentifiers.h>

enum customMessages {
    //Initial raknet variables
    ID_GAME_START = ID_USER_PACKET_ENUM,
    //Game related
    ID_GAME_ENDED,    
    ID_ACCUMULATED_TIME,
    //Player management
    ID_PLAYERS_POSITION,
    ID_REMOTE_PLAYER_MOVEMENT,
    ID_CREATE_PLAYER,
    ID_CREATE_REMOTE_PLAYER,
    //Item management
    ID_BOX_COLLISION,
    ID_CREATE_TRAP,
    ID_DESTROY_TRAP,
    ID_CREATE_RED_SHELL,
    ID_RED_SHELLS_POSITION,
    ID_RED_SHELL_MOVEMENT,
    ID_REMOTE_RED_SHELL_MOVEMENT,
    ID_DESTROY_RED_SHELL,
    ID_CREATE_BLUE_SHELL,
    ID_BLUE_SHELLS_POSITION,
    ID_REMOTE_BLUE_SHELL_MOVEMENT,
    ID_DESTROY_BLUE_SHELL
};