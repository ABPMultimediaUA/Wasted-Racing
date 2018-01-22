#include "ClientLobbyState.h"

//Extra functions
void introVideo();

void ClientLobbyState::init() {
    std::cout << "Attempting to connect to server" << std::endl;
    peer = RakNet::RakPeerInterface::GetInstance();
    RakNet::SocketDescriptor socket(0, 	0);
	socket.socketFamily = AF_INET;
    peer->Startup(1, &socket, 1, 50);
    RakNet::ConnectionAttemptResult result;
    result = peer->Connect("84.120.23.199", 64092, 0, 0);

    if(result == RakNet::CONNECTION_ATTEMPT_STARTED)
    {
        std::cout << "Connection Attempt Started Correctly" << std::endl;
    }
}

void ClientLobbyState::update(float &accumulatedTime) {
    RakNet::Packet* result;
    result = peer->Receive();
    if(result)
    {
        switch(result->data[0])
        {
            case ID_CONNECTION_REQUEST_ACCEPTED:
                std::cout << "Connection Accepted" << std::endl;
                break;
            case ID_CONNECTION_ATTEMPT_FAILED:
                std::cout << "Connection Failed" << std::endl;
                break;
            default:
                break;
        }
    }
    //DESCOMENTAR SI QUIERES QUE CAMBIE A MAIN_STATE
    //Game::getInstance().setState(&MainState::getInstance());
    
}

void ClientLobbyState::draw() {

}

void ClientLobbyState::close() {
    
}