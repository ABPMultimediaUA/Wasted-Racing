#include "ObjectManager.h"
#include "../GameEvent/EventManager.h"
#include <iostream>

//==============================================
// DELEGATES DECLARATIONS
//==============================================
//An event of creation or deletion will contain the shared pointer 
//pointing to the created/deleted object
//When an object is created or deleted, they are automatically added
//or deleted to the objectMap
void objectCreated(EventData eData); 
void objectDeleted(EventData eData); 
void gameClosed(EventData eData); 

//==============================================
// OBJECT MANAGER FUNCTIONS
//============================================== 
ObjectManager& ObjectManager::getInstance() {
    static ObjectManager instance;
    return instance;
}

void ObjectManager::init() {

    //Bind functions
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Create, objectCreated});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeleted});
    EventManager::getInstance().addListener(EventListener {EventType::Game_Close, gameClosed});

}

void ObjectManager::close() {

    while(!objectsMap.empty()){
        objectsMap.erase(objectsMap.begin());
    }

}

GameObject::Pointer ObjectManager::createObject(uint16_t id, GameObject::TransformationData transform) {

    GameObject::Pointer object = std::make_shared<GameObject>(id, transform);

    EventData data;
    data.Object = object;

    EventManager::getInstance().addEvent(Event {EventType::GameObject_Create, data});

    return object;
}


void ObjectManager::addObject(GameObject::Pointer ptr) {

    if(objectsMap.find(ptr.get()->getId()) != objectsMap.end())
        std::cerr << "Couldn't insert object. ID: " << ptr.get()->getId() << " already exists." << std::endl;
    else 
        objectsMap.insert(std::pair<uint16_t,GameObject::Pointer>(ptr.get()->getId(), ptr));


}

void ObjectManager::deleteObject(uint16_t id) {

    auto obj = getObject(id);
    if(obj!=nullptr)
        obj.get()->close();

    if(objectsMap.erase(id) == 0)
        std::cerr << "Couldn't erase object. ID: " << id << " doesn't exist." << std::endl;

}

GameObject::Pointer ObjectManager::getObject(uint16_t id) {

    auto object = objectsMap.find(id);

    if(object == objectsMap.end()){
        std::cerr << "Couldn't find object. ID: " << id << " doesn't exist." << std::endl;
        return nullptr;
    }
    else{
        return object->second;
    }
}

void ObjectManager::showObjects() {
    for(auto obj : objectsMap){}
        //std::cout << obj.second.get()->getId() << std::endl;
}

void ObjectManager::initObjects() {
    for(auto obj : objectsMap)
        obj.second.get()->init();
}


//==============================================
// PLAYER CREATOR
//  pos:        
//              init pos player
//  type:       
//              0 -> Punk
//              1 -> Witch
//              2 -> Cyborg
//              3 -> Crocodile
//  move:       
//              0 -> Player (Input)
//              1 -> IA
// id:
//              id player
//============================================== 

void ObjectManager::createPlayer(GameObject::TransformationData tansform, int type, int move, int id, LAPAL::plane3f terrain, IComponent::Pointer terrainComponent)
{
    if(type == PlayerType::punk)
    {
        createMove(createPunk(tansform, id, terrain, terrainComponent), move);
    }
    else if(type == PlayerType::witch)
    {
        createMove(createWitch(tansform, id, terrain, terrainComponent), move);
    }
    else if(type == PlayerType::cyborg)
    {
        createMove(createCyborg(tansform, id, terrain, terrainComponent), move);
    }
    else if(type == PlayerType::punk)
    {
        createMove(createCrocodile(tansform, id, terrain, terrainComponent), move);
    }
}

