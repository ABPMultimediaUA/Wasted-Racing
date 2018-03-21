#include "../Game.h"
#include <raknet/MessageIdentifiers.h>
#include <raknet/RakPeerInterface.h>
#include <raknet/RakNetTypes.h>
#include <iostream>

#define SERVER_PORT 60005
#define CLIENT_PORT 60006

void initClient(){
	//get instance of peer interface, which usually connects it all
	/*Raknet::RakPeerInterface* peer = RakNet: c:RakPeerInterface::GetInstance();

	peer->Startup(1, &SocketDescriptor(), 1) //Parameters: number of connections, SocketDescriptor function returning a pointer to it, and the mistery variable

	char* serverIP = "localhost"; 		     //For testing, own PC.

	peer->Connect(serverIP, SERVER_PORT, 0, 0); 	//Server IP (localhost for the test), server port (common port between client and server which should be always the same)
													//other 2 parameters to be defined*/
}

int main(){


    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "$$\\   $$\\ $$\\   $$\\           $$\\     $$\\                     "         << std::endl;
    std::cout << "$$ |  $$ |$$ | $$  |          $$ |    \\__|                    "              << std::endl;
    std::cout << "\\$$\\ $$  |$$ |$$  / $$$$$$\\ $$$$$$\\   $$\\ $$$$$$$\\   $$$$$$\\  "        << std::endl;
    std::cout << " \\$$$$  / $$$$$  /  \\____$$\\\\_$$  _|  $$ |$$  __$$\\ $$  __$$\\ "         << std::endl;
    std::cout << " $$  $$<  $$  $$<   $$$$$$$ | $$ |    $$ |$$ |  $$ |$$ /  $$ |"               << std::endl;
    std::cout << "$$  /\\$$\\ $$ |\\$$\\ $$  __$$ | $$ |$$\\ $$ |$$ |  $$ |$$ |  $$ |"          << std::endl;
    std::cout << "$$ /  $$ |$$ | \\$$\\\\$$$$$$$ | \\$$$$  |$$ |$$ |  $$ |\\$$$$$$$ |"          << std::endl;
    std::cout << "\\__|  \\__|\\__|  \\__|\\_______|  \\____/ \\__|\\__|  \\__| \\____$$ |"     << std::endl;   
    std::cout << "                                                    $$\\   $$ |"              << std::endl;
    std::cout << "                                                    \\$$$$$$  |"              << std::endl;
    std::cout << "                                                     \\______/ "              << std::endl;

	//initializing client
	initClient();

	Game::getInstance().Run();
	
	std::cout << "Thanks for playing XKating!" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
	
	return 0;  

}

