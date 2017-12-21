#include "ItemManager.h"

void objectDeleteItem(EventData eData);
void objectDeleteHolder(EventData);

ItemManager::ItemManager()
{

}

//Instance getter
ItemManager& ItemManager::getInstance(){
    static ItemManager instance;
    return instance;
}

void ItemManager::init(){

    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeleteItem});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeleteHolder});

}

void ItemManager::update(float dTime){
    
    for(unsigned int i = 0; i < ItemBoxes.size() ; i++){
        ItemBoxes.at(i)->update(dTime);
    }

    for(unsigned int i = 0; i < ItemComponents.size() ; i++){
        ItemComponents.at(i)->update(dTime);
    }
}

void ItemManager::close(){
    ItemHolders.clear();
    ItemComponents.clear();        
    ItemBoxes.clear();
}


//////////////////////////////////////////////////////
/////
/////       ItemHolderComponent DELETE
/////
//////////////////////////////////////////////////////

IComponent::Pointer ItemManager::createItemHolderComponent(GameObject& newGameObject){

    IComponent::Pointer component = std::make_shared<ItemHolderComponent>(newGameObject);
    
    newGameObject.addComponent(component);

    ItemHolders.push_back(std::dynamic_pointer_cast<ItemHolderComponent>(component));

    return component;
}

//////////////////////////////////////////////////////
/////
/////       ITEM BOX CREATOR
/////
//////////////////////////////////////////////////////

IComponent::Pointer ItemManager::createItemBox(GameObject& obj){

    IComponent::Pointer component = std::make_shared<ItemBoxComponent>(obj, 600);

    obj.addComponent(component);

    EventData data;
    data.Component = component;

    ItemBoxes.push_back(std::dynamic_pointer_cast<ItemBoxComponent>(component));
    EventManager::getInstance().addEvent(Event {EventType::ItemBoxComponent_Create, data});

    return component;
}


//////////////////////////////////////////////////////
/////
/////       ITEM CREATOR
/////
//////////////////////////////////////////////////////


IComponent::Pointer ItemManager::createItem(GameObject& obj){

    auto itemHolder = obj.getComponent<ItemHolderComponent>();
    int random = 1;//itemHolder->getItemType();
    
    if(random == IItemComponent::ItemType::redShell)
    {
        itemHolder->setItemType(-1);
        auto component = createRedShell(obj);
        std::dynamic_pointer_cast<ItemRedShellComponent>(component)->init();
        return component;
    }
    else if(random == IItemComponent::ItemType::blueShell)
    {
        itemHolder->setItemType(-1);
        auto component = createBlueShell(obj);
        std::dynamic_pointer_cast<ItemBlueShellComponent>(component)->init();
        return component;
    }
    else if(random == IItemComponent::ItemType::banana)
    {
        itemHolder->setItemType(-1);
        return createBanana(obj);
    }
    else if(random == IItemComponent::ItemType::mushroom)
    {
        itemHolder->setItemType(-1);
        auto component = createMushroom(obj);
        std::dynamic_pointer_cast<ItemMushroomComponent>(component)->init();
        deleteItem(component);
        return component;
    }
    else if(random == IItemComponent::ItemType::star)
    {
        itemHolder->setItemType(-1);
        auto component = createStar(obj);
        std::dynamic_pointer_cast<ItemStarComponent>(component)->init();
        deleteItem(component);
        return component;
    }
    return nullptr;
}



//////////////////////////////////////////////////////
/////
/////       ITEM CREATE
/////
//////////////////////////////////////////////////////


