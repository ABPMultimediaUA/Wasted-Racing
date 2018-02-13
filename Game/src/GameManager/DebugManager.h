#pragma once

#include "RenderManager.h"
#include "NetworkManager.h"
#include "../GameEvent/EventManager.h"
#include "../GameObject/InputComponent.h"
#include "../GameObject/NetworkComponent/DebugNetworkComponent.h"
#include "../GameServer/CustomIdentifiers.h"

class NetworkManager;

class DebugManager{
    public: 

        //Constructor
        DebugManager() {};

        //Destructor
        ~DebugManager() {};

        //Initialization
        void init();

        //Update
        void update();

        //Shutdown
        void close();

        //Static class getter
        static DebugManager& getInstance(){
            static DebugManager instance;
            return instance;
        };

        //DebugNetworkComponent creator
	    IComponent::Pointer createDebugNetworkComponent(GameObject& newGameObject);
    
        //==============================================
        // INITIALIZE DEBUGS
        //==============================================
        //Initialize network debugging
        void initDebugNetwork();

        //==============================================
        // CLEAN DEBUGS
        //==============================================
        //Erase all the elements that are part of the network debug
        void cleanDebugNetwork();

        
        //==============================================================
        // Getters and setters
        //==============================================================
        void setDebugAI       (bool s){ debugAI = s;           };
        void setDebugBehaviour(bool s){ debugBehaviour = s;    };
        void setDebugCamera   (bool s){ debugCamera = s;       };
        void setDebugNetwork  (bool s){ debugNetwork = s;      };
        bool getDebugAI       ()      { return debugAI;        };
        bool getDebugBehaviour()      { return debugBehaviour; };
        bool getDebugCamera   ()      { return debugCamera;    };
        bool getDebugNetwork  ()      { return debugNetwork;   };

    private:
        //==============================================================
        // Private data
        //==============================================================
        
        //VIGILS
        //Checks if debug AI is on
        bool debugAI;

        //Checks if debug of battle behaviour is on
        bool debugBehaviour;

        //Checks if camera debug is on
        bool debugCamera;

        //Checks if network debug is on
        bool debugNetwork;

        //MANAGERS
        NetworkManager* networkManager;
        RenderManager*  renderManager;

        //-----------------TEST
        int32_t id_magica;
        int lol;

        //==============================================
        // UPDATE DEBUGS
        //==============================================
        //Updates the AI debug
        void updateDebugAI();

        //Updates the battle behaviour debug
        void updateDebugBehaviour();

        //Updates the camera debug
        void updateDebugCamera();

        //Updates de network debug
        void updateDebugNetwork();
};