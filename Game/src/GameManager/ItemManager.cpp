 #include "ItemManager.h"



void ItemManager::init(){

}

void ItemManager::update(){

}

//Instance getter
static ItemManager& getInstance(){
    static ItemManager instance;
    return instance;
}


//ItemHolderComponent Creator
IComponent::Pointer ItemManager::createItemHolderComponent(GameObject& obj){

    


}

