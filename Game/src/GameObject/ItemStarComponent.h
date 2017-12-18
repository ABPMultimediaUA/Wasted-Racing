#pragma once

#include "IItemComponent.h"
#include "MoveComponent.h"

class ItemStarComponent : public IItemComponent
{

    private:

        GameObject::Pointer player;
        float speed;
        float consTime;
        float decTime;

    public:

        ItemStarComponent(GameObject& newGameObject, GameObject::Pointer obj);

        ~ItemStarComponent();

        void init();

        void update(float dTime);

        void close();

};
 

 
 
