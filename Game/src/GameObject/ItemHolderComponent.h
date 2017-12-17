#pragma once

#include "IComponent.h"
#include "GameObject.h"

class ItemHolderComponent : public IComponent {

    public:

        //Constructor
        ItemHolderComponent(GameObject& newGameObject);

        //Destructor
        ~ItemHolderComponent() {}

        //Initializer
        virtual void init();

        //Update
        virtual void update(float dTime);

        //Closer
        virtual void close();


};