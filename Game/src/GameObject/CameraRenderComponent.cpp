#include "CameraRenderComponent.h"
#include "../GameManager/RenderManager.h"
#include <iostream>

//Initilizer
void CameraRenderComponent::init() {
    RenderManager::getInstance().getRenderFacade()->setCameraTarget(gameObject);
}

//Update
void CameraRenderComponent::update(float dTime) {

}

//Closer
void CameraRenderComponent::close() {

}

//Drawer
void CameraRenderComponent::draw() {
       
} 