#pragma once

#include "IItemComponent.h"
#include "GameObject.h"
#include "MoveComponent.h"

class ItemMushroomComponent : public IItemComponent
{

    private:

        GameObject::Pointer player;

    public:

        ItemMushroomComponent(GameObject& newGameObject, GameObject::Pointer obj);

        virtual ~ItemMushroomComponent();

        virtual void init();

        virtual void update(float dTime);

        virtual void close();

};
 

 
