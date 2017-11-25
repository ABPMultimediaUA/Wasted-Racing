#pragma once

#include "GameObject/GameObject.h"
#include "GameManager/InputManager.h"
#include "GameManager/RenderManager.h"
#include "GameManager/ObjectManager.h"
#include "GameEvent/EventManager.h"

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

    //Selected renderEngine
    int renderEngine;
    //Selected inputEngine
    int inputEngine;

    //Stay in the main loop
    bool stay;

};

