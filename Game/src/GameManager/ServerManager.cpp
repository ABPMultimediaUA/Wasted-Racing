#include "ServerManager.h"
#include <iostream>

//====================================================
//  ADDITIONAL FUNCTIONS
//====================================================
/*void loadMapServer();
std::vector<std::string> splitServer(const std::string& s, const char& c);*/

//====================================================
//  MAIN FUNCTIONS
//====================================================
//Function provided by RakNet
unsigned char ServerManager::GetPacketIdentifier(RakNet::Packet *p)
{
	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
		return (unsigned char) p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
	else
		return (unsigned char) p->data[0];
}

void ServerManager::init()
{
    //First we get the instance of the peer
	peer=RakNet::RakPeerInterface::GetInstance();
	//Then we initialize the socket and set his socketFamily (for default communication, use AF_INET)
	RakNet::SocketDescriptor socket(PORT, 	0);
	socket.socketFamily = AF_INET;

	//Set number max of clients based on defined value
	peer->Startup(MAXCLIENTS, &socket, 1);
	peer->SetMaximumIncomingConnections(MAXCLIENTS);

	//Set initial variables
	nPlayers = 0;
	nObjects = 0;
	started  = false;
	loopTime = 1.f/30.f;

	/////
	//Set game variables
	/////
	
	//Initial data set
    globalVariables = &GlobalVariables::getInstance(); //Initialize global variables bush
    globalVariables->setServer(true);                  //The server

    /*eventManager    = &EventManager::getInstance();     //Initilize event manager
    objectManager   = &ObjectManager::getInstance();    //Initialize object manager
    physicsManager  = &PhysicsManager::getInstance();   //Initialize physics manager
    waypointManager = &WaypointManager::getInstance();  //Initialize Waypoint Manager 
    aiManager       = &AIManager::getInstance();        //Initialize AI manager
    sensorManager   = &SensorManager::getInstance();    //Initialize Sensor manager
    itemManager     = &ItemManager::getInstance();      //Initialize Sensor manager
    scoreManager    = &ScoreManager::getInstance();     //Initialize Score Manager*/

    //================================================================
    //INITIALIZE ALL MANAGERS
    //================================================================

    //Initalize the rest
    /*objectManager->init();
    physicsManager->init();
    waypointManager->init();
    aiManager->init();
    sensorManager->init();
    itemManager->init();
    scoreManager->init();

    //Add initial objects
    loadMapServer();*/

    //================================================================
    //SET LISTENERS
    //================================================================
    //EventManager::getInstance().addListener(EventListener {EventType::TrapComponent_Collision, collideTrap});
    //EventManager::getInstance().addListener(EventListener {EventType::BlueShellComponent_Collision, collideBlueShell});
    //EventManager::getInstance().addListener(EventListener {EventType::RedShellComponent_Collision, collideRedShell});
}

void ServerManager::run()
{
	//Initialize timer
    auto lastTime = std::chrono::high_resolution_clock::now();
    float accumulatedTime = 0;
	//float serverTime = 0;

	//We run the server until we decide to close it, we should change the condition later
	while(true)
	{
		//Measure elapsed time
        auto currTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = currTime - lastTime;
        lastTime = currTime;
        accumulatedTime += (float)elapsed.count();
        //serverTime += (float)elapsed.count();

		//Interpolate positions
		//interpolate(accumulatedTime);

		//General update
		update(accumulatedTime);

		/*if(accumulatedTime > loopTime){
			//Update managers
			updateManagers(accumulatedTime);

			accumulatedTime = 0;
		}

		//______________________________________________
		////////////////////////////>>>SERVER SPEED TEST
		//////
		//Broadcast all positions
		//////
		if(started && nPlayers>=1 && serverTime>0.033f){
			broadcastPositionPlayers();
			serverTime = 0;
		}*/
		//______________________________________________
	}
}

