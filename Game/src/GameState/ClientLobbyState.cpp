#include "ClientLobbyState.h"


//Extra functions
void introVideo();

void ClientLobbyState::init() {
    networkManager = &NetworkManager::getInstance();
    networkManager->initLobby();
}

void ClientLobbyState::update(float &accumulatedTime) {
    networkManager->updateLobby();
    if(networkManager->getStarted())
    {
        Game::getInstance().setState(IGameState::stateType::MULTIMATCH);
    }
}

void ClientLobbyState::draw() {

}

void ClientLobbyState::close() {
    
}