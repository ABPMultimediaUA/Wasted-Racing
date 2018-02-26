#include "InputComponent.h"

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
void jumpUpI(EventData eData);
void jumpDownI(EventData eData);
void driftUpI(EventData eData);
void driftDownI(EventData eData);
void useItemDownI(EventData eData);

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
    EventManager::getInstance().addListener(EventListener {EventType::Key_Jump_Down, jumpDownI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Jump_Up, jumpUpI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Drift_Down, driftDownI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Drift_Up, driftUpI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_UseItem_Down, useItemDownI});

}

void InputComponent::update(float dTime){

}

void InputComponent::close(){

}

//==============================================
// DELEGATES
//==============================================
void advanceDownI(EventData eData) {
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
    comp->changeAccInc(comp->getMovemententData().max_acc);
    comp->isMoving(true);
}
void advanceUpI(EventData eData) {
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
    comp->changeAccInc(0.0);
    comp->isMoving(false);
}
void brakeDownI(EventData eData) {
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
    comp->isBraking(true);
}
void brakeUpI(EventData eData) {
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
        comp->isBraking(false);
}
void turnLeftDownI(EventData eData) {
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
    comp->changeSpinIncrement(0.1);
    comp->isSpinning(true);
}
void turnLeftUpI(EventData eData) {
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
    comp->changeSpinIncrement(0.0);
    comp->isSpinning(false);
}
void turnRightDownI(EventData eData) {
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
   comp->changeSpinIncrement(-0.1);
    comp->isSpinning(true);
}
void turnRightUpI(EventData eData) {
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
    comp->changeSpinIncrement(0.0);
    comp->isSpinning(false);
}
void jumpDownI(EventData eData){
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
    comp->isJumping(true);
}
void jumpUpI(EventData eData){
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
    comp->isJumping(false);
}
void driftDownI(EventData eData){
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
    comp->isDrifting(true);
}
void driftUpI(EventData eData){
    auto comp = GlobalVariables::getInstance().getPlayer()->getComponent<MoveComponent>();
    comp->isDrifting(false);
}

void useItemDownI(EventData eData){
    EventManager::getInstance().addEvent(Event {EventType::Item_Create, eData});
}