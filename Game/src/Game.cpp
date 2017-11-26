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

    //Initilize managers
    eventManager = &EventManager::getInstance();
    eventManager->init();

    //First we initialize renderManager, who creates a device and passes this reference to the inputManager
    renderManager = &RenderManager::getInstance();
    renderManager->init(Game::renderEngine);

    //Once we've initialized the renderManager, we can do the same with our inputManager
    inputManager = &InputManager::getInstance();
    inputManager->init(Game::inputEngine);

    objectManager = &ObjectManager::getInstance();
    objectManager->init();

    addObjects();
}

//====================================================
//  GAME UPDATE
//====================================================
void Game::update() {

    inputManager->update();
    renderManager->update();
    eventManager->update();
    
}

//====================================================
//  GAME DRAW
//====================================================
void Game::draw() {

}

//====================================================
//  GAME CLOSE
//====================================================
void Game::close() {

    renderManager->close();
    inputManager->close();
    eventManager->close();

}

//====================================================
//  GAME LOOP
//====================================================
void Game::Run() {

    Game::init();

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
    transform.position = glm::vec3(1,0,1);
    auto ob1 = ObjectManager::getInstance().createObject(id, transform);
    id = 5;
    transform.position = glm::vec3(2,0,2);
    auto ob2 = ObjectManager::getInstance().createObject(id, transform);
    id = 6;
    transform.position = glm::vec3(3,0,3);
    auto ob3 = ObjectManager::getInstance().createObject(id, transform);
    id = 7;
    transform.position = glm::vec3(5,0,5);
    auto ob4 = ObjectManager::getInstance().createObject(id, transform);
    id = 8;
    transform.position = glm::vec3(6,0,6);
    auto ob5 = ObjectManager::getInstance().createObject(id, transform);

    //===============================================================
    // CREATE FIVE RENDER COMPONENTS
    //===============================================================
    std::shared_ptr<IComponent> cp1 = std::make_shared<ObjectRenderComponent>(*ob1.get());
    ob1.get()->addComponent(cp1);
    data.Component = cp1;
    EventManager::getInstance().addEvent(Event {EventType::RenderComponent_Create, data});

    std::shared_ptr<IComponent> cp2 = std::make_shared<ObjectRenderComponent>(*ob2.get());
    data.Component = cp2;
    ob2.get()->addComponent(cp2);
    EventManager::getInstance().addEvent(Event {EventType::RenderComponent_Create, data});

    std::shared_ptr<IComponent> cp3 = std::make_shared<ObjectRenderComponent>(*ob3.get());
    data.Component = cp3;
    ob3.get()->addComponent(cp3);
    EventManager::getInstance().addEvent(Event {EventType::RenderComponent_Create, data});

    std::shared_ptr<IComponent> cp4 = std::make_shared<ObjectRenderComponent>(*ob4.get());
    data.Component = cp4;
    ob4.get()->addComponent(cp4);
    EventManager::getInstance().addEvent(Event {EventType::RenderComponent_Create, data});

    std::shared_ptr<IComponent> cp5 = std::make_shared<ObjectRenderComponent>(*ob5.get());
    data.Component = cp5;
    ob5.get()->addComponent(cp5);
    EventManager::getInstance().addEvent(Event {EventType::RenderComponent_Create, data});

    //===============================================================
    // ADD AN INPUT COMPONENT TO THE FIRST OBJECT
    //===============================================================
    std::shared_ptr<IComponent> iCP = std::make_shared<InputComponent>(*ob1.get());
    ob1.get()->addComponent(iCP);
    data.Component = iCP;
    EventManager::getInstance().addEvent(Event {EventType::InputComponent_Create, data});


    //===============================================================
    // Update to distribute all creation events
    //===============================================================
    EventManager::getInstance().update();

    //===============================================================
    // Split renderManager static QuadTree
    //===============================================================
    RenderManager::getInstance().splitQuadTree();
    RenderManager::getInstance().getComponentTree().debugStructure(1);
    
}