#include "ObjectRenderComponent.h"
#include "../../GameManager/RenderManager.h"

//Initilizer
void ObjectRenderComponent::init() {
    RenderManager::getInstance().getRenderFacade()->addObject(this);
}

//Update
void ObjectRenderComponent::update(float dTime) {

}

//Closer
void ObjectRenderComponent::close() {
    RenderManager::getInstance().getRenderFacade()->deleteObject(this);
}

//Drawer
void ObjectRenderComponent::draw() {
       
} 