#include "SelectionState.h"

//==============================================
// Additional functions
//==============================================
void addAI();
void addStateChange(EventData eData);

void SelectionState::init() {
    //Bind all managers that are going to be used
    eventManager  = &EventManager::getInstance();
    inputManager  = &InputManager::getInstance();
    renderManager = &RenderManager::getInstance();
    objectManager = &ObjectManager::getInstance();
    audioManager = &AudioManager::getInstance();

    eventManager->addEvent(Event {Game_PlayerSelection});

    Game::getInstance().setAccumulatedTime(0);

    EventManager::getInstance().addListener(EventListener {EventType::Key_Pressed, addStateChange});
}

void SelectionState::update(float &accumulatedTime) {

    if(GlobalVariables::getInstance().getFixedPlayer()) {

        if(!load){
            EventManager::getInstance().addEvent(Event {EventType::Game_LoadingScreen});
            load = true;
        }
        else if (!GlobalVariables::getInstance().getGameLoaded()) {

            //Load map
            Game::getInstance().loadMap();

            //Add AI's to the game
            addAI();

            GlobalVariables::getInstance().setGameLoaded(true);

        }

    }

    

    

    //Update input manager
    inputManager->update();
    
    //Update audio manager
    audioManager->update();

    //Event manager has to be the last to be updated
    eventManager->update();

    //Sets if the game keeps running or not
    //:::>Change with event that closes the game
    Game::getInstance().setStay(objectManager->getGameRunning());
}

void SelectionState::draw() {
    renderManager->draw();
}

void SelectionState::close() {
    
}

//Additional functions
void addAI(){
    GameObject::TransformationData transform;
    uint16_t id;

    id = 25000;
    transform.position = glm::vec3(-35,0, -20);

    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, 0, 0, id, 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position).get()->getTerrain(), 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position));

    id = 25001;
    transform.position = glm::vec3(-35,0,-10);
    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, 1, 1, id, 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position).get()->getTerrain(), 
                                                 PhysicsManager::getInstance().getTerrainFromPos(transform.position));

    id = 25002;
    transform.position = glm::vec3(-35,0,0);
    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, 2, 1, id, 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position).get()->getTerrain(), 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position));

    id = 25003;
    transform.position = glm::vec3(-35,0,10);
    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, 3, 1, id, 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position).get()->getTerrain(), 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position));

    /*id = 25004;
    transform.position = glm::vec3(-35,0,20);
    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, 0, 1, id, 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position).get()->getTerrain(), 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position));

    id = 25005;
    transform.position = glm::vec3(-35,0,-30);
    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, 0, 1, id, 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position).get()->getTerrain(), 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position));

    id = 25006;
    transform.position = glm::vec3(-35,0,-40);
    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, 0, 1, id, 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position).get()->getTerrain(), 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position));*/
}


void addStateChange(EventData eData) {

    if(GlobalVariables::getInstance().getGameState() == IGameState::stateType::SELECTION && GlobalVariables::getInstance().getGameLoaded()){
        //Change state
        Game::getInstance().setState(IGameState::stateType::PREMATCH);
    }
    
}