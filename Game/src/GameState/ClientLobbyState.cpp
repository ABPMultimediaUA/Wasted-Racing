#include "ClientLobbyState.h"

//Extra functions
void introVideo();

void ClientLobbyState::init() {

}

void ClientLobbyState::update(float &accumulatedTime) {

    std::cout<<"Hola soy un bonito lobby de cliente, qué quieres que haga?"<<std::endl;
    //DESCOMENTAR SI QUIERES QUE CAMBIE A MAIN_STATE
    //Game::getInstance().setState(&MainState::getInstance());
    
}

void ClientLobbyState::draw() {

}

void ClientLobbyState::close() {
    
}