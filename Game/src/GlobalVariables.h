#pragma once

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
        void setGameState(IGameState* t){ 
                                            game = t;
                                            game->init();
                                        };
        IGameState* getGameState()      { return game; };

    private:

        //==========================================
        // GLOBAL VARIABLES
        //==========================================
        IGameState* game; 
};