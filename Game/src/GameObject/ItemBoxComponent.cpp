#include "ItemBoxComponent.h"
#include "GameObject.h"
#include "ItemHolderComponent.h"

void ItemBoxComponent::init(){

} 

void ItemBoxComponent::update(float dTime){
    
}

void ItemBoxComponent::close(){
    
}

void ItemBoxComponent::deactivateBox(){

    auto trans = getGameObject().getTransformData();

    trans.scale.x = 0;
    trans.scale.y = 0;
    trans.scale.z = 0;

    getGameObject().setTransformData(trans);

}

void ItemBoxComponent::asignItem(GameObject& obj){

//    auto itemHolder = obj.getComponent<ItemHolderComponent>();


}
