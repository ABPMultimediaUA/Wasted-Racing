#include "NetworkManager.h"

void NetworkManager::init() {
    //Get instance of the peer
    peer = RakNet::RakPeerInterface::GetInstance();

    //Match not started
    started = false;
}

void NetworkManager::update() {

}

void NetworkManager::close() {

}

void NetworkManager::initLobby(){
    std::cout << "Attempting to connect to server" << std::endl;
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

void NetworkManager::updateLobby(){
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
                break;
            case ID_CONNECTION_ATTEMPT_FAILED:
                std::cout << "Connection Failed" << std::endl;
                break;
            case ID_GAME_START:
                std::cout << "Game started " << std::endl;
                started = true;
                break;
            default:
                break;
        }
    }
}