#pragma once

#include "IGameState.h"
#include "MainState.h"
#include "../Game.h"
#include "../GameManager/NetworkManager.h"
#include <iostream>

class Game;

class ClientLobbyState : public IGameState {

public: 

    //Constructor
    ClientLobbyState (){ type = IGameState::CLIENTLOBBY; };

    //Destructor
    virtual ~ClientLobbyState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update(float &accumulatedTime);

    //Drawer
    virtual void draw();

    //Shutdown
    virtual void close();

    //Static class getter
    static ClientLobbyState& getInstance() {
        static ClientLobbyState instance;
        return instance;
    };
private:
    //==============================================================
    // Private data
    //==============================================================

    //NetworkManager
    NetworkManager* networkManager;

    //Update's loop time
    const float loopTime = 1.0f/30.0f;
};