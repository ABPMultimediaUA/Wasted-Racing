#pragma once

#include "../GameObject/WaypointComponent.h"
#include <vector>

class WaypointManager{

public: 

    //Constructor
    WaypointManager() {}

    //Destructor
    ~WaypointManager() {
    }

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

    //Component list getter
    std::vector<IComponent::Pointer>& getComponentList() {
        return waypointComponentList;
    }

private:
    std::vector<IComponent::Pointer>      waypointComponentList;

};