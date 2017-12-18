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
    //I doubt it too

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

IComponent::Pointer WaypointManager::createPathPlanningComponent(GameObject::Pointer newGameObject)
{
    IComponent::Pointer component = std::make_shared<PathPlanningComponent>(*newGameObject.get());

    newGameObject.get()->addComponent(component);

    
    
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

