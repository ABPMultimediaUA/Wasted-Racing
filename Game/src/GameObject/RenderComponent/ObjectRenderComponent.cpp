#include "ObjectRenderComponent.h"
#include "../../GameManager/RenderManager.h"
//:::>With proper initialization could be just a type information class

//Initilizer
void ObjectRenderComponent::init() {
    //:::> Shouldn't the render manager initialize it?
    //<___ I did it
    //RenderManager::getInstance().getRenderFacade()->addObject(this);
    //___>
}

//Update
void ObjectRenderComponent::update(float dTime) {

}

//Closer
void ObjectRenderComponent::close() {
    //:::> Shouldn't the render manager erase it?
    //<___ I did it in the render manager
    RenderManager::getInstance().getRenderFacade()->deleteObject(this);
    //___>
}
  
//Drawer
void ObjectRenderComponent::draw() {
       
} 