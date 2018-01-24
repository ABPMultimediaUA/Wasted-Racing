#include "MatchState.h"

//additional functions
void addAI();

void MatchState::init() {

    audioManager    = &AudioManager::getInstance();     //Initialize true audio manager
    eventManager    = &EventManager::getInstance();     //Initilize event manager
    renderManager   = &RenderManager::getInstance();    //First we initialize renderManager, who creates a device and passes this reference to the inputManager
    inputManager    = &InputManager::getInstance();     //Once we've initialized the renderManager, we can do the same with our inputManager
    objectManager   = &ObjectManager::getInstance();    //Initialize object manager
    physicsManager  = &PhysicsManager::getInstance();   //Initialize physics manager
    waypointManager = &WaypointManager::getInstance();  //Initialize Waypoint Manager 
    aiManager       = &AIManager::getInstance();        //Initialize AI manager
    sensorManager   = &SensorManager::getInstance();    //Initialize Sensor manager
    itemManager     = &ItemManager::getInstance();      //Initialize Sensor manager
    scoreManager    = &ScoreManager::getInstance();     //Initialize Score Manager

    //Add AI's to the game
    //addAI();

    //Update event manager
    eventManager->update();
}

void MatchState::update(float &accumulatedTime) {
    //If time surpassed the loopTime
    if(accumulatedTime > loopTime){
        //Update managers
        updateManagers(accumulatedTime);

        Game::getInstance().setStay(objectManager->getGameRunning());
        accumulatedTime = 0;
    }
    //Always interpolate
    physicsManager->interpolate(accumulatedTime, loopTime);
    renderManager->getRenderFacade()->interpolateCamera(accumulatedTime, loopTime);

}

void MatchState::updateManagers(float dTime){
    //Input manager has to be the first to be updated
    inputManager->update();

    physicsManager->update(dTime);

    aiManager->update();

    renderManager->update();

    waypointManager->update(dTime);

    sensorManager->update();

    itemManager->update(dTime);
    
    scoreManager->update();

    audioManager->update();

    //Event manager has to be the last to be updated
    eventManager->update();
}

void MatchState::draw() {
    renderManager->draw();
}

void MatchState::close() {

}


//Additional functions
void addAI(){
    GameObject::TransformationData transform;
    uint16_t id;

    id = 25001;
    transform.position = glm::vec3(-35,0,-10);
    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, 0, 1, id, 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position).get()->getTerrain(), 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position));

    id = 25002;
    transform.position = glm::vec3(-35,0,0);
    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, 0, 1, id, 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position).get()->getTerrain(), 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position));

    id = 25003;
    transform.position = glm::vec3(-35,0,10);
    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, 0, 1, id, 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position).get()->getTerrain(), 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position));

    id = 25004;
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
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position));
}