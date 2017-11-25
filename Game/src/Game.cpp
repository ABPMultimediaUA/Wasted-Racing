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
    
    std::cout << std::endl;
    ObjectManager::getInstance().createObject(id, transform);
    ObjectManager::getInstance().createObject(id, transform);

    id = 2;
    ObjectManager::getInstance().createObject(id, transform);
    id = 3;
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
    
}