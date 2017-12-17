#include "ItemMushroomComponent.h"



ItemMushroomComponent::ItemMushroomComponent(GameObject& newGameObject, GameObject::Pointer obj) : IItemComponent(newGameObject)
{
    player = obj;
}

ItemMushroomComponent::~ItemMushroomComponent()
{

}

void ItemMushroomComponent::init()
{
    player->getComponent<MoveComponent>()->changeMaxSpeedOverTime(200.f, 0.1f, 2.f);
}

void ItemMushroomComponent::update(float dTime)
{
    
}

void ItemMushroomComponent::close()
{
    
} 
 