IComponent::Pointer ItemManager::createRedShell(GameObject& obj)
{
    uint16_t id = 6000 + ItemComponents.size();
    GameObject::TransformationData transform;

    auto pos = obj.getTransformData().position;

    transform.position = glm::vec3(pos.x+20*cos(obj.getTransformData().rotation.y),
                                    pos.y, pos.z-20*sin(obj.getTransformData().rotation.y));
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(0.2, 0.2, 0.2);

    auto ob = ObjectManager::getInstance().createObject(id, transform);

    IComponent::Pointer component = std::make_shared<ItemRedShellComponent>(*ob.get(), obj);

    ob.get()->addComponent(component);

    LAPAL::movementData mData;
    mData.mov = false;
    mData.jump = false;
    mData.spi = false;
    mData.angInc = 0;
    mData.angle = obj.getComponent<MoveComponent>()->getMovemententData().angle;
    mData.spin = 0;
    mData.spin_inc = 1;
    mData.max_spin = 1;
    mData.brake_spin = 0.2;
    mData.rotateX = 0.f;
    mData.rotateZ = 0.f;
    mData.rotate_inc = 0.15f;
    mData.max_rotate = 3.f;
    mData.vel = 200.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;
    mData.max_vel = 200.0f + obj.getComponent<MoveComponent>()->getMovemententData().vel;
    mData.brake_vel = 0.f;
    mData.velY = 2000.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;;
    mData.acc = 200.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;;
    mData.max_acc = 200.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;;
    mData.dAcc = 200.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;;
    mData.brake_acc = 200.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;;


    auto terrain = obj.getComponent<MoveComponent>()->getTerrain();
    auto idd = obj.getId();
    IComponent::Pointer terrainComp;

    auto list = PhysicsManager::getInstance().getMovingCharacterList();
    for(int i = 0; i < list.size(); i++)
    {
        if(list[i].moveComponent.get()->getGameObject().getId() == idd) 
            terrainComp = list[i].terrainComponent;
    }


    auto terrainComponent = obj.getComponent<TerrainComponent>();

    RenderManager::getInstance().createObjectRenderComponent(*ob.get(), ObjectRenderComponent::Shape::Cube);
    std::shared_ptr<IComponent> collision = PhysicsManager::getInstance().createCollisionComponent(*ob.get(), 2, false, CollisionComponent::Type::RedShell);

    std::shared_ptr<IComponent> move = PhysicsManager::getInstance().createMoveComponent(*ob.get(), mData, terrain, 1);
    PhysicsManager::getInstance().createMovingCharacter(move, terrainComp, collision);
    WaypointManager::getInstance().createPathPlanningComponent(ob);

    AIManager::getInstance().createAIDrivingComponent(*ob.get());
    SensorManager::getInstance().createVSensorComponent(*ob.get(), 55.f, obj.getComponent<MoveComponent>()->getMovemententData().angle);

    ItemComponents.push_back(std::dynamic_pointer_cast<IItemComponent>(component));

    return component;
}

IComponent::Pointer ItemManager::createBlueShell(GameObject& obj)
{
    uint16_t id = 6000 + ItemComponents.size();
    GameObject::TransformationData transform;

    auto pos = obj.getTransformData().position;

    transform.position = glm::vec3(pos.x+20*cos(obj.getTransformData().rotation.y),
                                    pos.y, pos.z-20*sin(obj.getTransformData().rotation.y));
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(0.2, 0.2, 0.2);

    auto ob = ObjectManager::getInstance().createObject(id, transform);

    IComponent::Pointer component = std::make_shared<ItemBlueShellComponent>(*ob.get(), obj);

    ob.get()->addComponent(component);

    LAPAL::movementData mData;
    mData.mov = false;
    mData.jump = false;
    mData.spi = false;
    mData.angInc = 0;
    mData.angle = obj.getComponent<MoveComponent>()->getMovemententData().angle;
    mData.spin = 0;
    mData.spin_inc = 1;
    mData.max_spin = 1;
    mData.brake_spin = 0.2;
    mData.rotateX = 0.f;
    mData.rotateZ = 0.f;
    mData.rotate_inc = 0.15f;
    mData.max_rotate = 3.f;
    mData.vel = 200.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;
    mData.max_vel = 200.0f + obj.getComponent<MoveComponent>()->getMovemententData().vel;
    mData.brake_vel = 0.f;
    mData.velY = 200.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;;
    mData.acc = 200.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;;
    mData.max_acc = 200.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;;
    mData.dAcc = 200.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;;
    mData.brake_acc = 200.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;;


    auto terrain = obj.getComponent<MoveComponent>()->getTerrain();
    auto idd = obj.getId();
    IComponent::Pointer terrainComp;

    auto list = PhysicsManager::getInstance().getMovingCharacterList();
    for(int i = 0; i < list.size(); i++)
    {
        if(list[i].moveComponent.get()->getGameObject().getId() == idd) 
            terrainComp = list[i].terrainComponent;
    }


    auto terrainComponent = obj.getComponent<TerrainComponent>();

    RenderManager::getInstance().createObjectRenderComponent(*ob.get(), ObjectRenderComponent::Shape::Cube);
    std::shared_ptr<IComponent> collision = PhysicsManager::getInstance().createCollisionComponent(*ob.get(), 2, false, CollisionComponent::Type::BlueShell);

    std::shared_ptr<IComponent> move = PhysicsManager::getInstance().createMoveComponent(*ob.get(), mData, terrain, 1);
    PhysicsManager::getInstance().createMovingCharacter(move, terrainComp, collision);
    WaypointManager::getInstance().createPathPlanningComponent(ob);

    AIManager::getInstance().createAIDrivingComponent(*ob.get());
    SensorManager::getInstance().createVSensorComponent(*ob.get(), 55.f, obj.getComponent<MoveComponent>()->getMovemententData().angle);

    ItemComponents.push_back(std::dynamic_pointer_cast<IItemComponent>(component));

    return component;
}

