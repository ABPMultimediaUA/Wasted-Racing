 #pragma once

#include "IComponent.h"
#include "GameObject.h"

class IItemComponent : public IComponent {

    public:

    IItemComponent(GameObject& newGameObject) : IComponent(newGameObject) {}

};
