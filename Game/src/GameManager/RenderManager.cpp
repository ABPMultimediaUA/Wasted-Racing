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

IComponent::Pointer RenderManager::createObjectRenderComponent(GameObject& newGameObject, ObjectRenderComponent::Shape newShape, const char* newStr, float radius, float length, int tesselation, bool transparency) {

    IComponent::Pointer component = std::make_shared<ObjectRenderComponent>(newGameObject, newShape, newStr);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    auto comp = newGameObject.getComponent<ObjectRenderComponent>();

    renderFacade->addObject(component.get(), radius, length, tesselation, transparency);

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
        auto player = npcs[i]->getGameObject();
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
    
    float pi = 3.14159265358979323846;

    if(AIDebug < AIDebugPoint.size())
    {
        AIDebugPoint.clear();
        auto npcs = ItemManager::getInstance().getItemHolderComponents();
        for(unsigned int i = 0; i < npcs.size(); ++i)
        {
            auto player = npcs[i]->getGameObject();
            AIDebugPoint.push_back(player); 
        }



        glm::vec3 point;
        GameObject::TransformationData transform;

        //Update next point marker
        point = AIDebugPoint[AIDebug].getComponent<PathPlanningComponent>()->getNextPoint();

        transform.position = point;
        transform.position.y = transform.position.y + 20;
        transform.rotation = glm::vec3(0, 0, 3.14);
        transform.scale    = glm::vec3(1, 1, 1);

        marker->setTransformData(transform);
        RenderManager::getInstance().getRenderFacade()->updateObjectTransform(marker->getId(), transform);

        //Update Collision Cylinder
        point = AIDebugPoint[AIDebug].getTransformData().position;

        transform.position = point;
        transform.rotation = glm::vec3(0, 0, 0);
        transform.scale    = glm::vec3(1, 1, 1);

        collisionCylinder->setTransformData(transform);
        RenderManager::getInstance().getRenderFacade()->updateObjectTransform(collisionCylinder->getId(), transform);

        if(AIDebug != 0)
        {
            //Update Visibility Area
            point = AIDebugPoint[AIDebug].getTransformData().position;

            transform.position = point;
            transform.rotation = glm::vec3(0, 0, 0);
            transform.scale    = glm::vec3(1, 1, 1);

            visibilityArea->setTransformData(transform);
            RenderManager::getInstance().getRenderFacade()->updateObjectTransform(visibilityArea->getId(), transform);

            //Update Vision Cone
            point = AIDebugPoint[AIDebug].getTransformData().position;

            auto rot = AIDebugPoint[AIDebug].getTransformData().rotation;
            auto rad = AIDebugPoint[AIDebug].getComponent<VSensorComponent>()->getMaxDistance();
            auto ang = AIDebugPoint[AIDebug].getComponent<VSensorComponent>()->getAngleVision();
            auto length = AIDebugPoint[AIDebug].getComponent<VSensorComponent>()->getMaxLength();

            transform.position = glm::vec3(point.x+cos(rot.y),
                                    point.y, point.z-sin(rot.y));
            transform.rotation = glm::vec3(rot.x, rot.y+pi/2, rot.z);
            transform.scale    = glm::vec3(2*rad*sin(ang), length, rad*cos(ang));

            visionTriangle->setTransformData(transform);
            RenderManager::getInstance().getRenderFacade()->updateObjectTransform(visionTriangle->getId(), transform);
        }
    }
}

void RenderManager::createRenderNPC()
{

    uint16_t id;
    glm::vec3 point;
    GameObject::TransformationData transform;
    float rad;
    float length;
    float pi = 3.14159265358979323846;

    if(lap == false)
    {
        //Create marker
        id = 5000;
        point = AIDebugPoint[AIDebug].getComponent<PathPlanningComponent>()->getNextPoint();

        transform.position = point;
        transform.position.y = transform.position.y + 20;
        transform.rotation = glm::vec3(0, 0, 3.14);
        transform.scale    = glm::vec3(1, 1, 1);
        marker = ObjectManager::getInstance().createObject(id, transform);

        RenderManager::getInstance().createObjectRenderComponent(*marker.get(), ObjectRenderComponent::Shape::Arrow, "pool.jpg");

        //Create Collision Cylinder
        id = 5001;
        point = AIDebugPoint[AIDebug].getTransformData().position;

        transform.position = point;
        transform.rotation = glm::vec3(0, 0, 0);
        transform.scale    = glm::vec3(1, 1, 1);
        collisionCylinder = ObjectManager::getInstance().createObject(id, transform);

        rad = AIDebugPoint[AIDebug].getComponent<CollisionComponent>()->getRadius();
        length = AIDebugPoint[AIDebug].getComponent<CollisionComponent>()->getLength();

        RenderManager::getInstance().createObjectRenderComponent(*collisionCylinder.get(), ObjectRenderComponent::Shape::Cylinder, "semiTransparente.png", rad, length, 10.f, true);
    }
    else if(lap == true && AIDebug != 0)
    {
        //Create Visibility Area
        id = 5002;
        point = AIDebugPoint[AIDebug].getTransformData().position;

        transform.position = point;
        transform.rotation = glm::vec3(0, 0, 0);
        transform.scale    = glm::vec3(1, 1, 1);
        visibilityArea = ObjectManager::getInstance().createObject(id, transform);

        rad = AIDebugPoint[AIDebug].getComponent<VSensorComponent>()->getMaxDistance();

        RenderManager::getInstance().createObjectRenderComponent(*visibilityArea.get(), ObjectRenderComponent::Shape::Cylinder, "semiTransparent.jpg", rad, 1, 10.f, false);
        
        //Create Vision Cone
        id = 5003;
        point = AIDebugPoint[AIDebug].getTransformData().position;

        auto rot = AIDebugPoint[AIDebug].getTransformData().rotation;

        rad = AIDebugPoint[AIDebug].getComponent<VSensorComponent>()->getMaxDistance();
        length = AIDebugPoint[AIDebug].getComponent<VSensorComponent>()->getMaxLength();

        auto ang = AIDebugPoint[AIDebug].getComponent<VSensorComponent>()->getAngleVision();

        transform.position = glm::vec3(point.x+cos(rot.y),
                                point.y, point.z-sin(rot.y));
        transform.rotation = glm::vec3(rot.x, rot.y+pi/2, rot.z);
        transform.scale    = glm::vec3(2*rad*sin(ang), length, rad*cos(ang));
        visionTriangle = ObjectManager::getInstance().createObject(id, transform);

        RenderManager::getInstance().createObjectRenderComponent(*visionTriangle.get(), ObjectRenderComponent::Shape::Mesh, "triangle.obj");
    }

    //Create camera render
    auto obj = ObjectManager::getInstance().getObject(AIDebugPoint[AIDebug].getId());
    RenderManager::getInstance().createCameraRenderComponent(*obj.get());
}

void RenderManager::deleteRenderNPC()
{
    if(lap == true)
    {
        EventData data;

        //Delete marker
        data.Id = marker->getId();

        EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});

        //Delete Collision Cylinder
        data.Id = collisionCylinder->getId();

        EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});

        //Delete Collision Cylinder
        data.Id = visibilityArea->getId();

        EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});

        //Delete Vision Cone
        data.Id = visionTriangle->getId();

        EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});

        //Change camera to initial player
        auto obj = ObjectManager::getInstance().getObject(AIDebugPoint[0].getId());
        RenderManager::getInstance().createCameraRenderComponent(*obj.get());

        AIDebugPoint.clear();
    }
}