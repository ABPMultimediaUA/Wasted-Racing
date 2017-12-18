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

        //Getter
        virtual int getItemType(){
            return itemType;
        }

        //Setter
        virtual void setItemType(int i){
            itemType = i;
        }

    private:

        int itemType = -1;


};