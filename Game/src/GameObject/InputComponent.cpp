#include "InputComponent.h"
#include "../GameEvent/EventManager.h"
#include <iostream>

//==============================================
// DELEGATES DECLARATIONS
//==============================================
void advanceDownI(EventData eData); 
void advanceUpI(EventData eData); 
void brakeDownI(EventData eData); 
void brakeUpI(EventData eData); 
void turnLeftDownI(EventData eData); 
void turnLeftUpI(EventData eData); 
void turnRightDownI(EventData eData); 
void turnRightUpI(EventData eData); 

//==============================================
// INPUT COMPONENT FUNCTIONS
//============================================== 

void InputComponent::init(){

    //Bind functions
    EventManager::getInstance().addListener(EventListener {EventType::Key_Advance_Down, advanceDownI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Advance_Up, advanceUpI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Brake_Down, brakeDownI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Brake_Up, brakeUpI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnLeft_Down, turnLeftDownI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnLeft_Up, turnLeftUpI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnRight_Down, turnRightDownI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnRight_Up, turnRightUpI});
  
}

void InputComponent::update(float dTime){

}

void InputComponent::close(){

}

//==============================================
// DELEGATES
//==============================================
void advanceDownI(EventData eData) {
    std::cout << "Advance button pressed!" << std::endl;
}
void advanceUpI(EventData eData) {
    std::cout << "Advance button released!" << std::endl;
}
void brakeDownI(EventData eData) {
    std::cout << "Brake button pressed!" << std::endl;
}
void brakeUpI(EventData eData) {
    std::cout << "Brake button released!" << std::endl;
}
void turnLeftDownI(EventData eData) {
    std::cout << "Left turn button pressed!" << std::endl;
}
void turnLeftUpI(EventData eData) {
    std::cout << "Left turn button released!" << std::endl;
}
void turnRightDownI(EventData eData) {
    std::cout << "Right turn button pressed!" << std::endl;
}
void turnRightUpI(EventData eData) {
    std::cout << "Right turn button released!" << std::endl;
}