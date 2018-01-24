#pragma once

#include "../GameObject/GameObject.h"
#include "../GameObject/NetworkComponent/RemotePlayerComponent.h"
#include <raknet/RakPeerInterface.h> 
#include <raknet/MessageIdentifiers.h>
#include <raknet/BitStream.h>
#include "../GameServer/CustomIdentifiers.h"
#include <iostream>

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

    //Create remote player
    IComponent::Pointer createRemotePlayerComponent(GameObject& newGameObject);

    //==============================================================
    // Getters and setters
    //==============================================================
    std::vector<IComponent::Pointer>& getRemotePlayerComponentList(){    return remotePlayerComponentList;} //Remote player component list getter
    void setPlayer(GameObject::Pointer p)                           {    player = p;                      };
    void setStarted(bool s)                                         {    started = s;                     };
    GameObject::Pointer getPlayer()                                 {    return player;                   };
    bool getStarted()                                               {    return started;                  };

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

};