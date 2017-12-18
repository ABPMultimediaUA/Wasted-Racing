#pragma once

#include "../GameObject/WaypointComponent.h"
#include "../GameObject/PathPlanningComponent.h"
#include <vector>

class WaypointManager{

public: 

    //Constructor
    WaypointManager();

    //Destructor
    ~WaypointManager();

    //Initialization
    void init();

    //Update
    void update();

    //Shutdown
    void close();

    //Static class getter
    static WaypointManager& getInstance();

    //Create a new Waypoint Component
    IComponent::Pointer createWaypointComponent(GameObject::Pointer newGameObject, float r, int lvl);
    IComponent::Pointer createPathPlanningComponent(GameObject::Pointer newGameObject);


    //Component list getter
    std::vector<IComponent::Pointer>& getComponentList() {
        return waypointComponentList;
    }


    //Getters
    std::vector<GameObject::Pointer> getWaypoints();

    //Setters



private:

    std::vector<IComponent::Pointer>      waypointComponentList;

    std::vector<GameObject::Pointer> *listSubNodes;


};