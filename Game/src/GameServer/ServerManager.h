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
            //Close all managers
            if(physicsManager!=nullptr){
                physicsManager->close();
            }
            if(eventManager!=nullptr){
                eventManager->close();
            }
            if(waypointManager!=nullptr){
                waypointManager->close();
            }
            if(aiManager!=nullptr){
                aiManager->close();
            }
            if(sensorManager!=nullptr){
                sensorManager->close();
            }
            if(itemManager!=nullptr){
                itemManager->close();
            }
            if(scoreManager!=nullptr){
                scoreManager->close();
            }
        };

        //Initialize the server
        void init();

        //Runs the server
        void run();

        //Receives the packets and updates the info of the game
        void update(float dTime);

        //==============================================================
        // Getters and setters
        //==============================================================
        int getNPlayers()                            {  return nPlayers;       };
        int getNObjects()                            {  return nObjects;       };
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

        //Vector of players addresses
        std::vector<RakNet::SystemAddress> players;

        ///////////////////////////////////////////////////
        // MANAGERS 
        ///////////////////////////////////////////////////
        //Global Variables
        GlobalVariables* globalVariables;
        //Object manager
        ObjectManager* objectManager;
        //Event manager
        EventManager* eventManager;
        //Physics manager
        PhysicsManager* physicsManager;
        //Waypoint manager
        WaypointManager* waypointManager;
        //AI manager
        AIManager* aiManager;
        //Sensor manager
        SensorManager* sensorManager;
        //Item manager
        ItemManager* itemManager;
        //Score manager
        ScoreManager* scoreManager;

        ///////////////////////////////////////////////////
        // FUNCTIONS 
        ///////////////////////////////////////////////////

        ///////
        //PLayer related
        ///////
        //Create player in physical world
        void addPlayer();

        //Make the player do what the player commanded
        void actPlayer(RakNet::Packet* packet);

        ///////
        //Game related
        ///////
        
        //Function to start the match from the lobby
        void startGame();

        //Function to end the match
        void endGame(RakNet::Packet* packet);

        //Function provided by RakNet
        unsigned char GetPacketIdentifier(RakNet::Packet *p);



        //Broadcast one packet to the rest of the players
        void broadcastData(RakNet::Packet* packet);

        //Broadcasts one playable object to the rest of the players
        void broadcastObject(RakNet::Packet* packet);

};