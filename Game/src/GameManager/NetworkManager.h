#pragma once

#include <raknet/RakPeerInterface.h> 
#include <raknet/MessageIdentifiers.h>
#include <raknet/BitStream.h>
#include "../GameServer/CustomIdentifiers.h"
#include <iostream>

class NetworkManager{

public: 

    //Constructor
    NetworkManager() {}

    //Destructor
    ~NetworkManager() {}

    //Initialization
    void init();

    //Update
    void update();

    //Shutdown
    void close();

    //Static class getter
    static NetworkManager& getInstance();

    //Initializes lobby
    void initLobby();

    //Updates the lobby
    void updateLobby();

    //==============================================================
    // Getters and setters
    //==============================================================
    void setStarted(bool s){    started = s;    };
    bool getStarted()      {    return started; };

private:
    //==============================================================
    // Private data
    //==============================================================
    //RakPeerInterface, manages the connection
    RakNet::RakPeerInterface* peer;

    //MatchState started
    bool started;

};