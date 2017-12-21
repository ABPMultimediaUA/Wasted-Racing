#include "RenderManager.h"
#include "../GameFacade/RenderIrrlicht.h"
#include "../GameObject/ObjectRenderComponent.h"
#include "../GameObject/CameraRenderComponent.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================
void addObjectRenderComponent(EventData data);
void addCameraRenderComponent(EventData data); 
void objectDeletedRender(EventData eData);

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
    //maxObjPerNode = 2;
    //updateRange = 2;
    //x0 = 0; x1 = 10; y0 = 0;  y1 = 10; //Map dimensions

    //Bind listeners
    EventManager::getInstance().addListener(EventListener {EventType::ObjectRenderComponent_Create, addObjectRenderComponent});
    EventManager::getInstance().addListener(EventListener {EventType::CameraRenderComponent_Create, addCameraRenderComponent});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeletedRender});
}

void RenderManager::update() {
    RenderManager::renderFacade->updateWindow();
}

void RenderManager::draw() {
    renderFacade->renderDraw();
}

void RenderManager::close(){
    renderComponentList.clear();
}

void RenderManager::splitQuadTree(){
    //renderComponentTree.init(maxObjPerNode, updateRange, renderComponentList, x0, x1, y0, y1);
    //renderComponentTree.divide();
}

IComponent::Pointer RenderManager::createObjectRenderComponent(GameObject& newGameObject, ObjectRenderComponent::Shape newShape) {

    IComponent::Pointer component = std::make_shared<ObjectRenderComponent>(newGameObject, newShape);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    EventManager::getInstance().addEvent(Event {EventType::ObjectRenderComponent_Create, data});

    return component;
}

IComponent::Pointer RenderManager::createCameraRenderComponent(GameObject& newGameObject) {

    IComponent::Pointer component = std::make_shared<CameraRenderComponent>(newGameObject);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    EventManager::getInstance().addEvent(Event {EventType::CameraRenderComponent_Create, data});

    return component;
}


//==============================================
// DELEGATES
//============================================== 
void addObjectRenderComponent(EventData data) {
    RenderManager::getInstance().getComponentList().push_back(data.Component);
    data.Component.get()->init();
}
void addCameraRenderComponent(EventData data) {
    RenderManager::getInstance().setCameraComponent(data.Component);
    data.Component.get()->init();
}

void objectDeletedRender(EventData eData) {

    auto& renderComponentList = RenderManager::getInstance().getComponentList();

    for(unsigned int i = 0; i<renderComponentList.size(); ++i) {
        if(eData.Id == renderComponentList.at(i).get()->getGameObject().getId()) {
            renderComponentList.erase(renderComponentList.begin() + i);
            
            return;
        }
    }
}