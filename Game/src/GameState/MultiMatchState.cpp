#include "MultiMatchState.h"

//Additional functions
void createPlayer();

void MultiMatchState::init() {

    audioManager    = &AudioManager::getInstance();     //Initialize true audio manager
    aiManager       = &AIManager::getInstance();        //Initialize AI manager
    eventManager    = &EventManager::getInstance();     //Initialize event manager
    renderManager   = &RenderManager::getInstance();    //First we initialize renderManager, who creates a device and passes this reference to the inputManager
    inputManager    = &InputManager::getInstance();     //Once we've initialized the renderManager, we can do the same with our inputManager
    objectManager   = &ObjectManager::getInstance();    //Initialize object manager
    waypointManager = &WaypointManager::getInstance();  //Initialize Waypoint Manager
    sensorManager   = &SensorManager::getInstance();    //Initialize Sensor manager
    physicsManager  = &PhysicsManager::getInstance();   //Initialize physics manager
    itemManager     = &ItemManager::getInstance();      //Initialize Sensor manager
    scoreManager    = &ScoreManager::getInstance();     //Initialize Score Manager
    networkManager  = &NetworkManager::getInstance();   //Initialize Sensor manager
    debugManager    = &DebugManager::getInstance();     //Initialize Debug manager
    
    Game::getInstance().setAccumulatedTime(0);

    //Initalize visual part
    renderManager->initHUD();
}

void MultiMatchState::update(float &accumulatedTime) {

    //No gelding
    inputManager->update();
    //___>
    debugManager->update();
    //<___
    
    //::::> La predicción de movimiento consiste en corregir los eventos anteriores por unos nuevos, en base a lo que te ha
    //llegado de la comunicacion con otros individuos. Cada player se encarga de analizar las físicas y la IA de sus propios objetos.
    //Se pasa esta información al resto de jugadores y estos actualizan los objetos foráneos y etc. Los problemas llegan con 
    //el retardo de la linea, y las interacciones físicas entre objetos de diferentes sesiones. Entonces el sistema físico tiene
    //que calcular lo que un juego normal para todos. Eso hasta ahí. Y la decisión de las IA's se puede calcular particularme, pero
    //las decisiones físicas tienen que ser individuales.

    //AHORA: el problema es el retardo.
    //SOLUCION: Cada evento que se manda online también lleva la hora del servidor de cuando se mandó. La hora del servidor
    //es común a todos los jugadores de la sesión. Con eso se comprueba q llamadas llegan primero. ¿pero q llamadas? Las simultáneas de colisión
    // y eso. Entonces el juego debe de tener un salvaguardado y q se compruebe con este "lag" de fondo, y diga si algo ha chocado o no,
    // dependiendo de quien haya registrado el evento antes. Esta decisión se puede revocar hasta 1 segundo después si se recibe
    //la llamada adecuada. Hacen falta denominadores comunes a los eventos o que se hagan las llamadas en los diferentes sistemas
    //en el momento en que ocurrió. Pero eso significaría llevar un tracking temporal de cada evento hasta un segundo. Con eso
    //podríamos insertar nuestro evento en cualquier parte del sistema y recalcular esos trozos por las partes a las q afecten.

    renderManager->update(accumulatedTime);

    //If time surpassed the loopTime
    if(accumulatedTime > loopTime){
        //Update managers
        updateManagers(accumulatedTime);
        Game::getInstance().setStay(objectManager->getGameRunning());
        accumulatedTime = 0;
    }

    //Do before interpolation, since it receives new positions that break the spot
    networkManager->update();

    //Always interpolate
    interpolate(accumulatedTime);
}

void MultiMatchState::updateManagers(float dTime){
    physicsManager->update(dTime);

    //aiManager->update(dTime);

    //waypointManager->update(dTime);

    sensorManager->update();

    itemManager->update(dTime);
    
    scoreManager->update();

    audioManager->update();    
    
    //Event manager has to be the last to be updated
    eventManager->update();
}

void MultiMatchState::draw() {
    renderManager->draw();
    //renderManager->drawHUD();
}

void MultiMatchState::interpolate(float &accumulatedTime) {
    //Interpolate position of objects
    physicsManager->interpolate(accumulatedTime,loopTime);

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