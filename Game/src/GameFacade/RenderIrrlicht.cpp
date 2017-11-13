#include "RenderIrrlicht.h"


void RenderIrrlicht::openWindow(){

    video::E_DRIVER_TYPE driverType = video::E_DRIVER_TYPE::EDT_OPENGL;

    RenderIrrlicht::device = createDevice(driverType, core::dimension2d<u32>(window.size.x, window.size.y),
        16, window.fullscreen, false, window.vsync, inputManager);

    RenderIrrlicht::videoDriver = device->getVideoDriver();
    RenderIrrlicht::sceneManager = device->getSceneManager();

}

void RenderIrrlicht::updateWindow() {

}

void RenderIrrlicht::closeWindow() {

}

void RenderIrrlicht::renderObject(std::string id) {

}

void RenderIrrlicht::renderDraw() {
 
}