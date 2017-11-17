#pragma once

#include <string>

#include "GameManager/InputManager.h"
#include "GameManager/RenderManager.h"

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
    void engineSetter(int n){
        engine = n;
    }

    //==============================================================
    // Private data
    //==============================================================

    //Input manager
    InputManager* inputManager;

    //Render manager
    RenderManager* renderManager;

    //Selected engine
    int engine;

    //Stay in the main loop
    bool stay;

};

