#include "NetworkManager.h"


//////////////////////////////////////////////
//            THINGS TO DO HERE
//////////////////////////////////////////////
//////////////////////////////////////////////
/*
+>Add time stamp from server
+>Add score callers
+>Add item callers
+>ADd camera distance calculations
+>Add ristra de mensajes para las posiciones macho
>Add Debug callers
*/
//////////////////////////////////////////////
//////////////////////////////////////////////

//////////////////////////////////////////////////////
//////////// DELEGATE FUNCTIONS
//////////////////////////////////////////////////////
/*void itemBoxCollisionEvent(EventData eData);
void startLineCollisionEvent(EventData eData);
void createBananaEvent(EventData eData);
void destroyBananaEvent(EventData eData);
void createRedShellEvent(EventData eData);
void destroyRedShellEvent(EventData eData);
void createBlueShellEvent(EventData eData);
void destroyBlueShellEvent(EventData eData);   */ 
void sendAdvanceDown(EventData eData);
void sendAdvanceUp(EventData eData);
void sendBrakeDown(EventData eData);
void sendBrakeUp(EventData eData);
void sendTurnLeftDown(EventData eData);
void sendTurnLeftUp(EventData eData);
void sendTurnRightDown(EventData eData);
void sendTurnRightUp(EventData eData);
void sendJumpDown(EventData eData);
void sendJumpUp(EventData eData);
void sendDriftDown(EventData eData);
void sendDriftUp(EventData eData);
void sendUseItemDown(EventData eData);
//=============================================

//==============================================
// PLAYER FUNCTIONS
//============================================== 

void NetworkManager::sendInput(EventType event)
{
    //Stream of raknet bits
    RakNet::BitStream stream;

    stream.Write((unsigned char)ID_INPUT);  //Send message to receive input from the player
    stream.Write((int)server_id);           //Send Id of the player that created it
    stream.Write((EventType) event);        //Type of input sent

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);  //Send the message
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
    ////////////
    //Add remote player component and set its server_id
    ////////////
    NetworkManager::getInstance().createRemotePlayerComponent(*player.get());

    bool found = false;
    std::shared_ptr<RemotePlayerComponent> rPlayer;
    for(unsigned int i = 0; i<remotePlayerComponentList.size() && found == false; i++)
    {
        rPlayer = std::dynamic_pointer_cast<RemotePlayerComponent>(remotePlayerComponentList[i]);
        if(rPlayer.get()->getServerId() == -1)
        {
            found = true;
            rPlayer.get()->setServerId(server_id);
        }
    }

    ////////////
    //DEBUG AREA
    ////////////
    //If debug state is on
    if(debugNetworkState){
        //Server initiated this, so id must be -1 (or the player itself)
        lastSenders.push_back(server_id);
    }

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

    ObjectManager::getInstance().createPlayer(trans, 0, 2, 25001+id, 
                                                PhysicsManager::getInstance().getTerrainFromPos(trans.position).get()->getTerrain(), 
                                                PhysicsManager::getInstance().getTerrainFromPos(trans.position));

    bool found = false;
    std::shared_ptr<RemotePlayerComponent> rPlayer;
    for(unsigned int i = 0; i<remotePlayerComponentList.size() && found == false; i++)
    {
        rPlayer = std::dynamic_pointer_cast<RemotePlayerComponent>(remotePlayerComponentList[i]);
        if(rPlayer.get()->getServerId() == -1)
        {
            found = true;
            rPlayer.get()->setServerId(id);

            ////////////
            //DEBUG AREA
            ////////////
            //If debug state is on
            if(debugNetworkState){
                //Server initiated this, so id must be -1 (or the id of the player)
                lastSenders.push_back(id);
            }
        }
    }
}

