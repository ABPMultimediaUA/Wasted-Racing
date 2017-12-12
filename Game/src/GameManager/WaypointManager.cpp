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

IComponent::Pointer WaypointManager::createWaypointComponent(GameObject& newGameObject, float r, int lvl)
{
    IComponent::Pointer component = std::make_shared<WaypointComponent>(newGameObject, r, lvl);

    newGameObject.addComponent(component);

    return component;
}


//==============================================
// DELEGATES
//============================================== 


//==============================================
//Constructor and Destructor
//==============================================

WaypointManager::WaypointManager()
{
    listSubNodes = new std::vector<GameObject::Pointer>;
}

WaypointManager::~WaypointManager()
{
//check how remove vector
    delete listSubNodes;
}

//==============================================
//Getters
//==============================================

std::vector<GameObject::Pointer> WaypointManager::getWaypoints()
{
    return *listSubNodes;
}

float WaypointManager::getDistLastWay()
{
    return distLastWay;
}

int WaypointManager::getLastPosVector()
{
    return lastPosVector;
}

//==============================================
//Setters
//==============================================

void WaypointManager::addWaypoints(GameObject::Pointer newGameObject)
{
    listSubNodes->push_back(newGameObject);
}

void WaypointManager::setDistLastWay(GameObject::Pointer n, glm::vec3 pos)
{
    distLastWay = (n->getTransformData().position.x - pos.x) * (n->getTransformData().position.x - pos.x) +
                (n->getTransformData().position.y - pos.y) * (n->getTransformData().position.y - pos.y) +
                (n->getTransformData().position.z - pos.z) * (n->getTransformData().position.z - pos.z);
}

void WaypointManager::setLastPosVector(int lvl)
{
    lastPosVector = lvl;
}


