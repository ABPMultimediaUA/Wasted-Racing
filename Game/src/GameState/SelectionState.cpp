#include "SelectionState.h"

//==============================================
// Additional functions
//==============================================
void addAI(int selectedPlayer);
void addStateChange(EventData eData);

void SelectionState::init() {

    if(!initialized) {
        //Bind all managers that are going to be used
        eventManager  = &EventManager::getInstance();
        inputManager  = &InputManager::getInstance();
        renderManager = &RenderManager::getInstance();
        objectManager = &ObjectManager::getInstance();
        audioManager = &AudioManager::getInstance();

        EventManager::getInstance().addListener(EventListener {EventType::Key_Pressed, addStateChange});

        initialized = true;

        //Add selection  scene
        GameObject::TransformationData tr;
        tr.position = glm::vec3(0,-30,-8);
        tr.rotation = glm::vec3(0,0,0);
        tr.scale    = glm::vec3(2,2,2);
        GameObject::Pointer background = ObjectManager::getInstance().createObject(60000, tr);

        RenderManager::getInstance().createObjectRenderComponent(*background.get(),ObjectRenderComponent::Shape::Mesh, "background.obj");

        GameObject::TransformationData tr1;
        tr1.position = glm::vec3(-5,-30,0);
        tr1.rotation = glm::vec3(0,glm::pi<float>(),0);
        tr1.scale    = glm::vec3(0.2,0.2,0.2);
        GameObject::Pointer player1 = ObjectManager::getInstance().createObject(60001, tr1);

        RenderManager::getInstance().createObjectRenderComponent(*player1.get(),ObjectRenderComponent::Shape::Mesh, "punk.obj");

        GameObject::TransformationData tr2;
        tr2.position = glm::vec3(-5,-30,-6);
        tr2.rotation = glm::vec3(0,0,0);
        tr2.scale    = glm::vec3(0.35,0.35,0.35);
        GameObject::Pointer player2 = ObjectManager::getInstance().createObject(60002, tr2);

        RenderManager::getInstance().createObjectRenderComponent(*player2.get(),ObjectRenderComponent::Shape::Mesh, "croco.obj");

        GameObject::TransformationData tr3;
        tr3.position = glm::vec3(-5,-30,-12);
        tr3.rotation = glm::vec3(0,-glm::half_pi<float>(),0);
        tr3.scale    = glm::vec3(0.25,0.25,0.25);
        GameObject::Pointer player3 = ObjectManager::getInstance().createObject(60003, tr3);

        RenderManager::getInstance().createObjectRenderComponent(*player3.get(),ObjectRenderComponent::Shape::Mesh, "cyborg.obj");

        GameObject::TransformationData tr4;
        tr4.position = glm::vec3(-5,-30,-18);
        tr4.rotation = glm::vec3(0,glm::pi<float>(),0);
        tr4.scale    = glm::vec3(0.2,0.2,0.2);
        GameObject::Pointer player4 = ObjectManager::getInstance().createObject(60004, tr4);

        RenderManager::getInstance().createObjectRenderComponent(*player4.get(),ObjectRenderComponent::Shape::Mesh, "witch.obj");

        GameObject::TransformationData tr5;
        tr5.position = glm::vec3(-20,-10,-9);
        tr5.rotation = glm::vec3(0,0,0);
        tr5.scale    = glm::vec3(1,1,1);
        GameObject::Pointer light = ObjectManager::getInstance().createObject(60005, tr5);

        RenderManager::getInstance().createLightRenderComponent(*light.get(),LightRenderComponent::Type::Point, 100); 
    }

    eventManager->addEvent(Event {Game_PlayerSelection});

    Game::getInstance().setAccumulatedTime(0);

    GlobalVariables::getInstance().setIgnoreInput(true);

    cameraPositions[4] = glm::vec3(10,-27,0);
    cameraPositions[5] = glm::vec3(0,-30,0);

    RenderManager::getInstance().getRenderFacade()->setCameraTarget(cameraPositions[4], cameraPositions[5]);
    
    currPlayer = 0;
    ongoing = false;
    GlobalVariables::getInstance().setFixedPlayer(false);
}

