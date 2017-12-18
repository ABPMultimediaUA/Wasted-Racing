#include "ItemStarComponent.h"



ItemStarComponent::ItemStarComponent(GameObject& newGameObject, GameObject& obj) : IItemComponent(newGameObject), player(obj)
{
    speed = 200.f;
    consTime = 5.f;
    decTime = 0.5f;
}

ItemStarComponent::~ItemStarComponent()
{

}

void ItemStarComponent::init()
{
    player.getComponent<MoveComponent>()->changeMaxSpeedOverTime(200.f, 5.f, 0.5f);
}

void ItemStarComponent::update(float dTime)
{
    
}

void ItemStarComponent::close()
{
    
} 
 
