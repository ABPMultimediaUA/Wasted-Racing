#include "ClientLobbyState.h"


//Extra functions
void introVideo();

void ClientLobbyState::init() {

    networkManager = &NetworkManager::getInstance();
    objectManager = &ObjectManager::getInstance();
    renderManager = &RenderManager::getInstance();
    inputManager  = &InputManager::getInstance();
    eventManager = &EventManager::getInstance();

    renderManager->getRenderFacade()->createRectangle2D(glm::vec2(renderManager->getRenderFacade()->getWindow().size.x/2-600, renderManager->getRenderFacade()->getWindow().size.y/2-331), "media/img/lobbyProv.png");
    renderManager->getRenderFacade()->drawGUI();

    if(!networkManager->getConnected())
    {
        networkManager->initLobby();
    }
}

void ClientLobbyState::update(float &accumulatedTime) {

    inputManager->update();

    networkManager->updateLobby();
    if(networkManager->getStarted())
    {
        renderManager->getRenderFacade()->deleteRectangle2D();
        renderManager->getRenderFacade()->drawGUI();
        Game::getInstance().setState(IGameState::stateType::MULTIMATCH);
    }

    eventManager->update();

    Game::getInstance().setStay(objectManager->getGameRunning());
}

void ClientLobbyState::draw() {

}

void ClientLobbyState::close() {
    
}