void ServerManager::update(float dTime)
{
	//identifier of the packet
	unsigned char identifier;

	//Loop all packets
	for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
	{
		identifier=GetPacketIdentifier(packet);
		//List elements
		switch(identifier)
		{
			//Lost signal
			case ID_REMOTE_CONNECTION_LOST:
				nPlayers--;
				std::cout << "Client disconnected from the server" << std::endl;
				std::cout << "Number of players: " << nPlayers << std::endl;
				break;
			case ID_CONNECTION_LOST:
				nPlayers--;
				std::cout << "Client disconnected from the server" << std::endl;
				std::cout << "Number of players: " << nPlayers << std::endl;
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				nPlayers--;
				std::cout << "Client disconnected from the server" << std::endl;
				std::cout << "Number of players: " << nPlayers << std::endl;
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				nPlayers--;
				std::cout << "Client disconnected from the server" << std::endl;
				std::cout << "Number of players: " << nPlayers << std::endl;
				break;
			case ID_NEW_INCOMING_CONNECTION:
				std::cout << "New client in the server" << std::endl;
				std::cout << "Number of players: " << nPlayers << std::endl;
				//Add player
				players.push_back(packet->systemAddress);

				//Create in-game player
				addPlayer();

				//Add number of players
				nPlayers++;
				break;

			//Game related
			case ID_GAME_START:
				startGame();
				break;
			case ID_GAME_ENDED:
				endGame(packet);
				break;

			//Player related
			case ID_REMOTE_PLAYER_MOVEMENT:
				actPlayer(packet);
				break;

			/*case ID_INPUT:
				actPlayer(packet);
				break;*/

			/*case ID_CREATE_BANANA:
 				broadcastObject(packet);
 				nObjects++;
 				std::cout << "Objeto numero "<<nObjects<< " creado." << std::endl;
 				break;

 			case ID_DESTROY_BANANA:
 				broadcastData(packet);
 				break;
				 
 			case ID_CREATE_RED_SHELL:
 				broadcastObject(packet);
 				nObjects++;
 				std::cout << "Objeto numero "<<nObjects<< " creado." << std::endl;
 				break;

 			case ID_DESTROY_RED_SHELL:
 				broadcastData(packet);
 				break;

 			case ID_CREATE_BLUE_SHELL:
 				broadcastObject(packet);
 				nObjects++;
 				std::cout << "Objeto numero "<<nObjects<< " creado." << std::endl;
 				break;

 			case ID_DESTROY_BLUE_SHELL:
 				broadcastData(packet);
 				break;

 			case ID_REMOTE_RED_SHELL_MOVEMENT:
 				broadcastData(packet);
 				break;

 			case ID_REMOTE_BLUE_SHELL_MOVEMENT:
 				broadcastData(packet);
 				break;

 			case ID_BOX_COLLISION:
 				broadcastData(packet);
				break;*/

            default:
                std::cout << "Receiving new unindentified packet" << std::endl;
                break;
		}
	}


}

/*void ServerManager::updateManagers(float dTime)
{
	//Do calculations if the game has started
	if(started){
		//Input manager has to be the first to be updated
		physicsManager->update(dTime);

		aiManager->update(dTime);

		waypointManager->update(dTime);

		sensorManager->update();

		itemManager->update(dTime);
		
		scoreManager->update();
		
		//Event manager has to be the last to be updated
		eventManager->update();
	}
}

void ServerManager::interpolate(float dTime)
{
	physicsManager->interpolate(dTime, loopTime);
}*/

//==============================================================
// Sending data
//==============================================================
/*void ServerManager::broadcastPositionPlayers()
{
	//Create stream
	RakNet::BitStream stream;

	int server_id; 						//Server ID of the sender
	GameObject* ob; 					//Player Game Object

	//Write type
	stream.Write((unsigned char)ID_PLAYERS_POSITION);

	//Send info of all the players
	for(unsigned int i = 0; i < nPlayers; i++)
	{	
		//get player by its server id
		server_id = i;
		ob = getPlayer(server_id);

		//get transformation data
		auto trans = ob->getTransformData();

		//Write data
		stream.Write((int)server_id);
		stream.Write((float)trans.position.x);
		stream.Write((float)trans.position.y);
		stream.Write((float)trans.position.z);
		stream.Write((float)trans.rotation.x);
		stream.Write((float)trans.rotation.y);
		stream.Write((float)trans.rotation.z);
	}

	//send it
	peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}*/

