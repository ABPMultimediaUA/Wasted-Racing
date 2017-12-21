#pragma once

#include "IItemComponent.h"
#include "../GameManager/ScoreManager.h"
#include "PathPlanningComponent.h"
#include "VSensorComponent.h"
#include "MoveComponent.h"
#include "AIDrivingComponent.h"

class ItemRedShellComponent : public IItemComponent
{

    private:

        GameObject& player;
        float speed;
        float consTime;
        float decTime;
        int lastVector;
        int myPos;
        glm::vec3 objective;
        ScoreComponent::Pointer enemy;

    public:

        ItemRedShellComponent(GameObject& newGameObject, GameObject& obj);

        ~ItemRedShellComponent();

        void init();

        void update(float dTime);

        void close();

        //Getters
        float getSpeed()        {       return speed;       };
        float getConsTime()     {       return consTime;    };
        float getDecTime()      {       return decTime;     };

};
 

 
 
