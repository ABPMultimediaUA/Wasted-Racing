#include "ItemMushroomComponent.h"



ItemMushroomComponent::ItemMushroomComponent(GameObject& newGameObject, GameObject::Pointer obj) : IItemComponent(newGameObject)
{
    player = obj;
    speed = 100.f;
    consTime = 0.1f;
    decTime = 2.f;
}

ItemMushroomComponent::~ItemMushroomComponent()
{

}

void ItemMushroomComponent::init()
{
    player->getComponent<MoveComponent>()->changeMaxSpeedOverTime(speed, consTime, decTime);
}

void ItemMushroomComponent::update(float dTime)
{
    
}

void ItemMushroomComponent::close()
{
    
} 
 
