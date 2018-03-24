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

        //Updates the managers with the time given
        void updateManagers(float dTime);

        //Interpolate positions of all variables needed to
        void interpolate(float dTime);

        //==============================================================
        // Component creator
        //==============================================================
        //Create remote player component
        IComponent::Pointer createRemotePlayerComponent(GameObject& newGameObject, int server_id);

        //Create remote player component
        IComponent::Pointer createRemoteItemComponent(GameObject& newGameObject, int server_id, int type);
        //==============================================================
        // Getters and setters
        //==============================================================
        int getNPlayers()                            {  return nPlayers;      };
        int getNObjects()                            {  return nObjects;      };
        RakNet::SystemAddress getPlayerAddress(int i){  return players.at(i); };
        GameObject* getPlayer(int server_id)         {
                for(unsigned int i = 0; i<remotePlayerComponentList.size(); i++)
                {
                    auto rPlayer = std::dynamic_pointer_cast<RemotePlayerComponent>(remotePlayerComponentList[i]);
                    if(rPlayer.get()->getServerId() == server_id)
                    {
                        return &rPlayer.get()->getGameObject();
                    }
                }
                return nullptr;
        }
        IComponent::Pointer getRemotePlayerComponent(int server_id)
                                                     {
                for(unsigned int i = 0; i<remotePlayerComponentList.size(); i++)
                {
                    auto rPlayer = std::dynamic_pointer_cast<RemotePlayerComponent>(remotePlayerComponentList[i]);
                    if(rPlayer.get()->getServerId() == server_id)
                    {
                        return rPlayer;
                    }
                }
                return nullptr;
        }
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

        //Vector of player instances (as Remote Player component)
        std::vector<IComponent::Pointer> remotePlayerComponentList;

        //List of remotePlayerComponent
        std::vector<IComponent::Pointer> remoteTrapComponentList;

        //List of remotePlayerComponent
        std::vector<IComponent::Pointer> remoteRedShellComponentList;

        //List of remotePlayerComponent
        std::vector<IComponent::Pointer> remoteBlueShellComponentList;

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

        ///////
        //Sending data
        ///////

        //Broadcast position of all players
        void broadcastPositionPlayers();

        //Send information about an object event (creation, destruction, collision with box) expressing who did it by server id, which object by server id, and what type of event
        void objectEvent(int who, int which, customMessages what);

        //Broadcast one packet to the rest of the players
        void broadcastData(RakNet::Packet* packet);

        //Broadcasts one playable object to the rest of the players
        void broadcastObject(RakNet::Packet* packet);

};