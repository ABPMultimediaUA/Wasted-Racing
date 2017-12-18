#include "Game.h"

#include <memory>
#include <iostream>

void addObjects();

//====================================================
//  GAME INITIALIZATION
//====================================================
void Game::init() {

    //Say game loop is active
    Game::stay = true;

    //Set engine to default
    Game::renderEngineSetter(0);
    Game::inputEngineSetter(0);

    //Initialize true audio manager
    audioManager = &AudioManager::getInstance();
    audioManager->init();

    //Initilize managers
    eventManager = &EventManager::getInstance();
    eventManager->init();

    //First we initialize renderManager, who creates a device and passes this reference to the inputManager
    renderManager = &RenderManager::getInstance();
    renderManager->init(Game::renderEngine);

    //Once we've initialized the renderManager, we can do the same with our inputManager
    inputManager = &InputManager::getInstance();
    inputManager->init(Game::inputEngine);

    //Initialize object manager
    objectManager = &ObjectManager::getInstance();
    objectManager->init();

    //Initialize physics manager
    physicsManager = &PhysicsManager::getInstance();
    physicsManager->init();

    //Initialize Waypoint Manager
    waypointManager = &WaypointManager::getInstance();
    waypointManager->init();

    //Initialize AI manager
    aiManager = &AIManager::getInstance();
    aiManager->init();

    //Initialize Sensor manager
    sensorManager = &SensorManager::getInstance();
    sensorManager->init();

    //Initialize Sensor manager
    itemManager = &ItemManager::getInstance();
    itemManager->init();

    addObjects();
}

//====================================================
//  GAME UPDATE
//====================================================
void Game::update() {

    //Input manager has to be the first to be updated
    inputManager->update();

    physicsManager->update(0.02);

    aiManager->update();

    renderManager->update();

    sensorManager->update();

    itemManager->update();
    

    //Event manager has to be the last to be updated
    eventManager->update();

    audioManager->update();

}

//====================================================
//  GAME DRAW
//====================================================
void Game::draw() {

    renderManager->draw();
}

//====================================================
//  GAME CLOSE
//====================================================
void Game::close() {

    physicsManager->close();
    renderManager->close();
    inputManager->close();
    eventManager->close();
    waypointManager->close();
    aiManager->close();
    sensorManager->close();
    itemManager->close();
}

//====================================================
//  GAME LOOP
//====================================================
void Game::Run() {

    Game::init();
    int temp = 600;
    while(Game::stay){
        Game::update();
        Game::draw();
        temp--;
        if(temp == 0)
        {
            EventData data;
            data.Object = ObjectManager::getInstance().getObject(5);

            EventManager::getInstance().addEvent(Event {EventType::Key_UseItem_Down, data});
            temp = 600;
        }
    }

    Game::close();
} 



