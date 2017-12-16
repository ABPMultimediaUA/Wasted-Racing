#include "ItemManager.h"



ItemManager::ItemManager()
{

}

//Instance getter
ItemManager& ItemManager::getInstance(){
    static ItemManager instance;
    return instance;
}

void ItemManager::init(){

}

void ItemManager::update(){

}

void ItemManager::close(){

}


//ItemHolderComponent Creator
IComponent::Pointer ItemManager::createItemHolderComponent(GameObject::Pointer newGameObject){

    IComponent::Pointer component;
    component = std::make_shared<ItemHolderComponent>(*newGameObject.get());
    
    newGameObject.get()->addComponent(component);

    ItemHolders.push_back(newGameObject);

    return component;
}
