#include "PreMatchState.h"

void PreMatchState::init() {
    //Bind all managers that are going to be used
    eventManager  = &EventManager::getInstance();
    inputManager  = &InputManager::getInstance();
    renderManager = &RenderManager::getInstance();
    objectManager = &ObjectManager::getInstance();
    audioManager = &AudioManager::getInstance();

    Game::getInstance().setAccumulatedTime(0);
}

void PreMatchState::update(float &accumulatedTime) {

    i++;
    if(i>100){
        Game::getInstance().setState(IGameState::stateType::MATCH);
    }
        
    std::cout << i << std::endl;

    //Update input manager
    inputManager->update();
    
    //Update audio manager
    audioManager->update();

    //Event manager has to be the last to be updated
    eventManager->update();

    //Sets if the game keeps running or not
    //:::>Change with event that closes the game
    Game::getInstance().setStay(objectManager->getGameRunning());
}

void PreMatchState::draw() {
    renderManager->draw();
}

void PreMatchState::close() {
    
}