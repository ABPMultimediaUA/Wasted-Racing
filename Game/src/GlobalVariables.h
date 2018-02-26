#pragma once

#include "GameState/IGameState.h"
#include "GameObject/GameObject.h"

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
        void setGameState(IGameState::stateType t){ game = t;       };
        IGameState::stateType getGameState()      { return game;    };
        void setPlayer(GameObject* p)             { player = p;     };
        GameObject* getPlayer()                   { return player;  };

    private:

        //==========================================
        // GLOBAL VARIABLES
        //==========================================
        IGameState::stateType game; 
        GameObject* player; 

        //==========================================
        // AI DATA
        //==========================================
        
};