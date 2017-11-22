#include "RenderIrrlicht.h"


void RenderIrrlicht::openWindow(){

    irr::video::E_DRIVER_TYPE driverType = irr::video::E_DRIVER_TYPE::EDT_OPENGL;

    device = irr::createDevice(driverType, irr::core::dimension2d<irr::u32>(window.size.x, window.size.y),
        16, window.fullscreen, false, window.vsync);

    videoDriver = device->getVideoDriver();
    sceneManager = device->getSceneManager();

}

void RenderIrrlicht::updateWindow() {
    device->run();
}

void RenderIrrlicht::closeWindow() {

}

void RenderIrrlicht::renderObject(int id) {

}

void RenderIrrlicht::renderDraw() {
 
}