GameObject::Pointer ObjectManager::createPunk(GameObject::TransformationData tansform, int id, LAPAL::plane3f terrain, IComponent::Pointer terrainComponent)
{
    auto ob = ObjectManager::getInstance().createObject(id, tansform);
    
    LAPAL::movementData mData;
    mData.mov = false;
    mData.jump = false;
    mData.spi = false;
    mData.angle = 0.0f;
    mData.spin = 0;
    mData.spin_inc = 0.01;
    mData.max_spin = 0.1;
    mData.brake_spin = 0.2;
    mData.rotateX = 0.f;
    mData.rotateZ = 0.f;
    mData.rotate_inc = 0.15f;
    mData.max_rotate = 3.f;
    mData.vel = 0;
    mData.max_vel = 150.0f;
    mData.brake_vel = 10.f;
    mData.velY = 10.f;
    mData.acc = 0;
    mData.max_acc = 20.f;
    mData.dAcc = 10.f;
    mData.brake_acc = 10.f;

    RenderManager::getInstance().createObjectRenderComponent(*ob.get(), ObjectRenderComponent::Shape::Mesh);
    std::shared_ptr<IComponent> collision = PhysicsManager::getInstance().createCollisionComponent(*ob.get(), 2, true, CollisionComponent::Type::Default);

    std::shared_ptr<IComponent> move = PhysicsManager::getInstance().createMoveComponent(*ob.get(), mData, terrain, 1);
    PhysicsManager::getInstance().createMovingCharacter(move, terrainComponent, collision);

    ItemManager::getInstance().createItemHolderComponent(*ob.get());

    ScoreManager::getInstance().createScoreComponent(*ob.get());

    WaypointManager::getInstance().createPathPlanningComponent(ob);

    return ob;

}

GameObject::Pointer ObjectManager::createWitch(GameObject::TransformationData tansform, int id, LAPAL::plane3f terrain, IComponent::Pointer terrainComponent)
{
    auto ob = ObjectManager::getInstance().createObject(id, tansform);
    
    LAPAL::movementData mData;
    mData.mov = false;
    mData.jump = false;
    mData.spi = false;
    mData.angle = 0.0f;
    mData.spin = 0;
    mData.spin_inc = 0.001;
    mData.max_spin = 0.1;
    mData.brake_spin = 0.2;
    mData.rotateX = 0.f;
    mData.rotateZ = 0.f;
    mData.rotate_inc = 0.15f;
    mData.max_rotate = 3.f;
    mData.vel = 0;
    mData.max_vel = 155.0f;
    mData.brake_vel = 10.f;
    mData.velY = 10.f;
    mData.acc = 0;
    mData.max_acc = 18.f;
    mData.dAcc = 12.f;
    mData.brake_acc = 12.f;

    RenderManager::getInstance().createObjectRenderComponent(*ob.get(), ObjectRenderComponent::Shape::Mesh);
    std::shared_ptr<IComponent> collision = PhysicsManager::getInstance().createCollisionComponent(*ob.get(), 2, true, CollisionComponent::Type::Default);

    std::shared_ptr<IComponent> move = PhysicsManager::getInstance().createMoveComponent(*ob.get(), mData, terrain, 1);
    PhysicsManager::getInstance().createMovingCharacter(move, terrainComponent, collision);

    ItemManager::getInstance().createItemHolderComponent(*ob.get());

    ScoreManager::getInstance().createScoreComponent(*ob.get());

    WaypointManager::getInstance().createPathPlanningComponent(ob);

    return ob;

}

