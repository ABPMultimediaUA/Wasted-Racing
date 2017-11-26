#pragma once

#include "IInputFacade.h"
#include "../GameEvent/EventManager.h"
#include "../GameManager/RenderManager.h"

#include <irrlicht.h>

//Define macros
#define DetectKeyInput(TheKey,Event_Down,Event_Up) \
    if(event.KeyInput.PressedDown && event.KeyInput.Key == irr::EKEY_CODE::TheKey) { \
        if(!KeyIsDown[irr::EKEY_CODE::TheKey]){ \
            EventManager::getInstance().addEvent(Event {EventType::Event_Down}); \
            KeyIsDown[irr::EKEY_CODE::TheKey] = true; \
        } \
    } \
    else { \
        if(KeyIsDown[irr::EKEY_CODE::TheKey] && event.KeyInput.Key == irr::EKEY_CODE::TheKey){ \
            EventManager::getInstance().addEvent(Event {EventType::Event_Up}); \
            KeyIsDown[irr::EKEY_CODE::TheKey] = false; \
        } \
    };

struct SMouseState
{
        irr::core::position2di Position;
};



class InputIrrlicht : public IInputFacade, public irr::IEventReceiver {

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    InputIrrlicht() {
        for (irr::u32 i=0; i<irr::KEY_KEY_CODES_COUNT; ++i)
            KeyIsDown[i] = false;
    }

    //Destructor
    virtual ~InputIrrlicht() {

    }

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

    virtual bool OnEvent(const irr::SEvent& event);

    // This is used to check whether a key is being held down
    virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const
    {
        return KeyIsDown[keyCode];
    }

private: 

    bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
    SMouseState MouseState;
    
    irr::IrrlichtDevice *device;

};