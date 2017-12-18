#include "ObjectManager.h"
#include "../GameEvent/EventManager.h"
#include <iostream>

//==============================================
// DELEGATES DECLARATIONS
//==============================================
//An event of creation or deletion will contain the shared pointer 
//pointing to the created/deleted object
//When an object is created or deleted, they are automatically added
//or deleted to the objectMap
void objectCreated(EventData eData); 
void objectDeleted(EventData eData); 

//==============================================
// OBJECT MANAGER FUNCTIONS
//============================================== 
ObjectManager& ObjectManager::getInstance() {
    static ObjectManager instance;
    return instance;
}

void ObjectManager::init() {

    //Bind functions
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Create, objectCreated});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeleted});

}

void ObjectManager::close() {

    objectsMap.clear();

}

GameObject::Pointer ObjectManager::createObject(uint16_t id, GameObject::TransformationData transform) {

    GameObject::Pointer object = std::make_shared<GameObject>(id, transform);

    EventData data;
    data.Object = object;

    EventManager::getInstance().addEvent(Event {EventType::GameObject_Create, data});

    return object;
}


void ObjectManager::addObject(GameObject::Pointer ptr) {

    if(objectsMap.find(ptr.get()->getId()) != objectsMap.end())
        std::cerr << "Couldn't insert object. ID: " << ptr.get()->getId() << " already exists." << std::endl;
    else 
        objectsMap.insert(std::pair<uint16_t,GameObject::Pointer>(ptr.get()->getId(), ptr));


}

void ObjectManager::deleteObject(uint16_t id) {

    auto obj = getObject(id);
    if(obj!=nullptr)
        obj.get()->close();

    if(objectsMap.erase(id) == 0)
        std::cerr << "Couldn't erase object. ID: " << id << " doesn't exist." << std::endl;

}

GameObject::Pointer ObjectManager::getObject(uint16_t id) {

    auto object = objectsMap.find(id);

    if(object == objectsMap.end()){
        std::cerr << "Couldn't find object. ID: " << id << " doesn't exist." << std::endl;
        return nullptr;
    }
    else{
        return object->second;
    }
}

void ObjectManager::showObjects() {
    for(auto obj : objectsMap)
        std::cout << obj.second.get()->getId() << std::endl;
}

void ObjectManager::initObjects() {
    for(auto obj : objectsMap)
        obj.second.get()->init();
}

//==============================================
// DELEGATES
//============================================== 
void objectCreated(EventData eData) {

    ObjectManager::getInstance().addObject(eData.Object);

}
void objectDeleted(EventData eData) {

    ObjectManager::getInstance().deleteObject(eData.Id);

}