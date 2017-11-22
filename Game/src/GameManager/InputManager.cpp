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

    //InputFacade is already initialized in RenderIrrlicht
    //Important!!! Once we swich to our own render engine, we have to init input also from there
    inputFacade->openInput(device);
 
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