void NetworkManager::updatePlayersPosition(RakNet::Packet* packet)
{
    //Auxiliar variables
    float x, y, z, rx, ry, rz;
    int id;
    int size = remotePlayerComponentList.size();

    //Read data
    RakNet::BitStream parser(packet->data, packet->length, false);

    //Ignore packet type
    parser.IgnoreBytes(1);

    //Through all the players
    for(int i = 0; i<size; i++)
    {
        //Read data
        parser.Read(id);
        parser.Read(x);
        parser.Read(y);
        parser.Read(z);
        parser.Read(rx);
        parser.Read(ry);
        parser.Read(rz);

        bool found = false;
        for(int j = 0; j < size && found == false; j++)
        {
            //Get player data
            auto rPlayer = std::dynamic_pointer_cast<RemotePlayerComponent>(remotePlayerComponentList[j]);

            //If it is the searched player
            if(rPlayer->getServerId() == id)
            {
                //Cut loop
                found = true;

                //Read transform data
                auto trans = rPlayer.get()->getGameObject().getTransformData();

                //Assign data
                trans.position.x = x;
                trans.position.y = y;
                trans.position.z = z;

                trans.rotation.x = rx;
                trans.rotation.y = ry;
                trans.rotation.z = rz;
                
                //Assign new position
                rPlayer.get()->getGameObject().setNewTransformData(trans);
                RenderManager::getInstance().getRenderFacade()->updateObjectTransform(rPlayer.get()->getGameObject().getId(), trans);

                //If there is debug, update position of the cylinders
                //if(debugNetworkState){
                    
                //}
            }
        }
    }
}

//=============================================
// CREATE ITEMS
//=============================================

////////////
//CREATE TRAP
////////////
//Send message to the server to create banana
void NetworkManager::createBanana(EventData eData)
{
    //Stream of raknet bits
    RakNet::BitStream stream;

    stream.Write((unsigned char)ID_CREATE_BANANA);  //Send message create banana to server
    stream.Write((int)server_id);                   //Send Id of the player that created it

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);  //Send the message
}

void NetworkManager::destroyBanana(EventData eData)
{
    //Stream of raknet bits
    RakNet::BitStream stream;

    int s_id = ObjectManager::getInstance().getObject(eData.Id).get()->getComponent<RemoteItemComponent>()->getServerId();//get server id of the object

    stream.Write((unsigned char)ID_DESTROY_BANANA); //Send type of event (destroy banana)
    stream.Write((int) s_id);                       //Send server id of the banana
    stream.Write((int) server_id);                   //Send Id of the player that collided with it

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true); //Send the message
}

void NetworkManager::remoteCreateBanana(RakNet::Packet* packet){
    int s_id; //server player creator id
    int o_id; //object server id

    //Read that from the packet
    RakNet::BitStream parser(packet->data, packet->length, false);
    parser.IgnoreBytes(1);
    parser.Read(s_id);
    parser.Read(o_id);

    //Search for the player that created the banana to create it in its position
    bool found = false;
    std::shared_ptr<RemotePlayerComponent> rPlayer;
    for(unsigned int i = 0; i<remotePlayerComponentList.size() && found == false; i++)
    {
        rPlayer = std::dynamic_pointer_cast<RemotePlayerComponent>(remotePlayerComponentList[i]);
        if(rPlayer.get()->getServerId() == s_id)    //find the player creator of the banana
        {
            found = true;
            auto object = ItemManager::getInstance().createBanana(rPlayer.get()->getGameObject());

            //set the server id of the banana
            object.get()->getGameObject().getComponent<RemoteItemComponent>()->setServerId(o_id);
        }
    }
    //If not found by other players, it was our player who did it
    if(found == false)
    {
        auto object = ItemManager::getInstance().createBanana(*player.get());
        object.get()->getGameObject().getComponent<RemoteItemComponent>()->setServerId(o_id);
    }

    ////////////
    //DEBUG AREA
    ////////////
    //If debug state is on
    if(debugNetworkState){
        //Player who created the banana
        lastSenders.push_back(s_id);
    }
}

