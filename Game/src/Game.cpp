#include "Game.h"

#include <memory>
#include <iostream>
#include <rapidxml/rapidxml.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <stdio.h>
 
#define SERVER_PORT 12345
#define MAX_CONNS 2000

void addObjects();
void loadMap();
std::vector<std::string> split(const std::string& s, const char& c);

//====================================================
//  GAME INITIALIZATION
//====================================================
void Game::init() {

    //Say game loop is active
    Game::stay = true;

    //Set engine to default
    Game::renderEngineSetter(0);
    Game::inputEngineSetter(0);

    //Initialize true audio manager
    audioManager = &AudioManager::getInstance();
    audioManager->init();

    //Initilize managers
    eventManager = &EventManager::getInstance();
    eventManager->init();

    //First we initialize renderManager, who creates a device and passes this reference to the inputManager
    renderManager = &RenderManager::getInstance();
    renderManager->init(Game::renderEngine);

    //Once we've initialized the renderManager, we can do the same with our inputManager
    inputManager = &InputManager::getInstance();
    inputManager->init(Game::inputEngine);

    //Initialize object manager
    objectManager = &ObjectManager::getInstance();
    objectManager->init();

    //Initialize physics manager
    physicsManager = &PhysicsManager::getInstance();
    physicsManager->init();

    //Initialize Waypoint Manager 
    waypointManager = &WaypointManager::getInstance();
    waypointManager->init();

    //Initialize AI manager
    aiManager = &AIManager::getInstance();
    aiManager->init();

    //Initialize Sensor manager
    sensorManager = &SensorManager::getInstance();
    sensorManager->init();

    //Initialize Sensor manager
    itemManager = &ItemManager::getInstance();
    itemManager->init();

    //Initialize Score Manager
    scoreManager = &ScoreManager::getInstance();
    scoreManager->init();

    addObjects();
}

//====================================================
//  GAME UPDATE
//====================================================
void Game::update(float dTime) {

    //Input manager has to be the first to be updated
    if(timeWait <= 0) 
    {
        inputManager->update();
        
        aiManager->update();
    }
    else
    {
        timeWait -= dTime;
    }
    physicsManager->update(dTime);

    renderManager->update();

    waypointManager->update(dTime);

    sensorManager->update();

    itemManager->update(dTime);
    
    scoreManager->update();

    audioManager->update();

    //Event manager has to be the last to be updated
    eventManager->update();


}

//====================================================
//  GAME DRAW
//====================================================
void Game::draw() {

    renderManager->draw();
}

//====================================================
//  GAME CLOSE
//====================================================
void Game::close() {

    physicsManager->close();
    renderManager->close();
    inputManager->close();
    eventManager->close();
    waypointManager->close();
    aiManager->close();
    sensorManager->close();
    itemManager->close();
    scoreManager->close();
}

//====================================================
//  GAME LOOP
//====================================================
void Game::Run() {

    Game::init();

    auto lastTime = std::chrono::high_resolution_clock::now();
    float accumulatedTime = 0;
    const float maxTime = 1.0f/60.0f;

    while(Game::stay){

        //Measure elapsed time
        auto currTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = currTime - lastTime;
        lastTime = currTime;

        accumulatedTime += (float)elapsed.count();

        //Update the game once every maxTime
        if(accumulatedTime > maxTime){
            Game::update(accumulatedTime);
            Game::stay = objectManager->getGameRunning();
            accumulatedTime = 0;
        }

        //Always interpolate and draw the game 
        physicsManager->interpolate(accumulatedTime, maxTime);
        renderManager->getRenderFacade()->updateCamera();

        Game::draw();
        
    }
    Game::close();
}


