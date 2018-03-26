#include "ItemManager.h"
#include "../GlobalVariables.h"
//////////////////////////////////////////////
//            THINGS TO DO HERE
//////////////////////////////////////////////
//////////////////////////////////////////////
/*
+>CLEAN CREATION FUNCTIONS
>ADD Remote component events to create them later
*/
//////////////////////////////////////////////
//////////////////////////////////////////////
//==============================================
// DELEGATES DECLARATIONS
//==============================================
void createItemEvent(EventData eData);
void objectDeleteItem(EventData eData);
void objectDeleteHolder(EventData);

//==============================================
// MAIN FUNCTIONS
//==============================================
ItemManager::ItemManager()
{
    //Initial ID of every item
    ids = 6000;
}

//Instance getter
ItemManager& ItemManager::getInstance(){
    static ItemManager instance;
    return instance;
}

void ItemManager::init(){
    //Global Variables holder
    globalVariables = &GlobalVariables::getInstance();

    //Initalize listeners
    EventManager::getInstance().addListener(EventListener {EventType::Item_Create, createItemEvent});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeleteItem});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeleteHolder});

}

void ItemManager::update(float dTime){
    
    for(unsigned int i = 0; i < ItemBoxes.size() ; i++){
        ItemBoxes[i]->update(dTime);
    }

    for(unsigned int i = 0; i < ItemComponents.size() ; i++){
        ItemComponents[i]->update(dTime);
    }
}

void ItemManager::close(){
    ItemHolders.clear();
    ItemComponents.clear();        
    ItemBoxes.clear();
}


//==============================================
// COMPONENT CREATOR
//==============================================

IComponent::Pointer ItemManager::createItemHolderComponent(GameObject& newGameObject){
    //Make the shared pointer of the item holder
    IComponent::Pointer component = std::make_shared<ItemHolderComponent>(newGameObject);
    
    //Attach to game object
    newGameObject.addComponent(component);

    //Add to list of item holders
    //:::>It should be an event in the future with scheduling
    ItemHolders.push_back(std::dynamic_pointer_cast<ItemHolderComponent>(component));

    return component;
}

//==============================================
// BOX CREATOR
//==============================================

IComponent::Pointer ItemManager::createItemBox(GameObject& obj){

    //Create sharerd pointer
    IComponent::Pointer component = std::make_shared<ItemBoxComponent>(obj);

    //Add to object
    obj.addComponent(component);

    //Add to list of item boxes
    ItemBoxes.push_back(std::dynamic_pointer_cast<ItemBoxComponent>(component));
    
    //____>Not needed now until scheduling
    //:::>Not needed
    //EventData data;
    //data.Component = component;
    //EventManager::getInstance().addEvent(Event {EventType::ItemBoxComponent_Create, data});

    //Create render component
    RenderManager::getInstance().createObjectRenderComponent(obj, ObjectRenderComponent::Shape::Cube, "itemBox.jpg");

    //Create collision component
    PhysicsManager::getInstance().createCollisionComponent(obj, 5, 5, false, CollisionComponent::Type::ItemBox);


    return component;
}


//==============================================
// ITEM CREATOR
//==============================================

IComponent::Pointer ItemManager::createItem(GameObject& obj){

    //Get item
    auto itemHolder = obj.getComponent<ItemHolderComponent>();
    int random = itemHolder->getItemType();

    //-----------------------
    //Generate the right item
    //-----------------------
    //Red shell item
    if(random == IItemComponent::ItemType::redShell)
    {
        //Set item to no item
        itemHolder->setItemType(-1);

        //Send event of creation
        EventData eData;
        EventManager::getInstance().addEvent(Event {EventType::RedShell_Create, eData});
        
        //Create item and initialize it
        auto component = createRedShell(obj, 0);
        std::dynamic_pointer_cast<ItemRedShellComponent>(component)->init();     
        return component;
    }

    //Blue shell item
    else if(random == IItemComponent::ItemType::blueShell)
    {
        //Set item to no item
        itemHolder->setItemType(-1);

        //Send event of creation
        EventData eData;
        EventManager::getInstance().addEvent(Event {EventType::BlueShell_Create, eData});

        //Create item and initialize it
        auto component = createBlueShell(obj, 0);
        std::dynamic_pointer_cast<ItemBlueShellComponent>(component)->init();
        return component;
    }

    //Trap item
    else if(random == IItemComponent::ItemType::trap)
    {
        //Set item to no item
        itemHolder->setItemType(-1);

        //Send event of creation
        EventData eData;
        EventManager::getInstance().addEvent(Event {EventType::Trap_Create, eData});

        //Return initialization of item
        return createTrap(obj, 0);
    }

    //Mushroom item
    else if(random == IItemComponent::ItemType::mushroom)
    {
        //Set item to no item
        itemHolder->setItemType(-1);

        //Create item and initialize it
        auto component = createMushroom(obj);
        std::dynamic_pointer_cast<ItemMushroomComponent>(component)->init();

        //Item is instantly used, so it must be erased
        deleteItem(component);
    }

    //Star item
    else if(random == IItemComponent::ItemType::star)
    {
        //Set item to no item
        itemHolder->setItemType(-1);
        auto component = createStar(obj);

        //Create item and initialize it
        std::dynamic_pointer_cast<ItemStarComponent>(component)->init();

        //Item is instantly used, so it must be erased
        deleteItem(component);
    }
    return nullptr;
}


