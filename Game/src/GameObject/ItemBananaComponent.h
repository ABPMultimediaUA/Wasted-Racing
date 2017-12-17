 #pragma once

#include "IItemComponent.h"

class ItemBananaComponent : public IItemComponent
{

    private:


    public:

        ItemBananaComponent(GameObject& newGameObject);

        virtual ~ItemBananaComponent();

        virtual void init();

        virtual void update(float dTime);

        virtual void close();

};
 