void NetworkManager::remoteDestroyBanana(RakNet::Packet* packet){
    int s_id;      //server object id
    int sender_id; //Server id of the player that collided with the banana

    //Read data
    RakNet::BitStream parser(packet->data, packet->length, false);
    parser.IgnoreBytes(1);
    parser.Read(s_id);
    parser.Read(sender_id);

    //Search banana inside the remote component list (external bananas)
    bool found = false;
    std::shared_ptr<RemoteItemComponent> rItem;
    for(unsigned int i = 0; i<remoteBananaComponentList.size() && found == false; i++)
    {
        rItem = std::dynamic_pointer_cast<RemoteItemComponent>(remoteBananaComponentList[i]);
        if(rItem.get()->getServerId() == s_id)    //find the Id of the item
        {
            found = true;
            //Remove from list
            remoteBananaComponentList.erase(remoteBananaComponentList.begin()+i);

            //Send erase object event
            EventData data;
            data.Id = rItem.get()->getGameObject().getId();

            EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});
        }
    }

    ////////////
    //DEBUG AREA
    ////////////
    //If debug state is on
    if(debugNetworkState){
        //Player who destroyed the banana
        lastSenders.push_back(sender_id);
    }

}


////////////
//CREATE TIRE
////////////

//Send message to the server to create red shell
void NetworkManager::createRedShell(EventData eData)
{
    //Stream of raknet bits
    RakNet::BitStream stream;

    stream.Write((unsigned char)ID_CREATE_RED_SHELL);  //Send message create red shell to server
    stream.Write((int)server_id);                      //Send Id of the player that created it

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);  //Send the message
}

void NetworkManager::destroyRedShell(EventData eData)
{
    RakNet::BitStream stream;

    int s_id = ObjectManager::getInstance().getObject(eData.Id).get()->getComponent<RemoteItemComponent>()->getServerId();//get server id of the object

    stream.Write((unsigned char)ID_DESTROY_RED_SHELL);
    stream.Write((int) s_id);                          //Send server id of the red shell
    stream.Write((int) server_id);                     //Send Id of the player that collided with it or managed its destruction

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void NetworkManager::remoteCreateRedShell(RakNet::Packet* packet){
    int s_id; //server player creator id
    int o_id; //object server id

    //Read data from the packet
    RakNet::BitStream parser(packet->data, packet->length, false);
    parser.IgnoreBytes(1);
    parser.Read(s_id);
    parser.Read(o_id);

    //Search for the player to create the object in its position
    bool found = false;
    std::shared_ptr<RemotePlayerComponent> rPlayer;
    for(unsigned int i = 0; i<remotePlayerComponentList.size() && found == false; i++)
    {
        rPlayer = std::dynamic_pointer_cast<RemotePlayerComponent>(remotePlayerComponentList[i]);
        if(rPlayer.get()->getServerId() == s_id)    //find the player creator of the red shell
        {
            found = true;
            auto object = ItemManager::getInstance().createRedShell(rPlayer.get()->getGameObject());//Create object
            std::dynamic_pointer_cast<ItemRedShellComponent>(object)->init();                       //Initialize object
            object.get()->getGameObject().getComponent<RemoteItemComponent>()->setServerId(o_id);   //Set server id
            object.get()->getGameObject().getComponent<RemoteItemComponent>()->setParentId(s_id);   //Set parent id
        }
    }

    //If not found by other players, it was our player who did it
    if(found == false)
    {
        auto object = ItemManager::getInstance().createRedShell(*player.get());                 //Create object
        std::dynamic_pointer_cast<ItemRedShellComponent>(object)->init();                       //Initialize object
        object.get()->getGameObject().getComponent<RemoteItemComponent>()->setServerId(o_id);   //Set server id
        object.get()->getGameObject().getComponent<RemoteItemComponent>()->setParentId(s_id);   //Set parent id
    }

    ////////////
    //DEBUG AREA
    ////////////
    //If debug state is on
    if(debugNetworkState){
        //Player who created the banana
        lastSenders.push_back(s_id);
    }

}

void NetworkManager::remoteDestroyRedShell(RakNet::Packet* packet){
    int s_id;   //server object id
    int sender_id; //Server id of the player that collided with the banana

    //Read data from packet
    RakNet::BitStream parser(packet->data, packet->length, false);
    parser.IgnoreBytes(1);
    parser.Read(s_id);
    parser.Read(sender_id);

    //Search for the item to destroy it
    bool found = false;
    std::shared_ptr<RemoteItemComponent> rItem;
    for(unsigned int i = 0; i<remoteRedShellComponentList.size() && found == false; i++)
    {
        rItem = std::dynamic_pointer_cast<RemoteItemComponent>(remoteRedShellComponentList[i]);
        if(rItem.get()->getServerId() == s_id)    //find the Id of the item
        {
            found = true;
            //Remove from list
            remoteRedShellComponentList.erase(remoteRedShellComponentList.begin()+i);

            //Send erase object event
            EventData data;
            data.Id = rItem.get()->getGameObject().getId();

            EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});
        }
    }

    ////////////
    //DEBUG AREA
    ////////////
    //If debug state is on
    if(debugNetworkState){
        //Player who destroyed the banana
        lastSenders.push_back(sender_id);
    }
}