void addObjects(){

    //Send an event saying object has been created
    uint16_t id = 1;
    GameObject::TransformationData transform;
   
    //===============================================================
    // CREATE POSITIONED OBJECTS
    //===============================================================
    //Planes:
    id = 4;
    transform.position = glm::vec3(0,-3,0);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(50,0.01,20);
    auto ob1 = ObjectManager::getInstance().createObject(id, transform);

    id = 10;
    transform.position = glm::vec3(500,47,0);
    transform.rotation = glm::vec3(0,0,11.365);
    transform.scale    = glm::vec3(51,0.01,20);
    auto ob7 = ObjectManager::getInstance().createObject(id, transform);

    id = 15;
    transform.position = glm::vec3(875,97,-150);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(25,0.01,50);
    auto ob15 = ObjectManager::getInstance().createObject(id, transform);

    id = 16;
    transform.position = glm::vec3(800,97,-500);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(40,0.01,20);
    auto ob16 = ObjectManager::getInstance().createObject(id, transform);

    id = 17;
    transform.position = glm::vec3(550,87.25,-500);
    transform.rotation = glm::vec3(0,0,10.78252579);
    transform.scale    = glm::vec3(10.4,0.01,20);
    auto ob17 = ObjectManager::getInstance().createObject(id, transform);

    id = 18;
    transform.position = glm::vec3(125,77,-500);
    transform.rotation = glm::vec3(0,0,0);
    transform.scale    = glm::vec3(75,0.01,20);
    auto ob18 = ObjectManager::getInstance().createObject(id, transform);

    id = 19;
    transform.position = glm::vec3(-125,37,-250);
    transform.rotation = glm::vec3(14.94,0,0);
    transform.scale    = glm::vec3(25,0.01,31.05);
    auto ob19 = ObjectManager::getInstance().createObject(id, transform);

    //Start Line:
    id = 12;
    transform.position = glm::vec3(0,-2.85,-3);
    transform.rotation = glm::vec3(1.5,0,0);
    transform.scale    = glm::vec3(10,10,118);
    auto ob9 = ObjectManager::getInstance().createObject(id, transform);

    //Obstacles:
    /*id = 200;
    transform.position = glm::vec3(50,0,-30);
    transform.rotation = glm::vec3(45,45,0);
    transform.scale    = glm::vec3(1,1,1);
    auto ob200 = ObjectManager::getInstance().createObject(id, transform);*/

    //WAYPOINTS

    id = 100;
    transform.position = glm::vec3(-495, 1, -38);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob100 = ObjectManager::getInstance().createObject(id, transform);

    id = 101;
    transform.position = glm::vec3(-618, 1, -83);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob101 = ObjectManager::getInstance().createObject(id, transform);

    id = 102;
    transform.position = glm::vec3(-632, 1, -185);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob102 = ObjectManager::getInstance().createObject(id, transform);
    
    id = 103;
    transform.position = glm::vec3(-503, 1, -279);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob103 = ObjectManager::getInstance().createObject(id, transform);

    id = 104;
    transform.position = glm::vec3(-486, 1, -475);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob104 = ObjectManager::getInstance().createObject(id, transform);

    id = 105;
    transform.position = glm::vec3(-481, 1, -954);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob105 = ObjectManager::getInstance().createObject(id, transform);

    id = 106;
    transform.position = glm::vec3(-530, 1, -1079);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob106 = ObjectManager::getInstance().createObject(id, transform);

    id = 107;
    transform.position = glm::vec3(-411, 1, -1200);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob107 = ObjectManager::getInstance().createObject(id, transform);

    id = 108;
    transform.position = glm::vec3(-297, 1, -1268);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob108 = ObjectManager::getInstance().createObject(id, transform);

    id = 109;
    transform.position = glm::vec3(-114, 1, -1227);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob109 = ObjectManager::getInstance().createObject(id, transform);

    id = 110;
    transform.position = glm::vec3(156, 104, -774);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob110 = ObjectManager::getInstance().createObject(id, transform);

    id = 111;
    transform.position = glm::vec3(237, 131, -729);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob111 = ObjectManager::getInstance().createObject(id, transform);

    id = 112;
    transform.position = glm::vec3(343, 152, -753);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob112 = ObjectManager::getInstance().createObject(id, transform);

    id = 113;
    transform.position = glm::vec3(400, 165, -840);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob113 = ObjectManager::getInstance().createObject(id, transform);

    id = 114;
    transform.position = glm::vec3(396, 175, -912);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob114 = ObjectManager::getInstance().createObject(id, transform);

    id = 115;
    transform.position = glm::vec3(353, 183, -984);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob115 = ObjectManager::getInstance().createObject(id, transform);

    id = 116;
    transform.position = glm::vec3(267, 196, -1080);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob116 = ObjectManager::getInstance().createObject(id, transform);

    id = 117;
    transform.position = glm::vec3(219, 214, -1179);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob117 = ObjectManager::getInstance().createObject(id, transform);

    id = 118;
    transform.position = glm::vec3(219, 228, -1276);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob118 = ObjectManager::getInstance().createObject(id, transform);

    id = 119;
    transform.position = glm::vec3(270, 237, -1342);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob119 = ObjectManager::getInstance().createObject(id, transform);

    id = 120;
    transform.position = glm::vec3(366, 247, -1376);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob120 = ObjectManager::getInstance().createObject(id, transform);

    id = 121;
    transform.position = glm::vec3(564, 261, -1351);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob121 = ObjectManager::getInstance().createObject(id, transform);

    id = 122;
    transform.position = glm::vec3(667, 283, -1361);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob122 = ObjectManager::getInstance().createObject(id, transform);

    id = 123;
    transform.position = glm::vec3(759, 306, -1392);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob123 = ObjectManager::getInstance().createObject(id, transform);

    id = 124;
    transform.position = glm::vec3(840, 326, -1438);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob124 = ObjectManager::getInstance().createObject(id, transform);

    id = 125;
    transform.position = glm::vec3(923, 341, -1504);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob125 = ObjectManager::getInstance().createObject(id, transform);

    id = 126;
    transform.position = glm::vec3(1160, 344,-1733);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob126 = ObjectManager::getInstance().createObject(id, transform);

    id = 127;
    transform.position = glm::vec3(1241, 344,-1785);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob127 = ObjectManager::getInstance().createObject(id, transform);

    id = 128;
    transform.position = glm::vec3(1324, 344,-1808);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob128 = ObjectManager::getInstance().createObject(id, transform);

    id = 129;
    transform.position = glm::vec3(1445, 344,-1791);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob129 = ObjectManager::getInstance().createObject(id, transform);

    id = 130;
    transform.position = glm::vec3(1557, 344,-1716);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob130 = ObjectManager::getInstance().createObject(id, transform);

    id = 131;
    transform.position = glm::vec3(1610, 344,-1617);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob131 = ObjectManager::getInstance().createObject(id, transform);

    id = 132;
    transform.position = glm::vec3(1621, 344,-1512);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob132 = ObjectManager::getInstance().createObject(id, transform);

    id = 133;
    transform.position = glm::vec3(1585, 344,-1409);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob133 = ObjectManager::getInstance().createObject(id, transform);

    id = 134;
    transform.position = glm::vec3(1496, 344,-1306);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob134 = ObjectManager::getInstance().createObject(id, transform);

    id = 135;
    transform.position = glm::vec3(1257, 295,-1088);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob135 = ObjectManager::getInstance().createObject(id, transform);

    id = 136;
    transform.position = glm::vec3(604, 3,-498);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob136 = ObjectManager::getInstance().createObject(id, transform);

    id = 137;
    transform.position = glm::vec3(448, 1,-345);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob137 = ObjectManager::getInstance().createObject(id, transform);

    id = 138;
    transform.position = glm::vec3(407, 0,-280);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob138 = ObjectManager::getInstance().createObject(id, transform);

    id = 139;
    transform.position = glm::vec3(500, 0,-141);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob139 = ObjectManager::getInstance().createObject(id, transform);

    id = 140;
    transform.position = glm::vec3(510, 1,-62);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob140 = ObjectManager::getInstance().createObject(id, transform);

    id = 141;
    transform.position = glm::vec3(454, 0,13);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob141 = ObjectManager::getInstance().createObject(id, transform);

    id = 142;
    transform.position = glm::vec3(0, 0,0);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob142 = ObjectManager::getInstance().createObject(id, transform);


    //ItemBox:
    id = 25;
    transform.position = glm::vec3(175, 1, 40);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(0.5, 0.5, 0.5);
    auto ob25 = ObjectManager::getInstance().createObject(id, transform);

    id = 26;
    transform.position = glm::vec3(175, 1, 20);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(0.5, 0.5, 0.5);
    auto ob26 = ObjectManager::getInstance().createObject(id, transform);

    id = 27;
    transform.position = glm::vec3(175, 1, 0);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(0.5, 0.5, 0.5);
    auto ob27 = ObjectManager::getInstance().createObject(id, transform);

    id = 28;
    transform.position = glm::vec3(175, 1, -20);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(0.5, 0.5, 0.5);
    auto ob28 = ObjectManager::getInstance().createObject(id, transform);

    //////////////////////////

    id = 29;
    transform.position = glm::vec3(1068, 91, 1000);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(0.5, 0.5, 0.5);
    auto ob29 = ObjectManager::getInstance().createObject(id, transform);

    id = 30;
    transform.position = glm::vec3(1053, 90, 988);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(0.5, 0.5, 0.5);
    auto ob30 = ObjectManager::getInstance().createObject(id, transform);

    id = 31;
    transform.position = glm::vec3(1038, 89, 976);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(0.5, 0.5, 0.5);
    auto ob31 = ObjectManager::getInstance().createObject(id, transform);

    id = 32;
    transform.position = glm::vec3(1023, 88, 964);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(0.5, 0.5, 0.5);
    auto ob32 = ObjectManager::getInstance().createObject(id, transform);

    //////////////////////////

    id = 33;
    transform.position = glm::vec3(-120, -1, 492);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(0.5, 0.5, 0.5);
    auto ob33 = ObjectManager::getInstance().createObject(id, transform);

    id = 34;
    transform.position = glm::vec3(-135, 0, 480);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(0.5, 0.5, 0.5);
    auto ob34 = ObjectManager::getInstance().createObject(id, transform);

    id = 35;
    transform.position = glm::vec3(-150, 1, 468);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(0.5, 0.5, 0.5);
    auto ob35 = ObjectManager::getInstance().createObject(id, transform);

    id = 36;
    transform.position = glm::vec3(-165, 2, 456);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(0.5, 0.5, 0.5);
    auto ob36 = ObjectManager::getInstance().createObject(id, transform);

    //Circuit
    id = 37;
    transform.position = glm::vec3(0, -3, 0);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob37 = ObjectManager::getInstance().createObject(id, transform);

    //===============================================================
    // ADD WAYPOINT COMPONENT
    //===============================================================
    WaypointManager::getInstance().createWaypointComponent(ob141, 80, 0);
    WaypointManager::getInstance().createWaypointComponent(ob140, 80, 1);
    WaypointManager::getInstance().createWaypointComponent(ob139, 80, 2);
    WaypointManager::getInstance().createWaypointComponent(ob138, 80, 3);
    WaypointManager::getInstance().createWaypointComponent(ob137, 80, 4);
    WaypointManager::getInstance().createWaypointComponent(ob136, 100, 5);
    WaypointManager::getInstance().createWaypointComponent(ob135, 100, 6);
    WaypointManager::getInstance().createWaypointComponent(ob134, 80, 7);
    WaypointManager::getInstance().createWaypointComponent(ob133, 80, 8);
    WaypointManager::getInstance().createWaypointComponent(ob132, 80, 9);
    WaypointManager::getInstance().createWaypointComponent(ob131, 80, 10);
    WaypointManager::getInstance().createWaypointComponent(ob130, 80, 11);
    WaypointManager::getInstance().createWaypointComponent(ob129, 80, 12);
    WaypointManager::getInstance().createWaypointComponent(ob128, 80, 13);
    WaypointManager::getInstance().createWaypointComponent(ob127, 80, 14);
    WaypointManager::getInstance().createWaypointComponent(ob126, 80, 15);
    WaypointManager::getInstance().createWaypointComponent(ob125, 80, 16);
    WaypointManager::getInstance().createWaypointComponent(ob124, 80, 17);
    WaypointManager::getInstance().createWaypointComponent(ob123, 80, 18);
    WaypointManager::getInstance().createWaypointComponent(ob122, 80, 19);
    WaypointManager::getInstance().createWaypointComponent(ob121, 80, 20);
    WaypointManager::getInstance().createWaypointComponent(ob120, 80, 21);
    WaypointManager::getInstance().createWaypointComponent(ob119, 80, 22);
    WaypointManager::getInstance().createWaypointComponent(ob118, 80, 23);
    WaypointManager::getInstance().createWaypointComponent(ob117, 80, 24);
    WaypointManager::getInstance().createWaypointComponent(ob116, 80, 25);
    WaypointManager::getInstance().createWaypointComponent(ob115, 80, 26);
    WaypointManager::getInstance().createWaypointComponent(ob114, 80, 27);
    WaypointManager::getInstance().createWaypointComponent(ob113, 80, 28);
    WaypointManager::getInstance().createWaypointComponent(ob112, 80, 29);
    WaypointManager::getInstance().createWaypointComponent(ob111, 80, 30);
    WaypointManager::getInstance().createWaypointComponent(ob110, 80, 31);
    WaypointManager::getInstance().createWaypointComponent(ob109, 50, 32);
    WaypointManager::getInstance().createWaypointComponent(ob108, 100, 33);
    WaypointManager::getInstance().createWaypointComponent(ob107, 80, 34);
    WaypointManager::getInstance().createWaypointComponent(ob106, 80, 35);
    WaypointManager::getInstance().createWaypointComponent(ob105, 80, 36);
    WaypointManager::getInstance().createWaypointComponent(ob104, 80, 37);
    WaypointManager::getInstance().createWaypointComponent(ob103, 80, 38);
    WaypointManager::getInstance().createWaypointComponent(ob102, 80, 39);
    WaypointManager::getInstance().createWaypointComponent(ob101, 80, 40);
    WaypointManager::getInstance().createWaypointComponent(ob100, 80, 41);
    WaypointManager::getInstance().createWaypointComponent(ob142, 80, 42);

    //===============================================================
    // CREATE RENDER COMPONENTS
    //===============================================================
    //Planes
    /*
    std::shared_ptr<IComponent> cp1 = RenderManager::getInstance().createObjectRenderComponent(*ob1.get(), ObjectRenderComponent::Shape::Cube);

    std::shared_ptr<IComponent> cp6 = RenderManager::getInstance().createObjectRenderComponent(*ob7.get(), ObjectRenderComponent::Shape::Cube);

    std::shared_ptr<IComponent> cp12 = RenderManager::getInstance().createObjectRenderComponent(*ob15.get(), ObjectRenderComponent::Shape::Cube);

    std::shared_ptr<IComponent> cp13 = RenderManager::getInstance().createObjectRenderComponent(*ob16.get(), ObjectRenderComponent::Shape::Cube);

    std::shared_ptr<IComponent> cp14 = RenderManager::getInstance().createObjectRenderComponent(*ob17.get(), ObjectRenderComponent::Shape::Cube);

    std::shared_ptr<IComponent> cp15 = RenderManager::getInstance().createObjectRenderComponent(*ob18.get(), ObjectRenderComponent::Shape::Cube);

    std::shared_ptr<IComponent> cp16 = RenderManager::getInstance().createObjectRenderComponent(*ob19.get(), ObjectRenderComponent::Shape::Cube);
    */

    //StartLine
    //std::shared_ptr<IComponent> cp9 = RenderManager::getInstance().createObjectRenderComponent(*ob9.get(), ObjectRenderComponent::Shape::StarLine);

    //Road
    std::shared_ptr<IComponent> cp17 = RenderManager::getInstance().createObjectRenderComponent(*ob37.get(), ObjectRenderComponent::Shape::Road);

    //Obstacles
    //std::shared_ptr<IComponent> cp200 = RenderManager::getInstance().createObjectRenderComponent(*ob200.get(), ObjectRenderComponent::Shape::Sphere);



    //===============================================================
    // ADD AN INPUT COMPONENT TO THE FIRST OBJECT
    //===============================================================

    //===============================================================
    // ADD AN INPUT COMPONENT TO THE AI OBJECTS
    //===============================================================


    //===============================================================
    // ADD A CAMERA COMPONENT TO THE FIRST OBJECT
    //===============================================================

    //===============================================================
    // ADD COLLISION COMPONENTS TO ALL OBJECTS
    //===============================================================
    //std::shared_ptr<IComponent> collisionCP3 = PhysicsManager::getInstance().createCollisionComponent(*ob5.get(), 5, false, CollisionComponent::Type::Default);
    //std::shared_ptr<IComponent> collisionCP4 = PhysicsManager::getInstance().createCollisionComponent(*ob8.get(), 5, true, CollisionComponent::Type::Default);
    
    //std::shared_ptr<IComponent> collisionCP200 = PhysicsManager::getInstance().createCollisionComponent(*ob200.get(), 5, true, CollisionComponent::Type::Default);


    //Start Line
    LAPAL::plane3f terrainX;
    terrainX.p1 = (LAPAL::vec3f( -5 , 0, 55));
    terrainX.p2 = (LAPAL::vec3f( 5 , 0, 55));
    terrainX.p3 = (LAPAL::vec3f( 5 , 0, -63));
    terrainX.p4 = (LAPAL::vec3f( -5 , 0, -63));
    std::shared_ptr<IComponent> collisionCP5 = PhysicsManager::getInstance().createCollisionComponent(*ob9.get(), terrainX, false, CollisionComponent::Type::StartLine);
    

    //===============================================================
    // ADD TERRAIN COMPONENT
    //===============================================================
    /*EventManager::getInstance().update();
    LAPAL::plane3f terrain;
    terrain.p1 = (LAPAL::vec3f(-250.f ,0.f ,100.f));
    terrain.p2 = (LAPAL::vec3f(250.f  ,0.f ,100.f));
    terrain.p3 = (LAPAL::vec3f(250.f  ,0.f ,-100.f));
    terrain.p4 = (LAPAL::vec3f(-250.f ,0.f ,-100.f));
    terrain.fric = 0.2;
    terrain.direction = 0;
    LAPAL::calculateRotationsXZ(terrain);
    std::shared_ptr<IComponent> terrainCP1 = PhysicsManager::getInstance().createTerrainComponent(*ob1.get(), terrain);


    LAPAL::plane3f terrain1;
    terrain1.p1 = (LAPAL::vec3f(250,0,100));
    terrain1.p2 = (LAPAL::vec3f(750,100,100));
    terrain1.p3 = (LAPAL::vec3f(750,100,-100));
    terrain1.p4 = (LAPAL::vec3f(250,0,-100));
    terrain1.fric = 0.2;
    terrain1.direction = 0;
    LAPAL::calculateRotationsXZ(terrain1);
    std::shared_ptr<IComponent> terrainCP2 = PhysicsManager::getInstance().createTerrainComponent(*ob7.get(), terrain1);
    auto terrainCP2_0 = std::dynamic_pointer_cast<TerrainComponent>(terrainCP2);
    uint16_t idd = 4;
    terrainCP2_0.get()->connectSiblingBack(idd);

    LAPAL::plane3f terrain2;
    terrain2.p1 = (LAPAL::vec3f(750,100,100));
    terrain2.p2 = (LAPAL::vec3f(1000,100,100));
    terrain2.p3 = (LAPAL::vec3f(1000,100,-400));
    terrain2.p4 = (LAPAL::vec3f(750,100,-400));
    terrain2.fric = 0.2;
    terrain2.direction = 1;
    LAPAL::calculateRotationsXZ(terrain2);
    std::shared_ptr<IComponent> terrainCP3 = PhysicsManager::getInstance().createTerrainComponent(*ob15.get(), terrain2);
    auto terrainCP3_0 = std::dynamic_pointer_cast<TerrainComponent>(terrainCP3);
    idd = 10;
    terrainCP3_0.get()->connectSiblingBack(idd);

    LAPAL::plane3f terrain3;
    terrain3.p1 = (LAPAL::vec3f(600,100,-400));
    terrain3.p2 = (LAPAL::vec3f(1000,100,-400));
    terrain3.p3 = (LAPAL::vec3f(1000,100,-600));
    terrain3.p4 = (LAPAL::vec3f(600,100,-600));
    terrain3.fric = 0.2;
    terrain3.direction = 2;
    LAPAL::calculateRotationsXZ(terrain3);
    std::shared_ptr<IComponent> terrainCP4 = PhysicsManager::getInstance().createTerrainComponent(*ob16.get(), terrain3);
    auto terrainCP4_0 = std::dynamic_pointer_cast<TerrainComponent>(terrainCP4);
    idd = 15;
    terrainCP4_0.get()->connectSiblingLeft(idd);

    LAPAL::plane3f terrain4;
    terrain4.p1 = (LAPAL::vec3f(500,80,-400));
    terrain4.p2 = (LAPAL::vec3f(600,100,-400));
    terrain4.p3 = (LAPAL::vec3f(600,100,-600));
    terrain4.p4 = (LAPAL::vec3f(500,80,-600));
    terrain4.fric = 0.2;
    terrain4.direction = 2;
    LAPAL::calculateRotationsXZ(terrain4);
    std::shared_ptr<IComponent> terrainCP5 = PhysicsManager::getInstance().createTerrainComponent(*ob17.get(), terrain4);
    auto terrainCP5_0 = std::dynamic_pointer_cast<TerrainComponent>(terrainCP5);
    idd = 16;
    terrainCP5_0.get()->connectSiblingFront(idd);

    LAPAL::plane3f terrain5;
    terrain5.p1 = (LAPAL::vec3f(-250,80,-400));
    terrain5.p2 = (LAPAL::vec3f(500,80,-400));
    terrain5.p3 = (LAPAL::vec3f(500,80,-600));
    terrain5.p4 = (LAPAL::vec3f(-250,80,-600));
    terrain5.fric = 0.5;
    terrain5.direction = 2;
    LAPAL::calculateRotationsXZ(terrain5);
    std::shared_ptr<IComponent> terrainCP6 = PhysicsManager::getInstance().createTerrainComponent(*ob18.get(), terrain5);
    auto terrainCP6_0 = std::dynamic_pointer_cast<TerrainComponent>(terrainCP6);
    idd = 17;
    terrainCP6_0.get()->connectSiblingFront(idd);

    LAPAL::plane3f terrain6;
    terrain6.p1 = (LAPAL::vec3f(-250,0,-100));
    terrain6.p2 = (LAPAL::vec3f(0,0,-100));
    terrain6.p3 = (LAPAL::vec3f(0,80,-400));
    terrain6.p4 = (LAPAL::vec3f(-250,80,-400));
    terrain6.fric = 0.2;
    terrain6.direction = 3;
    LAPAL::calculateRotationsXZ(terrain6);
    std::shared_ptr<IComponent> terrainCP7 = PhysicsManager::getInstance().createTerrainComponent(*ob19.get(), terrain6);
    auto terrainCP7_0 = std::dynamic_pointer_cast<TerrainComponent>(terrainCP7);
    idd = 18;
    terrainCP7_0.get()->connectSiblingRight(idd);
    idd = 4;
    terrainCP7_0.get()->connectSiblingLeft(idd);*/

    

    //===============================================================
    // ADD RAMP COMPONENT
    //===============================================================
    //std::shared_ptr<IComponent> rampCP = PhysicsManager::getInstance().createRampComponent(*ob9.get(), 250.0f,2.f,2.0f);


    //===============================================================
    // ADD PLAYER 
    //===============================================================
    loadMap();

    auto terrainCMP = ObjectManager::getInstance().getObject((uint16_t) 1090).get()->getComponent<TerrainComponent>();
    auto terrCMP = terrainCMP.get()->getTerrain();

    id = 50;
    transform.position = glm::vec3(-20,10, 10);
    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, 1, 0, id, terrCMP, terrainCMP);

    //===============================================================
    // ADD AI 
    //===============================================================

    /*id = 55;
    transform.position = glm::vec3(-20,10,-30);
    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, 0, 1, id, terrCMP, terrainCMP);*/

    /*id = 56;
    transform.position = glm::vec3(-125,0,5);
    transform.rotation = glm::vec3(45,45,45);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, 1, 1, id, terrain, terrainCP1);*/

    //===============================================================
    // ADD AI 
    //===============================================================




    //===============================================================
    // ADD ITEMBOX COMPONENT
    //===============================================================
    std::shared_ptr<IComponent> itemboxCP20 = ItemManager::getInstance().createItemBox(*ob25.get());
    std::shared_ptr<IComponent> itemboxCP21 = ItemManager::getInstance().createItemBox(*ob26.get());
    std::shared_ptr<IComponent> itemboxCP22 = ItemManager::getInstance().createItemBox(*ob27.get());
    std::shared_ptr<IComponent> itemboxCP23 = ItemManager::getInstance().createItemBox(*ob28.get());

    std::shared_ptr<IComponent> itemboxCP24 = ItemManager::getInstance().createItemBox(*ob29.get());
    std::shared_ptr<IComponent> itemboxCP25 = ItemManager::getInstance().createItemBox(*ob30.get());
    std::shared_ptr<IComponent> itemboxCP26 = ItemManager::getInstance().createItemBox(*ob31.get());
    std::shared_ptr<IComponent> itemboxCP27 = ItemManager::getInstance().createItemBox(*ob32.get());

    std::shared_ptr<IComponent> itemboxCP28 = ItemManager::getInstance().createItemBox(*ob33.get());
    std::shared_ptr<IComponent> itemboxCP29 = ItemManager::getInstance().createItemBox(*ob34.get());
    std::shared_ptr<IComponent> itemboxCP30 = ItemManager::getInstance().createItemBox(*ob35.get());
    std::shared_ptr<IComponent> itemboxCP31 = ItemManager::getInstance().createItemBox(*ob36.get());

    //===============================================================
    // Update to distribute all creation events
    //===============================================================
    EventManager::getInstance().update();

    //===============================================================
    // Split renderManager static QuadTree
    //===============================================================
    RenderManager::getInstance().splitQuadTree();
    //RenderManager::getInstance().getComponentTree().debugStructure(1);
    
}

