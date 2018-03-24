#include "ObjectRenderComponent.h"
#include "../../GameManager/RenderManager.h"
//:::>With proper initialization could be just a type information class

//Initilizer
void ObjectRenderComponent::init() {
    //:::> Shouldn't the render manager initialize it?
    RenderManager::getInstance().getRenderFacade()->addObject(this);
}

//Update
void ObjectRenderComponent::update(float dTime) {

}

//Closer
void ObjectRenderComponent::close() {
    //:::> Shouldn't the render manager erase it?
    RenderManager::getInstance().getRenderFacade()->deleteObject(this);
}

//Drawer
void ObjectRenderComponent::draw() {
       
} 