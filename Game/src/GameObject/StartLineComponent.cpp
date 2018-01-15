#include "StartLineComponent.h"
#include <iostream>

StartLineComponent::StartLineComponent(GameObject& newGameObject) : IComponent(newGameObject)
{
    active = false;
}