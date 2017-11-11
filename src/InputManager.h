#pragma once

#include <irrlicht.h>

using namespace irr;

class InputManager : public IEventReceiver {

public:

    //Constructor
    InputManager() {
        for(u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
            KeyIsDown[i] = false;
    }

    //Key event catcher
    virtual bool OnEvent(const SEvent& event);

    //Key event checker
    virtual bool IsKeyDown(EKEY_CODE keyCode) const;

private: 

    //Variable to save every key state
    bool KeyIsDown[KEY_KEY_CODES_COUNT];

};