////////////
//CREATE BOMB
////////////

//Send message to the server to create blue shell
void NetworkManager::createBlueShell(EventData eData)
{
    //Stream of raknet bits
    RakNet::BitStream stream;

    stream.Write((unsigned char)ID_CREATE_BLUE_SHELL);  //Send message create blue shell to server
    stream.Write((int)server_id);                   //Send Id of the player that created it

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);  //Send the message
}

void NetworkManager::destroyBlueShell(EventData eData)
{
    RakNet::BitStream stream;

    int s_id = ObjectManager::getInstance().getObject(eData.Id).get()->getComponent<RemoteItemComponent>()->getServerId();//get server id of the object

    stream.Write((unsigned char)ID_DESTROY_BLUE_SHELL);
    stream.Write((int) s_id);   //Send server id of the blue shell
    stream.Write((int) server_id);                     //Send Id of the player that collided with it or managed its destruction

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void NetworkManager::remoteCreateBlueShell(RakNet::Packet* packet){
    int s_id; //server player creator id
    int o_id; //object server id

    RakNet::BitStream parser(packet->data, packet->length, false);
    parser.IgnoreBytes(1);
    parser.Read(s_id);
    parser.Read(o_id);

    //Search for the player to create the object in its position
    bool found = false;
    std::shared_ptr<RemotePlayerComponent> rPlayer;
    for(unsigned int i = 0; i<remotePlayerComponentList.size() && found == false; i++)
    {
        rPlayer = std::dynamic_pointer_cast<RemotePlayerComponent>(remotePlayerComponentList[i]);
        if(rPlayer.get()->getServerId() == s_id)    //find the player creator of the blue shell
        {
            found = true;
            auto object = ItemManager::getInstance().createBlueShell(rPlayer.get()->getGameObject());                //Create object
            std::dynamic_pointer_cast<ItemBlueShellComponent>(object)->init();                      //Initialize object
            object.get()->getGameObject().getComponent<RemoteItemComponent>()->setServerId(o_id);   //Set server id
            object.get()->getGameObject().getComponent<RemoteItemComponent>()->setParentId(s_id);   //Set parent id
        }
    }

    //If not found by other players, it was our player who did it
    if(found == false)
    {
        auto object = ItemManager::getInstance().createBlueShell(*player.get());                //Create object
        std::dynamic_pointer_cast<ItemBlueShellComponent>(object)->init();                      //Initialize object
        object.get()->getGameObject().getComponent<RemoteItemComponent>()->setServerId(o_id);   //Set server id
        object.get()->getGameObject().getComponent<RemoteItemComponent>()->setParentId(s_id);   //Set parent id
    }

    ////////////
    //DEBUG AREA
    ////////////
    //If debug state is on
    if(debugNetworkState){
        //Player who created the banana
        lastSenders.push_back(s_id);
    }
}

void NetworkManager::remoteDestroyBlueShell(RakNet::Packet* packet){
    int s_id;      //Server object id
    int sender_id; //Server id of the sender

    //Read packet data
    RakNet::BitStream parser(packet->data, packet->length, false);
    parser.IgnoreBytes(1);
    parser.Read(s_id);
    parser.Read(sender_id);  

    //Search for the item to destroy it
    bool found = false;
    std::shared_ptr<RemoteItemComponent> rItem;
    for(unsigned int i = 0; i<remoteBlueShellComponentList.size() && found == false; i++)
    {
        rItem = std::dynamic_pointer_cast<RemoteItemComponent>(remoteBlueShellComponentList[i]);
        if(rItem.get()->getServerId() == s_id)    //find the Id of the item
        {
            found = true;
            //Remove from list
            remoteBlueShellComponentList.erase(remoteBlueShellComponentList.begin()+i);

            //Send erase object event
            EventData data;
            data.Id = rItem.get()->getGameObject().getId();
            
            EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});
        }
    }
        
    ////////////
    //DEBUG AREA
    ////////////
    //If debug state is on
    if(debugNetworkState){
        //Player who destroyed the banana
        lastSenders.push_back(sender_id);
    }
}

