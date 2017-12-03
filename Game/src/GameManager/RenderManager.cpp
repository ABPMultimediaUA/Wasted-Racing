#include "RenderManager.h"
#include "../GameFacade/RenderIrrlicht.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================
void addObjectRenderComponent(EventData data);

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
    EventManager::getInstance().addListener(EventListener {EventType::ObjectRenderComponent_Create, addObjectRenderComponent});
}

void RenderManager::update() {
    RenderManager::renderFacade->updateWindow();
}

void RenderManager::draw() {
    renderFacade->renderDraw();
}

void RenderManager::close(){
    
}

void RenderManager::splitQuadTree(){
    renderComponentTree.init(maxObjPerNode, updateRange, renderComponentList, x0, x1, y0, y1);
    renderComponentTree.divide();
}

IComponent::Pointer RenderManager::createObjectRenderComponent(GameObject& newGameObject, ObjectRenderComponent::Shape newShape) {

    IComponent::Pointer component = std::make_shared<ObjectRenderComponent>(newGameObject, newShape);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    EventManager::getInstance().addEvent(Event {EventType::ObjectRenderComponent_Create, data});

    return component;
}


//==============================================
// DELEGATES
//============================================== 
void addObjectRenderComponent(EventData data) {
    RenderManager::getInstance().getComponentList().push_back(data.Component);
    data.Component.get()->init();
}