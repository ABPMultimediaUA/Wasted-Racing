#include "NetworkManager.h"

//////////////////////////////////////////////////////
//////////// DELEGATE FUNCTIONS
//////////////////////////////////////////////////////
void itemBoxCollisionEvent(EventData eData);
void startLineCollisionEvent(EventData eData);
void createBananaEvent(EventData eData);
void destroyBananaEvent(EventData eData);
//=============================================

NetworkManager& NetworkManager::getInstance() {
    static NetworkManager instance;
    return instance;
}

void NetworkManager::init() {
    //Get instance of the peer
    peer = RakNet::RakPeerInterface::GetInstance();

    //Match not started
    started = false;

    //Listeners
    EventManager::getInstance().addListener(EventListener {EventType::ItemBoxComponent_Collision,itemBoxCollisionEvent});
    EventManager::getInstance().addListener(EventListener {EventType::StartLineComponent_Collision, startLineCollisionEvent});
    EventManager::getInstance().addListener(EventListener {EventType::Banana_Create,createBananaEvent});
    EventManager::getInstance().addListener(EventListener {EventType::BananaComponent_Collision,destroyBananaEvent});
}

void NetworkManager::createPlayer(RakNet::Packet* packet)
{
    float x, y, z;
    RakNet::BitStream parser(packet->data, packet->length, false);
    parser.IgnoreBytes(1);
    //Server ID
    parser.Read(server_id);
    //Position
    parser.Read(x);
    parser.Read(y);
    parser.Read(z);

    auto trans = player.get()->getTransformData();
    trans.position.x = x;
    trans.position.y = y;
    trans.position.z = z;
    player.get()->setNewTransformData(trans);
    player.get()->setOldTransformData(trans);
    player.get()->setTransformData(trans);

}

void NetworkManager::createRemotePlayer(RakNet::Packet* packet)
{
    int id;
    float x, y, z;
    RakNet::BitStream parser(packet->data, packet->length, false);
    parser.IgnoreBytes(1);
    parser.Read(id);
    parser.Read(x);
    parser.Read(y);
    parser.Read(z);

    GameObject::TransformationData trans;
    trans.position.x = x;
    trans.position.y = y;
    trans.position.z = z;

    trans.rotation.x = 0;
    trans.rotation.y = 90;
    trans.rotation.z = 0;

    trans.scale.x    = 1;
    trans.scale.y    = 1;
    trans.scale.z    = 1;

    ObjectManager::getInstance().createPlayer(trans,0, 2, 25001+id, 
                                                PhysicsManager::getInstance().getTerrainFromPos(trans.position).get()->getTerrain(), 
                                                PhysicsManager::getInstance().getTerrainFromPos(trans.position));

    bool found = false;
    std::shared_ptr<RemotePlayerComponent> rPlayer;
    for(int i = 0; i<remotePlayerComponentList.size() && found == false; i++)
    {
        rPlayer = std::dynamic_pointer_cast<RemotePlayerComponent>(remotePlayerComponentList[i]);
        if(rPlayer.get()->getServerId() == -1)
        {
            found = true;
            rPlayer.get()->setServerId(id);
        }
    }
}


//=============================================
// CREATE ITEMS
//=============================================

//Send message to the server to create banana
void NetworkManager::createBanana(EventData eData)
{
    //Stream of raknet bits
    RakNet::BitStream stream;
    std::shared_ptr<RemotePlayerComponent> rPlayer;
    bool found = false;

    for(int i = 0; i<remotePlayerComponentList.size() && found == false; i++)
    {
        //take component from list
        rPlayer = std::dynamic_pointer_cast<RemotePlayerComponent>(remotePlayerComponentList[i]);

        //equipare
        if( rPlayer.get()->getGameObject().getId() == eData.Id )    //Get the player with the ID of the
        {
            found = true;
            stream.Write((unsigned char)ID_CREATE_BANANA);          //Send message create banana to server
            stream.Write((int)rPlayer.get()->getServerId());        //Send Id of the player that created it

            peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);  //Send the message
        }
    }


}

