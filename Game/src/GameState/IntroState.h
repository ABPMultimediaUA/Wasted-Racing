#pragma once

#include "IGameState.h"

class IntroState : public IGameState {

public: 

    //Constructor
    IntroState (){ type = IGameState::INTRO; };

    //Destructor
    virtual ~IntroState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update();

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

};