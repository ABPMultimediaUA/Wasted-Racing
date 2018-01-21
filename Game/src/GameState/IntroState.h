#pragma once

#include "IGameState.h"
#include "MainState.h"
#include "ClientLobbyState.h"
#include "../Game.h"
#include "../GameEvent/EventManager.h"
#include "../GameManager/InputManager.h"
#include "../GameManager/RenderManager.h"
#include <raknet/RakPeerInterface.h> 
#include <raknet/MessageIdentifiers.h>

class Game;

class IntroState : public IGameState {

public: 

    //Constructor
    IntroState (){ type = IGameState::INTRO; };

    //Destructor
    virtual ~IntroState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update(float &accumulatedTime);

    //Drawer
    virtual void draw();

    //Shutdown
    virtual void close();

    //Static class getter
    static IntroState& getInstance() {
        static IntroState instance;
        return instance;
    };
private:
    //==============================================================
    // Private data
    //==============================================================

    //Variables for client, delete when state is working correctly
    RakNet::RakPeerInterface* peer;

    //Input manager
    InputManager* inputManager;
    //Render manager
    RenderManager* renderManager;
    //Event manager
    EventManager* eventManager;

    //Update's loop time
    const float loopTime = 1.0f/30.0f;

    //Mode choosing
    bool stay = false;
};