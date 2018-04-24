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

    //Initial arrangements
    Game::getInstance().setAccumulatedTime(0);
    
    //Turn scheduling off initially
    schedulingOn = false;
    schedulingClock = new Clock();
    schedulingClock->init();

    //Add AI's to the game
    addAI();

    //Key Bindings
    EventManager::getInstance().addListener(EventListener {EventType::Key_Scheduling_Down, swapSchedulingDelegate});
}

void MatchState::update(float &accumulatedTime) {
    //divide with ratio so we can accelerate or slow down the game
    accumulatedTime /= ratio;
    
    schedulingClock->restart();

    //Out of loop
    renderManager->update(accumulatedTime);

    renderTime = schedulingClock->getElapsedTime();

    //If time surpassed the loopTime
    if(accumulatedTime >= loopTime){
        //Update managers
    schedulingClock->restart();
        inputManager->update();

    inputTime = schedulingClock->getElapsedTime();

        if(ratio != 0)
        {
    schedulingClock->restart();
            physicsManager->update(accumulatedTime);
    physicsTime = schedulingClock->getElapsedTime();

    schedulingClock->restart();
            aiManager->update(accumulatedTime);
    aiTime = schedulingClock->getElapsedTime();

    schedulingClock->restart();
            waypointManager->update(accumulatedTime);
    waypointTime = schedulingClock->getElapsedTime();

    schedulingClock->restart();
            sensorManager->update();
    sensorTime = schedulingClock->getElapsedTime();

    schedulingClock->restart();
            itemManager->update(accumulatedTime);
    itemTime = schedulingClock->getElapsedTime();

    schedulingClock->restart();
            scoreManager->update();
    scoreTime = schedulingClock->getElapsedTime();

    schedulingClock->restart();
            audioManager->update();
    audioTime = schedulingClock->getElapsedTime();
        }

    schedulingClock->restart();
        //Event manager has to be the last to be updated
        eventManager->update();
    eventTime = schedulingClock->getElapsedTime();
        Game::getInstance().setStay(objectManager->getGameRunning());
        accumulatedTime = 0;
    }
    
    //AI Scheduling and timing
    double timePassed = schedulingClock->getElapsedTime();
    schedulingClock->restart();
    aiManager->updateScheduling(timePassed, loopTime);

    //Always interpolate
    interpolate(accumulatedTime);

    AIInterpolateTime = schedulingClock->getElapsedTime();
    //ESTE TESTEO ES EL SIDITA
    /*system("clear");
    std::cout<<"------------------"<<std::endl;
    std::cout<<"Render manager:   "<<renderTime*1000<<std::endl;
    std::cout<<"Input manager:    "<<inputTime*1000<<std::endl;
    std::cout<<"Physics manager:  "<<physicsTime*1000<<std::endl;
    std::cout<<"AI manager:       "<<aiTime*1000<<std::endl;
    std::cout<<"Waypoint manager: "<<waypointTime*1000<<std::endl;
    std::cout<<"Sensor manager:   "<<sensorTime*1000<<std::endl;
    std::cout<<"Item manager:     " <<itemTime*1000<<std::endl;
    std::cout<<"Audio manager:    "<<audioTime*1000<<std::endl;
    std::cout<<"Score manager:    "<<scoreTime*1000<<std::endl;
    std::cout<<"Event manager:    "<<eventTime*1000<<std::endl;
    std::cout<<"AInter manager:   "<<AIInterpolateTime*1000<<std::endl;
    std::cout<<"------------------"<<std::endl;*/
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