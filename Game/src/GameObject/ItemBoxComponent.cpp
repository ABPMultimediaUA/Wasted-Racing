#include "ItemBoxComponent.h"
#include "GameObject.h"
#include "ItemHolderComponent.h"
#include "../GameManager/RenderManager.h"


//Constructor
ItemBoxComponent::ItemBoxComponent(GameObject& newGameObject, float dTime) : IComponent(newGameObject){

    boxTime = dTime;
    actTime = dTime;

}

void ItemBoxComponent::init(){

} 

void ItemBoxComponent::update(float dTime){

    if(getGameObject().getTransformData().scale.x == 0){
            actTime -= dTime;
            std::cout << "entro y actTime es " << actTime << std::endl;
    }

    if(actTime == 0){

        actTime = boxTime;
        auto trans = getGameObject().getTransformData();

        trans.scale.x = 1;
        trans.scale.y = 1;
        trans.scale.z = 1;

        getGameObject().setTransformData(trans);
        RenderManager::getInstance().getRenderFacade()->updateObjectTransform(getGameObject().getId(), trans);

    }

}

void ItemBoxComponent::close(){
    
}

void ItemBoxComponent::deactivateBox(){

    auto trans = getGameObject().getTransformData();

    trans.scale.x = 0;
    trans.scale.y = 0;
    trans.scale.z = 0;

    getGameObject().setTransformData(trans);
    RenderManager::getInstance().getRenderFacade()->updateObjectTransform(getGameObject().getId(), trans);

}

void ItemBoxComponent::asignItem(GameObject& obj){

    auto itemHolder = obj.getComponent<ItemHolderComponent>();

    if(itemHolder->getItemType() == -1){
        int random = rand() % 2 + 2;
        itemHolder->setItemType(random);
    }

}