//==============================================
// ITEM CREATOR DERIVATED FUNCTIONS
//==============================================

IComponent::Pointer ItemManager::createRedShell(GameObject& obj, int mode)
{
    //Initial Data
    uint16_t id = ItemManager::ids;
    GameObject::TransformationData transform;

    //Add total ids
    ItemManager::ids++;

    //Get launcher object transform
    auto pos = obj.getTransformData().position;

    //Set object offset position
    //:::> Hardcoded data makes jesus cry
    transform.position = glm::vec3(pos.x+20*cos(obj.getTransformData().rotation.y),
                                    pos.y, pos.z-20*sin(obj.getTransformData().rotation.y));
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(2,2,2);

    //Create object
    auto ob = ObjectManager::getInstance().createObject(id, transform);

    //Make red shell component and add it to the object
    IComponent::Pointer component = std::make_shared<ItemRedShellComponent>(*ob.get(), obj);
    ob.get()->addComponent(component);

    //Movement data
    //:::> TOO MUCH HARDCODE
    LAPAL::movementData mData;
    mData.mov = false;
    mData.jump = false;
    mData.spi = false;
    mData.angle = obj.getComponent<MoveComponent>()->getMovemententData().angle;
    mData.spin = 0;
    mData.spin_inc = 0;
    mData.max_spin = 1;
    mData.brake_spin = 0.2;
    mData.rotateX = 0.f;
    mData.rotateZ = 0.f;
    mData.rotate_inc = 0.15f;
    mData.max_rotate = 3.f;
    mData.vel = 300.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;
    mData.max_vel = 300.0f + obj.getComponent<MoveComponent>()->getMovemententData().vel;
    mData.brake_vel = 0.f;
    mData.velY = 300.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;
    mData.acc = 300.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;
    mData.max_acc = 300.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;
    mData.dAcc = 300.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;
    mData.brake_acc = 300.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;

    //Initial component data
    auto terrain = obj.getComponent<MoveComponent>()->getTerrain();
    auto idd = obj.getId();
    IComponent::Pointer terrainComp;

    //Get terrain component of the terrain in which the object is located
    auto list = PhysicsManager::getInstance().getMovingCharacterList();
    for(unsigned int i=0; i < list.size(); i++)
    {
        if(list[i].moveComponent.get()->getGameObject().getId() == idd) 
            terrainComp = list[i].terrainComponent;
    }
    //:::> wtf is this doing here
    //<___
    //auto terrainComponent = obj.getComponent<TerrainComponent>();
    //___>
    //--------------------------
    //Online dependant instances
    //Create the network component with the type of the object assigned

    //<___
    /*if(globalVariables->getOnline())
    {
        NetworkManager::getInstance().createRemoteItemComponent(*ob.get(), 1);
    }*/

    //Create remote object type
    if(mode == 1)
    {
        NetworkManager::getInstance().createRemoteItemComponent(*ob.get(), 1);

        //--------------------------
        //Create collision component
        RenderManager::getInstance().createObjectRenderComponent(*ob.get(), ObjectRenderComponent::Shape::Mesh, "ball.obj");
        
        //Create move component with the movement data
        std::shared_ptr<IComponent> move = PhysicsManager::getInstance().createMoveComponent(*ob.get(), mData, terrain, 1);
        PhysicsManager::getInstance().createMovingCharacter(move, nullptr, nullptr);
    }
    else{

        //--------------------------
        //Create collision component
        RenderManager::getInstance().createObjectRenderComponent(*ob.get(), ObjectRenderComponent::Shape::Mesh, "ball.obj");

        //:::>Better use enumerators rather than hardcoded if's and types
        std::shared_ptr<IComponent> collision = PhysicsManager::getInstance().createCollisionComponent(*ob.get(), 2, 2, false, CollisionComponent::Type::RedShell);

        //Create move component with the movement data
        std::shared_ptr<IComponent> move = PhysicsManager::getInstance().createMoveComponent(*ob.get(), mData, terrain, 1);
        PhysicsManager::getInstance().createMovingCharacter(move, terrainComp, collision);

        //Create path planning component
        auto listNodes = WaypointManager::getInstance().getWaypoints();
        WaypointManager::getInstance().createPathPlanningComponent(ob, listNodes);

        //Create AI components
        //:::>No hardcoded variables
        AIManager::getInstance().createAIDrivingComponent(*ob.get());
        SensorManager::getInstance().createVSensorComponent(*ob.get(), 55.f, obj.getComponent<MoveComponent>()->getMovemententData().angle, 0.f, 0);
    }
    //___>

    //Push into the list
    ItemComponents.push_back(std::dynamic_pointer_cast<IItemComponent>(component));

    return component;
}

