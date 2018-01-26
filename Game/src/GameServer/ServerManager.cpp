#include "ServerManager.h"
#include <iostream>

//Function provided by RakNet
unsigned char ServerManager::GetPacketIdentifier(RakNet::Packet *p)
{
	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
		return (unsigned char) p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
	else
		return (unsigned char) p->data[0];
}

void ServerManager::init()
{
    //First we get the instance of the peer
	peer=RakNet::RakPeerInterface::GetInstance();
	//Then we initialize the socket and set his socketFamily (for default communication, use AF_INET)
	RakNet::SocketDescriptor socket(PORT, 	0);
	socket.socketFamily = AF_INET;

	peer->Startup(MAXCLIENTS, &socket, 1);
	peer->SetMaximumIncomingConnections(MAXCLIENTS);

	nPlayers = 0;
	nObjects = 0;
	started  = false;
}

void ServerManager::run()
{
	//We run the server until we decide to close it, we should change the condition later
	while(true)
	{
		update();

		//After the update, we should use a clock to make the system sleep until the next update
	}
}

//Function to start the game when we are at the lobby
void ServerManager::startGame()
{
	if(!started && nPlayers > 1)
	{
		float x, y, z;
		RakNet::BitStream stream;
		started=true;
		std::cout << "Starting game" << std::endl;
		stream.Write((unsigned char)ID_GAME_START);
		peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
		for(int i=0; i<nPlayers; i++)
		{
			//This switch marks the predefined positions for the players. Later this will be on map's info
			switch(i)
			{
				case 0:
					x=-35.f;
					y=5.f;
					z=-20.f;
					break;
				case 1:
					z=-10.f;
					break;
				default:
					break;
			}
			//Reset and set the message to create player
			stream.Reset();
			stream.Write((unsigned char)ID_CREATE_PLAYER);
			//Its Network ID;
			stream.Write(i);
			//Its position, later will be removed
			stream.Write(x);
			stream.Write(y);
			stream.Write(z);
			peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, players[i], false);
			//Repeat for broadcast
			stream.Reset();
			stream.Write((unsigned char)ID_CREATE_REMOTE_PLAYER);
			stream.Write(i);
			stream.Write(x);
			stream.Write(y);
			stream.Write(z);
			peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, players[i], true);
		}
	}
}

void ServerManager::broadcastData(RakNet::Packet* packet)
{
	RakNet::BitStream stream(packet->data, packet->length, false);

	peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, packet->systemAddress, true);
}

void ServerManager::broadcastObject(RakNet::Packet* packet)
{
	RakNet::BitStream stream(packet->data, packet->length, false);
	stream.Write((int)nObjects);

	peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, packet->systemAddress, true);
}

void ServerManager::endGame(RakNet::Packet* packet)
{
	broadcastData(packet);

	started=false;
}

void ServerManager::update()
{
	//identifier of the packet
	unsigned char identifier;

	for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
	{
		identifier=GetPacketIdentifier(packet);
		switch(identifier)
		{
			case ID_REMOTE_CONNECTION_LOST:
				nPlayers--;
				std::cout << "Client disconnected from the server" << std::endl;
				std::cout << "Number of players: " << nPlayers << std::endl;
				break;
			case ID_CONNECTION_LOST:
				nPlayers--;
				std::cout << "Client disconnected from the server" << std::endl;
				std::cout << "Number of players: " << nPlayers << std::endl;
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				nPlayers--;
				std::cout << "Client disconnected from the server" << std::endl;
				std::cout << "Number of players: " << nPlayers << std::endl;
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				nPlayers--;
				std::cout << "Client disconnected from the server" << std::endl;
				std::cout << "Number of players: " << nPlayers << std::endl;
				break;
			case ID_NEW_INCOMING_CONNECTION:
				nPlayers++;
				std::cout << "New client in the server" << std::endl;
				std::cout << "Number of players: " << nPlayers << std::endl;
				players.push_back(packet->systemAddress);
				break;
			case ID_GAME_START:
				startGame();
				break;
			case ID_GAME_ENDED:
				endGame(packet);
				break;
			case ID_REMOTE_PLAYER_MOVEMENT:
				broadcastData(packet);
				break;
			case ID_CREATE_BANANA:
				broadcastObject(packet);
				nObjects++;
				std::cout << "Objeto numero "<<nObjects<< " creado." << std::endl;
				break;
			case ID_DESTROY_BANANA:
				broadcastData(packet);
				nObjects--;
				break;
			case ID_BOX_COLLISION:
				broadcastData(packet);
				break;
            default:
                std::cout << "Receiving new packet" << std::endl;
                break;
		}
	}

}