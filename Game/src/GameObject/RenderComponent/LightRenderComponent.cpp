#include "LightRenderComponent.h"
#include "../../GameManager/RenderManager.h"
//:::>With proper initialization could be just a type information class

//Initilizer
void LightRenderComponent::init() {
    //:::> Shouldn't the render manager initialize it?
    RenderManager::getInstance().getRenderFacade()->addLight(this);
}

//Update
void LightRenderComponent::update(float dTime) {

}

//Closer
void LightRenderComponent::close() {
    //:::> Shouldn't the render manager erase it?
    RenderManager::getInstance().getRenderFacade()->deleteObject(this);
}

//Drawer
void LightRenderComponent::draw() {
       
} 