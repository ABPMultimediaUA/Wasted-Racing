#include "InputManager.h"
#include "../GameFacade/InputGainput.h"

#include <iostream>

//==============================================
// DELEGATES DECLARATIONS
//==============================================
void selected(EventData eData);

//==============================================
// INPUT MANAGER FUNCTIONS
//==============================================
InputManager& InputManager::getInstance() {
    static InputManager instance;
    return instance;
}

void InputManager::init(){

    //Initialize our facade
    inputFacade = new InputGainput();

    inputFacade->openInput();

    //Bind functions
    EventManager::getInstance().addListener(EventListener {EventType::KeySelect, selected});

}

void InputManager::close(){

}

void InputManager::update(){

    inputFacade->updateInput();

}

//==============================================
// DELEGATES
//==============================================
void selected(EventData eData){
    std::cout << "EventType::KeySelected triggered!" << std::endl;
}