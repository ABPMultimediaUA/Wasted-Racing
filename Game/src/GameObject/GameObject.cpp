#include "GameObject.h"

//Init
void GameObject::init() {
    //Initilize all components
    for (auto comp : components)
		comp->init();
}

//Update
void GameObject::update(float dTime) {

}

//Add component
void GameObject::addComponent(IComponent::Pointer component) {
    components.push_back(component);
} 

GameObject::TransformationData& GameObject::getTransformData(){
		return t;
}