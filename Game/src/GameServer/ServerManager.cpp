#include "ServerManager.h"

//Function provided by RakNet
unsigned char ServerManager::GetPacketIdentifier(RakNet::Packet *p)
{
	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
		return (unsigned char) p->data[sizeof(MessageID) + sizeof(RakNet::Time)];
	else
		return (unsigned char) p->data[0];
}

void ServerManager::init()
{
    //First we get the instance of the peer
	peer=RakNet::RakPeerInterface::getInstance();
	//Then we initialize the socket and set his socketFamily (for default communication, use AF_INET)
	RakNet::SocketDescriptor socket(PORT, 	0);
	socket.socketFamily = AF_INET;

	peer->Startup(MAXCLIENTS, &socket, 1);
	peer->SetMaximumIncomingConnections(MAXCLIENTS);
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

void ServerManager::update()
{
	//identifier of the packet
	unsigned char identifier;

	for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
	{
		identifier=GetPacketIdentifier();
		switch(identifier)
		{

		}
	}

}