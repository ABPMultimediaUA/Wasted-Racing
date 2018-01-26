#include "ClientLobbyState.h"


//Extra functions
void introVideo();

void ClientLobbyState::init() {
    networkManager = &NetworkManager::getInstance();

    renderManager = &RenderManager::getInstance();
    renderManager->getRenderFacade()->createRectangle2D(glm::vec2(renderManager->getRenderFacade()->getWindow().size.x/2-600, renderManager->getRenderFacade()->getWindow().size.y/2-331), "media/img/lobbyProv.png");
    renderManager->getRenderFacade()->drawGUI();

    if(!networkManager->getConected())
    {
        networkManager->initLobby();
    }
}

void ClientLobbyState::update(float &accumulatedTime) {
    networkManager->updateLobby();
    if(networkManager->getStarted())
    {
        renderManager->getRenderFacade()->deleteRectangle2D();
        renderManager->getRenderFacade()->drawGUI();
        Game::getInstance().setState(IGameState::stateType::MULTIMATCH);
    }
}

void ClientLobbyState::draw() {

}

void ClientLobbyState::close() {
    
}