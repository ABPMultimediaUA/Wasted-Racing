#pragma once

#include "GameObject/GameObject.h"
#include "GameObject/ObjectRenderComponent.h"
#include "GameObject/InputComponent.h"
#include "GameObject/MoveComponent.h"
#include "GameObject/RampComponent.h"
#include "GameManager/InputManager.h"
#include "GameManager/RenderManager.h"
#include "GameManager/ObjectManager.h"
#include "GameManager/PhysicsManager.h"
#include "GameManager/WaypointManager.h"
#include "GameManager/AIManager.h"
#include "GameObject/WaypointComponent.h"
#include "GameEvent/EventManager.h"
//#include "GameFacade/AudioFMOD.h"


class Game {

public: 

    //Constructor
    Game() {}

    //Destructor
    ~Game() {}

    //Infinite game loop
    void Run();

private:

    //==============================================================
    // Private functions
    //==============================================================

    //Initialization
    void init();

    //Update
    void update();

    //Render draw
    void draw();

    //Shutdown
    void close();

    //Engine setter
    void renderEngineSetter(int n){
        renderEngine = n;
    }

    //Engine setter
    void inputEngineSetter(int n){
        inputEngine = n;
    }


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
<<<<<<< HEAD

    //AudioFMOD* audioManager;
=======
    //Waypoint manager
    WaypointManager* waypointManager;
    //Audio FMOD manager
    AudioFMOD* audioManager;
    //AI manager
    AIManager* aiManager;
>>>>>>> 775bb5f88f34057146e4fa553547ab26f2f522c3

    //Selected renderEngine
    int renderEngine;
    //Selected inputEngine
    int inputEngine;

    //Stay in the main loop
    bool stay;

};