GameObject::Pointer ObjectManager::createCyborg(GameObject::TransformationData tansform, int id, LAPAL::plane3f terrain, IComponent::Pointer terrainComponent)
{
    auto ob = ObjectManager::getInstance().createObject(id, tansform);
    
    LAPAL::movementData mData;
    mData.mov = false;
    mData.jump = false;
    mData.spi = false;
    mData.angle =  0.0f;
    mData.spin = 0;
    mData.spin_inc = 0.01;
    mData.max_spin = 0.1;
    mData.brake_spin = 0.2;
    mData.rotateX = 0.f;
    mData.rotateZ = 0.f;
    mData.rotate_inc = 0.15f;
    mData.max_rotate = 3.f;
    mData.vel = 0;
    mData.max_vel = 150.0f;
    mData.brake_vel = 10.f;
    mData.velY = 10.f;
    mData.acc = 0;
    mData.max_acc = 20.f;
    mData.dAcc = 5.f;
    mData.brake_acc = 5.f;

    RenderManager::getInstance().createObjectRenderComponent(*ob.get(), ObjectRenderComponent::Shape::Mesh);
    std::shared_ptr<IComponent> collision = PhysicsManager::getInstance().createCollisionComponent(*ob.get(), 2, true, CollisionComponent::Type::Default);

    std::shared_ptr<IComponent> move = PhysicsManager::getInstance().createMoveComponent(*ob.get(), mData, terrain, 1);
    PhysicsManager::getInstance().createMovingCharacter(move, terrainComponent, collision);

    ItemManager::getInstance().createItemHolderComponent(*ob.get());

    ScoreManager::getInstance().createScoreComponent(*ob.get());

    WaypointManager::getInstance().createPathPlanningComponent(ob);

    return ob;

}

GameObject::Pointer ObjectManager::createCrocodile(GameObject::TransformationData tansform, int id, LAPAL::plane3f terrain, IComponent::Pointer terrainComponent)
{
    auto ob = ObjectManager::getInstance().createObject(id, tansform);
    
    LAPAL::movementData mData;
    mData.mov = false;
    mData.jump = false;
    mData.spi = false;
    mData.angle =  0.0f;
    mData.spin = 0;
    mData.spin_inc = 0.01;
    mData.max_spin = 0.1;
    mData.brake_spin = 0.2;
    mData.rotateX = 0.f;
    mData.rotateZ = 0.f;
    mData.rotate_inc = 0.15f;
    mData.max_rotate = 3.f;
    mData.vel = 0;
    mData.max_vel = 150.0f;
    mData.brake_vel = 10.f;
    mData.velY = 10.f;
    mData.acc = 0;
    mData.max_acc = 20.f;
    mData.dAcc = 5.f;
    mData.brake_acc = 5.f;

    RenderManager::getInstance().createObjectRenderComponent(*ob.get(), ObjectRenderComponent::Shape::Mesh);
    std::shared_ptr<IComponent> collision = PhysicsManager::getInstance().createCollisionComponent(*ob.get(), 2, true, CollisionComponent::Type::Default);

    std::shared_ptr<IComponent> move = PhysicsManager::getInstance().createMoveComponent(*ob.get(), mData, terrain, 1);
    PhysicsManager::getInstance().createMovingCharacter(move, terrainComponent, collision);

    ItemManager::getInstance().createItemHolderComponent(*ob.get());

    ScoreManager::getInstance().createScoreComponent(*ob.get());

    WaypointManager::getInstance().createPathPlanningComponent(ob);

    return ob;

}

void ObjectManager::createMove(GameObject::Pointer obj, int move)
{
    auto mData = obj->getComponent<MoveComponent>()->getMovemententData();
    if(move == 0)
    {
        InputManager::getInstance().createInputComponent(*obj.get());
        RenderManager::getInstance().createCameraRenderComponent(*obj.get());
    }
    else if(move == 1)
    {
        AIManager::getInstance().createAIDrivingComponent(*obj.get());
        AIManager::getInstance().createAIBattleComponent(*obj.get());
        SensorManager::getInstance().createVSensorComponent(*obj.get(), 55.f, mData.angle, 100.f);
        SensorManager::getInstance().createMSensorComponent(*obj.get(), 5.f, mData.angle);
        //RenderManager::getInstance().createCameraRenderComponent(*obj.get());
    }
}

//==============================================
// DELEGATES
//============================================== 
void objectCreated(EventData eData) {

    ObjectManager::getInstance().addObject(eData.Object);

}
void objectDeleted(EventData eData) {

    ObjectManager::getInstance().deleteObject(eData.Id);

}

void gameClosed(EventData eData) {

    ObjectManager::getInstance().setGameRunning(false);

}