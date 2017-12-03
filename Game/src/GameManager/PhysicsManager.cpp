#include "PhysicsManager.h"
#include "../GameObject/GameObject.h"
#include "../GameEvent/EventManager.h"
#include <memory>

//==============================================
// DELEGATES DECLARATIONS
//==============================================
void addMoveComponent(EventData eData); 

//==============================================
// PHYSICS MANAGER FUNCTIONS
//============================================== 
PhysicsManager& PhysicsManager::getInstance() {
    static PhysicsManager instance;
    return instance;
}

void PhysicsManager::init() {

    //Bind listeners
    EventManager::getInstance().addListener(EventListener {EventType::MoveComponent_Create, addMoveComponent});

}

void PhysicsManager::update(const float dTime) {

    for(unsigned int i=0; i<moveComponentList.size(); ++i){
        moveComponentList.at(i).get()->update(dTime);
    }

}

void PhysicsManager::close() {

}

IComponent::Pointer PhysicsManager::createMoveComponent(GameObject& newGameObject, LAPAL::movementData newMData, float newMass) {

    IComponent::Pointer component = std::make_shared<MoveComponent>(newGameObject, newMData, newMass);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    EventManager::getInstance().addEvent(Event {EventType::MoveComponent_Create, data});

    return component;
}

//==============================================
// DELEGATES
//============================================== 
void addMoveComponent(EventData data) {
    PhysicsManager::getInstance().getMoveComponentList().push_back(data.Component);
    data.Component.get()->init();
}