void SelectionState::update(float &accumulatedTime) {

    int selectedPlayer = GlobalVariables::getInstance().getSelectedPlayer(); 

    if(GlobalVariables::getInstance().getFixedPlayer()) {

        if(!load){
            EventManager::getInstance().addEvent(Event {EventType::Game_LoadingScreen});
            load = true;
        }
        else if (!GlobalVariables::getInstance().getGameLoaded()) {

            //Load map
            Game::getInstance().loadMap();

            //Add AI's to the game
            addAI(selectedPlayer);

            GlobalVariables::getInstance().setGameLoaded(true);
            GlobalVariables::getInstance().setIgnoreInput(false);

        }
        else if(load && GlobalVariables::getInstance().getGameLoaded()){
            EventManager::getInstance().addEvent(Event {EventType::Game_LoadingScreen});
            GlobalVariables::getInstance().setIgnoreInput(false);
        }

    }

    if(!ongoing && currPlayer != selectedPlayer) {
        ongoing = true;
        currTime = maxTime;
        GlobalVariables::getInstance().setSelecting(true);
        if(currPlayer == 0 && selectedPlayer == 3) {
            cameraPositions[0] = glm::vec3(10,-27,0);
            cameraPositions[1] = glm::vec3(0,-30,0);
            cameraPositions[2] = glm::vec3(10,-27,-18);
            cameraPositions[3] = glm::vec3(0,-30,-18);
        }
        else if (currPlayer == 3 && selectedPlayer == 0) {
            cameraPositions[0] = glm::vec3(10,-27,-18);
            cameraPositions[1] = glm::vec3(0,-30,-18);
            cameraPositions[2] = glm::vec3(10,-27,0);
            cameraPositions[3] = glm::vec3(0,-30,0); 
        }
        else if (currPlayer > selectedPlayer) {
            cameraPositions[0] = cameraPositions[4];
            cameraPositions[1] = cameraPositions[5];
            cameraPositions[2] = glm::vec3(10,-27,cameraPositions[4].z + 6);
            cameraPositions[3] = glm::vec3(0,-30,cameraPositions[5].z + 6);
        }
        else if (currPlayer < selectedPlayer) {
            cameraPositions[0] = cameraPositions[4];
            cameraPositions[1] = cameraPositions[5];
            cameraPositions[2] = glm::vec3(10,-27,cameraPositions[4].z - 6);
            cameraPositions[3] = glm::vec3(0,-30,cameraPositions[5].z - 6);
        }
    }

    if(ongoing) {
        cameraPositions[4] = cameraPositions[0] + ((maxTime-currTime) * (cameraPositions[2]-cameraPositions[0]))/maxTime;
        cameraPositions[5] = cameraPositions[1] + ((maxTime-currTime) * (cameraPositions[3]-cameraPositions[1]))/maxTime;

        RenderManager::getInstance().getRenderFacade()->setCameraTarget(cameraPositions[4], cameraPositions[5]);
        
        currTime -= accumulatedTime;

        if(currTime < 0) {
            ongoing = false;
            currPlayer = GlobalVariables::getInstance().getSelectedPlayer();
            GlobalVariables::getInstance().setSelecting(false);
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
void addAI(int selectedPlayer){
    GameObject::TransformationData transform;
    uint16_t id;

    int pl1, pl2, pl3;

    if(selectedPlayer == 0) {
        pl1 = 1; pl2 = 2; pl3 = 3;
    } else if (selectedPlayer == 1) {
        pl1 = 0; pl2 = 2; pl3 = 3;
    } else if (selectedPlayer == 2) {
        pl1 = 0; pl2 = 1; pl3 = 3;
    } else {
        pl1 = 0; pl2 = 1; pl3 = 2;
    }


    id = 25000;
    transform.position = glm::vec3(-35,0, -20);

    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, selectedPlayer, 0, id, 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position).get()->getTerrain(), 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position));

    id = 25001;
    transform.position = glm::vec3(-35,0,-10);
    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, pl1, 1, id, 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position).get()->getTerrain(), 
                                                 PhysicsManager::getInstance().getTerrainFromPos(transform.position));

    id = 25002;
    transform.position = glm::vec3(-35,0,0);
    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, pl2, 1, id, 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position).get()->getTerrain(), 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position));

    id = 25003;
    transform.position = glm::vec3(-35,0,10);
    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, pl3, 1, id, 
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