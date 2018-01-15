#include "Game.h"

#include <memory>
#include <iostream>
#include <rapidxml/rapidxml.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <stdio.h>

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
    GameObject::TransformationData transform;
    u_int16_t id;
    //Circuit
    id = 37;
    transform.position = glm::vec3(0, -3, 0);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob37 = ObjectManager::getInstance().createObject(id, transform);

    //Add light
    id = 12211;
    transform.position = glm::vec3(0, 1000, 0);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob12211 = ObjectManager::getInstance().createObject(id, transform);
    RenderManager::getInstance().createLightRenderComponent(*ob12211.get(),LightRenderComponent::Type::Point,5000);

    //===============================================================
    // ADD WAYPOINT COMPONENT
    //===============================================================
    id = 141;
    transform.position = glm::vec3(440, 0,5);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);
    auto ob141 = ObjectManager::getInstance().createObject(id, transform);
    
    WaypointManager::getInstance().createWaypointComponent(ob141, 80, 0);
    //Road
    std::shared_ptr<IComponent> cp17 = RenderManager::getInstance().createObjectRenderComponent(*ob37.get(), ObjectRenderComponent::Shape::Road);

    //===============================================================
    // ADD PLAYER 
    //===============================================================
    loadMap();

    auto terrainCMP = ObjectManager::getInstance().getObject((uint16_t) 1067).get()->getComponent<TerrainComponent>();
    auto terrCMP = terrainCMP.get()->getTerrain();

    id = 50;
    transform.position = glm::vec3(-25,10, -20);
    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, 1, 0, id, terrCMP, terrainCMP);

    //===============================================================
    // ADD AI 
    //===============================================================

    /*id = 55;
    transform.position = glm::vec3(-25,10,20);
    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, 0, 1, id, terrCMP, terrainCMP);

    id = 56;
    transform.position = glm::vec3(-125,0,5);
    transform.rotation = glm::vec3(45,45,45);
    transform.scale    = glm::vec3(1,1,1);
    ObjectManager::getInstance().createPlayer(transform, 1, 1, id, terrain, terrainCP1);*/

    //===============================================================
    // Update to distribute all creation events
    //===============================================================
    EventManager::getInstance().update();
    
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
                LAPAL::calculateRotationsXZ(terrain);

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