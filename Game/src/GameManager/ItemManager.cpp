#include "ItemManager.h"



void createItemEvent(EventData eData);
void objectDeletedBanana(EventData eData);
void objectDeletedMushroom(EventData eData);
void objectDeletedStar(EventData eData);

ItemManager::ItemManager()
{

}

//Instance getter
ItemManager& ItemManager::getInstance(){
    static ItemManager instance;
    return instance;
}

void ItemManager::init(){

    EventManager::getInstance().addListener(EventListener {EventType::Key_UseItem_Down, createItemEvent});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeletedBanana});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeletedMushroom});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeletedStar});

}

void ItemManager::update(){

}

void ItemManager::close(){

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
/////       ITEM CREATOR
/////
//////////////////////////////////////////////////////

IComponent::Pointer ItemManager::createItem(GameObject::Pointer obj){

    //int random = rand() % 5;
    int random = 3;
    if(random == IItemComponent::ItemType::redShell)
    {
        return createRedShell(obj);
    }
    else if(random == IItemComponent::ItemType::blueShell)
    {
        return createBlueShell(obj);
    }
    else if(random == IItemComponent::ItemType::banana)
    {
        return createBanana(obj);
    }
    else if(random == IItemComponent::ItemType::mushroom)
    {
        auto component = createMushroom(obj);
        std::dynamic_pointer_cast<ItemMushroomComponent>(component)->init();
        deleteMushroom(component);
        return component;
    }
    else if(random == IItemComponent::ItemType::star)
    {
        auto component = createStar(obj);
        std::dynamic_pointer_cast<ItemStarComponent>(component)->init();
        deleteStar(component);
        return component;
    }
    return nullptr;
}


//////////////////////////////////////////////////////
/////
/////       ITEM CREATE
/////
//////////////////////////////////////////////////////


IComponent::Pointer ItemManager::createRedShell(GameObject::Pointer obj)
{
    uint16_t id = 6000 + ItemComponents.size();
    GameObject::TransformationData transform;

    auto pos = obj.get()->getTransformData().position;

    transform.position = glm::vec3(pos.x, pos.y, pos.z);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);

    auto ob = ObjectManager::getInstance().createObject(id, transform);

    IComponent::Pointer component = std::make_shared<ItemRedShellComponent>(*ob.get());

    ob.get()->addComponent(component);

    ItemComponents.push_back(std::dynamic_pointer_cast<IItemComponent>(component));

    return component;
}

IComponent::Pointer ItemManager::createBlueShell(GameObject::Pointer obj)
{
    uint16_t id = 6000 + ItemComponents.size();
    GameObject::TransformationData transform;

    auto pos = obj.get()->getTransformData().position;

    transform.position = glm::vec3(pos.x, pos.y, pos.z);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);

    auto ob = ObjectManager::getInstance().createObject(id, transform);

    IComponent::Pointer component = std::make_shared<ItemBlueShellComponent>(*ob.get());

    ob.get()->addComponent(component);

    ItemComponents.push_back(std::dynamic_pointer_cast<IItemComponent>(component));

    return component;
}

IComponent::Pointer ItemManager::createBanana(GameObject::Pointer obj)
{
    uint16_t id = 6000 + ItemComponents.size();
    GameObject::TransformationData transform;

    auto pos = obj.get()->getTransformData().position;

    transform.position = glm::vec3(pos.x-10*cos(obj.get()->getTransformData().rotation.y * M_PI/180),
                                    pos.y, pos.z+10*sin(obj.get()->getTransformData().rotation.y * M_PI/180));
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(0.1, 0.1, 0.1);

    auto ob = ObjectManager::getInstance().createObject(id, transform);

    IComponent::Pointer component = std::make_shared<ItemBananaComponent>(*ob.get());

    ob.get()->addComponent(component);

    RenderManager::getInstance().createObjectRenderComponent(*ob.get(), ObjectRenderComponent::Shape::Cube);
    PhysicsManager::getInstance().createCollisionComponent(*ob.get(), 1, false, CollisionComponent::Type::Banana);


    ItemComponents.push_back(std::dynamic_pointer_cast<IItemComponent>(component));

    return component;
}

IComponent::Pointer ItemManager::createMushroom(GameObject::Pointer obj)
{
    uint16_t id = 6000 + ItemComponents.size();
    GameObject::TransformationData transform;

    auto pos = obj.get()->getTransformData().position;

    transform.position = glm::vec3(pos.x, pos.y, pos.z);
    transform.rotation = glm::vec3(0, 0, 0);
    transform.scale    = glm::vec3(1, 1, 1);

    auto ob = ObjectManager::getInstance().createObject(id, transform);

    IComponent::Pointer component = std::make_shared<ItemMushroomComponent>(*ob.get(), obj);

    ob.get()->addComponent(component);

    ItemComponents.push_back(std::dynamic_pointer_cast<IItemComponent>(component));

    return component;
}

IComponent::Pointer ItemManager::createStar(GameObject::Pointer obj)
{
    uint16_t id = 6000 + ItemComponents.size();
    GameObject::TransformationData transform;

    auto pos = obj.get()->getTransformData().position;

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

void ItemManager::deleteMushroom(IComponent::Pointer component)
{

    EventData data;
    data.Id = component->getGameObject().getId();

    EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});

}

void ItemManager::deleteStar(IComponent::Pointer component)
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

void createItemEvent(EventData eData)
{
    ItemManager::getInstance().createItem(eData.Object);
}

void objectDeletedBanana(EventData eData) {

    auto bananaComponentList = ItemManager::getInstance().getItemComponents();

    for(unsigned int i = 0; i<bananaComponentList.size(); ++i) {
        if(eData.Id == bananaComponentList.at(i).get()->getGameObject().getId()) {
            bananaComponentList.erase(bananaComponentList.begin() + i);
            return;
        }
    }
}

void objectDeletedMushroom(EventData eData) {

    auto mushroomComponentList = ItemManager::getInstance().getItemComponents();

    for(unsigned int i = 0; i<mushroomComponentList.size(); ++i) {
        if(eData.Id == mushroomComponentList.at(i).get()->getGameObject().getId()) {
            mushroomComponentList.erase(mushroomComponentList.begin() + i);
            return;
        }
    }
}

void objectDeletedStar(EventData eData) {

    auto starComponentList = ItemManager::getInstance().getItemComponents();

    for(unsigned int i = 0; i<starComponentList.size(); ++i) {
        if(eData.Id == starComponentList.at(i).get()->getGameObject().getId()) {
            starComponentList.erase(starComponentList.begin() + i);
            return;
        }
    }
}

