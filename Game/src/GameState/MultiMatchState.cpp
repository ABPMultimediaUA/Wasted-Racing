#include "MultiMatchState.h"

//Additional functions
void createPlayer();

void MultiMatchState::init() {

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
    networkManager  = &NetworkManager::getInstance();   //Initialize Sensor manager
    debugManager    = &DebugManager::getInstance();     //Initialize Debug manager
    
    Game::getInstance().setAccumulatedTime(0);

    createPlayer();
}

void MultiMatchState::update(float &accumulatedTime) {

    //No gelding
    networkManager->update();
    debugManager->update();
    renderManager->update(accumulatedTime);
    
    //If time surpassed the loopTime
    if(accumulatedTime > loopTime){
        //Update managers
        updateManagers(accumulatedTime);
        Game::getInstance().setStay(objectManager->getGameRunning());
        accumulatedTime = 0;
    }

    //Always interpolate
    interpolate(accumulatedTime);
}

void MultiMatchState::updateManagers(float dTime){
    //Input manager has to be the first to be updated
        std::cout<<"Debug manual porque mira: 5555AAAAA"<<std::endl;
    inputManager->update();
        std::cout<<"Debug manual porque mira: 555BBBB"<<std::endl;

    physicsManager->update(dTime);
        std::cout<<"Debug manual porque mira: CCCC"<<std::endl;

    aiManager->update(dTime);
        std::cout<<"Debug manual porque mira: 555DDDDDD"<<std::endl;

    waypointManager->update(dTime);
        std::cout<<"Debug manual porque mira: 555EEEEE"<<std::endl;

    sensorManager->update();
        std::cout<<"Debug manual porque mira: 5555FFFF"<<std::endl;

    itemManager->update(dTime);
        std::cout<<"Debug manual porque mira: 555GGGGGA"<<std::endl;
    
    scoreManager->update();
        std::cout<<"Debug manual porque mira: 5555HHHHHHH"<<std::endl;

    audioManager->update();
        std::cout<<"Debug manual porque mira: 5555IIIIII"<<std::endl;
    
    //Event manager has to be the last to be updated
    eventManager->update();
        std::cout<<"Debug manual porque mira: 5555JJJJJJJJJ"<<std::endl;
}

void MultiMatchState::draw() {
    renderManager->draw();
}

void MultiMatchState::interpolate(float &accumulatedTime) {
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


void MultiMatchState::close() {

}

void createPlayer(){

}