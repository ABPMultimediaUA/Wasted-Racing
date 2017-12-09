#pragma once

#include "GameObject/GameObject.h"
#include "GameObject/ObjectRenderComponent.h"
#include "GameObject/InputComponent.h"
#include "GameObject/MoveComponent.h"
#include "GameManager/InputManager.h"
#include "GameManager/RenderManager.h"
#include "GameManager/ObjectManager.h"
#include "GameManager/PhysicsManager.h"
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

    //AudioFMOD* audioManager;

    //Selected renderEngine
    int renderEngine;
    //Selected inputEngine
    int inputEngine;

    //Stay in the main loop
    bool stay;

};

