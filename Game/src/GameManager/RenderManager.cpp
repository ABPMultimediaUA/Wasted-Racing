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

void RenderManager::update(float dTime) {
    RenderManager::renderFacade->updateWindow();
    updateAIDebug();
    updateCameraDebug();
    updateBattleDebug(dTime);
}

void RenderManager::draw() {
    renderFacade->renderDraw();
}

void RenderManager::close(){
    //Clear render component list
    renderComponentList.clear();

    //Clear all interface elements

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
    if(activeDebugCamera == false && activeDebugBehaviour == false)
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
            if(activeDebugAI == false)
            {
                activeDebugAI = true;
            }
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
            //Next point marker
            createRenderNPC();

            //Lines to objects
            deleteLinesObjects();
            createLinesObjects();

            if(lap == false)
            {
                lap = true;
            }
        }
        else
        {
            if(activeDebugAI == true)
            {
                activeDebugAI = false;
            }
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

            //Delete Lines to objects
            deleteLinesObjects();

            //Set debug inactive
            AIDebug = 0;
            lap = false;
        }
    }
}

void RenderManager::updateAIDebug()
{
    
    //float pi = 3.14159265358979323846;

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
        auto rot = AIDebugPoint[AIDebug].getTransformData().rotation;

        //Update next point marker
        point = AIDebugPoint[AIDebug].getComponent<PathPlanningComponent>()->getNextPoint();

        if(moveNextPos <= maxMoveNextPos/1.5)
        {
            point.y = point.y + moveNextPos;
            moveNextPos = maxMoveNextPos;
        }
        else if(moveNextPos > maxMoveNextPos/1.5)
        {
            point.y = point.y + moveNextPos;
            moveNextPos = moveNextPos - 0.1; 
        } 

        transform.position = point;
        transform.rotation = glm::vec3(0, 0, 3.14);
        transform.scale    = glm::vec3(1, 1, 1);

        marker->setTransformData(transform);
        RenderManager::getInstance().getRenderFacade()->updateObjectTransform(marker->getId(), transform);

        //Update Collision Cylinder
        point = AIDebugPoint[AIDebug].getTransformData().position;

        transform.position = point;
        transform.rotation = glm::vec3(rot.x, 0, rot.z);
        transform.scale    = glm::vec3(1, 1, 1);

        collisionCylinder->setTransformData(transform);
        RenderManager::getInstance().getRenderFacade()->updateObjectTransform(collisionCylinder->getId(), transform);

        if(AIDebug != 0)
        {

            auto radV = AIDebugPoint[AIDebug].getComponent<VSensorComponent>()->getMaxDistance();
            auto angV = AIDebugPoint[AIDebug].getComponent<VSensorComponent>()->getAngleVision();
            auto lengthV = AIDebugPoint[AIDebug].getComponent<VSensorComponent>()->getMaxLength();
            auto seenObjects = AIDebugPoint[AIDebug].getComponent<VSensorComponent>()->getSeenObjects();

            //Update Visibility Area
            point = AIDebugPoint[AIDebug].getTransformData().position;

            transform.position = point;
            transform.rotation = glm::vec3(rot.x, rot.y, rot.z);
            transform.scale    = glm::vec3(1, 1, 1);

            visibilityArea->setTransformData(transform);
            RenderManager::getInstance().getRenderFacade()->updateObjectTransform(visibilityArea->getId(), transform);

            //Update Vision Cone
            point = AIDebugPoint[AIDebug].getTransformData().position;


            transform.position = glm::vec3(point.x+cos(rot.y),
                                    point.y, point.z-sin(rot.y));
            transform.rotation = glm::vec3(rot.x, rot.y, rot.z);
            transform.scale    = glm::vec3(radV*cos(angV), lengthV, 2*radV*sin(angV));

            visionTriangle->setTransformData(transform);
            RenderManager::getInstance().getRenderFacade()->updateObjectTransform(visionTriangle->getId(), transform);


            //Lines to objects
            deleteLinesObjects();
            createLinesObjects();

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
    auto rot = AIDebugPoint[AIDebug].getTransformData().rotation;
    //float pi = 3.14159265358979323846;

    if(lap == false)
    {
        //Create marker
        id = 30000;
        point = AIDebugPoint[AIDebug].getComponent<PathPlanningComponent>()->getNextPoint();

        transform.position = point;
        transform.position.y = transform.position.y + 20;
        transform.rotation = glm::vec3(0, 0, 3.14);
        transform.scale    = glm::vec3(1, 1, 1);
        marker = ObjectManager::getInstance().createObject(id, transform);

        RenderManager::getInstance().createObjectRenderComponent(*marker.get(), ObjectRenderComponent::Shape::Arrow, "blackWithTransparency.png");

        //Create Collision Cylinder
        id = 30001;
        point = AIDebugPoint[AIDebug].getTransformData().position;

        rad = AIDebugPoint[AIDebug].getComponent<CollisionComponent>()->getRadius();
        length = AIDebugPoint[AIDebug].getComponent<CollisionComponent>()->getLength();

        transform.position = point;
        transform.rotation = glm::vec3(rot.x, 0, rot.z);
        transform.scale    = glm::vec3(1, 1, 1);
        collisionCylinder = ObjectManager::getInstance().createObject(id, transform);


        RenderManager::getInstance().createObjectRenderComponent(*collisionCylinder.get(), ObjectRenderComponent::Shape::Cylinder, "whiteWithTransparency.png", rad, length, 10.f, true);
    }
    else if(lap == true && AIDebug == 1)
    {

        auto angV = AIDebugPoint[AIDebug].getComponent<VSensorComponent>()->getAngleVision();
        rad = AIDebugPoint[AIDebug].getComponent<VSensorComponent>()->getMaxDistance();
        length = AIDebugPoint[AIDebug].getComponent<VSensorComponent>()->getMaxLength();

        auto seenObjects = AIDebugPoint[AIDebug].getComponent<VSensorComponent>()->getSeenObjects();

        //Create Visibility Area
        id = 30002;
        point = AIDebugPoint[AIDebug].getTransformData().position;

        transform.position = point;
        transform.rotation = glm::vec3(rot.x, rot.y, rot.z);
        transform.scale    = glm::vec3(1, 1, 1);
        visibilityArea = ObjectManager::getInstance().createObject(id, transform);

        rad = AIDebugPoint[AIDebug].getComponent<VSensorComponent>()->getMaxDistance();

        RenderManager::getInstance().createObjectRenderComponent(*visibilityArea.get(), ObjectRenderComponent::Shape::Cylinder, "redWithTransparency.png", rad, length, 10.f, true);
        
        //Create Vision Triangle
        id = 30003;
        point = AIDebugPoint[AIDebug].getTransformData().position;

        transform.position = glm::vec3(point.x+cos(rot.y),
                                point.y, point.z-sin(rot.y));
        transform.rotation = glm::vec3(rot.x, rot.y, rot.z);
        transform.scale    = glm::vec3(2*rad*sin(angV), length, 2*rad*cos(angV));
        visionTriangle = ObjectManager::getInstance().createObject(id, transform);

        RenderManager::getInstance().createObjectRenderComponent(*visionTriangle.get(), ObjectRenderComponent::Shape::Portion, "blackWithTransparency.png");
        
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

void RenderManager::createLinesObjects()
{

    if(lap == true)
    {
        uint16_t id;
        glm::vec3 point;
        GameObject::TransformationData transform;
        auto seenObjects = AIDebugPoint[AIDebug].getComponent<VSensorComponent>()->getSeenObjects();
        auto seenObjects2 = AIDebugPoint[AIDebug].getComponent<MSensorComponent>()->getMapCollisions();
        seenObjects.insert(seenObjects.end(),seenObjects2.begin(),seenObjects2.end());      //Adding the 2 collision maps

        //float pi = 3.14159265358979323846;

        //Create Lines Objects
        for(unsigned int i = 0; i < seenObjects.size(); i++)
        {
            id = 30100+i;

            auto position = seenObjects[i]->getPosition();

            if(moveSeen <= maxMoveSeen/2)
            {
                position.y = position.y + seenObjects[i]->getLength() + moveSeen;
                moveSeen = maxMoveSeen;
            }
            else if(moveSeen > maxMoveSeen/2)
            {
                position.y = position.y + seenObjects[i]->getLength() + moveSeen;
                moveSeen = moveSeen - 0.1; 
            } 

            transform.position = position;            
            transform.rotation = glm::vec3(0, 0, 180);
            transform.scale    = glm::vec3(0.5, 0.5, 0.5);
            auto obj = ObjectManager::getInstance().createObject(id, transform);

            RenderManager::getInstance().createObjectRenderComponent(*obj.get(), ObjectRenderComponent::Shape::Arrow, "redWithTransparency.png");

            linesObjects.push_back(obj);  
        }
    }
}

void RenderManager::deleteLinesObjects()
{
    if(lap == true)
    {
        EventData data;

        //Delete Lines Objects
        for(unsigned int i = 0; i < linesObjects.size(); i++)
        {
            data.Id = linesObjects[i]->getId();

            EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});
        }
        linesObjects.clear();
    }
}


//==============================================
// CAMERA DEBUG
//============================================== 

void RenderManager::renderCameraDebug()
{

    if(activeDebugAI == false && activeDebugBehaviour == false)
    {
        AIDebugPointC.clear();
        auto npcs = ItemManager::getInstance().getItemHolderComponents();
        for(unsigned int i = 0; i < npcs.size(); ++i)
        {
            auto player = npcs[i]->getGameObject();
            AIDebugPointC.push_back(player); 
        }
        
        if(AIDebugC < AIDebugPointC.size()-1)
        {   
            if(activeDebugCamera == false)
            {
                activeDebugCamera = true;
            }
            if(lapC == true)
            {
                AIDebugC++;
            }

            createCameraRender();

            if(lapC == false)
            {
                lapC = true;
            }
        }
        else
        {
            if(activeDebugCamera == true)
            {
                activeDebugCamera = false;
            }
            deleteCameraRender();

            AIDebugPointC.clear();
            
            //Set debug inactive
            AIDebugC = 0;
            lapC = false;
        }
    }

}

void RenderManager::updateCameraDebug()
{
    if(lapC == true)
    {
        auto camera = AIDebugPointC[AIDebugC].getComponent<CameraRenderComponent>();
        if(camera != nullptr)
        {
            float dist = camera->getDistance();
            float maxDist = camera->getMaxDistance();
            std::string text = "";
            text += std::to_string(dist);
            renderFacade->setDescriptionText(text);
            if(maxDist - dist == 0)
            {
                renderFacade->setSubDescriptionText("FALSE");
                if(collisionC == true)
                {
                    renderFacade->deleteRectangleCol2D();
                    collisionC = false;
                }
            }
            else
            {
                renderFacade->setSubDescriptionText("TRUE");
                if(collisionC == false)
                {
                    renderFacade->createRectangleCol2D(glm::vec2(960,0), "media/img/red_rectangle.png");
                    collisionC = true;
                }
            }

        }
    } 
}

void RenderManager::createCameraRender()
{
    if(lapC == false)
    {
        renderFacade->createRectangle2D(glm::vec2(960,0), "media/img/white_rectangle.png");

        renderFacade->createTitleText(glm::vec2(1060,50),"DISTANCIA JUGADOR-CAMARA");

        renderFacade->createDescriptionText(glm::vec2(1100,70), "");

        renderFacade->createSubTitleText(glm::vec2(1080,110),"COLISION CAMARA");

        renderFacade->createSubDescriptionText(glm::vec2(1105,130), "");
    }

    //Create camera render
    auto obj = ObjectManager::getInstance().getObject(AIDebugPointC[AIDebugC].getId());
    RenderManager::getInstance().createCameraRenderComponent(*obj.get());
}

void RenderManager::deleteCameraRender()
{
    if(lapC == true)
    {
        renderFacade->deleteRectangle2D();
        if(collisionC == true)
        {
            renderFacade->deleteRectangleCol2D();
            collisionC = false;
        }
        renderFacade->deleteTitleText();
        renderFacade->deleteDescriptionText();
        renderFacade->deleteSubTitleText();
        renderFacade->deleteSubDescriptionText();

        //Create camera render
        auto obj = ObjectManager::getInstance().getObject(AIDebugPointC[0].getId());
        RenderManager::getInstance().createCameraRenderComponent(*obj.get());
    }
}


//==============================================
// BEHAVIOUR DEBUG
//============================================== 

void RenderManager::renderBattleDebug()
{

    if(activeDebugAI == false && activeDebugCamera == false)
    {
        AIDebugPointB.clear();
        auto npcs = ItemManager::getInstance().getItemHolderComponents();
        for(unsigned int i = 0; i < npcs.size(); ++i)
        {
            auto player = npcs[i]->getGameObject();
            AIDebugPointB.push_back(player); 
        }
        
        if(AIDebugB < AIDebugPointB.size()-1)
        {   
            if(activeDebugBehaviour == false)
            {
                activeDebugBehaviour = true;
            }
            if(lapB == true)
            {
                AIDebugB++;
            }

            createBattleRender();

            if(lapB == false)
            {
                lapB = true;
            }
        }
        else
        {
            if(activeDebugBehaviour == true)
            {
                activeDebugBehaviour = false;
            }
            deleteBattleRender();

            AIDebugPointB.clear();
            
            //Set debug inactive
            AIDebugB = 0;
            lapB = false;
        }
    }

}

void RenderManager::createBattleRender()
{
    if(lapB == false)
    {
        renderFacade->createRectangle2D(glm::vec2(960,0), "media/img/white_rectangle.png");
        renderFacade->createTitleText(glm::vec2(1100,50),title);
        renderFacade->createDescriptionText(glm::vec2(1000,80),root);
        renderFacade->setDescriptionText("NO BEHAVIOUR, YOU ARE THE PLAYER, PUSH F10 AGAIN");
    }
    else if(lapB == true && AIDebugB != 0)
    {
        root = " ";
        renderFacade->setDescriptionText(root);
        if(timeP != maxTimeP)
        {
            renderFacade->createRectangleCol2D(glm::vec2(960,0), "media/img/green_rectangle.png");
            success = true;
            end = false;
        }
    }
    //Create camera render
    auto obj = ObjectManager::getInstance().getObject(AIDebugPointB[AIDebugB].getId());
    RenderManager::getInstance().createCameraRenderComponent(*obj.get());
}
 
void RenderManager::deleteBattleRender() 
{
    if(lapB == true)
    {
        renderFacade->deleteRectangle2D();
        renderFacade->deleteTitleText();
        renderFacade->deleteDescriptionText();
        if(timeP != maxTimeP)
        {
            renderFacade->deleteRectangleCol2D();
        }

        //Create camera render
        auto obj = ObjectManager::getInstance().getObject(AIDebugPointB[0].getId());
        RenderManager::getInstance().createCameraRenderComponent(*obj.get());
    }
}

void RenderManager::updateBattleDebug(float dTime)
{
    
    if(lapB == true && AIDebugB != 0)
    {
        if(end == false)
        { 
            renderFacade->setDescriptionText(root);
            if(success == true)
            {
                if(timeP <= 0)
                {
                    root = " ";
                    renderFacade->setDescriptionText(root);
                    renderFacade->deleteRectangleCol2D();
                    success = false;
                    timeP = maxTimeP;
                }
                else
                {
                    timeP -= dTime;
                }
            }
        }
        else
        {
            renderFacade->createRectangleCol2D(glm::vec2(960,0), "media/img/green_rectangle.png");
            success = true;
            end = false;
        }
    }
} 

//==============================================
// VISUAL INTERFACE
//==============================================
int32_t RenderManager::createImage(glm::vec2 pos, std::string img)
{
    //Invoke creating function and return id
    return renderFacade->addImage(pos,img);
}

void RenderManager::deleteImage(int32_t id)
{
    //Invoke façade function
    renderFacade->deleteImage(id);
}

void RenderManager::deleteImages(std::vector<int32_t>* ids)
{
    //Invoke façade function for all the elements
    for(int i = 0; i < ids->size(); i++){
        renderFacade->deleteImage(ids->at(i));
    }
}

void RenderManager::cleanImages()
{
    //Invoke façade function
    renderFacade->cleanImages();
}

int32_t RenderManager::createRectangleColor(glm::vec2 pos, glm::vec2 size, int r, int g, int b, int a)
{
    return renderFacade->addRectangleColor(pos,size,r,g,b,a);
}

void RenderManager::changeRectangleColor(int32_t id, int r, int g, int b, int a)
{
    return renderFacade->changeRectangleColor(id,r,g,b,a);
}
    
void RenderManager::deleteRectangleColor(int32_t id)
{

}

void RenderManager::cleanRectangles()
{

}

int32_t RenderManager::createText(glm::vec2 pos, std::string text)
{
    //Invoke façade function
    return renderFacade->addText(pos,text);
}

int32_t RenderManager::createText(glm::vec2 pos, glm::vec2 size, std::string text)
{
    //Invoke façade function
    return renderFacade->addText(pos,size,text);
}

void RenderManager::changeText(int32_t id, std::string text)
{
    //Invoke façade function
    renderFacade->changeText(id, text);
}

void RenderManager::deleteText(int32_t id)
{
    //Invoke façade function
    renderFacade->deleteText(id);
}

void RenderManager::deleteTexts(std::vector<int32_t>* ids)
{
    //Invoke façade function for every member
    for(int i = 0; i < ids->size(); i++){
        renderFacade->deleteText(ids->at(i));
    }
}

void RenderManager::cleanTexts()
{
    //Invoke façade function
    renderFacade->cleanTexts();
}

void RenderManager::cleanVI()
{
    //Invoke façade function
    renderFacade->cleanInterface();
}