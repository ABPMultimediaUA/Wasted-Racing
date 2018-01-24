#include "ClientLobbyState.h"


//Extra functions
void introVideo();

void ClientLobbyState::init() {
    std::cout << "Attempting to connect to server" << std::endl;
    peer = RakNet::RakPeerInterface::GetInstance();
    RakNet::SocketDescriptor socket(0, 	0);
	socket.socketFamily = AF_INET;
    peer->Startup(1, &socket, 1);
    RakNet::ConnectionAttemptResult result;
    result = peer->Connect("127.0.0.1", 39017, 0, 0);

    if(result == RakNet::CONNECTION_ATTEMPT_STARTED)
    {
        std::cout << "Connection Attempt Started Correctly" << std::endl;
    }
}

void ClientLobbyState::update(float &accumulatedTime) {
    RakNet::Packet* result;
    RakNet::BitStream stream;
    result = peer->Receive();
    if(result)
    {
        switch(result->data[0])
        {
            case ID_CONNECTION_REQUEST_ACCEPTED:
                std::cout << "Connection Accepted" << std::endl;
                stream.Write((unsigned char)ID_GAME_START);
                peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
                Game::getInstance().setState(&MultiMatchState::getInstance());
                break;
            case ID_CONNECTION_ATTEMPT_FAILED:
                std::cout << "Connection Failed" << std::endl;
                break;
            default:
                break;
        }
    }
}

void ClientLobbyState::draw() {

}

void ClientLobbyState::close() {
    
}