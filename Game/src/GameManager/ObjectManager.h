#pragma once

#include "../GameObject/GameObject.h"
#include <map>

class ObjectManager{

public: 

    //Constructor
    ObjectManager() {}

    //Destructor
    ~ObjectManager() {}

    //Initialization
    void init();

    //Shutdown
    void close();

    //Create an object and fire creation event
    GameObject::Pointer createObject(uint16_t id, GameObject::TransformationData transform);

    //Add an object
    void addObject(GameObject::Pointer ptr);

    //Delete an object
    void deleteObject(uint16_t id);

    //Get an object
    GameObject::Pointer getObject(uint16_t id);

    //Show the ids of the current objects
    void showObjects();

    //Init all objects
    void initObjects();

    //Static class getter
    static ObjectManager& getInstance();

private:

    //Map of objects
    std::map<uint16_t, GameObject::Pointer> objectsMap;

};