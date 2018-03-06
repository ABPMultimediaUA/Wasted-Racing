#include "ItemBoxComponent.h"


//Constructor
ItemBoxComponent::ItemBoxComponent(GameObject& newGameObject) : IComponent(newGameObject){

    boxTime = 5.f;
    actTime = 5.f;

}

void ItemBoxComponent::init(){

} 

void ItemBoxComponent::update(float dTime){

    //Rotate the object (rotating motion)
    auto trans = getGameObject().getTransformData();
    trans.rotation.y += 1*M_PI/180;
    getGameObject().setTransformData(trans);

    //Update in render if this is not the server
    //____>AÑADIR EVENTO MÁGICO QUE UPDATEE ESTO PA QUE NO HAYAN ERRORES MACHO Q MIRA EH
    //RenderManager::getInstance().getRenderFacade()->updateObjectTransform(getGameObject().getId(), trans);

    if(getGameObject().getTransformData().scale.x == 0){
            actTime -= dTime;
    }

    if(actTime <= 0){

        active = 1;

        actTime = boxTime;
        auto trans = getGameObject().getTransformData();

        trans.scale.x = 0.5;
        trans.scale.y = 0.5;
        trans.scale.z = 0.5;

        getGameObject().setTransformData(trans);
        //RenderManager::getInstance().getRenderFacade()->updateObjectTransform(getGameObject().getId(), trans);

    }

}

void ItemBoxComponent::close(){
    
}

void ItemBoxComponent::deactivateBox(){

    active = 0;

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
        srand (time(NULL));
        int random;
        if(obj.getComponent<ScoreComponent>()->getPosition() == 1)
        {
            random = rand() % 3 + 2;
        }
        else
        {
            random = rand() % 5;
        }

        itemHolder->setItemType(random);
        
    }

}
