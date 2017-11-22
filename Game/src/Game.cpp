#include "Game.h"

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

    //First we initialize renderManager, who creates a device and passes 
    //this reference to the inputManager
    renderManager = &RenderManager::getInstance();
    renderManager->init(Game::renderEngine);
//
    ////Once we've initialized the renderManager, we can do the same with
    ////our inputManager
    inputManager = &InputManager::getInstance();
    inputManager->init(Game::inputEngine);

}

//====================================================
//  GAME UPDATE
//====================================================
void Game::update() {

    //inputManager->update();
    //renderManager->update();
    //eventManager->update();
    
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