#include "NetworkManager.h"

NetworkManager& NetworkManager::getInstance() {
    static NetworkManager instance;
    return instance;
}

void NetworkManager::init() {
    //Get instance of the peer
    peer = RakNet::RakPeerInterface::GetInstance();

    //Match not started
    started = false;
}

void NetworkManager::createPlayer(RakNet::Packet* packet)
{
    int x, y, z;
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
    player.get()->setTransformData(trans);

}

void NetworkManager::createRemotePlayer(RakNet::Packet* packet)
{
    int x, y, z, id;
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

void NetworkManager::broadcastPosition()
{
    RakNet::BitStream stream;
    auto trans = player.get()->getTransformData();

    stream.Write((unsigned char)ID_REMOTE_PLAYER_MOVEMENT);
    stream.Write(server_id);
    stream.Write(trans.position.x);
    stream.Write(trans.position.y);
    stream.Write(trans.position.z);

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void NetworkManager::moveRemotePlayer(RakNet::Packet* packet)
{
    RakNet::BitStream parser(packet->data, packet->length, false);
    float x, y, z;
    int id;

    parser.IgnoreBytes(1);
    parser.Read(id);
    parser.Read(x);
    parser.Read(y);
    parser.Read(z);

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
            rPlayer.get()->getGameObject().setTransformData(trans);
        }
    }

}

void NetworkManager::update() {
    RakNet::Packet* packet;
    packet = peer->Receive();
    if(packet)
    {
        switch(packet->data[0])
        {
            case ID_CREATE_PLAYER:
                createPlayer(packet);
                break;
            case ID_CREATE_REMOTE_PLAYER:
                createRemotePlayer(packet);
                break;
            case ID_REMOTE_PLAYER_MOVEMENT:
                moveRemotePlayer(packet);
                break;
            default:
                std::cout << "ALGO PASO" << std::endl;
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

IComponent::Pointer NetworkManager::createRemotePlayerComponent(GameObject& newGameObject) {

        IComponent::Pointer component = std::make_shared<RemotePlayerComponent>(newGameObject);

        newGameObject.addComponent(component);

        remotePlayerComponentList.push_back(component);

        EventData data;
        data.Component = component;

        EventManager::getInstance().addEvent(Event {EventType::RemotePlayerComponent_Create, data});

        return component;
    }
