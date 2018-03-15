#pragma once

#include <iostream>
#include "../Game.h"

class Game;

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
    //ObjectManager
    ObjectManager* objectManager;
    //InputManager
    InputManager* inputManager;
    //EventManager
    EventManager* eventManager;

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