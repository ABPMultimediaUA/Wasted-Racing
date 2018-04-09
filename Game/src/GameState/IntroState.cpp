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
    objectManager = &ObjectManager::getInstance();
    audioManager = &AudioManager::getInstance();

    //Bind functions
    EventManager::getInstance().addListener(EventListener {EventType::Key_Multiplayer_Down, multiplayerActivated});   //hear for multiplayer selecting
    EventManager::getInstance().addListener(EventListener {EventType::Key_Singleplayer_Down, singleplayerActivated});   //hear for multiplayer selecting

    //Set an image on the main menu
    renderManager->getRenderFacade()->createRectangle2D(glm::vec2(renderManager->getRenderFacade()->getWindow().size.x/2-600, renderManager->getRenderFacade()->getWindow().size.y/2-331), "media/img/menuProv.png");
    renderManager->getRenderFacade()->drawGUI();

    Game::getInstance().setAccumulatedTime(0);

}

void IntroState::update(float &accumulatedTime) {
    //Update input manager
    inputManager->update();

    //Render manager update
    renderManager->update(accumulatedTime);
    audioManager->update();

    //Event manager has to be the last to be updated
    eventManager->update();

    Game::getInstance().setStay(objectManager->getGameRunning());
}

void IntroState::draw() {

    renderManager->draw();

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