/*void ServerManager::objectEvent(int who, int which, customMessages what)
{
	//Create stream
	RakNet::BitStream stream;

	//Write type
	stream.Write((unsigned char) what);
	stream.Write((int)who);
	stream.Write((int)which);

	//send it
	peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}*/

void ServerManager::broadcastData(RakNet::Packet* packet)
{
	RakNet::BitStream stream(packet->data, packet->length, false);

	peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, packet->systemAddress, true);
}

void ServerManager::broadcastObject(RakNet::Packet* packet)
{
	RakNet::BitStream stream(packet->data, packet->length, false);
	stream.Write((int)nObjects);

	peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

//==============================================================
// Player related
//==============================================================
/*void ServerManager::addPlayer()
{
	//Set position in map
    GameObject::TransformationData trans;
    trans.position.x = -35.f;
    trans.position.y = 5.f;
    trans.position.z = -20.f + 10.f * nPlayers;

    trans.rotation.x = 0;
    trans.rotation.y = 90;
    trans.rotation.z = 0;

    trans.scale.x    = 1;
    trans.scale.y    = 1;
    trans.scale.z    = 1;

	//Create player
    objectManager->createPlayer(trans, 0, 2, 25000+nPlayers, 
                                                PhysicsManager::getInstance().getTerrainFromPos(trans.position).get()->getTerrain(), 
                                                PhysicsManager::getInstance().getTerrainFromPos(trans.position));

	//Update the manager to instantiate creation
	eventManager->update();

	//Create its server id component
	auto ob = objectManager->getObject(25000+nPlayers);
	createRemotePlayerComponent(*ob.get(), nPlayers);
}

void ServerManager::actPlayer(RakNet::Packet* packet)
{
	//Additional variables
	int server_id; 								//Server ID of the sender
	EventType e; 								//Event type of the input
	EventData eData; 							//Event to send if needed
	GameObject* ob; 							//Player Game Object
	std::shared_ptr<MoveComponent> move; 		//Move Component of the player (if used)
	int type; 									//Type of object created
	IComponent::Pointer Component; 			//Shared pointer of the created item

	//Parse packet
	RakNet::BitStream parser(packet->data, packet->length, false);
    //Ignore message
	parser.IgnoreBytes(1);
	//Read values
    parser.Read(server_id);
	parser.Read(e);

	//Execute action for the given player
	switch(e)
	{
		case Key_Advance_Down:
		    ob = getPlayer(server_id);
			move = ob->getComponent<MoveComponent>();
			move->changeAccInc(move->getMovemententData().max_acc);
			move->isMoving(true);
			break;

		case Key_Advance_Up:
			ob = getPlayer(server_id);
			move = ob->getComponent<MoveComponent>();
			move->changeAccInc(0.0);
    		move->isMoving(false);
			break;

		case Key_Brake_Down:
			ob = getPlayer(server_id);
			move = ob->getComponent<MoveComponent>();
			move->isBraking(true);
			break;

		case Key_Brake_Up:
			ob = getPlayer(server_id);
			move = ob->getComponent<MoveComponent>();
			move->isBraking(false);
			break;

		case Key_TurnLeft_Down:
			ob = getPlayer(server_id);
			move = ob->getComponent<MoveComponent>();
			move->changeSpinIncrement(0.1);
    		move->isSpinning(true);
			break;

		case Key_TurnLeft_Up:
			ob = getPlayer(server_id);
			move = ob->getComponent<MoveComponent>();
			move->changeSpinIncrement(0.0);
    		move->isSpinning(false);
			break;

		case Key_TurnRight_Down:
			ob = getPlayer(server_id);
			move = ob->getComponent<MoveComponent>();
   			move->changeSpinIncrement(-0.1);
    		move->isSpinning(true);
			break;

		case Key_TurnRight_Up:
			ob = getPlayer(server_id);
			move = ob->getComponent<MoveComponent>();
    		move->changeSpinIncrement(0.0);
    		move->isSpinning(false);
			break;

		case Key_Jump_Down:
			ob = getPlayer(server_id);
			move = ob->getComponent<MoveComponent>();
			move->isJumping(true);
			break;
		case Key_Jump_Up:
			ob = getPlayer(server_id);
			move = ob->getComponent<MoveComponent>();
			move->isJumping(false);
			break;
		case Key_Drift_Down:
			ob = getPlayer(server_id);
			move = ob->getComponent<MoveComponent>();
    		move->isDrifting(true);
			break;
		case Key_Drift_Up:
			ob = getPlayer(server_id);
			move = ob->getComponent<MoveComponent>();
    		move->isDrifting(false);
			break;
		case Key_UseItem_Down:
			//Get player
			ob = getPlayer(server_id);
			Component = getRemotePlayerComponent(server_id);

			//Get item type
			type = ob->getComponent<ItemHolderComponent>()->getItemType();

			//send event to create the object to other players
			if(type == IItemComponent::ItemType::trap)
			{
				objectEvent(server_id, nObjects, ID_CREATE_TRAP);
			}
			
			if(type == IItemComponent::ItemType::redShell)
			{
				objectEvent(server_id, nObjects, ID_CREATE_RED_SHELL);
			}

			if(type == IItemComponent::ItemType::blueShell)
			{
				objectEvent(server_id, nObjects, ID_CREATE_BLUE_SHELL);
			}

			//Create object
			Component = itemManager->createItem(*ob);

            //create the remote component of the object if it is trap, missile or tire
			if(Component != nullptr)
			{
				//Create component on the object
				Component = createRemoteItemComponent(*ob, nObjects, type);
				//set server id
				std::dynamic_pointer_cast<RemoteItemComponent>(Component).get()->setServerId(nObjects);	
			}

			//Add number of objects
			nObjects++;

			break;

		default:
			break;
	}
}*/

//==============================================================
// Game related
//==============================================================
//Function to start the game when we are at the lobby
void ServerManager::startGame()
{
	//If game is started
	if(!started && nPlayers > 1)
	{
		//Initial variables
		float x, y, z;
		RakNet::BitStream stream;
		started=true;
		std::cout << "Starting game" << std::endl;
		stream.Write((unsigned char)ID_GAME_START);
		peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
		for(int i=0; i<nPlayers; i++)
		{
			//___>TO IMPLEMENT
			//This switch marks the predefined positions for the players. Later this will be on map's info
			switch(i)
			{
				case 0:
					x=-35.f;
					y=5.f;
					z=-20.f;
					break;
				case 1:
					z=-10.f;
					break;
				default:
					break;
			}
			//Reset and set the message to create player
			stream.Reset();
			stream.Write((unsigned char)ID_CREATE_PLAYER);
			//Its Network ID;
			stream.Write(i);
			//Its position, later will be removed
			stream.Write(x);
			stream.Write(y);
			stream.Write(z);
			peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, players[i], false);
			//Repeat for broadcast
			stream.Reset();
			stream.Write((unsigned char)ID_CREATE_REMOTE_PLAYER);
			stream.Write(i);
			stream.Write(x);
			stream.Write(y);
			stream.Write(z);
			peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, players[i], true);
		}
	}
}

