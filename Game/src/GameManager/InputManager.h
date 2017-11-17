#pragma once

#include <irrlicht.h>

using namespace irr;

class InputManager : public IEventReceiver {

public:

    //Constructor
    InputManager() {}

    //Initializer
    void init();

    //Updater
    void update();

    //Closer
    void close();

    //Key event catcher
    virtual bool OnEvent(const SEvent& event);

    //Key event checker
    virtual bool IsKeyDown(EKEY_CODE keyCode) const;

    //Static class getter
    static InputManager& getInstance();

private: 

    //Variable to save every key state
    bool KeyIsDown[KEY_KEY_CODES_COUNT];

};