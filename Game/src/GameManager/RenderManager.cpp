#include "RenderManager.h"
#include "../GameFacade/RenderIrrlicht.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================
void addComponent(EventData data);

//==============================================
// RENDER MANAGER FUNCTIONS
//============================================== 

RenderManager& RenderManager::getInstance() {
    static RenderManager instance;
    return instance;
}

void RenderManager::init(int engine) {
    
    //Render Engine Initlialization
    if(engine == 0){
        renderFacade = new RenderIrrlicht();
    }
    else {

    }

    renderFacade->init(1280, 720, false, false);
    renderFacade->openWindow();

    //QuadTree data init
    maxObjPerNode = 2;
    updateRange = 2;
    x0 = 0; x1 = 10; y0 = 0;  y1 = 10; //Map dimensions

    //Bind listeners
    EventManager::getInstance().addListener(EventListener {EventType::RenderComponent_Create, addComponent});
}

void RenderManager::update() {
    RenderManager::renderFacade->updateWindow();
}

void RenderManager::close(){
    
}

void RenderManager::splitQuadTree(){
    renderComponentTree.init(maxObjPerNode, updateRange, renderComponentList, x0, x1, y0, y1);
    renderComponentTree.divide();
}

//==============================================
// DELEGATES
//============================================== 
void addComponent(EventData data) {
    RenderManager::getInstance().getComponentList().push_back(data.Component);
}