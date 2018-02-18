#pragma once

#include "IInputFacade.h"
#include "../GameEvent/EventManager.h"
#include "../GameManager/RenderManager.h"

#include <redpanda/RedPandaStudio.h>


class InputRedPanda : public IInputFacade {

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    InputRedPanda() {}

    //Destructor
    virtual ~InputRedPanda() {}

    //==============================================================
    // Engine Related functions
    //==============================================================

    //Creates a window depending on the engine
    virtual void openInput(uintptr_t device);

    //Updates window info in the engine
    virtual void updateInput();

    //Closes engine window
    virtual void closeInput();

    //==============================================================
    // Irrlicht Related functions
    //==============================================================

private: 
    
    sf::RenderWindow *device;

};