IComponent::Pointer ItemManager::createBanana(GameObject& obj)
{
    uint16_t id = 6000 + ItemComponents.size();
    GameObject::TransformationData transform;

    auto pos = obj.getTransformData().position;

    transform.position = glm::vec3(pos.x-10*cos(obj.getTransformData().rotation.y),
                                    pos.y, pos.z+10*sin(obj.getTransformData().rotation.y));
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(0.2, 0.2, 0.2);

    auto ob = ObjectManager::getInstance().createObject(id, transform);

    IComponent::Pointer component = std::make_shared<ItemBananaComponent>(*ob.get());

    ob.get()->addComponent(component);

    RenderManager::getInstance().createObjectRenderComponent(*ob.get(), ObjectRenderComponent::Shape::Cube);
    PhysicsManager::getInstance().createCollisionComponent(*ob.get(), 1, false, CollisionComponent::Type::Banana);


    ItemComponents.push_back(std::dynamic_pointer_cast<IItemComponent>(component));

    return component;
}

IComponent::Pointer ItemManager::createMushroom(GameObject& obj)
{
    uint16_t id = 6000 + ItemComponents.size();
    GameObject::TransformationData transform;

    auto pos = obj.getTransformData().position;

    transform.position = glm::vec3(pos.x, pos.y, pos.z);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);

    auto ob = ObjectManager::getInstance().createObject(id, transform);

    IComponent::Pointer component = std::make_shared<ItemMushroomComponent>(*ob.get(), obj);

    ob.get()->addComponent(component);

    ItemComponents.push_back(std::dynamic_pointer_cast<IItemComponent>(component));

    return component;
}

IComponent::Pointer ItemManager::createStar(GameObject& obj)
{
    uint16_t id = 6000 + ItemComponents.size();
    GameObject::TransformationData transform;

    auto pos = obj.getTransformData().position;

    transform.position = glm::vec3(pos.x, pos.y, pos.z);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);

    auto ob = ObjectManager::getInstance().createObject(id, transform);

    IComponent::Pointer component = std::make_shared<ItemStarComponent>(*ob.get(), obj);

    ob.get()->addComponent(component);

    ItemComponents.push_back(std::dynamic_pointer_cast<IItemComponent>(component));

    return component;
}

//////////////////////////////////////////////////////
/////
/////       ITEM DELETE
/////
//////////////////////////////////////////////////////

void ItemManager::deleteItem(IComponent::Pointer component)
{

    EventData data;
    data.Id = component->getGameObject().getId();

    EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});

}

//////////////////////////////////////////////////////
/////
/////       DELEGATES
/////
//////////////////////////////////////////////////////

void objectDeleteItem(EventData eData) {

    auto& itemComponentList = ItemManager::getInstance().getItemComponents();

    for(unsigned int i = 0; i<itemComponentList.size(); ++i) {
        if(eData.Id == itemComponentList.at(i).get()->getGameObject().getId()) {
            itemComponentList.erase(itemComponentList.begin() + i);
            return;
        }
    }
}

void objectDeleteHolder(EventData eData) {

    auto& holderComponentList = ItemManager::getInstance().getItemHolderComponents();

    for(unsigned int i = 0; i<holderComponentList.size(); ++i) {
        if(eData.Id == holderComponentList.at(i).get()->getGameObject().getId()) {
            holderComponentList.erase(holderComponentList.begin() + i);
            return;
        }
    }
}
