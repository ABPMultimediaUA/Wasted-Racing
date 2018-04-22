#include "LightRenderComponent.h"
#include "../../GameManager/RenderManager.h"
//:::>With proper initialization could be just a type information class

//Initilizer
void LightRenderComponent::init() {
    //:::> Shouldn't the render manager initialize it?
    //<___  Did it in the render manager
    //RenderManager::getInstance().getRenderFacade()->addLight(this);
    //___>
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