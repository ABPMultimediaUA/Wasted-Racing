#pragma once

#include "IComponent.h"
#include "GameObject.h"

class ItemHolderComponent : public IComponent {

    public:
        ItemHolderComponent(GameObject& newGameObject);

        ~ItemHolderComponent() {}

        virtual void init();

        virtual void update(float dTime);

        virtual void close();

};