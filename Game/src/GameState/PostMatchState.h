#pragma once

#include "IGameState.h"
#include "../Game.h"

class PostMatchState : public IGameState {

public: 

    //Constructor
    PostMatchState (){ type = IGameState::PAUSE; };

    //Destructor
    virtual ~PostMatchState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update(float &accumulatedTime);

    //Drawer
    virtual void draw();

    //Shutdown
    virtual void close();

    //Static class getter
    static PostMatchState& getInstance() {
        static PostMatchState instance;
        return instance;
    };

private:

    //==============================================================
    // Private data
    //==============================================================
    //Input manager
    InputManager* inputManager;
    //Event manager
    EventManager* eventManager;
    //Render manager
    RenderManager* renderManager;
    //Object manager
    ObjectManager* objectManager;
    //Audio Manager
    AudioManager* audioManager;

};