#include "MatchState.h"
#include "../Game.h"

//==============================================
// Delegate functions
//==============================================
void swapSchedulingDelegate(EventData data);

//==============================================
// Additional functions
//==============================================
void addAI();

//==============================================
// MAN FUNCTIONS
//==============================================
void MatchState::init() {

    if(initialized == false){

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

        //Turn scheduling off initially
        schedulingOn = false;
        schedulingClock = new Clock();
        schedulingClock->init();

        //Add AI's to the game
        addAI();

        //Key Bindings
        EventManager::getInstance().addListener(EventListener {EventType::Key_Scheduling_Down, swapSchedulingDelegate});

        //Set as initialized
        initialized = true;
    }

    Game::getInstance().setAccumulatedTime(0);
}

void MatchState::update(float &accumulatedTime) {
    //divide with ratio so we can accelerate or slow down the game
    accumulatedTime /= ratio;
    
    //Out of loop
    renderManager->update(accumulatedTime);

    //If time surpassed the loopTime
    if(accumulatedTime >= loopTime){
        //Update managers
        updateManagers(accumulatedTime);
        Game::getInstance().setStay(objectManager->getGameRunning());
        accumulatedTime = 0;
    }

    //AI Scheduling and timing
    double timePassed = schedulingClock->getElapsedTime();
    schedulingClock->restart();
    aiManager->updateScheduling(timePassed, loopTime);

    //Always interpolate
    interpolate(accumulatedTime);

}

void MatchState::updateManagers(float dTime){
    //Input manager has to be the first to be updated
    inputManager->update();

    if(ratio != 0)
    {
        physicsManager->update(dTime);

        aiManager->update(dTime);

        waypointManager->update(dTime);

        sensorManager->update();

        itemManager->update(dTime);
        
        scoreManager->update();

        audioManager->update();
    }

    //Event manager has to be the last to be updated
    eventManager->update();
}

void MatchState::draw() {
    renderManager->draw();
}

void MatchState::interpolate(float &accumulatedTime) {
    //Interpolate positions
    physicsManager->interpolate(accumulatedTime, loopTime);

    //Update each position in Render Manager
    for(unsigned int i=0; i<physicsManager->getMovingCharacterList().size(); ++i){
        //Interpolate every moving object
        RenderManager::getInstance().getRenderFacade()->updateObjectTransform(
                physicsManager->getMovingCharacterList()[i].moveComponent.get()->getGameObject().getId(), 
                physicsManager->getMovingCharacterList()[i].moveComponent.get()->getGameObject().getTransformData()
        );
    }

    //Update camera position
    renderManager->getRenderFacade()->interpolateCamera(accumulatedTime, loopTime);
}

void MatchState::close() {

}


//Additional functions
void addAI(){
    GameObject::TransformationData transform;
    uint16_t id;

    /*id = 25000;
    transform.position = glm::vec3(-35,0, -20);

    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, 1, 0, id, 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position).get()->getTerrain(), 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position));*/
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


//==============================================
// DELEGATE FUNCTIONS
//==============================================
void swapSchedulingDelegate(EventData data){
    MatchState::getInstance().swapScheduling();
}