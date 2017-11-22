#include "Game.h"

//====================================================
//  GAME INITIALIZATION
//====================================================
void Game::init() {

    //Say game loop is active
    Game::stay = true;

    //Set engine to default
    Game::engineSetter(0);

    //Initilize managers
    eventManager = &EventManager::getInstance();
    eventManager->init();

    renderManager = &RenderManager::getInstance();
    renderManager->init(Game::engine);
    
    inputManager = &InputManager::getInstance();
    inputManager->init();


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