void addObjects(){

    //Send an event saying object has been created
    uint16_t id = 1;
    GameObject::TransformationData transform;
    
    /*transform.position = glm::vec3(0,0,0);
    std::cout << std::endl;
    ObjectManager::getInstance().createObject(id, transform);
    ObjectManager::getInstance().createObject(id, transform);
    id = 2;
    ObjectManager::getInstance().createObject(id, transform);
    id = 3;
    transform.position = glm::vec3(4,0,4);
    ObjectManager::getInstance().createObject(id, transform);
    EventManager::getInstance().update();
    std::cout << std::endl;
    ObjectManager::getInstance().showObjects();
    std::cout << std::endl;
    id = 4;
    ObjectManager::getInstance().getObject(id);
    id = 2;
    EventData data;
    data.Object = ObjectManager::getInstance().getObject(id);
    EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});
    EventManager::getInstance().update();
    std::cout << std::endl;
    ObjectManager::getInstance().showObjects();
    std::cout << std::endl;
    EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});
    EventManager::getInstance().update();
    std::cout << std::endl;
    ObjectManager::getInstance().showObjects();
    std::cout << std::endl;*/

    //================================================
    // EXPECTED OUTPUT
    //================================================
        /*
        Couldn't insert object. ID: 1 already exists.
        
        1
        2
        3
        
        Couldn't find object. ID: 4 doesn't exist.
        
        1
        3
        
        Couldn't erase object. ID: 2 doesn't exist.
        
        1
        3
        */

    //===============================================================
    // CREATE FIVE POSITIONED OBJECTS
    //===============================================================
    //Planes:
    id = 4;
    transform.position = glm::vec3(0,-5,0);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(50,0.01,20);
    auto ob1 = ObjectManager::getInstance().createObject(id, transform);

    id = 10;
    transform.position = glm::vec3(500,45,0);
    transform.rotation = glm::vec3(0,0,11.365);
    transform.scale    = glm::vec3(51,0.01,20);
    auto ob7 = ObjectManager::getInstance().createObject(id, transform);

    id = 15;
    transform.position = glm::vec3(875,95,-150);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(25,0.01,50);
    auto ob15 = ObjectManager::getInstance().createObject(id, transform);

    id = 16;
    transform.position = glm::vec3(800,95,-500);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(40,0.01,20);
    auto ob16 = ObjectManager::getInstance().createObject(id, transform);

    id = 17;
    transform.position = glm::vec3(550,0,-500);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(10,0.01,20);
    auto ob17 = ObjectManager::getInstance().createObject(id, transform);

    id = 18;
    transform.position = glm::vec3(125,75,-500);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(75,0.01,20);
    auto ob18 = ObjectManager::getInstance().createObject(id, transform);

    id = 19;
    transform.position = glm::vec3(-125,35,-250);
    transform.rotation = glm::vec3(14.94,0,0);
    transform.scale    = glm::vec3(25,0.01,31.05);
    auto ob19 = ObjectManager::getInstance().createObject(id, transform);



    //Player:
    id = 5;
    transform.position = glm::vec3(-30,10,0);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(0.5,0.5,0.5);
    auto ob2 = ObjectManager::getInstance().createObject(id, transform);

    //AI:
    id = 8;
    transform.position = glm::vec3(60,0,60);
    transform.rotation = glm::vec3(45,45,45);
    transform.scale    = glm::vec3(1,1,1);
    auto ob5 = ObjectManager::getInstance().createObject(id, transform);

    //Obstacles:
    id = 6;
    transform.position = glm::vec3(30,0,30);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(1,1,1);
    auto ob3 = ObjectManager::getInstance().createObject(id, transform);
    id = 7;
    transform.position = glm::vec3(50,50,50);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(1,1,1);
    auto ob4 = ObjectManager::getInstance().createObject(id, transform);
    id = 9;
    transform.position = glm::vec3(0,0,0);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(1,1,1);
    auto ob6 = ObjectManager::getInstance().createObject(id, transform);
    id = 11;
    transform.position = glm::vec3(100, 0, 80);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob8 = ObjectManager::getInstance().createObject(id, transform);

    id = 12;
    transform.position = glm::vec3(0,-4,0);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(10,10,30);
    auto ob9 = ObjectManager::getInstance().createObject(id, transform);

    id = 13;
    transform.position = glm::vec3(200, 0, 60);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob10 = ObjectManager::getInstance().createObject(id, transform);

    id = 14;
    transform.position = glm::vec3(300, 0, 60);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob11 = ObjectManager::getInstance().createObject(id, transform);

    //===============================================================
    // ADD WAYPOINT COMPONENT
    //===============================================================
    std::shared_ptr<IComponent> wp1 = WaypointManager::getInstance().createWaypointComponent(ob8, 1, 1);
    std::shared_ptr<IComponent> wp2 = WaypointManager::getInstance().createWaypointComponent(ob10, 1, 2);
    std::shared_ptr<IComponent> wp3 = WaypointManager::getInstance().createWaypointComponent(ob11, 1, 3);

    //===============================================================
    // CREATE FIVE RENDER COMPONENTS
    //===============================================================
    std::shared_ptr<IComponent> cp1 = RenderManager::getInstance().createObjectRenderComponent(*ob1.get(), ObjectRenderComponent::Shape::Cube);

    std::shared_ptr<IComponent> cp2 = RenderManager::getInstance().createObjectRenderComponent(*ob2.get(), ObjectRenderComponent::Shape::Cube);

    std::shared_ptr<IComponent> cp3 = RenderManager::getInstance().createObjectRenderComponent(*ob3.get(), ObjectRenderComponent::Shape::Sphere);

    std::shared_ptr<IComponent> cp5 = RenderManager::getInstance().createObjectRenderComponent(*ob5.get(), ObjectRenderComponent::Shape::Cube);

    std::shared_ptr<IComponent> cp6 = RenderManager::getInstance().createObjectRenderComponent(*ob7.get(), ObjectRenderComponent::Shape::Cube);

    std::shared_ptr<IComponent> cp7 = RenderManager::getInstance().createObjectRenderComponent(*ob8.get(), ObjectRenderComponent::Shape::Sphere);

    std::shared_ptr<IComponent> cp9 = RenderManager::getInstance().createObjectRenderComponent(*ob9.get(), ObjectRenderComponent::Shape::Plane);

    std::shared_ptr<IComponent> cp10 = RenderManager::getInstance().createObjectRenderComponent(*ob10.get(), ObjectRenderComponent::Shape::Sphere);

    std::shared_ptr<IComponent> cp11 = RenderManager::getInstance().createObjectRenderComponent(*ob11.get(), ObjectRenderComponent::Shape::Sphere);

    std::shared_ptr<IComponent> cp12 = RenderManager::getInstance().createObjectRenderComponent(*ob15.get(), ObjectRenderComponent::Shape::Cube);

    std::shared_ptr<IComponent> cp13 = RenderManager::getInstance().createObjectRenderComponent(*ob16.get(), ObjectRenderComponent::Shape::Cube);

    std::shared_ptr<IComponent> cp14 = RenderManager::getInstance().createObjectRenderComponent(*ob17.get(), ObjectRenderComponent::Shape::Cube);

    std::shared_ptr<IComponent> cp15 = RenderManager::getInstance().createObjectRenderComponent(*ob18.get(), ObjectRenderComponent::Shape::Cube);

    std::shared_ptr<IComponent> cp16 = RenderManager::getInstance().createObjectRenderComponent(*ob19.get(), ObjectRenderComponent::Shape::Cube);

    //===============================================================
    // ADD AN INPUT COMPONENT TO THE FIRST OBJECT
    //===============================================================
    std::shared_ptr<IComponent> iCP = InputManager::getInstance().createInputComponent(*ob2.get());

    //===============================================================
    // ADD AN INPUT COMPONENT TO THE AI OBJECTS
    //===============================================================
    //std::shared_ptr<IComponent> iCP2 = InputManager::getInstance().createInputComponent(*ob5.get());


    //===============================================================
    // ADD A CAMERA COMPONENT TO THE FIRST OBJECT
    //===============================================================
    std::shared_ptr<IComponent> cameraCP = RenderManager::getInstance().createCameraRenderComponent(*ob2.get());

    //===============================================================
    // ADD COLLISION COMPONENTS TO ALL OBJECTS
    //===============================================================
    std::shared_ptr<IComponent> collisionCP1 = PhysicsManager::getInstance().createCollisionComponent(*ob2.get(), 5, true, CollisionComponent::Type::Default);
    std::shared_ptr<IComponent> collisionCP2 = PhysicsManager::getInstance().createCollisionComponent(*ob3.get(), 5, true, CollisionComponent::Type::Default);
    std::shared_ptr<IComponent> collisionCP3 = PhysicsManager::getInstance().createCollisionComponent(*ob5.get(), 5, false, CollisionComponent::Type::Default);
    std::shared_ptr<IComponent> collisionCP4 = PhysicsManager::getInstance().createCollisionComponent(*ob8.get(), 5, true, CollisionComponent::Type::Default);
    LAPAL::plane3f terrainX;
    terrainX.p1 = (LAPAL::vec3f( -5 , 0, 15));
    terrainX.p2 = (LAPAL::vec3f( 5 , 0, 15));
    terrainX.p3 = (LAPAL::vec3f( 5 , 0, -15));
    terrainX.p4 = (LAPAL::vec3f( -5 , 0, -15));
    std::shared_ptr<IComponent> collisionCP5 = PhysicsManager::getInstance().createCollisionComponent(*ob9.get(), terrainX, false, CollisionComponent::Type::Ramp);

    //===============================================================
    // ADD TERRAIN COMPONENT
    //===============================================================
    EventManager::getInstance().update();
    LAPAL::plane3f terrain;
    terrain.p1 = (LAPAL::vec3f(-250.f ,0.f ,100.f));
    terrain.p2 = (LAPAL::vec3f(250.f  ,0.f ,100.f));
    terrain.p3 = (LAPAL::vec3f(250.f  ,0.f ,-100.f));
    terrain.p4 = (LAPAL::vec3f(-250.f ,0.f ,-100.f));
    terrain.fric = 0.2;
    LAPAL::calculateRotationsXZ(terrain);
    std::shared_ptr<IComponent> terrainCP1 = PhysicsManager::getInstance().createTerrainComponent(*ob1.get(), terrain);


    LAPAL::plane3f terrain1;
    terrain1.p1 = (LAPAL::vec3f(250,0,100));
    terrain1.p2 = (LAPAL::vec3f(750,100,100));
    terrain1.p3 = (LAPAL::vec3f(750,100,-100));
    terrain1.p4 = (LAPAL::vec3f(250,0,-100));
    terrain1.fric = 0.2;
    LAPAL::calculateRotationsXZ(terrain1);
    std::shared_ptr<IComponent> terrainCP2 = PhysicsManager::getInstance().createTerrainComponent(*ob7.get(), terrain1);
    auto terrainCP2_0 = std::dynamic_pointer_cast<TerrainComponent>(terrainCP2);
    uint16_t idd = 4;
    terrainCP2_0.get()->connectSiblingBack(idd);

    LAPAL::plane3f terrain2;
    terrain2.p1 = (LAPAL::vec3f(750,100,100));
    terrain2.p2 = (LAPAL::vec3f(1000,100,100));
    terrain2.p3 = (LAPAL::vec3f(1000,100,-400));
    terrain2.p4 = (LAPAL::vec3f(750,100,-400));
    terrain2.fric = 0.2;
    LAPAL::calculateRotationsXZ(terrain2);
    std::shared_ptr<IComponent> terrainCP3 = PhysicsManager::getInstance().createTerrainComponent(*ob15.get(), terrain2);
    auto terrainCP3_0 = std::dynamic_pointer_cast<TerrainComponent>(terrainCP3);
    idd = 10;
    terrainCP3_0.get()->connectSiblingBack(idd);

    LAPAL::plane3f terrain3;
    terrain3.p1 = (LAPAL::vec3f(600,100,-400));
    terrain3.p2 = (LAPAL::vec3f(1000,100,-400));
    terrain3.p3 = (LAPAL::vec3f(1000,100,-600));
    terrain3.p4 = (LAPAL::vec3f(600,100,-600));
    terrain3.fric = 0.2;
    LAPAL::calculateRotationsXZ(terrain3);
    std::shared_ptr<IComponent> terrainCP4 = PhysicsManager::getInstance().createTerrainComponent(*ob16.get(), terrain3);
    auto terrainCP4_0 = std::dynamic_pointer_cast<TerrainComponent>(terrainCP4);
    idd = 15;
    terrainCP4_0.get()->connectSiblingLeft(idd);

    LAPAL::plane3f terrain4;
    terrain4.p1 = (LAPAL::vec3f(500,0,-400));
    terrain4.p2 = (LAPAL::vec3f(600,0,-400));
    terrain4.p3 = (LAPAL::vec3f(600,0,-600));
    terrain4.p4 = (LAPAL::vec3f(500,0,-600));
    terrain4.fric = 0.2;
    LAPAL::calculateRotationsXZ(terrain4);
    std::shared_ptr<IComponent> terrainCP5 = PhysicsManager::getInstance().createTerrainComponent(*ob17.get(), terrain4);
    auto terrainCP5_0 = std::dynamic_pointer_cast<TerrainComponent>(terrainCP5);
    idd = 16;
    terrainCP5_0.get()->connectSiblingFront(idd);

    LAPAL::plane3f terrain5;
    terrain5.p1 = (LAPAL::vec3f(-250,80,-400));
    terrain5.p2 = (LAPAL::vec3f(500,80,-400));
    terrain5.p3 = (LAPAL::vec3f(500,80,-600));
    terrain5.p4 = (LAPAL::vec3f(-250,80,-600));
    terrain5.fric = 0.2;
    LAPAL::calculateRotationsXZ(terrain5);
    std::shared_ptr<IComponent> terrainCP6 = PhysicsManager::getInstance().createTerrainComponent(*ob18.get(), terrain5);
    auto terrainCP6_0 = std::dynamic_pointer_cast<TerrainComponent>(terrainCP6);
    idd = 17;
    terrainCP6_0.get()->connectSiblingFront(idd);

    LAPAL::plane3f terrain6;
    terrain6.p1 = (LAPAL::vec3f(-250,0,-100));
    terrain6.p2 = (LAPAL::vec3f(0,0,-100));
    terrain6.p3 = (LAPAL::vec3f(0,80,-400));
    terrain6.p4 = (LAPAL::vec3f(-250,80,-400));
    terrain6.fric = 0.2;
    LAPAL::calculateRotationsXZ(terrain6);
    std::shared_ptr<IComponent> terrainCP7 = PhysicsManager::getInstance().createTerrainComponent(*ob19.get(), terrain6);
    auto terrainCP7_0 = std::dynamic_pointer_cast<TerrainComponent>(terrainCP7);
    idd = 18;
    terrainCP7_0.get()->connectSiblingRight(idd);
    idd = 4;
    terrainCP7_0.get()->connectSiblingLeft(idd);

    

    //===============================================================
    // ADD A MOVE COMPONENT TO THE FIRST OBJECT
    //===============================================================
    LAPAL::movementData mData;

    mData.spin_inc = 0.1;
    mData.max_spin = 0.03;
    mData.brake_spin = 0.2;

    mData.rotate_inc = 0.15;
    mData.max_rotate = 3;

    mData.max_vel = 100.0;
    mData.brake_vel = 5;
    mData.velY = 10;

    mData.max_acc = 10;
    mData.dAcc = 1;
    mData.brake_acc = 0.4;

    std::shared_ptr<IComponent> moveCP1 = PhysicsManager::getInstance().createMoveComponent(*ob2.get(), mData, terrain, 1);

    //===============================================================
    // ADD RAMP COMPONENT
    //===============================================================
    std::shared_ptr<IComponent> rampCP = PhysicsManager::getInstance().createRampComponent(*ob9.get(), 200.0f,0.01f,4.0f);

    //===============================================================
    // ADD A MOVE COMPONENT TO THE AI
    //===============================================================
    LAPAL::movementData mData2;
    mData2.mov = false;
    mData2.jump = false;
    mData2.spi = false;
    mData2.angInc = 0;
    mData2.angle = 0;
    mData2.spin = 0;
    mData2.spin_inc = 0.0005;
    mData2.max_spin = 0.03;
    mData2.brake_spin = 0.2;
    mData2.rotateX = 0.f;
    mData2.rotateZ = 0.f;
    mData2.rotate_inc = 0.15f;
    mData2.max_rotate = 3.f;
    mData2.vel = 0;
    mData2.max_vel = 50.0f;
    mData2.brake_vel = 5.f;
    mData2.velY = 10.f;
    mData2.acc = 0;
    mData2.max_acc = 10.f;
    mData2.dAcc = 1.f;
    mData2.brake_acc = 0.4;

    std::shared_ptr<IComponent> moveCP2 = PhysicsManager::getInstance().createMoveComponent(*ob5.get(), mData2, terrain, 1);

    //===============================================================
    // SETS ALL MOVING CHARACTERS
    //===============================================================
    PhysicsManager::getInstance().createMovingCharacter(moveCP1, terrainCP1, collisionCP1);
    PhysicsManager::getInstance().createMovingCharacter(moveCP2, terrainCP1, collisionCP3);

    //===============================================================
    // ADD AI COMPONENTS
    //===============================================================
    AIManager::getInstance().createAIDrivingComponent(*ob5.get());
    SensorManager::getInstance().createVSensorComponent(*ob5.get(), 75.f, ob5.get()->getTransformData().rotation.y);

    //===============================================================
    // Update to distribute all creation events
    //===============================================================
    EventManager::getInstance().update();

    //===============================================================
    // Split renderManager static QuadTree
    //===============================================================
    RenderManager::getInstance().splitQuadTree();
    //RenderManager::getInstance().getComponentTree().debugStructure(1);
    
}