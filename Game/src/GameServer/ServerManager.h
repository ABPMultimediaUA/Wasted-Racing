#pragma once

#include "../Game.h"
#include <raknet/RakPeerInterface.h> 
#include <raknet/MessageIdentifiers.h>
#include <raknet/RakNetTypes.h>
#include <vector>
#include "CustomIdentifiers.h"

//Maximum number of clients simultaneously
#define MAXCLIENTS 8
//Port of the server
#define PORT 39017

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
        RakNet::RakPeerInterface* peer;
        RakNet::Packet* packet;

        //If true, the game has started, if not, then we are in the lobby and accept new incomming connections
        bool started;

        //Number of players in the server (actually)
        int nPlayers;

        //Vector of players addresses
        std::vector<RakNet::SystemAddress> players;

        ///////////////////////////////////////////////////
        // FUNCTIONS 
        ///////////////////////////////////////////////////
        //Function provided by RakNet
        unsigned char GetPacketIdentifier(RakNet::Packet *p);

        //Function to start the match from the lobby
        void startGame();

        //Broadcast one players position
        void broadcastPosition(RakNet::Packet* packet);

        //Broadcast the creation of the banana
        void broadcastCreateBanana(RakNet::Packet* packet);

        //Broadcast the deletion of the banana
        void broadcastDestroyBanana(RakNet::Packet* packet);

};