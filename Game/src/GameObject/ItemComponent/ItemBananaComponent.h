 #pragma once

#include "IItemComponent.h"

class ItemBananaComponent : public IItemComponent
{

    private:

        float speed;
        float consTime;
        float decTime;

    public:

        ItemBananaComponent(GameObject& newGameObject);

        virtual ~ItemBananaComponent();

        virtual void init();

        virtual void update(float dTime);

        virtual void close();

        //Getters
        float getSpeed()        {       return speed;       };
        float getConsTime()     {       return consTime;    };
        float getDecTime()      {       return decTime;     };

};
 
