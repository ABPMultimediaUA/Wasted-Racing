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

    audioManager = new AudioFMOD();
    audioManager->openAudioEngine();

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

    addObjects();
}

//====================================================
//  GAME UPDATE
//====================================================
void Game::update() {

    //Input manager has to be the first to be updated
    inputManager->update();

    physicsManager->update(0.02);
    //audioManager->playSound();
    renderManager->update();
    //Event manager has to be the last to be updated
    eventManager->update();

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

}

//====================================================
//  GAME LOOP
//====================================================
void Game::Run() {

    Game::init();
    audioManager->playSound();

    while(Game::stay){
        Game::update();
        Game::draw();
    }

    Game::close();
}


void addObjects(){

    //Send an event saying object has been created
    uint16_t id = 1;
    GameObject::TransformationData transform;
    
    transform.position = glm::vec3(0,0,0);
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
    std::cout << std::endl;

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
    id = 4;
    transform.position = glm::vec3(0,-5,0);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(50,0.01,50);
    auto ob1 = ObjectManager::getInstance().createObject(id, transform);
    id = 5;
    transform.position = glm::vec3(20,0,20);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(0.5,0.5,0.5);
    auto ob2 = ObjectManager::getInstance().createObject(id, transform);
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
    id = 8;
    transform.position = glm::vec3(60,0,60);
    transform.rotation = glm::vec3(45,45,45);
    transform.scale    = glm::vec3(1,1,1);
    auto ob5 = ObjectManager::getInstance().createObject(id, transform);

    id = 9;
    transform.position = glm::vec3(0,0,0);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(1,1,1);
    auto ob6 = ObjectManager::getInstance().createObject(id, transform);

    id = 10;
    transform.position = glm::vec3(500,38,0);
    transform.rotation = glm::vec3(0,0,10);
    transform.scale    = glm::vec3(50,0.01,50);
    auto ob7 = ObjectManager::getInstance().createObject(id, transform);

    //===============================================================
    // CREATE FIVE RENDER COMPONENTS
    //===============================================================
    std::shared_ptr<IComponent> cp1 = RenderManager::getInstance().createObjectRenderComponent(*ob1.get(), ObjectRenderComponent::Shape::Cube);

    std::shared_ptr<IComponent> cp2 = RenderManager::getInstance().createObjectRenderComponent(*ob2.get(), ObjectRenderComponent::Shape::Cube);

    std::shared_ptr<IComponent> cp3 = RenderManager::getInstance().createObjectRenderComponent(*ob3.get(), ObjectRenderComponent::Shape::Sphere);

    //std::shared_ptr<IComponent> cp4 = RenderManager::getInstance().createObjectRenderComponent(*ob4.get(), ObjectRenderComponent::Shape::Cube);

    std::shared_ptr<IComponent> cp5 = RenderManager::getInstance().createObjectRenderComponent(*ob5.get(), ObjectRenderComponent::Shape::Cube);

    std::shared_ptr<IComponent> cp6 = RenderManager::getInstance().createObjectRenderComponent(*ob7.get(), ObjectRenderComponent::Shape::Cube);

    //===============================================================
    // ADD AN INPUT COMPONENT TO THE FIRST OBJECT
    //===============================================================
    std::shared_ptr<IComponent> iCP = InputManager::getInstance().createInputComponent(*ob2.get());

    //===============================================================
    // ADD A CAMERA COMPONENT TO THE FIRST OBJECT
    //===============================================================
    std::shared_ptr<IComponent> cameraCP = RenderManager::getInstance().createCameraRenderComponent(*ob2.get());

    //===============================================================
    // ADD COLLISION COMPONENTS TO ALL OBJECTS
    //===============================================================
    std::shared_ptr<IComponent> collisionCP1 = PhysicsManager::getInstance().createCollisionComponent(*ob2.get(), 5);
    std::shared_ptr<IComponent> collisionCP2 = PhysicsManager::getInstance().createCollisionComponent(*ob3.get(), 5);
    std::shared_ptr<IComponent> collisionCP3 = PhysicsManager::getInstance().createCollisionComponent(*ob5.get(), 5);
    std::cout << std::endl;
    std::cout << collisionCP3.get() << std::endl;

    //===============================================================
    // ADD TERRAIN COMPONENT
    //===============================================================
    EventManager::getInstance().update();
    LAPAL::plane3f terrain;
    terrain.p1 = (LAPAL::vec3f(-250,0,250));
    terrain.p2 = (LAPAL::vec3f(250,0,250));
    terrain.p3 = (LAPAL::vec3f(250,0,-250));
    terrain.p4 = (LAPAL::vec3f(-250,0,-250));
    terrain.fric = 0.2;
    terrain.incAngle = 0;
    terrain.rotAngle = 0;
    std::shared_ptr<IComponent> terrainCP1 = PhysicsManager::getInstance().createTerrainComponent(*ob1.get(), terrain);
    terrain.p1 = (LAPAL::vec3f(-750,80,250));
    terrain.p2 = (LAPAL::vec3f(750,80,250));
    terrain.p3 = (LAPAL::vec3f(750,0,-250));
    terrain.p4 = (LAPAL::vec3f(-750,0,-250));
    terrain.fric = 0.2;
    terrain.incAngle = -0.17;
    terrain.rotAngle = 0;
    std::shared_ptr<IComponent> terrainCP2 = PhysicsManager::getInstance().createTerrainComponent(*ob7.get(), terrain);
    auto terrainCP2_0 = std::dynamic_pointer_cast<TerrainComponent>(terrainCP2);
    uint16_t idd = 4;
    terrainCP2_0.get()->connectPrevNext(idd);

    //===============================================================
    // ADD A MOVE COMPONENT TO THE FIRST OBJECT
    //===============================================================
    LAPAL::movementData mData;
    mData.vel = 0;
    mData.max_vel = 2.5;
    mData.acc = 0;
    mData.max_acc = 0.5;
    mData.dAcc = 0;
    mData.angle = 0;
    mData.spin = 0;
    mData.spin_inc = 0.00001;
    mData.max_spin = 0.01;
    mData.mov = false;
    mData.vel2d = glm::vec3(0,0,0);

    std::shared_ptr<IComponent> moveCP1 = PhysicsManager::getInstance().createMoveComponent(*ob2.get(), mData, terrain, 1);

    //===============================================================
    // SETS A MOVING CHARACTER
    //===============================================================
    PhysicsManager::getInstance().createMovingCharacter(moveCP1, terrainCP1, collisionCP1);

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