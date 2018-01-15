#include "LightRenderComponent.h"
#include "../../GameManager/RenderManager.h"

//Initilizer
void LightRenderComponent::init() {
    RenderManager::getInstance().getRenderFacade()->addLight(shared_from_this());
}

//Update
void LightRenderComponent::update(float dTime) {

}

//Closer
void LightRenderComponent::close() {
    RenderManager::getInstance().getRenderFacade()->deleteObject(shared_from_this());
}

//Drawer
void LightRenderComponent::draw() {
       
} 