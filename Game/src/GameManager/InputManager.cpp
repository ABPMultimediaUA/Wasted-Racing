#include "InputManager.h"
#include "../GameFacade/InputIrrlicht.h"

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

void InputManager::init(int engine){

   //Initialize our facade
   inputFacade = new InputIrrlicht();

   inputFacade->openInput();

   //Bind functions
   EventManager::getInstance().addListener(EventListener {EventType::Key_Select, selected});

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
    std::cout << "EventType::Key_Selected triggered!" << std::endl;
}