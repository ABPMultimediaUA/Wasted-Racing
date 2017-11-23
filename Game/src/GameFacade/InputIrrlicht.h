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
        if(KeyIsDown[irr::EKEY_CODE::TheKey]){ \
            EventManager::getInstance().addEvent(Event {EventType::Event_Up}); \
            KeyIsDown[irr::EKEY_CODE::TheKey] = false; \
        } \
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

    virtual bool OnEvent(const irr::SEvent& event)
    {

        if(event.EventType == irr::EET_KEY_INPUT_EVENT){
            DetectKeyInput(KEY_SPACE,Key_Select_Down,Key_Select_Up)
            DetectKeyInput(KEY_ESCAPE,Key_Back_Down,Key_Back_Up)
            DetectKeyInput(KEY_KEY_W,Key_Advance_Down,Key_Advance_Up)
            DetectKeyInput(KEY_KEY_S,Key_Brake_Down,Key_Brake_Up)
            DetectKeyInput(KEY_KEY_A,Key_TurnLeft_Down,Key_TurnLeft_Up)
            DetectKeyInput(KEY_KEY_D,Key_TurnRight_Down,Key_TurnRight_Up)
        }

        if(event.EventType == irr::EET_MOUSE_INPUT_EVENT){
        }

        return false;
    }

    // This is used to check whether a key is being held down
    virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const
    {
        return KeyIsDown[keyCode];
    }

private: 

    bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
    
    irr::IrrlichtDevice *device;

};