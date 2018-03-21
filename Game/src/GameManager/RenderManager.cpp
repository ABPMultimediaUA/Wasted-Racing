#include "RenderManager.h"

//////////////////////////////////////////////
//            THINGS TO DO HERE
//////////////////////////////////////////////
//////////////////////////////////////////////
/*
>Transmute everything to debug manager (of the debug part)
*/
//////////////////////////////////////////////
//////////////////////////////////////////////

//==============================================
// DELEGATES DECLARATIONS
//==============================================
void addObjectRenderComponent(EventData data);
void addLightRenderComponent(EventData data);
void addCameraRenderComponent(EventData data); 
void objectDeletedRender(EventData eData);
void updateTransformPosition(EventData eData);
void updateTransformRotation(EventData eData);
void updateTransformScale(EventData eData);




//==============================================
// MAIN FUNCTIONS
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
        renderFacade = new RenderRedPanda();
    }

    //Render init
    renderFacade->init(1280, 720, false, false);
    renderFacade->openWindow();
    initHUD();

    //Data init
    debugState = false;

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
    EventManager::getInstance().addListener(EventListener {EventType::Update_Transform_Position, updateTransformPosition});
    EventManager::getInstance().addListener(EventListener {EventType::Update_Transform_Rotation, updateTransformRotation});
    EventManager::getInstance().addListener(EventListener {EventType::Update_Transform_Scale, updateTransformScale});

    //Create Skybox
    uint16_t id;
    GameObject::TransformationData transform;
    id = 40000;

    transform.position = glm::vec3(0,0,0);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(1,1,1);
    GameObject::Pointer sky = ObjectManager::getInstance().createObject(id, transform);


    RenderManager::getInstance().createSkyBox(*sky.get(), ObjectRenderComponent::Shape::Skybox, "darkskies_up.tga", "darkskies_dn.tga", "darkskies_lf.tga", "darkskies_rt.tga", "darkskies_ft.tga", "darkskies_bk.tga");
}

void RenderManager::update(float dTime) {
    //Update HUD
    updateHUD();

    //Update camera collision
    renderFacade->getCameraTarget().getComponent<CameraRenderComponent>().get()->update(dTime);

    //Update debug if debug mode activated
    if(debugState){
        updateAIDebug();
        updateCameraDebug();
        updateBattleDebug(dTime);
    }else{
        renderFacade->updateItemIcon();
    }

}

void RenderManager::draw() {
    renderFacade->renderDraw();
}

void RenderManager::close(){
    //Clear all interface elements
    renderFacade->cleanInterface();

    //Clear render component list
    renderComponentList.clear();
}

void RenderManager::splitQuadTree(){
    //renderComponentTree.init(maxObjPerNode, updateRange, renderComponentList, x0, x1, y0, y1);
    //renderComponentTree.divide();
}

//==============================================
// CREATOR FUNCTIONS
//============================================== 

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

    //Set camera target to this camera, since it was created
    renderFacade->setCameraTarget(newGameObject);

    return component;
}

IComponent::Pointer RenderManager::createObjectRenderComponent(GameObject& newGameObject, ObjectRenderComponent::Shape newShape, const char* newStr, float radius, float length, int tesselation, bool transparency) {

    //Instantiate the component as shared pointer
    IComponent::Pointer component = std::make_shared<ObjectRenderComponent>(newGameObject, newShape, newStr);

    //Add component to object
    newGameObject.addComponent(component);

    //Create event data
    EventData data;
    data.Component = component;

    //Create event
    EventManager::getInstance().addEvent(Event {EventType::ObjectRenderComponent_Create, data});

    //add object to the render
    renderFacade->addObject(component.get(), radius, length, tesselation, transparency);

    return component;
}

