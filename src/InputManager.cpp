#include "InputManager.h"

bool InputManager::OnEvent(const SEvent& event){
    if(event.EventType == irr::EET_KEY_INPUT_EVENT)
        InputManager::KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

    return false;
}

bool InputManager::IsKeyDown(EKEY_CODE keyCode) const{
    return InputManager::KeyIsDown[keyCode];
}