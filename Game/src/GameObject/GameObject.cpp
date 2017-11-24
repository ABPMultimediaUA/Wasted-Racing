#include "GameObject.h"
#include "../GameEvent/EventManager.h"

//Init
void GameObject::init() {
    //Send an event saying object has been created
    EventManager::getInstance().addEvent(Event {EventType::GameObject_Create});

    //Initilize all components
    for (auto comp : components)
		comp->init();
}

//Update
void GameObject::update(float dTime) {

}

//Add component
void GameObject::addComponent(IComponent* component) {
    components.push_back(component);
}