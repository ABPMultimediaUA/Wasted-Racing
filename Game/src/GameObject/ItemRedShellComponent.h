#pragma once

#include "IItemComponent.h"

class ItemRedShellComponent : public IItemComponent
{

    private:

        float speed;

    public:

        ItemRedShellComponent(GameObject& newGameObject, GameObject::Pointer obj);

        ~ItemRedShellComponent();

        void init();

        void update(float dTime);

        void close();

};
 

 
 