//=============================================
// MOVING STUFF
//=============================================

/*void NetworkManager::broadcastPosition()
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
    stream.Write((int) server_id);

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}*/

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
    for(unsigned int i = 0; i<remotePlayerComponentList.size() && found == false; i++)
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
            RenderManager::getInstance().getRenderFacade()->updateObjectTransform(rPlayer.get()->getGameObject().getId(), trans);

            //If there is debug, update position of the cylinders
            //if(debugNetworkState){
                
            //}
        }
    }
}

/*void NetworkManager::broadcastPositionRedShell()
{
    RakNet::BitStream stream;
    
    for(unsigned int i = 0; i<remoteRedShellComponentList.size(); i++){
        auto red_shell = std::dynamic_pointer_cast<RemoteItemComponent>(remoteRedShellComponentList[i]);
        stream.Reset();
        if(red_shell.get()->getParentId() == server_id){
            int s_id = red_shell.get()->getServerId();
            auto trans = red_shell.get()->getGameObject().getTransformData();
            stream.Write((unsigned char)ID_REMOTE_RED_SHELL_MOVEMENT);
            stream.Write((int)s_id);
            stream.Write((float)trans.position.x);
            stream.Write((float)trans.position.y);
            stream.Write((float)trans.position.z);
            stream.Write((float)trans.rotation.x);
            stream.Write((float)trans.rotation.y);
            stream.Write((float)trans.rotation.z);

            peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
        }
    }
}*/

void NetworkManager::moveRemoteRedShell(RakNet::Packet* packet)
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
   
    std::shared_ptr<RemoteItemComponent> rPlayer;
    for(unsigned int i = 0; i<remoteRedShellComponentList.size(); i++)
    {
        rPlayer = std::dynamic_pointer_cast<RemoteItemComponent>(remoteRedShellComponentList[i]);
        if(rPlayer.get()->getServerId() == id)
        {
            auto trans = rPlayer.get()->getGameObject().getTransformData();
            trans.position.x = x;
            trans.position.y = y;
            trans.position.z = z;

            trans.rotation.x = rx;
            trans.rotation.y = ry;
            trans.rotation.z = rz;

            rPlayer.get()->getGameObject().setNewTransformData(trans);
            RenderManager::getInstance().getRenderFacade()->updateObjectTransform(rPlayer.get()->getGameObject().getId(), trans);
        }
    }
}

/*void NetworkManager::broadcastPositionBlueShell()
{
    RakNet::BitStream stream;
    for(unsigned int i = 0; i<remoteBlueShellComponentList.size(); i++){
        auto blue_shell = std::dynamic_pointer_cast<RemoteItemComponent>(remoteBlueShellComponentList[i]);
        if( blue_shell.get()->getParentId() == server_id ){
            int s_id = blue_shell.get()->getServerId();
            auto trans = blue_shell.get()->getGameObject().getTransformData();
            stream.Reset();
            stream.Write((int)s_id);
            stream.Write((float)trans.position.x);
            stream.Write((float)trans.position.y);
            stream.Write((float)trans.position.z);
            stream.Write((float)trans.rotation.x);
            stream.Write((float)trans.rotation.y);
            stream.Write((float)trans.rotation.z);

            peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
        }
    }
}*/

