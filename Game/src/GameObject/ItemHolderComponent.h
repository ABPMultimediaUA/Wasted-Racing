#pragma once

#include "GameObject.h"
#include "IComponent.h"

class ItemHolderComponent : public IComponent {

    ItemHolderComponent(GameObject& newGameObject) : IComponent(newGameObject) {}

    virtual ~ItemHolderComponent() {};

    virtual void init();

    virtual void update();

};