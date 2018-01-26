#pragma once

#include "IGameState.h"
#include "MainState.h"
#include "../GameFacade/RenderIrrlicht.h"
#include "../GameManager/RenderManager.h"
#include "../Game.h"
#include "../GameManager/NetworkManager.h"
#include <iostream>

class Game;
class NetworkManager;
class SensorManager;

class ClientLobbyState : public IGameState {

public: 

    //Constructor
    ClientLobbyState (){ 
        type = IGameState::CLIENTLOBBY;
        fileName = "media/img/lobbyProv.png";
     };

    //Destructor
    virtual ~ClientLobbyState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update(float &accumulatedTime);

    //Drawer
    virtual void draw();

    //Shutdown
    virtual void close();

    //Static class getter
    static ClientLobbyState& getInstance() {
        static ClientLobbyState instance;
        return instance;
    };
private:
    //==============================================================
    // Private data
    //==============================================================

    //NetworkManager
    NetworkManager* networkManager;

    //RenderManager
    RenderManager* renderManager;

    //Update's loop time
    const float loopTime = 1.0f/30.0f;

    //JUST FOR THE TEST
    irr::gui::IGUIImage* img;
    std::string fileName;

    //Irrlicht own window
    irr::IrrlichtDevice* device;

    //Irrlicht scene manager
    irr::scene::ISceneManager* sceneManager;

    //Irrlicht video manager
    irr::video::IVideoDriver* videoDriver;
};