void NetworkManager::moveRemoteBlueShell(RakNet::Packet* packet)
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
    std::shared_ptr<RemoteItemComponent> rPlayer;
    for(unsigned int i = 0; i<remoteBlueShellComponentList.size() && found == false; i++)
    {
        rPlayer = std::dynamic_pointer_cast<RemoteItemComponent>(remoteBlueShellComponentList[i]);
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
            RenderManager::getInstance().getRenderFacade()->updateObjectTransform(rPlayer.get()->getGameObject().getId(), trans);
        }
    }
}


//=============================================
// ITEM BOX
//=============================================

void NetworkManager::itemBoxCollision(EventData eData)
{
    RakNet::BitStream stream;

    //Send packet id
    stream.Write((unsigned char)ID_BOX_COLLISION);
    //Send box object id
    stream.Write(eData.CollComponent.get()->getGameObject().getId());
    //Send who sent it
    stream.Write(server_id);

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void NetworkManager::remoteItemBoxCollision(RakNet::Packet* packet)
{
    //Parse data
    RakNet::BitStream parser(packet->data, packet->length, false);
    uint16_t id;
    int sender_id;

    //Ignore packet time
    parser.IgnoreBytes(1);
    //Read data and generate collision
    parser.Read(id);
    ObjectManager::getInstance().getObject(id).get()->getComponent<ItemBoxComponent>().get()->deactivateBox();

    ////////////
    //DEBUG AREA
    ////////////
    //If debug state is on
    if(debugNetworkState){
        //Read from packet and push it in the list
        parser.Read(sender_id);
        lastSenders.push_back(sender_id);
    }

}


//=============================================
// END GAME
//=============================================

void NetworkManager::endGame()
{
    //Set online game as finished
    setStarted(false);

    //Send same information to other players
    RakNet::BitStream stream;
    stream.Write((unsigned char)ID_GAME_ENDED);
    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);

    //Send change state event
    EventData data;
    data.Id = IGameState::stateType::CLIENTLOBBY;
    EventManager::getInstance().addEvent(Event {EventType::State_Change, data});

    //Game::getInstance().setState(IGameState::stateType::CLIENTLOBBY);
}

void NetworkManager::remoteEndGame(RakNet::Packet* packet)
{
    //Set online game as finished
    setStarted(false);
    
    //Send change state event
    EventData data;
    data.Id = IGameState::stateType::CLIENTLOBBY;
    EventManager::getInstance().addEvent(Event {EventType::State_Change, data});

    //Game::getInstance().setState(IGameState::stateType::CLIENTLOBBY);
}


//=============================================
// PRINCIPAL FUNCTIONS
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

    //Conexion has not started
    connected = false;

    //Listeners
    /*EventManager::getInstance().addListener(EventListener {EventType::ItemBoxComponent_Collision,itemBoxCollisionEvent});
    EventManager::getInstance().addListener(EventListener {EventType::StartLineComponent_Collision, startLineCollisionEvent});
    EventManager::getInstance().addListener(EventListener {EventType::Banana_Create,createBananaEvent});
    EventManager::getInstance().addListener(EventListener {EventType::BananaComponent_Collision,destroyBananaEvent});
    EventManager::getInstance().addListener(EventListener {EventType::RedShell_Create,createRedShellEvent});
    EventManager::getInstance().addListener(EventListener {EventType::RedShellComponent_Collision,destroyRedShellEvent});
    EventManager::getInstance().addListener(EventListener {EventType::BlueShell_Create,createBlueShellEvent});
    EventManager::getInstance().addListener(EventListener {EventType::BlueShellComponent_Collision,destroyBlueShellEvent});*/

    //Bind keyboard functions (only needed)
    EventManager::getInstance().addListener(EventListener {EventType::Key_Advance_Down, sendAdvanceDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Advance_Up, sendAdvanceUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Brake_Down, sendBrakeDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Brake_Up, sendBrakeUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnLeft_Down, sendTurnLeftDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnLeft_Up, sendTurnLeftUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnRight_Down, sendTurnRightDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_TurnRight_Up, sendTurnRightUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Jump_Down, sendJumpDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Jump_Up, sendJumpUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Drift_Down, sendDriftDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Drift_Up, sendDriftUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_UseItem_Down, sendUseItemDown});
}