std::vector<std::string> split(const std::string& s, const char& c) {
	std::string buff{""};
	std::vector<std::string> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

void loadMap() {

    using namespace rapidxml;

    xml_document<> doc;
    xml_node<> * root_node;

    //Read the file and put it into a char array
    std::ifstream theFile("media/xml/circuit.xml");
	std::string buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	// Parse the buffer using the xml file parsing library into doc 
	doc.parse<0>(&buffer[0]);

    // Find our root node
	root_node = doc.first_node("object");

    // Iterate over the gameObjects, reading them
    for (xml_node<> * object = root_node; object; object = object->next_sibling()) {

        uint16_t id;
        GameObject::TransformationData transform;

        //Read ID from XML
        id = (uint16_t) std::stoi(object->first_attribute("id")->value());
        //Read POSITION from XML
        auto strVector = split(object->first_attribute("pos")->value(), ',');
        transform.position = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
        //Read ROTATION from XML
        strVector = split(object->first_attribute("rot")->value(), ',');
        transform.rotation = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
        //Read SCALE from XML
        strVector = split(object->first_attribute("rot")->value(), ',');
        transform.scale = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));

        //Create new OBJECT
        auto obj = ObjectManager::getInstance().createObject(id, transform);


        //Parse components
        xml_node<> * root_component = object->first_node("component");

        for (xml_node<> * component = root_component; component; component = component->next_sibling()){

            //Parse TERRAIN component
            if(strcmp(component->first_attribute("name")->value(),"terrain") == 0){

                xml_node<>* bbox = component->first_node("bbox");

                LAPAL::plane3f terrain;
                //Read P1 from XML
                auto strVector = split(bbox->first_attribute("p1")->value(), ',');
                terrain.p1 = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
                //Read P2 from XML
                strVector = split(bbox->first_attribute("p2")->value(), ',');
                terrain.p2 = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
                //Read P3 from XML
                strVector = split(bbox->first_attribute("p3")->value(), ',');
                terrain.p3 = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
                //Read P4 from XML
                strVector = split(bbox->first_attribute("p4")->value(), ',');
                terrain.p4 = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
                //Read FRICTION from XML
                terrain.fric = std::stof(bbox->first_attribute("friction")->value());
                //Calculate terrain angles in X and Z
                //LAPAL::calculateRotationsXZ(terrain);

                //Create TERRAIN component
                PhysicsManager::getInstance().createTerrainComponent(*obj.get(), terrain);

            }

        }
	}

    //Update every thing that has been created
    EventManager::getInstance().update();

    //Loop over terrain components, linking them
    for (xml_node<> * object = root_node; object; object = object->next_sibling()) {

        //Get current object ID
        uint16_t id = (uint16_t) std::stoi(object->first_attribute("id")->value());

        //Parse components
        xml_node<> * root_component = object->first_node("component");

        for (xml_node<> * component = root_component; component; component = component->next_sibling()){

            //Parse TERRAIN components
            if(strcmp(component->first_attribute("name")->value(),"terrain") == 0){

                //Get our component
                std::shared_ptr<TerrainComponent> cmp = ObjectManager::getInstance().getObject(id).get()->getComponent<TerrainComponent>();

                //If not null, connect our component with next object
                uint16_t connectID = (uint16_t) std::stoi(split(component->first_attribute("l0")->value(), ':')[0]);

                if( connectID > 65000 )
                    cmp.get()->setNext(nullptr);
                else 
                    cmp.get()->setNext(ObjectManager::getInstance().getObject(connectID).get()->getComponent<TerrainComponent>());

                //If not null, connect our component with right object
                connectID = (uint16_t) std::stoi(split(component->first_attribute("l1")->value(), ':')[0]);

                if( connectID > 65000 )
                    cmp.get()->setRight(nullptr);
                else 
                    cmp.get()->setRight(ObjectManager::getInstance().getObject(connectID).get()->getComponent<TerrainComponent>());

                //If not null, connect our component with prev object
                connectID = (uint16_t) std::stoi(split(component->first_attribute("l2")->value(), ':')[0]);

                if( connectID > 65000 )
                    cmp.get()->setPrev(nullptr);
                else 
                    cmp.get()->setPrev(ObjectManager::getInstance().getObject(connectID).get()->getComponent<TerrainComponent>());

                //If not null, connect our component with left object
                connectID = (uint16_t) std::stoi(split(component->first_attribute("l3")->value(), ':')[0]);

                if( connectID > 65000 )
                    cmp.get()->setLeft(nullptr);
                else 
                    cmp.get()->setLeft(ObjectManager::getInstance().getObject(connectID).get()->getComponent<TerrainComponent>());

            }
        }
    }
    //Update every thing that has been created
    EventManager::getInstance().update();

}