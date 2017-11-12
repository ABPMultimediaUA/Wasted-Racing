#include "Game.h"

void Game::init() {

    Game::stay = true;
    
    inputManager = &InputManager::getInstance();
    inputManager->init();

    Game::engineSetter(0);

    renderManager = &RenderManager::getInstance();
    renderManager->init(Game::engine);

}

void Game::update() {
    
    if(inputManager->IsKeyDown(irr::EKEY_CODE::KEY_ESCAPE))
        Game::close();

}

void Game::draw() {

}

void Game::close() {

    renderManager->close();

    Game::stay = false;

}

void Game::Run() {

    Game::init();

    while(Game::stay){
        Game::update();
        Game::draw();
    }

}