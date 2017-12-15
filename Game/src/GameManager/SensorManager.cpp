#include "SensorManager.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================

SensorManager& SensorManager::getInstance() {
    static SensorManager instance;
    return instance;
}

void SensorManager::init() {
    GameObject::TransformationData transform;

    transform.position = glm::vec3(500,0,60);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(1,1,1);
    auto obj = ObjectManager::getInstance().createObject(20, transform);

    transform.position = glm::vec3(200, 0, 60);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto obj2 = ObjectManager::getInstance().createObject(21, transform);

    seenObjects.push_back(obj);
    seenObjects.push_back(obj2);

}

void SensorManager::update() {
    //This should call every time to all the sensors to update them.
    //Actually we dont have Environment, so we can't get the objects
    //from the map to be seen
    for(unsigned int i=0; i<sensorComponentList.size(); ++i){
         std::dynamic_pointer_cast<VSensorComponent>(sensorComponentList[i]).get()->updateSeenObjects(seenObjects);
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

//==============================================
// DELEGATES
//============================================== 
