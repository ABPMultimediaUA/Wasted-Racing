#include "../Game.h"
#include <raknet/MessageIdentifiers.h>
#include <raknet/RakPeerInterface.h>
#include <raknet/RakNetTypes.h>
#include <iostream>

#define MAX_PLAYERS_PER_SERVER 4
#define MAX_CONNECTIONS_ALLOWED 10
#define CLIENT_PORT 60006
#define SERVER_PORT 60005

void initServer(){
	//get instance of peer interface, which usually connects it all
	Raknet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();

	peer->Startup(MAX_CONNECTIONS_ALLOWED, &SocketDescriptor(SERVER_PORT,0), 1);
	peer->SetMaximumIncomingConnections(MAX_PLAYERS_PER_SERVER);


}

int main(){

	std::cout << "Starting server..." << std::endl;

	initServer();

	for(;;) {

	}
	
	/*Game game;
	
	game.Run();*/
	
	std::cout << "Exiting server..." << std::endl;
	
	return 0;  

}

