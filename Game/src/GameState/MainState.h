#pragma once

#include <memory>
#include <iostream>
#include <rapidxml/rapidxml.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>

#include "../Game.h"

class Game;

class MainState : public IGameState {

public: 

    //Constructor
    MainState (){ type = IGameState::MAIN;  };

    //Destructor
    virtual ~MainState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update(float &accumulatedTime);

    //Update managers
    void updateManagers(float dTime);

    //Drawer
    virtual void draw();

    //Interpolate function
    void interpolate(float &accumulatedTime);
    
    //Shutdown
    virtual void close();

    //Static class getter
    static MainState& getInstance() {
        static MainState instance;
        return instance;
    };

private:
    
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
    //Waypoint manager
    WaypointManager* waypointManager;
    //Audio Manager
    AudioManager* audioManager;
    //AI manager
    AIManager* aiManager;
    //Sensor manager
    SensorManager* sensorManager;
    //Item manager
    ItemManager* itemManager;
    //Score manager
    ScoreManager* scoreManager;

    //Update's loop time
    const float loopTime = 1.0f/30.0f;

    //Irrlicht own window
    irr::IrrlichtDevice* device;

    //Irrlicht scene manager
    irr::scene::ISceneManager* sceneManager;

    //Irrlicht video manager
    irr::video::IVideoDriver* videoDriver;


};