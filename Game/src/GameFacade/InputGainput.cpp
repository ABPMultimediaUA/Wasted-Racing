#include "InputGainput.h"

#include <iostream>

void InputGainput::openInput() {

    inputMap->MapBool(EventType::KeySelect, keyboardId, gainput::KeyKpEnter);
    inputMap->MapBool(EventType::KeySelect, mouseId, gainput::MouseButtonLeft);
    inputMap->MapBool(EventType::KeySelect, padId, gainput::PadButtonA);
    inputMap->MapBool(EventType::KeySelect, touchId, gainput::Touch0Down);

}

void InputGainput::updateInput() { 

    inputManager.Update();

    if(inputMap->GetBoolWasDown(EventType::KeySelect)){
        EventManager::getInstance().addEvent(Event {EventType::KeySelect});
    }

}

void InputGainput::closeInput() {
      
}  