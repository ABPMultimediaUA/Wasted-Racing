#include "Game.h"

//Additional functions
void addObjects();
void loadMap();
std::vector<std::string> split(const std::string& s, const char& c);

//====================================================
//  GAME INITIALIZATION
//====================================================
void Game::init() {

    //Say game loop is active
    setStay(true);

    //Set engine to default
    setRenderEngine(0);
    setInputEngine(0);

    globalVariables = &GlobalVariables::getInstance(); //Initialize global variables bush
    audioManager    = &AudioManager::getInstance();     //Initialize true audio manager
    eventManager    = &EventManager::getInstance();     //Initilize event manager
    renderManager   = &RenderManager::getInstance();    //First we initialize renderManager, who creates a device and passes this reference to the inputManager
    inputManager    = &InputManager::getInstance();     //Once we've initialized the renderManager, we can do the same with our inputManager
    objectManager   = &ObjectManager::getInstance();    //Initialize object manager
    physicsManager  = &PhysicsManager::getInstance();   //Initialize physics manager
    waypointManager = &WaypointManager::getInstance();  //Initialize Waypoint Manager 
    aiManager       = &AIManager::getInstance();        //Initialize AI manager
    sensorManager   = &SensorManager::getInstance();    //Initialize Sensor manager
    itemManager     = &ItemManager::getInstance();      //Initialize Sensor manager
    scoreManager    = &ScoreManager::getInstance();     //Initialize Score Manager
    networkManager  = &NetworkManager::getInstance();     //Initialize Score Manager
    debugManager    = &DebugManager::getInstance();     //Initialize Score Manager

    //================================================================
    //INITIALIZE ALL MANAGERS
    //================================================================
    //Initialize true audio manager
    audioManager->init();
    //First we initialize renderManager, who creates a device and passes this reference to the inputManager
    renderManager->init(0);

    //Once we've initialized the renderManager, we can do the same with our inputManager
    inputManager->init(0);

    objectManager->init();
    physicsManager->init();
    waypointManager->init();
    aiManager->init();
    sensorManager->init();
    itemManager->init();
    scoreManager->init();
    networkManager->init();
    debugManager->init();

    //Initial state
    setState(IGameState::stateType::INTRO);

    addObjects();
}

//====================================================
//  GAME UPDATE
//====================================================
void Game::update(float dTime) {

}

void Game::updateServer() {

}

//====================================================
//  GAME DRAW
//====================================================
void Game::draw() {
}

//====================================================
//  GAME CLOSE
//====================================================
void Game::close() {
    globalVariables->getGameState()->close();

    //Close all managers
    physicsManager->close();
    renderManager->close();
    inputManager->close();
    eventManager->close();
    waypointManager->close();
    aiManager->close();
    audioManager->close();
    sensorManager->close();
    itemManager->close();
    scoreManager->close();
    networkManager->close();
    debugManager->close();
}

//====================================================
//  GAME LOOP
//====================================================
//Client side
void Game::Run() {
    //Initialize game
    init();
    
    //This is the client
    server = false;

    //Initialize timer
    auto lastTime = std::chrono::high_resolution_clock::now();
    accumulatedTime = 0;

    //Start the run
    //execute game while staying
    while(stay){
        //Measure elapsed time
        auto currTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = currTime - lastTime;
        lastTime = currTime;
        accumulatedTime += (float)elapsed.count();

        //Update the game once every maxTime
        globalVariables->getGameState()->update(accumulatedTime);

        //Always draw the game
        globalVariables->getGameState()->draw();
    }

    close();
}

void Game::RunServer() {
    //Initialize game
    init();
    
    //This is the client
    server = true;

    //Start the run
    //execute game while staying
    while(stay){
        updateServer();
    }

    close();
}

//===============================================================
// ADDITIONAL FUNCTIONS
//===============================================================

//State setter
void Game::setState(IGameState::stateType type){
        //State changer
        switch(type){
            case IGameState::stateType::INTRO:
                globalVariables->setGameState((IGameState* )&IntroState::getInstance());
                break;
            case IGameState::stateType::CLIENTLOBBY:
                globalVariables->setGameState((IGameState* )&ClientLobbyState::getInstance());
                break;
            case IGameState::stateType::MATCH:
                globalVariables->setGameState((IGameState* )&MatchState::getInstance());
                break;
            case IGameState::stateType::MULTIMATCH:
                globalVariables->setGameState((IGameState* )&MultiMatchState::getInstance());
                break;
            default:
                globalVariables->setGameState((IGameState* )&IntroState::getInstance());
                break;
        }
    }

