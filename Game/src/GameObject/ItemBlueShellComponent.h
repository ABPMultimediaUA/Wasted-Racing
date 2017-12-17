#pragma once

#include "IItemComponent.h"

class ItemBlueShellComponent : public IItemComponent
{

    private:


    public:

        ItemBlueShellComponent(GameObject& newGameObject);

        ~ItemBlueShellComponent();

        void init();

        void update(float dTime);

        void close();

};
 

