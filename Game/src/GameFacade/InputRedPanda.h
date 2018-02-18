#pragma once

#include "IInputFacade.h"
#include "../GameEvent/EventManager.h"
#include "../GameManager/RenderManager.h"

#include <redpanda/RedPandaStudio.h>

//Define macros
#define DetectKeyInput(TheKey,Event_Down,Event_Up) \
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::TheKey) { \
            EventManager::getInstance().addEvent(Event {EventType::Event_Down}); \
    } \
    else if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::TheKey){ \
            EventManager::getInstance().addEvent(Event {EventType::Event_Up}); \
    };


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
    // Input Related functions
    //==============================================================

private: 
    
    sf::RenderWindow *device;

};