IComponent::Pointer ItemManager::createBlueShell(GameObject& obj, int mode)
{
    //Initial Data
    uint16_t id = ItemManager::ids;
    GameObject::TransformationData transform;
    ItemManager::ids++;

    //Get launcher object transform
    auto pos = obj.getTransformData().position;

    //Set object offset position
    //:::>No hardcode pls
    transform.position = glm::vec3(pos.x+20*cos(obj.getTransformData().rotation.y),
                                    pos.y, pos.z-20*sin(obj.getTransformData().rotation.y));
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(2,2,2);

    auto ob = ObjectManager::getInstance().createObject(id, transform);

    IComponent::Pointer component = std::make_shared<ItemBlueShellComponent>(*ob.get(), obj);

    ob.get()->addComponent(component);

    LAPAL::movementData mData;
    mData.mov = false;
    mData.jump = false;
    mData.spi = false;
    mData.angle = obj.getComponent<MoveComponent>()->getMovemententData().angle;
    mData.spin = 0;
    mData.spin_inc = 0;
    mData.max_spin = 1;
    mData.brake_spin = 0.2;
    mData.rotateX = 0.f;
    mData.rotateZ = 0.f;
    mData.rotate_inc = 0.15f;
    mData.max_rotate = 3.f;
    mData.vel = 300.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;
    mData.max_vel = 300.0f + obj.getComponent<MoveComponent>()->getMovemententData().vel;
    mData.brake_vel = 0.f;
    mData.velY = 300.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;
    mData.acc = 300.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;
    mData.max_acc = 300.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;
    mData.dAcc = 300.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;
    mData.brake_acc = 300.f + obj.getComponent<MoveComponent>()->getMovemententData().vel;


    auto terrain = obj.getComponent<MoveComponent>()->getTerrain();
    auto idd = obj.getId();
    IComponent::Pointer terrainComp;

    auto list = PhysicsManager::getInstance().getMovingCharacterList();
    for(unsigned int i=0; i < list.size(); i++)
    {
        if(list[i].moveComponent.get()->getGameObject().getId() == idd) 
            terrainComp = list[i].terrainComponent;
    }


    auto terrainComponent = obj.getComponent<TerrainComponent>();

    //--------------------------
    //Online dependant instances
    //Create the network component with the type of the object assigned
    if(globalVariables->getOnline())
    {
        NetworkManager::getInstance().createRemoteItemComponent(*ob.get(), 2);
    }

    //--------------------------

    RenderManager::getInstance().createObjectRenderComponent(*ob.get(), ObjectRenderComponent::Shape::Mesh, "ball.obj");
    std::shared_ptr<IComponent> collision = PhysicsManager::getInstance().createCollisionComponent(*ob.get(), 2, 2, false, CollisionComponent::Type::BlueShell);

    std::shared_ptr<IComponent> move = PhysicsManager::getInstance().createMoveComponent(*ob.get(), mData, terrain, 1);
    PhysicsManager::getInstance().createMovingCharacter(move, terrainComp, collision);


    //Create path planning component
    auto listNodes = WaypointManager::getInstance().getWaypoints();
    WaypointManager::getInstance().createPathPlanningComponent(ob, listNodes);

    AIManager::getInstance().createAIDrivingComponent(*ob.get());
    SensorManager::getInstance().createVSensorComponent(*ob.get(), 55.f, obj.getComponent<MoveComponent>()->getMovemententData().angle, 0, 0);

    //Add to list of items
    ItemComponents.push_back(std::dynamic_pointer_cast<IItemComponent>(component));

    return component;
}

