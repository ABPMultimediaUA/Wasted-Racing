#include "SensorManager.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================

SensorManager& SensorManager::getInstance() {
    static SensorManager instance;
    return instance;
}

void SensorManager::init() {

}

void SensorManager::update() {
    //This should call every time to all the sensors to update them.
    //Actually we dont have Environment, so we can't get the objects
    //from the map to be seen
    GameObject::TransformationData transform;
    std::vector<GameObject::Pointer> v;

    transform.position = glm::vec3(30,0,30);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(1,1,1);
    auto obj = ObjectManager::getInstance().createObject(20, transform);

    transform.position = glm::vec3(100, 0, 0);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto obj2 = ObjectManager::getInstance().createObject(21, transform);

    v.push_back(obj);
    v.push_back(obj2);

    for(unsigned int i=0; i<sensorComponentList.size(); ++i){
         std::dynamic_pointer_cast<VSensorComponent>(sensorComponentList[i]).get()->updateSeenObjects(v);
    }

}

void SensorManager::close() {

}



//COMPONENT CREATORS
IComponent::Pointer SensorManager::createVSensorComponent(GameObject& newGameObject){

    IComponent::Pointer component = std::make_shared<VSensorComponent>(newGameObject);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    EventManager::getInstance().addEvent(Event {EventType::VSensorComponent_Create, data});

    return component;
}

//==============================================
// DELEGATES
//============================================== 
void addVSensorComponent(EventData data) {
    SensorManager::getInstance().getComponentList().push_back(data.Component);
    data.Component.get()->init();
}