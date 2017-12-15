#include "WaypointManager.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================

WaypointManager& WaypointManager::getInstance() {
    static WaypointManager instance;
    return instance;
}

void WaypointManager::init() {
    listSubNodes = new std::vector<GameObject::Pointer>;
}

void WaypointManager::update() {
    //I doubt this method should exist in this manager

}

void WaypointManager::close() {

}

IComponent::Pointer WaypointManager::createWaypointComponent(GameObject::Pointer newGameObject, float r, int lvl)
{
    IComponent::Pointer component = std::make_shared<WaypointComponent>(*newGameObject.get(), r, lvl);

    newGameObject.get()->addComponent(component);

    if(listSubNodes->size() == 0)
    {
        listSubNodes->push_back(newGameObject);
        listSubNodes->push_back(newGameObject);
    }
    else
    {
        listSubNodes->insert(listSubNodes->end()-1,newGameObject);
    }
    
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

void WaypointManager::setDistLastWay(GameObject::Pointer n, glm::vec3 pos)
{
    distLastWay = (n.get()->getTransformData().position.x - pos.x) * (n.get()->getTransformData().position.x - pos.x) +
                (n.get()->getTransformData().position.y - pos.y) * (n.get()->getTransformData().position.y - pos.y) +
                (n.get()->getTransformData().position.z - pos.z) * (n.get()->getTransformData().position.z - pos.z);
}

void WaypointManager::setLastPosVector(int lvl)
{
    lastPosVector = lvl;
}


