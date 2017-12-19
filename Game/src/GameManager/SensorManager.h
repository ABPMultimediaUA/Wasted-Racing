#pragma once

#include "../GameObject/ISensorComponent.h"
#include "../GameObject/VSensorComponent.h"
#include "../GameObject/MSensorComponent.h"
#include "ObjectManager.h"
#include "PhysicsManager.h"
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

    std::vector<IComponent::Pointer>& getComponentList() { //Visual sensors
        return sensorComponentList;
    }

    std::vector<IComponent::Pointer>& getMComponentList() {//Map sensors
        return sensorMComponentList;
    }

    //Static class getter
    static SensorManager& getInstance();

    //Component creators
    IComponent::Pointer createVSensorComponent(GameObject& newGameObject, float angV, float angI);
    IComponent::Pointer createMSensorComponent(GameObject& newGameObject, float angV, float angI);

private:
    std::vector<IComponent::Pointer> sensorComponentList;
    std::vector<IComponent::Pointer> sensorMComponentList;
    std::vector<GameObject> worldObjects;
};