IComponent::Pointer ItemManager::createTrap(GameObject& obj, int mode)
{
    //Initial Data
    uint16_t id = ItemManager::ids;
    GameObject::TransformationData transform;
    ItemManager::ids++;

    //Get launcher object transform
    auto pos = obj.getTransformData().position;

    //Set object offset position
    transform.position = glm::vec3(pos.x-10*cos(obj.getTransformData().rotation.y),
                                    pos.y, pos.z+10*sin(obj.getTransformData().rotation.y));
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);

    //Create object
    auto ob = ObjectManager::getInstance().createObject(id, transform);

    //Create trap component
    IComponent::Pointer component = std::make_shared<ItemTrapComponent>(*ob.get());

    //Add to object
    ob.get()->addComponent(component);

    //--------------------------
    //Online dependant instances
    //Create the network component with the type of the object assigned
    if(globalVariables->getOnline())
    {
        NetworkManager::getInstance().createRemoteItemComponent(*ob.get(), 0);
    }

    //--------------------------
    RenderManager::getInstance().createObjectRenderComponent(*ob.get(), ObjectRenderComponent::Shape::Mesh, "banana.obj");
    PhysicsManager::getInstance().createCollisionComponent(*ob.get(), 1, 1, false, CollisionComponent::Type::Trap);

    //add item component
    ItemComponents.push_back(std::dynamic_pointer_cast<IItemComponent>(component));

    return component;
}

IComponent::Pointer ItemManager::createMushroom(GameObject& obj)
{
    //Initial variables
    uint16_t id = ItemManager::ids;
    GameObject::TransformationData transform;
    //Add item ID
    ItemManager::ids++;

    //Get position from object
    auto pos = obj.getTransformData().position;

    //Set position of object
    transform.position = glm::vec3(pos.x, pos.y, pos.z);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);

    //Create object
    auto ob = ObjectManager::getInstance().createObject(id, transform);

    //Add item star component to the star object
    IComponent::Pointer component = std::make_shared<ItemMushroomComponent>(*ob.get(), obj);
    ob.get()->addComponent(component);

    //Add component to list of item components
    ItemComponents.push_back(std::dynamic_pointer_cast<IItemComponent>(component));

    return component;
}

IComponent::Pointer ItemManager::createStar(GameObject& obj)
{
    //Initial variables
    uint16_t id = ItemManager::ids;
    GameObject::TransformationData transform;
    //Add item ID
    ItemManager::ids++;

    //Get position from object
    auto pos = obj.getTransformData().position;

    //Set position of object
    transform.position = glm::vec3(pos.x, pos.y, pos.z);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);

    //Create object
    auto ob = ObjectManager::getInstance().createObject(id, transform);

    //Add item star component to the star object
    IComponent::Pointer component = std::make_shared<ItemStarComponent>(*ob.get(), obj);
    ob.get()->addComponent(component);

    //Add component to list of item components
    ItemComponents.push_back(std::dynamic_pointer_cast<IItemComponent>(component));

    return component;
}

//==============================================
// ITEM DELETE
//==============================================

void ItemManager::deleteItem(IComponent::Pointer component)
{
    //Send the event of deletion so the object manager erases them, with the id of the object that holds this component
    EventData data;
    data.Id = component->getGameObject().getId();
    EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});

}

//==============================================
// DELEGATES
//==============================================

void createItemEvent(EventData eData) {
    /*if(GlobalVariables::getInstance().getServer())
    {
        //Create item
        auto item = ItemManager::getInstance().createItem(eData.Component.get()->getGameObject());

        //If the item created is a normal object
        if(item != nullptr)
        {
            item.get()->getGameObject().getComponent<RemoteItemComponent>()->setServerId(eData.Id);
        }
    }
    else
    {*/
        //get the player with the input
        /*GameObject* player = GlobalVariables::getInstance().getPlayer();

        //Create the item
        ItemManager::getInstance().createItem(*player);*/
   // }
}

void objectDeleteItem(EventData eData) {

    //Get all items
    auto& itemComponentList = ItemManager::getInstance().getItemComponents();

    //Erase the element of the id given
    for(unsigned int i = 0; i<itemComponentList.size(); ++i) {
        if(eData.Id == itemComponentList[i].get()->getGameObject().getId()) {
            itemComponentList.erase(itemComponentList.begin() + i);
            return;
        }
    }
}

void objectDeleteHolder(EventData eData) {

    //get all holder components
    auto& holderComponentList = ItemManager::getInstance().getItemHolderComponents();

    //Erase the element of the id given
    for(unsigned int i = 0; i<holderComponentList.size(); ++i) {
        if(eData.Id == holderComponentList[i].get()->getGameObject().getId()) {
            holderComponentList.erase(holderComponentList.begin() + i);
            return;
        }
    }
}
