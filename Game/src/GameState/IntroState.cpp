#include "IntroState.h"


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
    EventManager::getInstance().addListener(EventListener {EventType::Key_Singleplayer_Down, singleplayerActivated});   //hear for multiplayer selecting

    //Set an image on the main menu
    renderManager->getRenderFacade()->createRectangle2D(glm::vec2(renderManager->getRenderFacade()->getWindow().size.x/2-600, renderManager->getRenderFacade()->getWindow().size.y/2-331), "media/img/menuProv.png");
    renderManager->getRenderFacade()->drawGUI();

}

void IntroState::update(float &accumulatedTime) {
    //Update input manager
    inputManager->update();

    //Render manager update
    renderManager->update(accumulatedTime);

    //Event manager has to be the last to be updated
    eventManager->update();
}

void IntroState::draw() {

  //  renderManager->draw();

}

void IntroState::close() {
    
}

//==============================================
// DELEGATES
//============================================== 
void multiplayerActivated(EventData eData) {

    //Remove image
    auto renderManager = &RenderManager::getInstance();
    renderManager->getRenderFacade()->deleteRectangle2D();
    renderManager->getRenderFacade()->drawGUI();

    //Close this state
    IntroState::getInstance().close();

    //Initialize Client Lobby state a new
    Game::getInstance().setState(IGameState::stateType::CLIENTLOBBY);
}
void singleplayerActivated(EventData eData) {

    //Remove image
    auto renderManager = &RenderManager::getInstance();
    renderManager->getRenderFacade()->deleteRectangle2D();
    renderManager->getRenderFacade()->drawGUI();

    //Close this state
    IntroState::getInstance().close();

    //Initialize match state
    Game::getInstance().setState(IGameState::stateType::MATCH);
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