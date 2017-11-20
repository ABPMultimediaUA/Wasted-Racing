#pragma once

#include "IInputFacade.h"
#include "../GameEvent/EventManager.h"
#include "../GameManager/RenderManager.h"

#include <gainput/gainput.h>

class InputGainput : public IInputFacade {

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    InputGainput() : IInputFacade() {
        auto window = RenderManager::getInstance().getRenderFacade()->getWindow();

        inputManager.SetDisplaySize(window.size.x, window.size.y);

        inputMap = new gainput::InputMap(inputManager);

        keyboardId = inputManager.CreateDevice<gainput::InputDeviceKeyboard>();
        mouseId    = inputManager.CreateDevice<gainput::InputDeviceMouse>();
        padId      = inputManager.CreateDevice<gainput::InputDevicePad>();
        touchId    = inputManager.CreateDevice<gainput::InputDeviceTouch>();
    }

    //Destructor
    virtual ~InputGainput() {
        delete inputMap;
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

private: 

    gainput::InputManager inputManager;
    gainput::InputMap     *inputMap;

    gainput::DeviceId keyboardId;
    gainput::DeviceId mouseId;
    gainput::DeviceId padId;
    gainput::DeviceId touchId;

};