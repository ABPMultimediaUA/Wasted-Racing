#include "ObjectRenderComponent.h"
#include "../GameManager/RenderManager.h"

//Initilizer
void ObjectRenderComponent::init() {
    RenderManager::getInstance().getRenderFacade()->addObject(shared_from_this());
}

//Update
void ObjectRenderComponent::update(float dTime) {

}

//Closer
void ObjectRenderComponent::close() {

}

//Drawer
void ObjectRenderComponent::draw() {
       
} 