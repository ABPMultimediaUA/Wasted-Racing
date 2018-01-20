#pragma once

#include "../Game.h"
#include <raknet/RakPeerInterface.h> 
#include <raknet/MessageIdentifiers.h>

//Maximum number of clients simultaneously
#define MAXCLIENTS 8
//Port of the server
#define PORT 32090

class ServerManager{
    public:
        //Constructor
        ServerManager() {};

        //Destructor
        ~ServerManager() {};

        //Initialize the server
        void init();

        //Runs the server
        void run();

        //Receives the packets and updates the info of the game
        void update();


    private:
        Raknet::RakPeerInterface* peer;
        RakNet::Packet* packet;

        //Function provided 
        unsigned char GetPacketIdentifier(Packet *p);

};