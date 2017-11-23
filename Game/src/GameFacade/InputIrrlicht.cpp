#include "InputIrrlicht.h"
#include "../GameEvent/EventManager.h"
#include "../GameManager/RenderManager.h"

#include <iostream>

void InputIrrlicht::openInput(uintptr_t dev) {

    device = reinterpret_cast<irr::IrrlichtDevice*>(dev);

}

void InputIrrlicht::updateInput() { 

    if(!device->run())
        EventManager::getInstance().addEvent(Event {EventType::Game_Close});

}

void InputIrrlicht::closeInput() {
       
}  