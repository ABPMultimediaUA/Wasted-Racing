#pragma once

#include "IGameState.h"

class MainState : public IGameState {

public: 

    //Constructor
    MainState (){ type = IGameState::MAIN; };

    //Destructor
    virtual ~MainState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update();

    //Drawer
    virtual void draw();

    //Shutdown
    virtual void close();

    //Static class getter
    static MainState& getInstance() {
        static MainState instance;
        return instance;
    };

private:

};