//adding minimum objects needed to play the game
void addObjects(){
    //===============================================================
    // add player 
    //===============================================================
    loadMap();

    uint16_t id = 25000;
    GameObject::TransformationData transform;
    
    transform.position = glm::vec3(-35,0, -20);
    transform.rotation = glm::vec3(0,90,0);
    transform.scale    = glm::vec3(1,1,1);
    
    ObjectManager::getInstance().createPlayer(transform, 1, 0, id, 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position).get()->getTerrain(), 
                                                PhysicsManager::getInstance().getTerrainFromPos(transform.position));
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
        strVector = split(object->first_attribute("sca")->value(), ',');
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

            //Parse WAYPOINT component
            if(strcmp(component->first_attribute("name")->value(),"waypoint") == 0){
                
                float radius = std::stof(component->first_attribute("radius")->value());
                
                int level = std::stoi(component->first_attribute("level")->value());

                //Create TERRAIN component
                WaypointManager::getInstance().createWaypointComponent(obj, radius, level);

            }

            //Parse LIGHT component
            if(strcmp(component->first_attribute("name")->value(),"light") == 0){
                
                float radius = std::stof(component->first_attribute("radius")->value());
                
                LightRenderComponent::Type type;

                if(component->first_attribute("type")->value()[0] == 'P')
                    type = LightRenderComponent::Type::Point;
                if(component->first_attribute("type")->value()[0] == 'D')
                    type = LightRenderComponent::Type::Directional;

                //Create LIGHT component
                RenderManager::getInstance().createLightRenderComponent(*obj.get(),type,radius);

            }

            //Parse ITEMBOX component
            if(strcmp(component->first_attribute("name")->value(),"itemBox") == 0){
                
                //Create TERRAIN component
                ItemManager::getInstance().createItemBox(*obj.get());

            }

            //Parse COLLISION component
            if(strcmp(component->first_attribute("name")->value(),"collision") == 0){

                xml_node<>* bbox = component->first_node("bbox");
                LAPAL::plane3f terrain;
                float radius, height;
                bool kinetic;
                CollisionComponent::Type type = CollisionComponent::Type::Default;

                //Parse Terrain if given
                if(bbox != nullptr) {
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
                }

                //Parse Radius if given
                if(component->first_attribute("radius") != nullptr)
                    radius = std::stof(component->first_attribute("radius")->value());

                //Parse Height if given
                if(component->first_attribute("height") != nullptr)
                    height = std::stof(component->first_attribute("height")->value());

                //Parse Kinetic
                if(strcmp(component->first_attribute("kinetic")->value(),"true") == 0)
                    kinetic = true;
                else 
                    kinetic = false;

                //Parse Type
                if(strcmp(component->first_attribute("type")->value(),"ramp") == 0)
                    type = CollisionComponent::Type::Ramp;
                if(strcmp(component->first_attribute("type")->value(),"start") == 0)
                    type = CollisionComponent::Type::StartLine;


                //Create COLLISION component
                if(bbox != nullptr)
                    PhysicsManager::getInstance().createCollisionComponent(*obj.get(), terrain, kinetic, 10, type);
                else
                    PhysicsManager::getInstance().createCollisionComponent(*obj.get(), radius, 10, kinetic, type);

            }

            //Parse RAMP component
            if(strcmp(component->first_attribute("name")->value(),"ramp") == 0){
                
                float vel = std::stof(component->first_attribute("vel")->value());
                float cTime = std::stof(component->first_attribute("cTime")->value());
                float dTime = std::stof(component->first_attribute("dTime")->value());

                //Create RAMP component
                PhysicsManager::getInstance().createRampComponent(*obj.get(), vel, cTime, dTime);

            }

            //Parse RENDER component
            if(strcmp(component->first_attribute("name")->value(),"render") == 0){

                ObjectRenderComponent::Shape shape;

                if(strcmp(component->first_attribute("type")->value(),"plane") == 0){
                    shape = ObjectRenderComponent::Shape::Plane;
                }
                if(strcmp(component->first_attribute("type")->value(),"mesh") == 0){
                    shape = ObjectRenderComponent::Shape::Mesh;
                }
                if(strcmp(component->first_attribute("type")->value(),"sphere") == 0){
                    shape = ObjectRenderComponent::Shape::Sphere;
                }
                if(strcmp(component->first_attribute("type")->value(),"cube") == 0){
                    shape = ObjectRenderComponent::Shape::Cube;
                }
                
                //Create RENDER component
                RenderManager::getInstance().createObjectRenderComponent(*obj.get(), shape, component->first_attribute("file")->value());

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