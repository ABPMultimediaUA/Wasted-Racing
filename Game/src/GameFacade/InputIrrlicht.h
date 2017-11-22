#pragma once

#include "IInputFacade.h"
#include "../GameEvent/EventManager.h"
#include "../GameManager/RenderManager.h"

#include <irrlicht.h>

class InputIrrlicht : public IInputFacade, irr::IEventReceiver {

public:


    struct SMouseState
    {
        irr::core::position2di Position;
        bool LeftButtonDown;
        bool RightButtonDown;
    };

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    InputIrrlicht() : IInputFacade() {

        for (irr::u32 i=0; i<irr::KEY_KEY_CODES_COUNT; ++i)
            KeyIsDown[i] = false;
        MouseState.Position.X = 0;
        MouseState.Position.Y = 0;
        MouseState.LeftButtonDown = false;
        MouseState.RightButtonDown = false;
    }

    //Destructor
    virtual ~InputIrrlicht() {

    }

    //==============================================================
    // Engine Related functions
    //==============================================================

    //Creates a window depending on the engine
    virtual void openInput();

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