void NetworkManager::update() 
{
    //Received packet
    RakNet::Packet* packet = nullptr;

    //Receive from the RakPeerInterface till there are none
    packet = peer->Receive();

    //If a packet exists
    while(packet)
    {
        //Based on the first byte, determine which packet type is it, then execute accordingly
        switch(packet->data[0])
        {
            case ID_GAME_ENDED:
                remoteEndGame(packet);
                lastPackets.push_back(ID_GAME_ENDED);
                //PROVISIONAL DATA
                lastData.push_back(packet->data);
                break;

            case ID_CREATE_PLAYER:
                createPlayer(packet);
                break;
                
            case ID_CREATE_REMOTE_PLAYER:
                createRemotePlayer(packet);
                break;

            case ID_REMOTE_PLAYER_MOVEMENT:
                moveRemotePlayer(packet);
                //lastPackets.push_back(ID_REMOTE_PLAYER_MOVEMENT);
                break;

            case ID_PLAYERS_POSITION:
                updatePlayersPosition(packet);
                //lastPackets.push_back(ID_REMOTE_PLAYER_MOVEMENT);
                break;

            case ID_BOX_COLLISION:
                remoteItemBoxCollision(packet);
                lastPackets.push_back(ID_BOX_COLLISION);
                //PROVISIONAL DATA
                lastData.push_back(packet->data);
                break;

            case ID_CREATE_BANANA:
                remoteCreateBanana(packet);
                lastPackets.push_back(ID_CREATE_BANANA);
                //PROVISIONAL DATA
                lastData.push_back(packet->data);
                break;

            case ID_DESTROY_BANANA:
                remoteDestroyBanana(packet);
                lastPackets.push_back(ID_DESTROY_BANANA);
                //PROVISIONAL DATA
                lastData.push_back(packet->data);
                break;

            case ID_CREATE_RED_SHELL:
                remoteCreateRedShell(packet);
                lastPackets.push_back(ID_CREATE_RED_SHELL);
                //PROVISIONAL DATA
                lastData.push_back(packet->data);
                break;

            case ID_DESTROY_RED_SHELL:
                remoteDestroyRedShell(packet);
                lastPackets.push_back(ID_DESTROY_RED_SHELL);
                //PROVISIONAL DATA
                lastData.push_back(packet->data);
                break;

            case ID_CREATE_BLUE_SHELL:
                remoteCreateBlueShell(packet);
                lastPackets.push_back(ID_CREATE_BLUE_SHELL);
                //PROVISIONAL DATA
                lastData.push_back(packet->data);
                break;

            case ID_DESTROY_BLUE_SHELL:
                remoteDestroyBlueShell(packet);
                lastPackets.push_back(ID_DESTROY_BLUE_SHELL);
                //PROVISIONAL DATA
                lastData.push_back(packet->data);
                break;

            case ID_REMOTE_RED_SHELL_MOVEMENT:
                moveRemoteRedShell(packet);
                //lastPackets.push_back(ID_REMOTE_RED_SHELL_MOVEMENT);
                break;

            case ID_REMOTE_BLUE_SHELL_MOVEMENT:
                moveRemoteBlueShell(packet);
                //lastPackets.push_back(ID_REMOTE_BLUE_SHELL_MOVEMENT);
                break;

            default:
                std::cout << "Mensaje recibido" << std::endl;
                break;
        }

        //Free packet
        peer->DeallocatePacket(packet);

        //Pick next one if there are more
        packet = peer->Receive();
    }

    //Always broadcast position of items in each iteration
    /*broadcastPosition();
    broadcastPositionRedShell();
    broadcastPositionBlueShell();*/
}

