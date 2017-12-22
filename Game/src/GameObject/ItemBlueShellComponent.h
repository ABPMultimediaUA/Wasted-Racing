#pragma once

#include "IItemComponent.h"
#include "../GameManager/ScoreManager.h"
#include "PathPlanningComponent.h"
#include "VSensorComponent.h"
#include "MoveComponent.h"
#include "AIDrivingComponent.h"

class ItemBlueShellComponent : public IItemComponent
{

    private:

        GameObject& player;
        float speed;
        float consTime;
        float decTime;
        unsigned int lastVector;
        glm::vec3 objective;
        ScoreComponent::Pointer enemy;

    public:

        ItemBlueShellComponent(GameObject& newGameObject, GameObject& obj);

        ~ItemBlueShellComponent();

        void init();

        void update(float dTime);

        void close();

        //Getters
        float getSpeed()        {       return speed;       };
        float getConsTime()     {       return consTime;    };
        float getDecTime()      {       return decTime;     };

};
 

