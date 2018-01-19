#pragma once

#include "GameState/IGameState.h"
#include "GameState/IntroState.h"
#include "GameState/MainState.h"
#include "GameState/MatchState.h"

class Game {

public: 

    //Constructor
    Game() {}

    //Destructor
    ~Game() {}

    //Infinite game loop
    void Run();

    //State setter
    void setState(IGameState* s){
        //state changer
        state = s;

        //everytime we change state, we must initialize its operating data
        state->init();
    }

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

    //Selected renderEngine
    int renderEngine;
    //Selected inputEngine
    int inputEngine;

    //Stay in the main loop
    bool stay;

    //current state
    IGameState* state;
};
