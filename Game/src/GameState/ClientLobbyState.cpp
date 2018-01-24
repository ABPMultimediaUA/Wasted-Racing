#include "ClientLobbyState.h"


//Extra functions
void introVideo();

void ClientLobbyState::init() {
    networkManager = &NetworkManager::getInstance();
    networkManager->initLobby();
}

void ClientLobbyState::update(float &accumulatedTime) {
    networkManager->update();
    if(networkManager->getStarted())
    {
        Game::getInstance().setState(&MultiMatchState::getInstance());
    }
}

void ClientLobbyState::draw() {

}

void ClientLobbyState::close() {
    
}