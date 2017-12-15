#pragma once

#include "../GameObject/ISensorComponent.h"
#include "../GameObject/VSensorComponent.h"
#include "ObjectManager.h"
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

    //Component creators
    IComponent::Pointer createVSensorComponent(GameObject& newGameObject, float angV, float angI);

private:
    std::vector<IComponent::Pointer> sensorComponentList;
    std::vector<GameObject::Pointer> seenObjects;
};