void NetworkManager::close() {

}


//==============================================
// LOBBY FUNCTIONS
//============================================== 
void NetworkManager::initLobby(){
    std::cout << "Attempting to connect to server" << std::endl;
    RakNet::SocketDescriptor socket(0, 	0);
	socket.socketFamily = AF_INET;
    peer->Startup(1, &socket, 1);
    RakNet::ConnectionAttemptResult result;
    result = peer->Connect("192.168.1.38", 39017, 0, 0);

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
                connected = true;
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

    //Push inside the correspondent list
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

//============================================== 
//Collision events
/*void itemBoxCollisionEvent(EventData eData) //Collision with an item
{
    NetworkManager::getInstance().itemBoxCollision(eData);
}

void startLineCollisionEvent(EventData eData) //Collision of the player with the racing line
{
    auto player = std::static_pointer_cast<MoveComponent>(eData.Component);

    auto line = player->getGameObject().getComponent<StartLineComponent>();

    //If there is a racing line
    if(line != nullptr) {
        //If who collided is the player
        if(player->getGameObject().getId() == 25000)
        {
            //If it is the last lap
            if(player->getGameObject().getComponent<ScoreComponent>()->getLap()>ScoreManager::getInstance().getMaxLaps())
            {
                //End the game
                NetworkManager::getInstance().endGame();
            }
        }
    }
}*/
//============================================== 
//Functions that create or destroy objects
/*void createBananaEvent(EventData eData)
{
    NetworkManager::getInstance().createBanana(eData);
}

void destroyBananaEvent(EventData eData){
    NetworkManager::getInstance().destroyBanana(eData);
}

void createRedShellEvent(EventData eData)
{
    NetworkManager::getInstance().createRedShell(eData);
}

void destroyRedShellEvent(EventData eData){
    NetworkManager::getInstance().destroyRedShell(eData);
}

void createBlueShellEvent(EventData eData)
{
    NetworkManager::getInstance().createBlueShell(eData);
}

void destroyBlueShellEvent(EventData eData){
    NetworkManager::getInstance().destroyBlueShell(eData);
}*/

//============================================== 
//Bing keyboard functions to be sent
void sendAdvanceDown(EventData eData)
{
    NetworkManager::getInstance().sendInput(EventType::Key_Advance_Down);
}
void sendAdvanceUp(EventData eData)
{
    NetworkManager::getInstance().sendInput(EventType::Key_Advance_Up);
}
void sendBrakeDown(EventData eData)
{
    NetworkManager::getInstance().sendInput(EventType::Key_Brake_Down);
}
void sendBrakeUp(EventData eData)
{
    NetworkManager::getInstance().sendInput(EventType::Key_Brake_Up);
}
void sendTurnLeftDown(EventData eData)
{
    NetworkManager::getInstance().sendInput(EventType::Key_TurnLeft_Down);
}
void sendTurnLeftUp(EventData eData)
{
    NetworkManager::getInstance().sendInput(EventType::Key_TurnLeft_Up);
}
void sendTurnRightDown(EventData eData)
{
    NetworkManager::getInstance().sendInput(EventType::Key_TurnRight_Down);
}
void sendTurnRightUp(EventData eData)
{
    NetworkManager::getInstance().sendInput(EventType::Key_TurnRight_Up);
}
void sendJumpDown(EventData eData)
{
    NetworkManager::getInstance().sendInput(EventType::Key_Jump_Down);
}
void sendJumpUp(EventData eData)
{
    NetworkManager::getInstance().sendInput(EventType::Key_Jump_Up);
}
void sendDriftDown(EventData eData)
{
    NetworkManager::getInstance().sendInput(EventType::Key_Drift_Down);
}
void sendDriftUp(EventData eData)
{
    NetworkManager::getInstance().sendInput(EventType::Key_Drift_Up);
}
void sendUseItemDown(EventData eData)
{
    NetworkManager::getInstance().sendInput(EventType::Key_UseItem_Down);
}

