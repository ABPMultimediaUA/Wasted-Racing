#include "ObjectManager.h"
#include "../GameEvent/EventManager.h"
#include <iostream>

//==============================================
// DELEGATES DECLARATIONS
//==============================================
//An event of creation or deletion will contain the shared pointer 
//pointing to the created/deleted object
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


void ObjectManager::addObject(GameObject::Pointer ptr) {

    if(objectsMap.find(ptr.get()->getId()) != objectsMap.end())
        std::cerr << "Couldn't insert object. ID: " << ptr.get()->getId() << " already exists." << std::endl;
    else 
        objectsMap.insert(std::pair<uint16_t,GameObject::Pointer>(ptr.get()->getId(), ptr));


}

void ObjectManager::deleteObject(uint16_t id) {

    objectsMap.erase(id);

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
        std::cout << obj.second.get()->getId();
}

void ObjectManager::initObjects() {
    for(auto obj : objectsMap)
        obj.second.get()->init();
}

//==============================================
// DELEGATES
//============================================== 
void objectCreated(EventData eData) {

    uintptr_t ptr = eData.at(0);
    GameObject::Pointer* object = reinterpret_cast<GameObject::Pointer*>(ptr);

    ObjectManager::getInstance().addObject(*object);

}
void objectDeleted(EventData eData) {

    GameObject::Pointer* object = reinterpret_cast<GameObject::Pointer*>(eData.at(0));

    ObjectManager::getInstance().deleteObject(object->get()->getId());

}