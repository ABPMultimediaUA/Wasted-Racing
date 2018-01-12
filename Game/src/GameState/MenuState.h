#pragma once

#include "IGameState.h"

class MenuState : public IGameState {

public: 

    //Constructor
    MenuState (){};

    //Destructor
    virtual ~MenuState() {}

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