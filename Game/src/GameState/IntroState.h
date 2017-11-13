#pragma once

#include "IGameState.h"

class IntroState : public IGameState {

public: 

    //Constructor
    IntroState () : IGameState() {};

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

private:

};