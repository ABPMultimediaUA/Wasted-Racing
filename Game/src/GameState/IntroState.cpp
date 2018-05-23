#include "IntroState.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

////////////////////
//Delegate functions
void multiplayerActivated(EventData eData);
void singleplayerActivated(EventData eData);

/////////////////
//Extra functions
//void introVideo();

//==============================================================
// MAIN FUNCTIONS
//==============================================================
void IntroState::init() {

    if(!initialized){
        //Bind all managers that are going to be used
        eventManager  = &EventManager::getInstance();
        inputManager  = &InputManager::getInstance();
        renderManager = &RenderManager::getInstance();
        objectManager = &ObjectManager::getInstance();
        audioManager = &AudioManager::getInstance();

        //Variables
        ip =  "192.168.0.1";

        //Bind functions
        EventManager::getInstance().addListener(EventListener {EventType::Key_Multiplayer_Down, multiplayerActivated});   //hear for multiplayer selecting
        EventManager::getInstance().addListener(EventListener {EventType::Key_Singleplayer_Down, singleplayerActivated});   //hear for multiplayer selecting

        //Set an image on the main menu
        background = renderManager->createImage( "media/img/menuProv.png", glm::vec2(renderManager->getRenderFacade()->getWindow().size.x/2-600, renderManager->getRenderFacade()->getWindow().size.y/2-331));

        initialized = true;
    
    }

        //Add selection  scene
        GameObject::TransformationData tr;
        tr.position = glm::vec3(0,-30,-8);
        tr.rotation = glm::vec3(0,0,0);
        tr.scale    = glm::vec3(1.4,1.4,1.4);
        GameObject::Pointer background = ObjectManager::getInstance().createObject(60000, tr);

        RenderManager::getInstance().createObjectRenderComponent(*background.get(),ObjectRenderComponent::Shape::Mesh, "background.obj");

        GameObject::TransformationData tr1;
        tr1.position = glm::vec3(-4,-30.5,0.5);
        tr1.rotation = glm::vec3(0,glm::half_pi<float>(),0);
        tr1.scale    = glm::vec3(0.3,0.3,0.3);
        GameObject::Pointer player1 = ObjectManager::getInstance().createObject(60001, tr1);


        RenderManager::getInstance().createAnimationRenderComponent(*player1.get(),"Punk/CharSelect/punkAnimation_000", 156, 0, "Punk/DefaultMaterial_Base_Color.png");

        GameObject::TransformationData tr2;
        tr2.position = glm::vec3(-4,-31,-6.3);
        tr2.rotation = glm::vec3(0,glm::half_pi<float>(),0);
        tr2.scale    = glm::vec3(0.3,0.3,0.3);
        GameObject::Pointer player2 = ObjectManager::getInstance().createObject(60002, tr2);

        RenderManager::getInstance().createAnimationRenderComponent(*player2.get(),"Crocodile/CharSelect/cocodrilaBonesSelect_000", 91, 0, "Crocodile/DefaultMaterial_Base_Color.png");

        GameObject::TransformationData tr3;
        tr3.position = glm::vec3(-4,-31,-12.5);
        tr3.rotation = glm::vec3(0,glm::half_pi<float>(),0);
        tr3.scale    = glm::vec3(0.22,0.22,0.22);
        GameObject::Pointer player3 = ObjectManager::getInstance().createObject(60003, tr3);

        RenderManager::getInstance().createAnimationRenderComponent(*player3.get(),"Cyborg/CharSelect/CyborgFINALAnimation_000", 183, 0, "Cyborg/DefaultMaterial_Base_Color.png");

        GameObject::TransformationData tr4;
        tr4.position = glm::vec3(-5,-29.2,-17.8);
        tr4.rotation = glm::vec3(0,glm::half_pi<float>(),0);
        tr4.scale    = glm::vec3(0.09,0.09,0.09);
        GameObject::Pointer player4 = ObjectManager::getInstance().createObject(60004, tr4);

        RenderManager::getInstance().createAnimationRenderComponent(*player4.get(),"Witch/CharSelect/bruja_000", 169, 0, "Witch/DefaultMaterial_Base_Color.png");

        GameObject::TransformationData tr5;
        tr5.position = glm::vec3(-100,-10,-9);
        tr5.rotation = glm::vec3(0,0,0);
        tr5.scale    = glm::vec3(1,1,1);
        GameObject::Pointer light = ObjectManager::getInstance().createObject(60005, tr5);

        RenderManager::getInstance().createLightRenderComponent(*light.get(),LightRenderComponent::Type::Point, 100); 
    
    Game::getInstance().setAccumulatedTime(0);

    GlobalVariables::getInstance().setIgnoreInput(true);
}

