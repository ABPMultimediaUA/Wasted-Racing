#pragma once

#include "IGameState.h"
#include "../Game.h"

class PreMatchState : public IGameState {

public: 

    //Constructor
    PreMatchState (){ type = IGameState::PAUSE; };

    //Destructor
    virtual ~PreMatchState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update(float &accumulatedTime);

    //Drawer
    virtual void draw();

    //Shutdown
    virtual void close();

    //Static class getter
    static PreMatchState& getInstance() {
        static PreMatchState instance;
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

    GameObject::Pointer camera;
    int i = 0;

};