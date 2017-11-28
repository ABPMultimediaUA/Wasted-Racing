#pragma once

#include "IGameState.h"

class MatchState : public IGameState {

public: 

    //Constructor
    MatchState () : IGameState() {};

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

private:

};