#include "ObjectRenderComponent.h"
#include "../../GameManager/RenderManager.h"

//Initilizer
void ObjectRenderComponent::init() {

}

//Update
void ObjectRenderComponent::update(float dTime) {

}

//Closer
void ObjectRenderComponent::close() {
    std::cout << "BORRO" << std::endl;
    RenderManager::getInstance().getRenderFacade()->deleteObject(this);
    std::cout << "BORRADO" << std::endl;
}
  
//Drawer
void ObjectRenderComponent::draw() {
       
} 