void IntroState::update(float &accumulatedTime) {

    EventData data;
    data.Component      = objectManager->getObject(60000).get()->getComponent<ObjectRenderComponent>();

    EventManager::getInstance().addEvent(Event {EventType::Music_Menu, data});
    
    //Update input manager
    inputManager->update();

    audioManager->update();

    //Event manager has to be the last to be updated
    eventManager->update();

    //Sets if the game keeps running or not
    //:::>Change with event that closes the game
    Game::getInstance().setStay(objectManager->getGameRunning());
}

void IntroState::draw() {
    renderManager->draw();
}

void IntroState::close() {
    renderManager->cleanVI();
}

//==============================================
// SINGLEPLAYER FUNCTIONS
//============================================== 
void IntroState::swapToMatch()
{
    //Close this state
    close();    
 
    //Initialize match state
    Game::getInstance().setState(IGameState::stateType::SELECTION);

}

//==============================================
// MULTIPLAYER FUNCTIONS
//==============================================
void IntroState::swapToClientLobby()
{
    //Load server
    execFileServer();

    //Close this state
    close();

    //Initalize networkManager here
   NetworkManager::getInstance().init();

    //Initialize Server IP
    NetworkManager::getInstance().setServerIP(ip);

    //Initialize client state
    Game::getInstance().setState(IGameState::stateType::CLIENTLOBBY);
}

void IntroState::execFileServer() {

    //Initialize reader
	std::ifstream fileReader;
	fileReader.open("Server.txt");

    if(!fileReader)
    {   
        std::cout<<"Couldn't open file"<<std::endl;
        return;
    }

    //Get the reader
    std::getline(fileReader,ip); // Saves the line in STRING.

    //Close reader
	fileReader.close();
}

//==============================================
// DELEGATES
//============================================== 
void multiplayerActivated(EventData eData) {

    //Change to client match state
    IntroState::getInstance().swapToClientLobby();
}
void singleplayerActivated(EventData eData) {
    //Change to match state
    IntroState::getInstance().swapToMatch();
}

//-----------------------------------
//-----------------------------------
//------------PROVISIONAL------------
//-----------------------------------
//-----------------------------------
/*void introVideo(){
	//Play intro video
	double currentSec = 0;

    char part1[] = "ffmpeg -i $PWD/media/video/logo.mp4 -ss 00:00:";
    char part2[] = " -vframes 1 $PWD/media/video/framelogo.bmp && y && echo";

    
	//loop for the full size of the video
	while(currentSec < 17.0){
        char * numbers = new char[7];   //numbers and decimals to put
        char * systemCall = new char[std::strlen(part1)+std::strlen(part2)+8];  //entire message

        if(currentSec<10.0){
            numbers[0] = '0';        //floor to unit
        }else{
            numbers = {'1',(char) (((int)currentSec%) % 10)} //obtain unit only
        }

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
}*/


/*void IntroState::execLocalServer() {
    //Initial variables
    std::array<char, 16> buffer;                                   //Buffer of result of ip (maximum 15 digits, 16 + \n)    
    std::shared_ptr<FILE> pipe(popen("hostname -I", "r"), pclose); //Initializing run of the program

    //See if order could be open
    if (!pipe)
    {
        throw std::runtime_error("Reading file failed.");
    }

    //Copy result in ip
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 16, pipe.get()) != nullptr)
            ip += buffer.data();
    }
}*/
