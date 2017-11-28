#include "InputManager.h"
#include "../GameFacade/InputIrrlicht.h"

#include <iostream>

//==============================================
// DELEGATES DECLARATIONS
//==============================================
void selectedDown(EventData eData); 
void selectedUp(EventData eData); 
void backDown(EventData eData); 
void backUp(EventData eData); 
void advanceDown(EventData eData); 
void advanceUp(EventData eData); 
void brakeDown(EventData eData); 
void brakeUp(EventData eData); 
void turnLeftDown(EventData eData); 
void turnLeftUp(EventData eData); 
void turnRightDown(EventData eData); 
void turnRightUp(EventData eData); 
void addInputComponent(EventData data);

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
    EventManager::getInstance().addListener(EventListener {EventType::Key_Select_Down, selectedDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Select_Up, selectedUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Back_Down, backDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Back_Up, backUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Advance_Down, advanceDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Advance_Up, advanceUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Brake_Down, brakeDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Brake_Up, brakeUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnLeft_Down, turnLeftDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnLeft_Up, turnLeftUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnRight_Down, turnRightDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnRight_Up, turnRightUp});
    EventManager::getInstance().addListener(EventListener {EventType::InputComponent_Create, addInputComponent});
  
}

void InputManager::close(){

}

void InputManager::update(){

    inputFacade->updateInput();

}

//==============================================
// DELEGATES
//==============================================
void selectedDown(EventData eData){
    std::cout << "Select button pressed!" << std::endl;
}
void selectedUp(EventData eData){
    std::cout << "Select button released!" << std::endl;
}
void backDown(EventData eData){
    std::cout << "Back button pressed!" << std::endl;
}
void backUp(EventData eData){
    std::cout << "Back button released!" << std::endl;
}
void advanceDown(EventData eData) {
    std::cout << "Advance button pressed!" << std::endl;
}
void advanceUp(EventData eData) {
    std::cout << "Advance button released!" << std::endl;
}
void brakeDown(EventData eData) {
    std::cout << "Brake button pressed!" << std::endl;
}
void brakeUp(EventData eData) {
    std::cout << "Brake button released!" << std::endl;
}
void turnLeftDown(EventData eData) {
    std::cout << "Left turn button pressed!" << std::endl;
}
void turnLeftUp(EventData eData) {
    std::cout << "Left turn button released!" << std::endl;
}
void turnRightDown(EventData eData) {
    std::cout << "Right turn button pressed!" << std::endl;
}
void turnRightUp(EventData eData) {
    std::cout << "Right turn button released!" << std::endl;
}
void addInputComponent(EventData data) {
    InputManager::getInstance().setComponent(data.Component);
    data.Component.get()->init();
}