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
    trans.rotation.y += 1*3.141592653589793/180;
    getGameObject().setTransformData(trans);

    //____>AÑADIR EVENTO MÁGICO QUE UPDATEE ESTO PA QUE NO HAYAN ERRORES MACHO Q MIRA EH
    //RenderManager::getInstance().getRenderFacade()->updateObjectTransform(getGameObject().getId(), trans);
    //Update the render
    EventData data;
    data.Id = getGameObject().getId();
    data.Vector = trans.rotation;
    EventManager::getInstance().addEvent(Event {EventType::Update_Transform_Rotation, data});


    if(getGameObject().getTransformData().scale.x == 0){
            actTime -= dTime;
    }

    if(actTime <= 0){

        active = 1;

        actTime = boxTime;
        auto trans = getGameObject().getTransformData();

        trans.scale.x = 0.65;
        trans.scale.y = 0.65;
        trans.scale.z = 0.65;

        getGameObject().setTransformData(trans);
        //RenderManager::getInstance().getRenderFacade()->updateObjectTransform(getGameObject().getId(), trans);

        //Update render
        EventData data;
        data.Id = getGameObject().getId();
        data.Vector = trans.scale;
        EventManager::getInstance().addEvent(Event {EventType::Update_Transform_Scale, data});
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

    //Update render
    EventData data;
    data.Id = getGameObject().getId();
    data.Vector = trans.scale;
    EventManager::getInstance().addEvent(Event {EventType::Update_Transform_Scale, data});

    //RenderManager::getInstance().getRenderFacade()->updateObjectTransform(getGameObject().getId(), trans);

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
