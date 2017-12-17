#pragma once

#include "GameObject.h"
#include "IItemComponent.h"

class ItemComponent : public IItemComponent {

    public:

        ItemComponent(GameObject& newGameObject) : IItemComponent(newGameObject) {}

        virtual ~ItemComponent() {};

        virtual void init();

        virtual void update();

        private:

};
