#include "Game.h"

//====================================================
//  GAME INITIALIZATION
//====================================================
void Game::init() {

    Game::stay = true;
    
    inputManager = &InputManager::getInstance();
    inputManager->init();

    Game::engineSetter(0);

    renderManager = &RenderManager::getInstance();
    renderManager->init(Game::engine);

}

//====================================================
//  GAME UPDATE
//====================================================
void Game::update() {

    renderManager->update();
    
    //if(inputManager->IsKeyDown(irr::EKEY_CODE::KEY_ESCAPE))
    //    Game::close();
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