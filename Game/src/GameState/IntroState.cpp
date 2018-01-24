#include "IntroState.h"
#include <iostream>
#include <raknet/BitStream.h>
#include "../GameServer/CustomIdentifiers.h"

//Delegate functions
void multiplayerActivated(EventData eData);
void singleplayerActivated(EventData eData);

//Extra functions
//void introVideo();


void IntroState::init() {
    //Bind all managers that are going to be used
    eventManager  = &EventManager::getInstance();
    inputManager  = &InputManager::getInstance();
    renderManager = &RenderManager::getInstance();

    //Bind functions
    EventManager::getInstance().addListener(EventListener {EventType::Key_Multiplayer_Down, multiplayerActivated});   //hear for multiplayer selecting
    EventManager::getInstance().addListener(EventListener {EventType::Key_Singleplayer_Down, singleplayerActivated});   //hear for multiplayer selecting*/
    std::cout << "Attempting to connect to server" << std::endl;
    peer = RakNet::RakPeerInterface::GetInstance();
    RakNet::SocketDescriptor socket(0, 	0);
	socket.socketFamily = AF_INET;
    peer->Startup(1, &socket, 1);
    RakNet::ConnectionAttemptResult result;
    result = peer->Connect("127.0.0.1", 39017, 0, 0);

    if(result == RakNet::CONNECTION_ATTEMPT_STARTED)
    {
        std::cout << "Connection Attempt Started Correctly" << std::endl;
    }
}

void IntroState::update(float &accumulatedTime) {
    //Update key events
    inputManager->update();

    //Event manager has to be the last to be updated
    eventManager->update();
    /*
    std::cout<<"Whatsapp boys"<<std::endl;

    Game game = Game::getInstance();
    MainState main = MainState::getInstance();
    game.setState(&main);
*/
    RakNet::Packet* result;
    RakNet::BitStream stream;
    result = peer->Receive();
    if(result)
    {
        switch(result->data[0])
        {
            case ID_CONNECTION_REQUEST_ACCEPTED:
                std::cout << "Connection Accepted" << std::endl;
                stream.Write((unsigned char)ID_GAME_START);
                peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
                break;
            case ID_CONNECTION_ATTEMPT_FAILED:
                std::cout << "Connection Failed" << std::endl;
                break;
            default:
                break;
        }
    }


   
    eventManager->update();
}

void IntroState::draw() {

}

void IntroState::close() {
    
}

//==============================================
// DELEGATES
//============================================== 
void multiplayerActivated(EventData eData) {
    //Close this state
    IntroState::getInstance().close();

    //Initialize Client Lobby state a new
    Game::getInstance().setState(&ClientLobbyState::getInstance());
}
void singleplayerActivated(EventData eData) {
    //Close this state
    IntroState::getInstance().close();

    //Initialize main state again
    Game::getInstance().setState(&MainState::getInstance());
}
//-----------------------------------
//-----------------------------------
//------------PROVISIONAL------------
//-----------------------------------
//-----------------------------------
void introVideo(){
	//Play intro video
	double currentSec = 0;

    char part1[] = "ffmpeg -i $PWD/media/video/logo.mp4 -ss 00:00:";
    char part2[] = " -vframes 1 $PWD/media/video/framelogo.bmp && y && echo";

    
	//loop for the full size of the video
	while(currentSec < 17.0){
        char * numbers = new char[7];   //numbers and decimals to put
        char * systemCall = new char[std::strlen(part1)+std::strlen(part2)+8];  //entire message

        /*if(currentSec<10.0){
            numbers[0] = '0';        //floor to unit
        }else{
            numbers = {'1',(char) (((int)currentSec%) % 10)} //obtain unit only
        }*/

        //double now = (double) ( (int) (currentSec * 1000) ) / 1000.0; //4 decimals

        std::string str = std::to_string(currentSec);
        if(currentSec < 10.0){
            strcat(numbers, "0");
        }
        strcat(numbers, str.c_str() );

        
        strcat(systemCall,part1);
        strcat(systemCall,numbers);
        strcat(systemCall,part2);
        std::cout<<"numbers"<<numbers<<std::endl;
        std::cout<<"final "<<systemCall<<std::endl;
		system(systemCall);

        //addcurrentTime
        currentSec+=1/60.0;
	}
}