IComponent::Pointer RenderManager::createSkyBox(GameObject& newGameObject, ObjectRenderComponent::Shape newShape, std::string top, std::string bot, std::string left, std::string right, std::string front, std::string back) {

    IComponent::Pointer component = std::make_shared<ObjectRenderComponent>(newGameObject, newShape);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    auto comp = newGameObject.getComponent<ObjectRenderComponent>();

    renderFacade->addSkybox(component.get(), top, bot, left, right, front, back);

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

void updateTransformPosition(EventData eData) {
    GameObject::TransformationData t = ObjectManager::getInstance().getObject(eData.Id).get()->getTransformData();
    t.position = eData.Vector;
    RenderManager::getInstance().getRenderFacade()->updateObjectTransform(eData.Id, t);
}

void updateTransformRotation(EventData eData) {
    GameObject::TransformationData t = ObjectManager::getInstance().getObject(eData.Id).get()->getTransformData();
    t.rotation = eData.Vector;
    RenderManager::getInstance().getRenderFacade()->updateObjectTransform(eData.Id, t);
}

void updateTransformScale(EventData eData) {
    GameObject::TransformationData t = ObjectManager::getInstance().getObject(eData.Id).get()->getTransformData();
    t.scale = eData.Vector;
    RenderManager::getInstance().getRenderFacade()->updateObjectTransform(eData.Id, t);
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
        seenObjects.insert(seenObjects.end(), seenObjects2.begin(), seenObjects2.end());
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
/////////////
//  HUD
/////////////

void RenderManager::initHUD()
{
    //Create all values
    lapHUD_ID = createText("LAP: ", glm::vec2(0, 300),255,255,255,255, glm::vec2(500,25));
    positionHUD_ID = createText("ITEM: ", glm::vec2(0, 325),255,255,255,255, glm::vec2(500,25));
    itemHUD_ID = createText("POSITION: ", glm::vec2(0, 350),255,255,255,255, glm::vec2(500,25));
}

void RenderManager::updateHUD()
{
    //If there is score
    auto score = ObjectManager::getInstance().getObject(renderFacade->getCameraTarget().getId()).get()->getComponent<ScoreComponent>();
    if(score)
    {
        //Get data
        int pos = ObjectManager::getInstance().getObject(renderFacade->getCameraTarget().getId()).get()->getComponent<ScoreComponent>().get()->getPosition();
        int lap = ObjectManager::getInstance().getObject(renderFacade->getCameraTarget().getId()).get()->getComponent<ScoreComponent>().get()->getLap();
        int maxLaps = ScoreManager::getInstance().getMaxLaps();
        int item = ObjectManager::getInstance().getObject(renderFacade->getCameraTarget().getId()).get()->getComponent<ItemHolderComponent>().get()->getItemType();

        //Change position text
        std::string number = std::to_string(pos);
        std::string s("POSITION: "+number);
        changeText(positionHUD_ID, s);

        //Change lap text
        std::string number2 = std::to_string(lap);
        std::string number3 = std::to_string(maxLaps);
        s = std::string("LAP: "+number2+"/"+number3);
        changeText(lapHUD_ID, s);

        //Change item text
        s = std::string("ITEM: ");
        switch(item)
        {
            case -1: s+="EMPTY";
                    break;
            case 0: s+="RED SHELL";
                    break;
            case 1: s+="BLUE SHELL";
                    break;
            case 2: s+="BANANA";
                    break;
            case 3: s+="MUSHROOM";
                    break;
            case 4: s+="STAR";
                    break;
        }
        changeText(itemHUD_ID, s);
    }
    else
    {
        changeText(lapHUD_ID, "LAP:  ");
        changeText(positionHUD_ID, "POSITION:  ");
        changeText(itemHUD_ID, "ITEM:  ");
    }
}

/////////////
//  IMAGE
/////////////

int32_t RenderManager::createImage(std::string img, glm::vec2 pos)
{
    //Invoke creating function and return id
    return renderFacade->addImage(img, pos);
}

void RenderManager::deleteImage(int32_t id)
{
    //Invoke façade function
    renderFacade->deleteImage(id);
}

void RenderManager::deleteImages(std::vector<int32_t>* ids)
{
    //Invoke façade function for all the elements
    for(unsigned int i = 0; i < ids->size(); i++){
        renderFacade->deleteImage(ids->at(i));
    }
}

void RenderManager::cleanImages()
{
    //Invoke façade function
    renderFacade->cleanImages();
}

/////////////
//  RECTANGLE
/////////////

int32_t RenderManager::createRectangleColor(glm::vec2 pos, glm::vec2 size, int r, int g, int b, int a)
{
    return renderFacade->addRectangleColor(pos,size,r,g,b,a);
}

void RenderManager::changeRectangleColor(int32_t id, int r, int g, int b, int a)
{
    return renderFacade->changeRectangleColor(id, r, g, b, a);
}
    
void RenderManager::deleteRectangleColor(int32_t id)
{
    renderFacade->deleteRectangleColor(id);
}

void RenderManager::cleanRectangles()
{
    renderFacade->cleanRectangles();
}

/////////////
//  TEXT
/////////////
int32_t RenderManager::createText( std::string text, glm::vec2 pos, int r, int g, int b, int a, glm::vec2 size , std::string fontFile)
{
    //Invoke façade function
    return renderFacade->addText(text, pos,  r, g, b , a, size, fontFile);
}

void RenderManager::changeText(int32_t id, std::string text)
{
    //Invoke façade function
    renderFacade->changeText(id, text);
}

void RenderManager::changeFontText(int32_t id, std::string fontFile)
{
    renderFacade->changeFontText(id, fontFile);
}

void RenderManager::changeColorText(int32_t id, int r, int g, int b, int a)
{
    renderFacade->changeColorText(id, r, g, b, a);
}

void RenderManager::changeBackgroundColorText(int32_t id, int r, int g, int b, int a)
{
    renderFacade->changeBackgroundColorText(id, r, g, b, a);
}

void RenderManager::deleteText(int32_t id)
{
    renderFacade->deleteText(id);
}

void RenderManager::deleteTexts(std::vector<int32_t>* ids)
{
    //Invoke façade function for every member
    for(unsigned int i = 0; i < ids->size(); i++){
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
