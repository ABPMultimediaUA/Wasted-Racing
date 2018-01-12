#pragma once

#include "IGameState.h"

class MatchState : public IGameState {

public: 

    //Constructor
    MatchState () { type = IGameState::MAIN; };

    //Destructor
    virtual ~MatchState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update();

    //Drawer
    virtual void draw();

    //Shutdown
    virtual void close();

    //Static class getter
    static MatchState& getInstance() {
        static MatchState instance;
        return instance;
    };

private:

};