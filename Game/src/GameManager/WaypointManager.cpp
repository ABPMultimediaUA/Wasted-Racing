#include "WaypointManager.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================

WaypointManager& WaypointManager::getInstance() {
    static WaypointManager instance;
    return instance;
}

void WaypointManager::init() {

}

void WaypointManager::update() {
    //I doubt this method should exist in this manager

}

void WaypointManager::close() {

}

IComponent::Pointer WaypointManager::createWaypointComponent(GameObject& newGameObject, glm::vec3 pos, float r)
{
    IComponent::Pointer component = std::make_shared<WaypointComponent>(newGameObject, pos, r);

    newGameObject.addComponent(component);

    return component;
}


//==============================================
// DELEGATES
//============================================== 