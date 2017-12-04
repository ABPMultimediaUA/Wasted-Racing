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

    audioManager->playSound();

    

    //uint16_t id = 4;
    //auto obj = objectManager->getObject(id).get();
    //obj->getComponent<MoveComponent>().get()->update(0.1);
    //float a = obj->getTransformData().position.x;
    //float b = obj->getTransformData().position.z;
    //std::cout << a << " " << b << std::endl;
    //for(int i=0; i<100000000; i++);
    //
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
    transform.scale    = glm::vec3(100,0.01,100);
    auto ob1 = ObjectManager::getInstance().createObject(id, transform);
    id = 5;
    transform.position = glm::vec3(20,0,20);
    transform.rotation = glm::vec3(45,45,45);
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
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(1,1,1);
    auto ob5 = ObjectManager::getInstance().createObject(id, transform);

    //===============================================================
    // CREATE FIVE RENDER COMPONENTS
    //===============================================================
    std::shared_ptr<IComponent> cp1 = std::make_shared<ObjectRenderComponent>(*ob1.get(), ObjectRenderComponent::Shape::Cube);
    ob1.get()->addComponent(cp1);
    data.Component = cp1;
    EventManager::getInstance().addEvent(Event {EventType::RenderComponent_Create, data});
    cp1.get()->init();

    std::shared_ptr<IComponent> cp2 = std::make_shared<ObjectRenderComponent>(*ob2.get(), ObjectRenderComponent::Shape::Cube);
    data.Component = cp2;
    ob2.get()->addComponent(cp2);
    EventManager::getInstance().addEvent(Event {EventType::RenderComponent_Create, data});
    cp2.get()->init();

    std::shared_ptr<IComponent> cp3 = std::make_shared<ObjectRenderComponent>(*ob3.get(), ObjectRenderComponent::Shape::Sphere);
    data.Component = cp3;
    ob3.get()->addComponent(cp3);
    EventManager::getInstance().addEvent(Event {EventType::RenderComponent_Create, data});
    cp3.get()->init();

    std::shared_ptr<IComponent> cp4 = std::make_shared<ObjectRenderComponent>(*ob4.get(), ObjectRenderComponent::Shape::Cube);
    data.Component = cp4;
    ob4.get()->addComponent(cp4);
    EventManager::getInstance().addEvent(Event {EventType::RenderComponent_Create, data});
    cp4.get()->init();

    std::shared_ptr<IComponent> cp5 = std::make_shared<ObjectRenderComponent>(*ob5.get(), ObjectRenderComponent::Shape::Cube);
    data.Component = cp5;
    ob5.get()->addComponent(cp5);
    EventManager::getInstance().addEvent(Event {EventType::RenderComponent_Create, data});
    cp5.get()->init();

    //===============================================================
    // ADD AN INPUT COMPONENT TO THE FIRST OBJECT
    //===============================================================
    std::shared_ptr<IComponent> iCP = std::make_shared<InputComponent>(*ob2.get());
    ob2.get()->addComponent(iCP);
    data.Component = iCP;
    EventManager::getInstance().addEvent(Event {EventType::InputComponent_Create, data});

    //===============================================================
    // ADD A MOVE COMPONENT TO THE FIRST OBJECT
    //===============================================================
    LAPAL::movementData mData;
    std::shared_ptr<IComponent> moveCP = std::make_shared<MoveComponent>(*ob2.get(), mData);
    ob2.get()->addComponent(moveCP);
    //data.Component = moveCP;
    //EventManager::getInstance().addEvent(Event {EventType::MoveComponent_Create, data});


    //===============================================================
    // Update to distribute all creation events
    //===============================================================
    EventManager::getInstance().update();

    //===============================================================
    // Split renderManager static QuadTree
    //===============================================================
    //RenderManager::getInstance().splitQuadTree();
    //RenderManager::getInstance().getComponentTree().debugStructure(1);
    
}