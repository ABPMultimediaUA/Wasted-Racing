#pragma once

#include "RenderManager.h"
#include "../GameObject/AIComponent/PathPlanningComponent.h"
#include "../GameObject/AIComponent/WaypointComponent.h"
#include "../GameObject/ItemComponent/IItemComponent.h"
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
    void update(float dTime);

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

    std::vector<IComponent::Pointer>& getPathPlanningList() {
        return pathPlanningComponentList;
    }


    //Getters
    std::vector<GameObject::Pointer> getWaypoints();

    //Setters


    //Update PathPlanning
    void updatePathPlanning(IComponent::Pointer pathPlanning, float dTime);


private:

    std::vector<IComponent::Pointer>      waypointComponentList;
    std::vector<IComponent::Pointer>      pathPlanningComponentList;

    std::vector<GameObject::Pointer>      *listSubNodes;

    float distanceLoD; //////   PASAR A VARIABLE GLOBAL, ESTA EN AIMANAGER, WAYPOINTEMANAGER Y SENSORMANAGER

};