void ServerManager::endGame(RakNet::Packet* packet)
{
	broadcastData(packet);

	started=false;
}

//==============================================================
// Component creator
//==============================================================
/*IComponent::Pointer ServerManager::createRemotePlayerComponent(GameObject& newGameObject, int server_id) {

	//Create component
    IComponent::Pointer component = std::make_shared<RemotePlayerComponent>(newGameObject);

	//Add it to player
    newGameObject.addComponent(component);

	//Add it to list of players
    remotePlayerComponentList.push_back(component);

	//set server_id variable
	std::dynamic_pointer_cast<RemotePlayerComponent>(component).get()->setServerId(server_id);

	//Send event of its creation
    EventData data;
    data.Component = component;

    eventManager->addEvent(Event {EventType::RemotePlayerComponent_Create, data});

    return component;
}

IComponent::Pointer ServerManager::createRemoteItemComponent(GameObject& newGameObject, int server_id, int type) {

	//Create component
    IComponent::Pointer component = std::make_shared<RemoteItemComponent>(newGameObject);

	//Add to object
    newGameObject.addComponent(component);

    //Push inside the correspondent list
    if(type == 0){
        std::dynamic_pointer_cast<RemoteItemComponent>(component).get()->setType(0);
        remoteTrapComponentList.push_back(component);
    }
    if(type == 1){
        std::dynamic_pointer_cast<RemoteItemComponent>(component).get()->setType(1);
        remoteRedShellComponentList.push_back(component);
    }
    if(type == 2){
        std::dynamic_pointer_cast<RemoteItemComponent>(component).get()->setType(2);
        remoteBlueShellComponentList.push_back(component);
    }

    return component;
}*/

