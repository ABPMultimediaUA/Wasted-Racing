#include "DebugManager.h"

//////////////////////////////////////////////
//            THINGS TO DO HERE
//////////////////////////////////////////////
//////////////////////////////////////////////
/*
>Add lists of players connected, their server_id and game_id
>Add lists of functions that each sends and receives
>Add lists of calls and green in last server call
*/
//////////////////////////////////////////////
//////////////////////////////////////////////

//==============================================
// DELEGATE NAMESPACING
//==============================================
void swapDebugAI(EventData eData);
void swapDebugBehaviour(EventData eData);
void swapDebugCamera(EventData eData);
void swapDebugNetwork(EventData eData);

//==============================================
// MAIN FUNCTIONS
//==============================================
void DebugManager::init(){
    //Inital debugging is false
    debugAI        = false;
    debugBehaviour = false;
    debugCamera    = false;
    debugNetwork   = false;

    //Connection to the network manager (continuous access)
    networkManager = &NetworkManager::getInstance();

    //Add listeners
    EventManager::getInstance().addListener(EventListener {EventType::Key_DebugAI_Down,swapDebugAI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_DebugBehaviour_Down, swapDebugBehaviour});
    EventManager::getInstance().addListener(EventListener {EventType::Key_DebugCamera_Down,swapDebugCamera});
    EventManager::getInstance().addListener(EventListener {EventType::Key_DebugNetwork_Down,swapDebugNetwork});
}

void DebugManager::update(){
    //If a checker is on, update the debug given
    if(debugAI){
        updateDebugAI();
    }

    if(debugBehaviour){
        updateDebugBehaviour();
    }

    if(debugCamera){
        updateDebugCamera();
    }

    if(debugNetwork){
        updateDebugNetwork();
    }
}

void DebugManager::close(){

}

//==============================================
// PRIVATE FUNCTIONS
//==============================================

void DebugManager::updateDebugAI(){
    std::cout<<"Todo bien en Philadelphia"<<std::endl;
}

void DebugManager::updateDebugBehaviour(){
    std::cout<<"Todo bien en Memphis"<<std::endl;
}

void DebugManager::updateDebugCamera(){
    std::cout<<"Todo bien en Toronto"<<std::endl;
}

void DebugManager::updateDebugNetwork(){
    //Checks if player is connected to a server
    if(networkManager->getConnected()){
        //Lobby debug

        //Match debug
        if(networkManager->getStarted()){
            std::cout<<"_-_-_-_-_-_-_-_-_-_-_-_-_"<<std::endl;
            std::cout<<"Todo bien en Minneapolis"<<std::endl;
            //Switch the last packet info
            switch(networkManager->getLastPacket()){
                case ID_GAME_ENDED:
                    std::cout<<"El juego ha terminao"<<std::endl;
                    break;

                case ID_CREATE_PLAYER:
                    std::cout<<"Se crea al personaje principal"<<std::endl;
                    break;
                    
                case ID_CREATE_REMOTE_PLAYER:
                    std::cout<<"Se ha conectado otro jugador"<<std::endl;
                    break;

                case ID_REMOTE_PLAYER_MOVEMENT:
                    std::cout<<"Se ha movido otro jugador"<<std::endl;
                    break;

                case ID_BOX_COLLISION:
                    std::cout<<"Colisión con caja"<<std::endl;
                    break;

                default:
                    std::cout << "Mensaje recibido" << std::endl;
                    break;
            }
            std::cout<<"-_-_-_-_-_-_-_-_-_-_-_-_-"<<std::endl;
        }
    }
}

//==============================================
// MANAGING COMPONENTS
//==============================================

IComponent::Pointer createDebugNetworkComponent(GameObject& newGameObject){

    IComponent::Pointer component = std::make_shared<DebugNetworkComponent>(newGameObject);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    EventManager::getInstance().addEvent(Event {EventType::DebugNetworkComponent_Create, data});

    return component;
}


//==============================================
// DELEGATE FUNCTIONS
//==============================================
//Intializes AI debug
void swapDebugAI(EventData eData)
{
    DebugManager::getInstance().setDebugAI(!DebugManager::getInstance().getDebugAI());
}

//Intializes Battle Behaviour debug
void swapDebugBehaviour(EventData eData){
    DebugManager::getInstance().setDebugBehaviour(!DebugManager::getInstance().getDebugBehaviour());
}

//Intializes Camera debug
void swapDebugCamera(EventData eData)
{
    DebugManager::getInstance().setDebugCamera(!DebugManager::getInstance().getDebugCamera());
}

//Intializes Network debug
void swapDebugNetwork(EventData eData){
    DebugManager::getInstance().setDebugNetwork(!DebugManager::getInstance().getDebugNetwork());
}