void NetworkManager::destroyBanana(EventData eData)
{
    RakNet::BitStream stream;

    stream.Write((unsigned char)ID_DESTROY_BANANA);
    stream.Write((int)server_id);

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void NetworkManager::remoteCreateBanana(RakNet::Packet* packet){
    int s_id; //server player creator id
    int o_id; //object server id

    RakNet::BitStream parser(packet->data, packet->length, false);
    parser.IgnoreBytes(1);
    parser.Read(s_id);
    parser.Read(o_id);

    bool found = false;
    std::shared_ptr<RemotePlayerComponent> rPlayer;
    for(int i = 0; i<remotePlayerComponentList.size() && found == false; i++)
    {
        rPlayer = std::dynamic_pointer_cast<RemotePlayerComponent>(remotePlayerComponentList[i]);
        if(rPlayer.get()->getServerId() == s_id)    //find the player creator of the banana
        {
            found = true;
            ItemManager::getInstance().createBanana(rPlayer.get()->getGameObject());
        }
    }
}

void NetworkManager::remoteDestroyBanana(RakNet::Packet* packet){
    
}


//=============================================
// MOVING STUFF
//=============================================

void NetworkManager::broadcastPosition()
{
    RakNet::BitStream stream;
    auto trans = player.get()->getTransformData();

    stream.Write((unsigned char)ID_REMOTE_PLAYER_MOVEMENT);
    stream.Write((int)server_id);
    stream.Write((float)trans.position.x);
    stream.Write((float)trans.position.y);
    stream.Write((float)trans.position.z);
    stream.Write((float)trans.rotation.x);
    stream.Write((float)trans.rotation.y);
    stream.Write((float)trans.rotation.z);

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void NetworkManager::moveRemotePlayer(RakNet::Packet* packet)
{
    RakNet::BitStream parser(packet->data, packet->length, false);
    float x, y, z, rx, ry, rz;
    int id;

    parser.IgnoreBytes(1);
    parser.Read(id);
    parser.Read(x);
    parser.Read(y);
    parser.Read(z);
    parser.Read(rx);
    parser.Read(ry);
    parser.Read(rz);
   
    bool found = false;
    std::shared_ptr<RemotePlayerComponent> rPlayer;
    for(int i = 0; i<remotePlayerComponentList.size() && found == false; i++)
    {
        rPlayer = std::dynamic_pointer_cast<RemotePlayerComponent>(remotePlayerComponentList[i]);
        if(rPlayer.get()->getServerId() == id)
        {
            found = true;
            auto trans = rPlayer.get()->getGameObject().getTransformData();
            trans.position.x = x;
            trans.position.y = y;
            trans.position.z = z;

            trans.rotation.x = rx;
            trans.rotation.y = ry;
            trans.rotation.z = rz;

            rPlayer.get()->getGameObject().setNewTransformData(trans);
        }
    }
}

void NetworkManager::itemBoxCollision(EventData eData)
{
    RakNet::BitStream stream;

    stream.Write((unsigned char)ID_BOX_COLLISION);
    stream.Write(eData.CollComponent.get()->getGameObject().getId());

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void NetworkManager::remoteItemBoxCollision(RakNet::Packet* packet)
{
    RakNet::BitStream parser(packet->data, packet->length, false);
    uint16_t id;

    parser.IgnoreBytes(1);
    parser.Read(id);
    ObjectManager::getInstance().getObject(id).get()->getComponent<ItemBoxComponent>().get()->deactivateBox();

}


//=============================================
// END GAME
//=============================================

void NetworkManager::endGame()
{
    RakNet::BitStream stream;

    setStarted(false);

    stream.Write((unsigned char)ID_GAME_ENDED);
    
    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);

    Game::getInstance().setState(IGameState::stateType::CLIENTLOBBY);
}

void NetworkManager::remoteEndGame(RakNet::Packet* packet)
{
    setStarted(false);
    Game::getInstance().setState(IGameState::stateType::CLIENTLOBBY);
}


//=============================================
// PRINCIPAL FUNCTIONS
//=============================================

void NetworkManager::update() 
{
    RakNet::Packet* packet;
    packet = peer->Receive();
    if(packet)
    {
        switch(packet->data[0])
        {
            case ID_GAME_ENDED:
                remoteEndGame(packet);
                break;
            case ID_CREATE_PLAYER:
                createPlayer(packet);
                break;
            case ID_CREATE_REMOTE_PLAYER:
                createRemotePlayer(packet);
                break;
            case ID_REMOTE_PLAYER_MOVEMENT:
                moveRemotePlayer(packet);
                break;
            case ID_BOX_COLLISION:
                remoteItemBoxCollision(packet);
                break;
            case ID_CREATE_BANANA:
                remoteCreateBanana(packet);
                break;
            case ID_DESTROY_BANANA:
                remoteDestroyBanana(packet);
                break;
            default:
                std::cout << "ALGO PASÓ PERO NO SABEMOS QUË" << std::endl;
                break;
        }
        peer->DeallocatePacket(packet);
    }
    broadcastPosition();
}

void NetworkManager::close() {

}

void NetworkManager::initLobby(){
    std::cout << "Attempting to connect to server" << std::endl;
    RakNet::SocketDescriptor socket(0, 	0);
	socket.socketFamily = AF_INET;
    peer->Startup(1, &socket, 1);
    RakNet::ConnectionAttemptResult result;
    result = peer->Connect("192.168.1.32", 39017, 0, 0);

    if(result == RakNet::CONNECTION_ATTEMPT_STARTED)
    {
        std::cout << "Connection Attempt Started Correctly" << std::endl;
    }
}

void NetworkManager::updateLobby(){
    RakNet::Packet* result;
    RakNet::BitStream stream;
    result = peer->Receive();
    if(result)
    {
        switch(result->data[0])
        {
            case ID_CONNECTION_REQUEST_ACCEPTED:
                std::cout << "Connection Accepted" << std::endl;
                stream.Write((unsigned char)ID_GAME_START);
                peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
                break;
            case ID_CONNECTION_ATTEMPT_FAILED:
                std::cout << "Connection Failed" << std::endl;
                break;
            case ID_GAME_START:
                std::cout << "Game started " << std::endl;
                started = true;
                break;
            default:
                break;
        }
        peer->DeallocatePacket(result);
    }
}

//==============================================
// CREATE COMPONENTS
//============================================== 

IComponent::Pointer NetworkManager::createRemotePlayerComponent(GameObject& newGameObject) {

    IComponent::Pointer component = std::make_shared<RemotePlayerComponent>(newGameObject);

    newGameObject.addComponent(component);

    remotePlayerComponentList.push_back(component);

    EventData data;
    data.Component = component;

    EventManager::getInstance().addEvent(Event {EventType::RemotePlayerComponent_Create, data});

    return component;
}

IComponent::Pointer NetworkManager::createRemoteItemComponent(GameObject& newGameObject, int type) {

    IComponent::Pointer component = std::make_shared<RemoteItemComponent>(newGameObject);

    newGameObject.addComponent(component);

    if(type == 0){
        std::dynamic_pointer_cast<RemoteItemComponent>(component).get()->setType(0);
        remoteBananaComponentList.push_back(component);
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
}
//==============================================
// DELEGATES
//============================================== 

void itemBoxCollisionEvent(EventData eData)
{
    NetworkManager::getInstance().itemBoxCollision(eData);
}

void startLineCollisionEvent(EventData eData)
{
    auto player = std::static_pointer_cast<MoveComponent>(eData.Component);

    auto line = player->getGameObject().getComponent<StartLineComponent>();

    if(line != nullptr) {
        if(player->getGameObject().getId() == 25000)
        {
            if(player->getGameObject().getComponent<ScoreComponent>()->getLap()>ScoreManager::getInstance().getMaxLaps())
            {
                NetworkManager::getInstance().endGame();
            }
        }
    }
}

void createBananaEvent(EventData eData)
{
    NetworkManager::getInstance().createBanana(eData);
}

void destroyBananaEvent(EventData eData){
    NetworkManager::getInstance().destroyBanana(eData);
}