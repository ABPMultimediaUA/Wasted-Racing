#include "ItemBananaComponent.h"



ItemBananaComponent::ItemBananaComponent(GameObject& newGameObject) : IItemComponent(newGameObject)
{
    speed = 5.f;
    consTime = 0.1f;
    decTime = 1.f;
}

ItemBananaComponent::~ItemBananaComponent()
{

}

void ItemBananaComponent::init()
{

}

void ItemBananaComponent::update(float dTime)
{
    
}

void ItemBananaComponent::close()
{
    
}
