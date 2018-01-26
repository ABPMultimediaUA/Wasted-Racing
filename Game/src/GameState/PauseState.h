#pragma once

#include "IGameState.h"

class PauseState : public IGameState {

public: 

    //Constructor
    PauseState () : IGameState() {};

    //Destructor
    virtual ~PauseState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update(float &accumulatedTime);

    //Drawer
    virtual void draw();

    //Shutdown
    virtual void close();

private:

};