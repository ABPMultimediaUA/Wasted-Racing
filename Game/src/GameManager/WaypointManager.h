#pragma once

#include "../GameObject/WaypointComponent.h"
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

    //Draw
    void draw();

    //Shutdown
    void close();

    //Static class getter
    static WaypointManager& getInstance();

    //Create a new Waypoint Component
    IComponent::Pointer createWaypointComponent(GameObject::Pointer newGameObject, float r, int lvl);


    //Component list getter
    std::vector<IComponent::Pointer>& getComponentList() {
        return waypointComponentList;
    }


    //Getters
    std::vector<GameObject::Pointer> getWaypoints();
    float getDistLastWay();
    int getLastPosVector();

    //Setters
    void setDistLastWay(GameObject::Pointer n, glm::vec3 pos);
    void setLastPosVector(int lvl);



private:

    std::vector<IComponent::Pointer>      waypointComponentList;

    std::vector<GameObject::Pointer> *listSubNodes;

    float distLastWay = -1;
    int lastPosVector = 0;

};