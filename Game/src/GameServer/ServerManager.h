#pragma once
//External includes
#include <memory>
#include <chrono>
#include <stdio.h>
#include <iostream>
#include <rapidxml/rapidxml.hpp>
#include <string>
#include <fstream>
#include <stdio.h>
#include <raknet/RakPeerInterface.h> 
#include <raknet/MessageIdentifiers.h>
#include <raknet/RakNetTypes.h>
#include <vector>

//Basic include
#include "../GlobalVariables.h"
#include "CustomIdentifiers.h"

//Managers
#include "../GameManager/ObjectManager.h"
#include "../GameManager/PhysicsManager.h"
#include "../GameManager/WaypointManager.h"
#include "../GameManager/AIManager.h"
#include "../GameManager/SensorManager.h"
#include "../GameManager/ScoreManager.h"
#include "../GameManager/ItemManager.h"
#include "../GameEvent/EventManager.h"
#include "../GameObject/NetworkComponent/RemotePlayerComponent.h"
#include "../GameObject/NetworkComponent/RemoteItemComponent.h"

class ItemManager;

//Maximum number of clients simultaneously
#define MAXCLIENTS 8
//Port of the server
#define PORT 39017

class ServerManager{
    public:
        //Constructor
        ServerManager() {};

        //Destructor
        ~ServerManager() {
        };

        //Initialize the server
        void init();

        //Runs the server
        void run();

        //Receives the packets and updates the info of the game
        void update();
        //==============================================================
        // Getters and setters
        //==============================================================
        int getNPlayers()                            {  return nPlayers;      };
        int getNObjects()                            {  return nObjects;      };
        RakNet::SystemAddress getPlayerAddress(int i){  return players.at(i); };

    private:
        ///////////////////////////////////////////////////
        // PRIVATE DATA 
        ///////////////////////////////////////////////////
        RakNet::RakPeerInterface* peer;
        RakNet::Packet* packet;

        //If true, the game has started, if not, then we are in the lobby and accept new incomming connections
        bool started;

        //Number of players in the server (actually)
        int nPlayers;

        //Number of objects in the server (actually)
        int nObjects;

        //Time that has to pass till updating the managers
        float loopTime;

        //Vector of players addresses
        std::vector<RakNet::SystemAddress> players;

        ///////////////////////////////////////////////////
        // FUNCTIONS 
        ///////////////////////////////////////////////////

        ///////
        //Game related
        ///////
        
        //Function to start the match from the lobby
        void startGame();

        //Function to end the match
        void endGame(RakNet::Packet* packet);

        //Function provided by RakNet
        unsigned char GetPacketIdentifier(RakNet::Packet *p);

        ///////
        //Sending data
        ///////

        //Broadcast one packet to the rest of the players
        void broadcastData(RakNet::Packet* packet);

        //Broadcasts one playable object to the rest of the players
        void broadcastObject(RakNet::Packet* packet);

};