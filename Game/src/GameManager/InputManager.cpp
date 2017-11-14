#include "InputManager.h"

bool InputManager::OnEvent(const SEvent& event){
    if(event.EventType == irr::EET_KEY_INPUT_EVENT)
        InputManager::KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

    return false;
}

bool InputManager::IsKeyDown(EKEY_CODE keyCode) const{
    return InputManager::KeyIsDown[keyCode];
}

InputManager& InputManager::getInstance() {
    static InputManager instance;
    return instance;
}

void InputManager::init(){
    for(u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
            InputManager::KeyIsDown[i] = false;
}

void InputManager::close(){

}

void InputManager::update(){

}