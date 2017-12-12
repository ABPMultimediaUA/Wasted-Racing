#pragma once

#include "../GameObject/ISensorComponent.h"
#include <vector>

class SensorManager{

public: 

    //Constructor
    SensorManager() {}

    //Destructor
    ~SensorManager() {}

    //Initialization
    void init();

    //Update
    void update();

    //Shutdown
    void close();

    std::vector<IComponent::Pointer>& getComponentList() {
        return sensorComponentList;
    }

    //Static class getter
    static SensorManager& getInstance();

private:
    std::vector<IComponent::Pointer> sensorComponentList;
};