#include "SensorManager.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================

void objectDeleteMSensor(EventData data);
void objectDeleteVSensor(EventData data);

SensorManager& SensorManager::getInstance() {
    static SensorManager instance;
    return instance;
}

void SensorManager::init() {
    //worldObjects.push_back(obj);
    //worldObjects.push_back(obj2);

    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeleteVSensor});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeleteMSensor});

}

void SensorManager::update() {
    //This should call every time to all the sensors to update them.
    //Actually we dont have Environment, so we can't get the objects
    //from the map to be seen

    //Clean the list
    worldObjects.clear();

    //Fill list of world objects
    auto collisionList =  PhysicsManager::getInstance().getCollisionComponentList();
    for(unsigned int i = 0; i < collisionList.size(); ++i){
        if(collisionList[i] != nullptr && collisionList[i].get()->getGameObject().getComponent<ItemBoxComponent>() == nullptr && collisionList[i].get()->getGameObject().getComponent<IItemComponent>() == nullptr && collisionList[i].get()->getGameObject().getComponent<RampComponent>() == nullptr)
        {
            worldObjects.push_back(std::dynamic_pointer_cast<CollisionComponent>(collisionList[i]).get()->getGameObject());
        }   
    }

    //Update visual sensors
    for(unsigned int i=0; i<sensorComponentList.size(); ++i){
        if(sensorComponentList[i] != nullptr)
            std::dynamic_pointer_cast<VSensorComponent>(sensorComponentList[i]).get()->updateSeenObjects(worldObjects);
    }

    //Update map sensors
    for(unsigned int i=0; i<sensorMComponentList.size(); ++i){
        if(sensorMComponentList[i] != nullptr)
            std::dynamic_pointer_cast<MSensorComponent>(sensorMComponentList[i]).get()->updateMapCollisions();
    }
}

void SensorManager::close() {
    sensorComponentList.clear();
    sensorMComponentList.clear();
    worldObjects.clear();
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

void objectDeleteVSensor(EventData eData) {

    auto& vSensorList = SensorManager::getInstance().getComponentList();

    for(unsigned int i = 0; i<vSensorList.size(); ++i) {
        if(eData.Id == vSensorList[i].get()->getGameObject().getId()) {
            vSensorList.erase(vSensorList.begin() + i);
            return;
        }
    }
}

void objectDeleteMSensor(EventData eData) {

    auto& MSensorList = SensorManager::getInstance().getMComponentList();

    for(unsigned int i = 0; i<MSensorList.size(); ++i) {
        if(eData.Id == MSensorList[i].get()->getGameObject().getId()) {
            MSensorList.erase(MSensorList.begin() + i);
            return;
        }
    }
}