#include "SensorManager.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================

SensorManager& SensorManager::getInstance() {
    static SensorManager instance;
    return instance;
}

void SensorManager::init() {
    //worldObjects.push_back(obj);
    //worldObjects.push_back(obj2);

}

void SensorManager::update() {
    //This should call every time to all the sensors to update them.
    //Actually we dont have Environment, so we can't get the objects
    //from the map to be seen

    //Clean the list
    worldObjects.clear();

    //Fill list of world objects
    auto collisionList =  PhysicsManager::getInstance().getCollisionComponentList();
    for(unsigned i = 0; i < collisionList.size(); ++i){
        worldObjects.push_back(std::dynamic_pointer_cast<CollisionComponent>(collisionList[i]).get()->getGameObject());
    }

    //Update visual sensors
    for(unsigned int i=0; i<sensorComponentList.size(); ++i){
         std::dynamic_pointer_cast<VSensorComponent>(sensorComponentList[i]).get()->updateSeenObjects(worldObjects);
    }

    //Update map sensors
    for(unsigned int i=0; i<sensorMComponentList.size(); ++i){
         std::dynamic_pointer_cast<MSensorComponent>(sensorMComponentList[i]).get()->updateSeenObjects();
    }
}

void SensorManager::close() {

}



//COMPONENT CREATORS
IComponent::Pointer SensorManager::createVSensorComponent(GameObject& newGameObject, float angV, float angI){

    IComponent::Pointer component = std::make_shared<VSensorComponent>(newGameObject, angV, angI);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    SensorManager::getInstance().getComponentList().push_back(data.Component);
    data.Component.get()->init();

    EventManager::getInstance().addEvent(Event {EventType::VSensorComponent_Create, data});

    return component;
}

IComponent::Pointer SensorManager::createMSensorComponent(GameObject& newGameObject, float angV, float angI){

    IComponent::Pointer component = std::make_shared<MSensorComponent>(newGameObject, angV, angI);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    SensorManager::getInstance().getMComponentList().push_back(data.Component);
    data.Component.get()->init();

    EventManager::getInstance().addEvent(Event {EventType::MSensorComponent_Create, data});

    return component;
}

//==============================================
// DELEGATES
//============================================== 
