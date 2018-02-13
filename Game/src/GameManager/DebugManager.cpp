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
    renderManager  = &RenderManager::getInstance();

    //Add listeners
    EventManager::getInstance().addListener(EventListener {EventType::Key_DebugAI_Down,swapDebugAI});
    EventManager::getInstance().addListener(EventListener {EventType::Key_DebugBehaviour_Down, swapDebugBehaviour});
    EventManager::getInstance().addListener(EventListener {EventType::Key_DebugCamera_Down,swapDebugCamera});
    EventManager::getInstance().addListener(EventListener {EventType::Key_DebugNetwork_Down,swapDebugNetwork});

    //_______________TEST
    id_magica = -1;
    lol = 0;
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
// INITIALIZE DEBUGS
//==============================================
void DebugManager::initDebugNetwork()
{
    renderManager->createImage(glm::vec2(0,0), "media/img/green_rectangle.png");
    id_magica = renderManager->createRectangleColor(glm::vec2(200,200),glm::vec2(350,300), 255, 255, 0, 255);
    renderManager->createText(glm::vec2(200,200),"Qué pasa chavales");
}

//==============================================
// CLEAN DEBUGS
//==============================================
void DebugManager::cleanDebugNetwork()
{
    renderManager->cleanVI();
}

//==============================================
// UPDATE DEBUGS
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
                    //std::cout<<"Se ha movido otro jugador"<<std::endl;
                    break;

                case ID_BOX_COLLISION:
                    std::cout<<"Colisión con caja"<<std::endl;
                    break;
                
                case ID_CREATE_BANANA:
                    std::cout<<"Banana creada por: "<<std::endl;
                    break;

                case ID_DESTROY_BANANA:
                    std::cout<<"Banana destruida por: "<<std::endl;
                    break;
                
                case ID_CREATE_RED_SHELL:
                    std::cout<<"Caparazon rojo creado por: "<<std::endl;
                    break;

                case ID_DESTROY_RED_SHELL:
                    std::cout<<"Caparazon rojo destruido por: "<<std::endl;
                    break;

                case ID_CREATE_BLUE_SHELL:
                    std::cout<<"Caparazon azul creado por: "<<std::endl;
                    break;
                
                case ID_DESTROY_BLUE_SHELL:
                    std::cout<<"Caparazón azul destruido por: "<<std::endl;
                    break;

                case ID_REMOTE_RED_SHELL_MOVEMENT:
                    //std::cout<<"Caparazon rojo movido"<<std::endl;
                    break;

                case ID_REMOTE_BLUE_SHELL_MOVEMENT:
                    //std::cout<<"Caparazón azul movido"<<std::endl;
                    break;

                default:
                    std::cout << "Mensaje recibido" << std::endl;
                    break;
            }
        }
    }

    //_________________TEST
    lol++;
    if(lol>5000000){
        lol = 0;
    }
    renderManager->changeRectangleColor(id_magica, (lol)%256,(lol/2)%256,(lol/3)%256,255);
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
//Swaps AI debug state
void swapDebugAI(EventData eData)
{
    DebugManager::getInstance().setDebugAI(!DebugManager::getInstance().getDebugAI());
}

//Swaps Battle Behaviour debug state
void swapDebugBehaviour(EventData eData){
    DebugManager::getInstance().setDebugBehaviour(!DebugManager::getInstance().getDebugBehaviour());
}

//Swaps Camera debug state
void swapDebugCamera(EventData eData)
{
    DebugManager::getInstance().setDebugCamera(!DebugManager::getInstance().getDebugCamera());
}

//Swap Network debug state
void swapDebugNetwork(EventData eData){
    DebugManager::getInstance().setDebugNetwork(!DebugManager::getInstance().getDebugNetwork());
    
    //See if it shoulde be initialized or closed
    if(DebugManager::getInstance().getDebugNetwork()){
        DebugManager::getInstance().initDebugNetwork();
    }else{
        DebugManager::getInstance().cleanDebugNetwork();
    }
}
