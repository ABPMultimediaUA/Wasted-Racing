#pragma once

#include "IItemComponent.h"

class ItemStarComponent : public IItemComponent
{

    private:


    public:

        ItemStarComponent(GameObject& newGameObject);

        ~ItemStarComponent();

        void init();

        void update(float dTime);

        void close();

};
 

 
 
