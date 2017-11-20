#include "RenderManager.h"
#include "../GameFacade/RenderIrrlicht.h"

RenderManager& RenderManager::getInstance() {
    static RenderManager instance;
    return instance;
}

void RenderManager::init(int engine) {
    
    if(engine == 0){
        renderFacade = new RenderIrrlicht();
    }
    else {

    }

    renderFacade->init(1280, 720, false, false);
    renderFacade->openWindow();

}

void RenderManager::update() {
    RenderManager::renderFacade->updateWindow();
}

void RenderManager::close(){
    
}