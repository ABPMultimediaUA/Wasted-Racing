#pragma once

#include "IItemComponent.h"

class ItemMushroomComponent : public IItemComponent
{

    private:


    public:

        ItemMushroomComponent(GameObject& newGameObject);

        ~ItemMushroomComponent();

        void init();

        void update(float dTime);

        void close();

};
 

 
