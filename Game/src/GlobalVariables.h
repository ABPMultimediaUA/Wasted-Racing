#pragma once


//GLOBAL PROBLEMS
/*
    >RenderManager works in AIBattleComponent, even though not included. Change method when migrating to debug manager


*/
//////////////////

#include "GameState/IGameState.h"

class GlobalVariables{

    public:
        //Constructor
        GlobalVariables(){};

        //Destructor
        ~GlobalVariables() {};

        //static reference getter
        static GlobalVariables& getInstance(){
            static GlobalVariables instance;
            return instance;
        }

        //==========================================
        // GETTERS AND SETTERS
        //==========================================
        void setGameState(IGameState* t){ game = t;    };
        IGameState* getGameState()      { return game; };

    private:

        //==========================================
        // GLOBAL VARIABLES
        //==========================================
        IGameState* game; 
};