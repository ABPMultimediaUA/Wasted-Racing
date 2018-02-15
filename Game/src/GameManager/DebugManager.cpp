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
    //Clean all debugs
    cleanDebugAI();
    cleanDebugBehaviour();
    cleanDebugCamera();
    cleanDebugNetwork();
}

//==============================================
// INITIALIZE DEBUGS
//==============================================

void DebugManager::initDebugAI()
{

}

void DebugManager::initDebugBehaviour()
{

}

void DebugManager::initDebugCamera()
{

}

void DebugManager::initDebugNetwork()
{
    //Auxiliar data
    int32_t auxIdText;
    int widthText = 250;
    int separationText = 20;
    int leftOffset = 5;
    int actualHeight = 10;
    int textr = 0, textg = 0, textb = 0, textalpha = 255;
    std::string textFont("media/font/Razor_m.png");

    //Last message received board illuminated and value
    idLastMessageBoard = renderManager->createRectangleColor(glm::vec2(0,0),glm::vec2(widthText * 2,400), 255, 255, 255, 200);

    //Generate message and push its id to the list, then generate spacing down below
    auxIdText = renderManager->createText("Last received server message", glm::vec2(leftOffset,actualHeight), textr-20, textg-255, textb-255, textalpha, glm::vec2(widthText * 2,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);
    actualHeight += separationText+5;

    auxIdText = renderManager->createText("Player created: ", glm::vec2(leftOffset,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);
    actualHeight += separationText;

    auxIdText = renderManager->createText("Box collision: ", glm::vec2(leftOffset,actualHeight),textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);
    actualHeight += separationText;

    auxIdText = renderManager->createText("Trap created: ", glm::vec2(leftOffset,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);
    actualHeight += separationText;

    auxIdText = renderManager->createText("Trap destroyed: ", glm::vec2(leftOffset,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);
    actualHeight += separationText;

    auxIdText = renderManager->createText("Tire created: ", glm::vec2(leftOffset,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);
    actualHeight += separationText;

    auxIdText = renderManager->createText("Tire destroyed: ", glm::vec2(leftOffset,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);
    actualHeight += separationText;
    
    auxIdText = renderManager->createText("Bomb created: ", glm::vec2(leftOffset,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);
    actualHeight += separationText;

    auxIdText = renderManager->createText("Bomb destroyed: ", glm::vec2(leftOffset,actualHeight), textr, textg, textb, textalpha, glm::vec2(widthText,separationText), textFont);
    idLastMessageTexts.push_back(auxIdText);
    actualHeight += separationText;

    //Set a default value to the last id message
    idLastMessageText = auxIdText;

    //Each data for each player: system address, server_id actual position

    //Each object on the map data: player creator (server_id), actual position 

    //renderManager->createImage("media/img/green_rectangle.png", glm::vec2(0,0));
    //id_magica = renderManager->createRectangleColor(glm::vec2(200,200),glm::vec2(350,300), 255, 255, 0, 255);
    //renderManager->createText("Qué pasa chavales", glm::vec2(200,200), 125,125,255, 255, glm::vec2(200,200), "media/font/D3Groovitmapism.png");
}

//==============================================
// CLEAN DEBUGS
//==============================================

void DebugManager::cleanDebugAI()
{
    renderManager->cleanVI();
}

void DebugManager::cleanDebugBehaviour()
{
    renderManager->cleanVI();
}

void DebugManager::cleanDebugCamera()
{
    renderManager->cleanVI();
}

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
                    std::cout<<"Game ended."<<std::endl;
                    break;

                case ID_CREATE_PLAYER:
                    setActiveLastMessage(idLastMessageTexts.at(1));
                    break;
                    
                case ID_CREATE_REMOTE_PLAYER:
                    setActiveLastMessage(idLastMessageTexts.at(1));
                    break;

                case ID_REMOTE_PLAYER_MOVEMENT:
                    break;

                case ID_BOX_COLLISION:
                    setActiveLastMessage(idLastMessageTexts.at(2));
                    break;
                
                case ID_CREATE_BANANA:
                    setActiveLastMessage(idLastMessageTexts.at(3));
                    break;

                case ID_DESTROY_BANANA:
                    setActiveLastMessage(idLastMessageTexts.at(4));
                    break;
                
                case ID_CREATE_RED_SHELL:
                    setActiveLastMessage(idLastMessageTexts.at(5));
                    break;

                case ID_DESTROY_RED_SHELL:
                    setActiveLastMessage(idLastMessageTexts.at(6));
                    break;

                case ID_CREATE_BLUE_SHELL:
                    setActiveLastMessage(idLastMessageTexts.at(7));
                    break;
                
                case ID_DESTROY_BLUE_SHELL:
                    setActiveLastMessage(idLastMessageTexts.at(8));
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
}

//==============================================
// NETWORK PARTICULAR FUNCTIONS
//==============================================
void DebugManager::setActiveLastMessage(uint32_t id){
    //Change color of last message to white
    renderManager->changeBackgroundColorText( idLastMessageText, 255,255,255,255);

    //Change color of new message to green
    renderManager->changeBackgroundColorText( id, 0,255,0,255);
    idLastMessageText = id;
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

    //See if it shoulde be initialized or closed
    if(DebugManager::getInstance().getDebugAI()){
        //Clean rest of debugs
        DebugManager::getInstance().cleanDebugCamera();
        DebugManager::getInstance().cleanDebugBehaviour();
        DebugManager::getInstance().cleanDebugNetwork();

        DebugManager::getInstance().initDebugAI();

        //Change to debug state
        RenderManager::getInstance().setDebugState(true);
    }else{
        DebugManager::getInstance().cleanDebugAI();

        //Close normal state
        RenderManager::getInstance().setDebugState(false);
    }
}

//Swaps Battle Behaviour debug state
void swapDebugBehaviour(EventData eData){
    DebugManager::getInstance().setDebugBehaviour(!DebugManager::getInstance().getDebugBehaviour());

    //See if it shoulde be initialized or closed
    if(DebugManager::getInstance().getDebugBehaviour()){
        //Clean rest of debugs
        DebugManager::getInstance().cleanDebugCamera();
        DebugManager::getInstance().cleanDebugNetwork();
        DebugManager::getInstance().cleanDebugAI();

        DebugManager::getInstance().initDebugBehaviour();

        //Change to debug state
        RenderManager::getInstance().setDebugState(true);

    }else{
        DebugManager::getInstance().cleanDebugBehaviour();

        //Change to normal state
        RenderManager::getInstance().setDebugState(false);
    }
}

//Swaps Camera debug state
void swapDebugCamera(EventData eData)
{
    DebugManager::getInstance().setDebugCamera(!DebugManager::getInstance().getDebugCamera());

    //See if it shoulde be initialized or closed
    if(DebugManager::getInstance().getDebugCamera()){
        //Clean rest of debugs
        DebugManager::getInstance().cleanDebugNetwork();
        DebugManager::getInstance().cleanDebugBehaviour();
        DebugManager::getInstance().cleanDebugAI();

        DebugManager::getInstance().initDebugCamera();

        //Change to debug state
        RenderManager::getInstance().setDebugState(true);
    }else{
        DebugManager::getInstance().cleanDebugCamera();

        //Change to normal state
        RenderManager::getInstance().setDebugState(false);
    }

}

//Swap Network debug state
void swapDebugNetwork(EventData eData){
    DebugManager::getInstance().setDebugNetwork(!DebugManager::getInstance().getDebugNetwork());
    
    //See if it shoulde be initialized or closed
    if(DebugManager::getInstance().getDebugNetwork()){
        //Clean rest of debugs
        DebugManager::getInstance().cleanDebugCamera();
        DebugManager::getInstance().cleanDebugBehaviour();
        DebugManager::getInstance().cleanDebugAI();

        DebugManager::getInstance().initDebugNetwork();
    }else{
        DebugManager::getInstance().cleanDebugNetwork();
    }
}
