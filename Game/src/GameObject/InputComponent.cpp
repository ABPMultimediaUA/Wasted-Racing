#include "InputComponent.h"
#include "../GameEvent/EventManager.h"
#include "../GameManager/InputManager.h"
#include "../GameManager/RenderManager.h"
#include "GameObject.h"
#include "MoveComponent.h"
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
    auto comp = InputManager::getInstance().getComponent().get()->getGameObject().getComponent<MoveComponent>();
    comp->changeAccInc(1);
}
void advanceUpI(EventData eData) {
   auto comp = InputManager::getInstance().getComponent().get()->getGameObject().getComponent<MoveComponent>();
   comp->changeAccInc(0.0);
}
void brakeDownI(EventData eData) {
   auto comp = InputManager::getInstance().getComponent().get()->getGameObject().getComponent<MoveComponent>();
   comp->changeAccInc(-1);
}
void brakeUpI(EventData eData) {
   auto comp = InputManager::getInstance().getComponent().get()->getGameObject().getComponent<MoveComponent>();
   comp->changeAccInc(0.0);
}
void turnLeftDownI(EventData eData) {
    auto comp = InputManager::getInstance().getComponent().get()->getGameObject().getComponent<MoveComponent>();
    comp->changeAngle(1);
}
void turnLeftUpI(EventData eData) {
    auto comp = InputManager::getInstance().getComponent().get()->getGameObject().getComponent<MoveComponent>();
    comp->changeAngle(0.0);
}
void turnRightDownI(EventData eData) {
    auto comp = InputManager::getInstance().getComponent().get()->getGameObject().getComponent<MoveComponent>();
    comp->changeAngle(-1);
}
void turnRightUpI(EventData eData) {
    auto comp = InputManager::getInstance().getComponent().get()->getGameObject().getComponent<MoveComponent>();
    comp->changeAngle(0.0);
}