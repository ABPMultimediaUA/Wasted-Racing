#pragma once

#include <iostream>
#include "GameState/IGameState.h"
#include "GameState/IntroState.h"
#include "GameState/MainState.h"
#include "GameState/MatchState.h"
#include "GameState/ClientLobbyState.h"
#include "GameState/MultiMatchState.h"
#include "GameObject/GameObject.h"
#include "GameObject/RenderComponent/ObjectRenderComponent.h"
#include "GameObject/InputComponent.h"
#include "GameObject/PhysicsComponent/MoveComponent.h"
#include "GameObject/PhysicsComponent/RampComponent.h"
#include "GameObject/ItemComponent/ItemBoxComponent.h"
#include "GameObject/AIComponent/WaypointComponent.h"
#include "GameManager/InputManager.h"
#include "GameManager/RenderManager.h"
#include "GameManager/ObjectManager.h"
#include "GameManager/PhysicsManager.h"
#include "GameManager/WaypointManager.h"
#include "GameManager/AIManager.h"
#include "GameManager/SensorManager.h"
#include "GameManager/AudioManager.h"
#include "GameManager/ScoreManager.h"
#include "GameManager/ItemManager.h"
#include "GameManager/NetworkManager.h"
#include "GameEvent/EventManager.h"
#include "GameFacade/AudioFMOD.h"

#include <memory>
#include <iostream>
#include <rapidxml/rapidxml.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>

class Game {

public: 

    //Constructor
    Game() {}

    //Destructor
    ~Game() { Game::close(); }

    //Infinite game loop
    void Run();

    //State setter
    void setState(IGameState::stateType type);

    //Basic setters and getters
    void setStay(bool s)       {     stay = s;              }//Stay setter
    void setRenderEngine(int n){     renderEngine = n;      }//Engine setter
    void setInputEngine(int n) {     inputEngine = n;       }//Input setter
    int getRenderEngine()      {     return renderEngine;   }//Engine getter
    int getInputEngine()       {     return inputEngine;    }//Input getter

    //Static class getter
    static Game& getInstance() {
        static Game instance;
        return instance;
    };



private:

    //==============================================================
    // Private functions
    //==============================================================

    //Initialization
    void init();

    //Update
    void update(float dTime);

    //Render draw
    void draw();

    //Shutdown
    void close();

    //==============================================================
    // Private data
    //==============================================================
    //Object manager
    ObjectManager* objectManager;
    //Input manager
    InputManager* inputManager;
    //Render manager
    RenderManager* renderManager;
    //Event manager
    EventManager* eventManager;
    //Physics manager
    PhysicsManager* physicsManager;
    //Waypoint manager
    WaypointManager* waypointManager;
    //Audio Manager
    AudioManager* audioManager;
    //AI manager
    AIManager* aiManager;
    //Sensor manager
    SensorManager* sensorManager;
    //Item manager
    ItemManager* itemManager;
    //Score manager
    ScoreManager* scoreManager;
    //Network manager
    NetworkManager* networkManager;

    //Selected renderEngine
    int renderEngine;
    //Selected inputEngine
    int inputEngine;

    //Stay in the main loop
    bool stay;

    //current state
    IGameState* state;
};
