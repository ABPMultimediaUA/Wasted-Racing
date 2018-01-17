#include "LightRenderComponent.h"
#include "../../GameManager/RenderManager.h"

//Initilizer
void LightRenderComponent::init() {
    RenderManager::getInstance().getRenderFacade()->addLight(this);
}

//Update
void LightRenderComponent::update(float dTime) {

}

//Closer
void LightRenderComponent::close() {
    RenderManager::getInstance().getRenderFacade()->deleteObject(this);
}

//Drawer
void LightRenderComponent::draw() {
       
} 