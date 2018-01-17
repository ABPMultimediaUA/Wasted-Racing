#include "RenderManager.h"
#include "../GameFacade/RenderIrrlicht.h"
#include "../GameObject/RenderComponent/ObjectRenderComponent.h"
#include "../GameObject/RenderComponent/CameraRenderComponent.h"
#include "WaypointManager.h"
#include "AIManager.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================
void addObjectRenderComponent(EventData data);
void addLightRenderComponent(EventData data);
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
    EventManager::getInstance().addListener(EventListener {EventType::LightRenderComponent_Create, addLightRenderComponent});
    EventManager::getInstance().addListener(EventListener {EventType::CameraRenderComponent_Create, addCameraRenderComponent});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeletedRender});
    EventManager::getInstance().addListener(EventListener {EventType::ObjectRenderComponent_Delete, objectDeletedRender});
}

void RenderManager::update() {
    RenderManager::renderFacade->updateWindow();
    updateAIDebug();
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

IComponent::Pointer RenderManager::createObjectRenderComponent(GameObject& newGameObject, ObjectRenderComponent::Shape newShape, const char* newStr) {

    IComponent::Pointer component = std::make_shared<ObjectRenderComponent>(newGameObject, newShape, newStr);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    EventManager::getInstance().addEvent(Event {EventType::ObjectRenderComponent_Create, data});

    return component;
}

IComponent::Pointer RenderManager::createLightRenderComponent(GameObject& newGameObject, LightRenderComponent::Type newType, float newRadius) {

    IComponent::Pointer component = std::make_shared<LightRenderComponent>(newGameObject, newType, newRadius);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    EventManager::getInstance().addEvent(Event {EventType::LightRenderComponent_Create, data});

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

void addLightRenderComponent(EventData data) {
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
        if(eData.Id == renderComponentList[i].get()->getGameObject().getId()) {
            renderComponentList.erase(renderComponentList.begin() + i);
            
            return;
        }
    }
}


//==============================================
// AI DEBUG
//============================================== 

void RenderManager::renderAIDebug()
{
    auto listObj = WaypointManager::getInstance().getWaypoints();

    AIDebugPoint.clear();
    auto npcs = ItemManager::getInstance().getItemHolderComponents();
    for(unsigned int i = 0; i < npcs.size(); ++i)
    {
        auto player = npcs[i].get()->getGameObject();
        AIDebugPoint.push_back(player); 
    }
    
    if(AIDebug < AIDebugPoint.size()-1)
    {   
        if(lap == true)
        {
            AIDebug++;
        }
        //Waypoint marker
        if(lap == false)
        {
            for(unsigned int i = 0; i < listObj.size(); ++i) 
            {
                RenderManager::getInstance().createObjectRenderComponent(*listObj[i].get(), ObjectRenderComponent::Shape::Sphere, "pool.jpg");
            }
        }
        //deleteRenderNPC();

        //Next point marker
        createRenderNPC();

        if(lap == false)
        {
            lap = true;
        }
    }
    else
    {
        //Delete Waypoint marker
        for(unsigned int i = 0; i < listObj.size(); ++i) 
        {
            EventData data;
            data.Id = listObj[i]->getId();

            EventManager::getInstance().addEvent(Event {EventType::ObjectRenderComponent_Delete, data});

            auto component = listObj[i]->getComponent<ObjectRenderComponent>().get();
            renderFacade->deleteObject(component);
        }

        //Delete next point marker
        deleteRenderNPC();

        //Set debug inactive
        AIDebug = 0;
        lap = false;
    }
}

void RenderManager::updateAIDebug()
{
    if(AIDebug < AIDebugPoint.size())
    {
        //Update next point marker
        GameObject::TransformationData transform;

        auto point = AIDebugPoint[AIDebug].getComponent<PathPlanningComponent>()->getNextPoint();

        transform.position = point;
        transform.position.y = transform.position.y + 15;
        transform.rotation = glm::vec3(0, 0, 0);
        transform.scale    = glm::vec3(0.5, 1, 0.5);

        marker->setTransformData(transform);
        RenderManager::getInstance().getRenderFacade()->updateObjectTransform(marker->getId(), transform);
    }
}

void RenderManager::createRenderNPC()
{
    if(lap == false)
    {
        uint16_t id = 5000;
        GameObject::TransformationData transform;

        auto point = AIDebugPoint[AIDebug].getComponent<PathPlanningComponent>()->getNextPoint();

        transform.position = point;
        transform.position.y = transform.position.y + 15;
        transform.rotation = glm::vec3(0, 0, 0);
        transform.scale    = glm::vec3(0.5, 1, 0.5);
        auto obj = ObjectManager::getInstance().createObject(id, transform);
        marker = obj;

        RenderManager::getInstance().createObjectRenderComponent(*marker.get(), ObjectRenderComponent::Shape::Sphere, "pool.jpg");
    }
    //renderFacade->setCameraTarget(&AIDebugPoint[AIDebug]);
    RenderManager::getInstance().createCameraRenderComponent(AIDebugPoint[AIDebug]);
}

void RenderManager::deleteRenderNPC()
{
    if(lap == true)
    {
        EventData data;
        data.Id = marker->getId();

        EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});
        RenderManager::getInstance().createCameraRenderComponent(AIDebugPoint[0]);
    }
}