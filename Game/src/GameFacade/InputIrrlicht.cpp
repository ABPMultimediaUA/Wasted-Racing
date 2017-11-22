#include "InputIrrlicht.h"
#include "../GameEvent/EventManager.h"
#include "../GameManager/RenderManager.h"

#include <iostream>

void InputIrrlicht::openInput() {

}

void InputIrrlicht::updateInput() { 

    if(!device->run())
        EventManager::getInstance().addEvent(Event {EventType::Game_Close});

}

void InputIrrlicht::closeInput() {
       
}  

bool InputIrrlicht::OnEvent(const irr::SEvent& event) {

    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
            switch(event.MouseInput.Event) {
            case irr::EMIE_LMOUSE_PRESSED_DOWN:
                MouseState.LeftButtonDown = true;
                break;

            case irr::EMIE_LMOUSE_LEFT_UP:
                MouseState.LeftButtonDown = false;
                break;

            case irr::EMIE_RMOUSE_PRESSED_DOWN:
                MouseState.RightButtonDown = true;
                break;

            case irr::EMIE_RMOUSE_LEFT_UP:
                MouseState.RightButtonDown = false;
                break;

            case irr::EMIE_MOUSE_MOVED:
                MouseState.Position.X = event.MouseInput.X;
                MouseState.Position.Y = event.MouseInput.Y;
                break;

            default:
                break;
            }
        }

    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

    return true;

}