//==============================================================
// Additional functions
//==============================================================
/*std::vector<std::string> splitServer(const std::string& s, const char& c) {
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

void loadMapServer() {

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
        auto strVector = splitServer(object->first_attribute("pos")->value(), ',');
        transform.position = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
        //Read ROTATION from XML
        strVector = splitServer(object->first_attribute("rot")->value(), ',');
        transform.rotation = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
        //Read SCALE from XML
        strVector = splitServer(object->first_attribute("sca")->value(), ',');
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
                auto strVector = splitServer(bbox->first_attribute("p1")->value(), ',');
                terrain.p1 = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
                //Read P2 from XML
                strVector = splitServer(bbox->first_attribute("p2")->value(), ',');
                terrain.p2 = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
                //Read P3 from XML
                strVector = splitServer(bbox->first_attribute("p3")->value(), ',');
                terrain.p3 = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
                //Read P4 from XML
                strVector = splitServer(bbox->first_attribute("p4")->value(), ',');
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
                    auto strVector = splitServer(bbox->first_attribute("p1")->value(), ',');
                    terrain.p1 = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
                    //Read P2 from XML
                    strVector = splitServer(bbox->first_attribute("p2")->value(), ',');
                    terrain.p2 = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
                    //Read P3 from XML
                    strVector = splitServer(bbox->first_attribute("p3")->value(), ',');
                    terrain.p3 = glm::vec3(std::stof(strVector[0]),std::stof(strVector[1]),std::stof(strVector[2]));
                    //Read P4 from XML
                    strVector = splitServer(bbox->first_attribute("p4")->value(), ',');
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
                uint16_t connectID = (uint16_t) std::stoi(splitServer(component->first_attribute("l0")->value(), ':')[0]);

                if( connectID > 65000 )
                    cmp.get()->setNext(nullptr);
                else 
                    cmp.get()->setNext(ObjectManager::getInstance().getObject(connectID).get()->getComponent<TerrainComponent>());

                //If not null, connect our component with right object
                connectID = (uint16_t) std::stoi(splitServer(component->first_attribute("l1")->value(), ':')[0]);

                if( connectID > 65000 )
                    cmp.get()->setRight(nullptr);
                else 
                    cmp.get()->setRight(ObjectManager::getInstance().getObject(connectID).get()->getComponent<TerrainComponent>());

                //If not null, connect our component with prev object
                connectID = (uint16_t) std::stoi(splitServer(component->first_attribute("l2")->value(), ':')[0]);

                if( connectID > 65000 )
                    cmp.get()->setPrev(nullptr);
                else 
                    cmp.get()->setPrev(ObjectManager::getInstance().getObject(connectID).get()->getComponent<TerrainComponent>());

                //If not null, connect our component with left object
                connectID = (uint16_t) std::stoi(splitServer(component->first_attribute("l3")->value(), ':')[0]);

                if( connectID > 65000 )
                    cmp.get()->setLeft(nullptr);
                else 
                    cmp.get()->setLeft(ObjectManager::getInstance().getObject(connectID).get()->getComponent<TerrainComponent>());

            }
        }
    }

    //Update every thing that has been created
    EventManager::getInstance().update();

}*/