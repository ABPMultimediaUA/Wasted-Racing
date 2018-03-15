#pragma once

#include <vector>
#include "RenderManager.h"
#include "../GameObject/AIComponent/PathPlanningComponent.h"
#include "../GameObject/AIComponent/WaypointComponent.h"
#include "../GameObject/ItemComponent/IItemComponent.h"

class PathPlanningComponent;

class WaypointManager{

public: 

    //Constructor
    WaypointManager();

    //Destructor
    ~WaypointManager();

    //Static class getter
    static WaypointManager& getInstance() {
        static WaypointManager instance;
        return instance;
    }

    //Initialization
    void init();

    //Update
    void update(float dTime);

    //Shutdown
    void close();

    //Create a new Waypoint Component
    IComponent::Pointer createWaypointComponent(GameObject::Pointer newGameObject, float r, int lvl);
    IComponent::Pointer createPathPlanningComponent(GameObject::Pointer newGameObject, std::vector<GameObject::Pointer>& list);


    //==============================================
    // Setters & Getters
    //==============================================
    std::vector<IComponent::Pointer>& getComponentList()    { return waypointComponentList;     }   //Waypoint component list
    std::vector<IComponent::Pointer>& getPathPlanningList() { return pathPlanningComponentList; }   //PathPlanning component list
    std::vector<GameObject::Pointer> getWaypoints()         { return *listSubNodes;             }   //Waypoints of the map list

private:

    std::vector<IComponent::Pointer> waypointComponentList;
    std::vector<IComponent::Pointer> pathPlanningComponentList;
    std::vector<GameObject::Pointer> *listSubNodes;
    //Update PathPlanning
    void updatePathPlanning(IComponent::Pointer pathPlanning, float dTime);

    float distanceLoD; //////   PASAR A VARIABLE GLOBAL, ESTA EN AIMANAGER, WAYPOINTEMANAGER Y SENSORMANAGER

};