#pragma once

#include "ObjectManager.h"
#include "PhysicsManager.h"
#include "RenderManager.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/NetworkComponent/RemotePlayerComponent.h"
#include "../GameObject/NetworkComponent/RemoteItemComponent.h"
#include <raknet/RakPeerInterface.h> 
#include <raknet/MessageIdentifiers.h>
#include <raknet/BitStream.h>
#include "../GameServer/CustomIdentifiers.h"
#include <iostream>
#include <memory>

class NetworkManager{

public: 

    //Constructor
    NetworkManager() {}

    //Destructor
    ~NetworkManager() {}

    //Initialization
    void init();

    //Update
    void update();

    //Shutdown
    void close();

    //Static class getter
    static NetworkManager& getInstance();

    //Initializes lobby
    void initLobby();

    //Updates the lobby
    void updateLobby();

    //==============================================================
    // Create component
    //==============================================================
    //Create remote player component
    IComponent::Pointer createRemotePlayerComponent(GameObject& newGameObject);

    //Create remote player component
    IComponent::Pointer createRemoteItemComponent(GameObject& newGameObject, int type);

    //==============================================================
    // Create Objects
    //==============================================================

    //Collocates the player in the map
    void createPlayer(RakNet::Packet* packet);

    //Creates a remote player in our game
    void createRemotePlayer(RakNet::Packet* packet);

    //creates a banana item where it should be on the map
    void createBanana(RakNet::Packet* packet);

    //destroy a banana item where it should be on the map
    void destroyBanana(RakNet::Packet* packet);

    //==============================================================
    // Network Communication Functions
    //==============================================================

    //Broadcasts the player position to the rest of players
    void broadcastPosition();

    //Receives a remote player position
    void moveRemotePlayer(RakNet::Packet* packet);

    //==============================================================
    // Getters and setters
    //==============================================================
    std::vector<IComponent::Pointer>& getRemotePlayerComponentList()   {    return remotePlayerComponentList;   } //Remote player component list getter
    std::vector<IComponent::Pointer>& getRemoteBananaComponentList()   {    return remoteBananaComponentList;   } //Remote player component list getter
    std::vector<IComponent::Pointer>& getRemoteRedShellComponentList() {    return remoteRedShellComponentList; } //Remote player component list getter
    std::vector<IComponent::Pointer>& getRemoteBlueSHellComponentList(){    return remoteBlueShellComponentList;} //Remote player component list getter
    void setPlayer(GameObject::Pointer p)                              {    player = p;                         };
    void setStarted(bool s)                                            {    started = s;                        };
    GameObject::Pointer getPlayer()                                    {    return player;                      };
    bool getStarted()                                                  {    return started;                     };

private:
    //==============================================================
    // Private data
    //==============================================================
    //RakPeerInterface, manages the connection
    RakNet::RakPeerInterface* peer;

    //MatchState started
    bool started;

    //Own player
    GameObject::Pointer player;

    //Own id
    //############################CAMBIAR A INT DE TIPO ESPECIFICO
    int server_id;

    //List of remotePlayerComponent
    std::vector<IComponent::Pointer> remotePlayerComponentList;

    //List of remotePlayerComponent
    std::vector<IComponent::Pointer> remoteBananaComponentList;

    //List of remotePlayerComponent
    std::vector<IComponent::Pointer> remoteRedShellComponentList;

    //List of remotePlayerComponent
    std::vector<IComponent::Pointer> remoteBlueShellComponentList;
};