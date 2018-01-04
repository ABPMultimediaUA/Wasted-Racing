#pragma once

#include "IItemComponent.h"
#include "../../GameManager/ScoreManager.h"
#include "../AIComponent/PathPlanningComponent.h"
#include "../AIComponent/VSensorComponent.h"
#include "../MoveComponent.h"
#include "../AIComponent/AIDrivingComponent.h"

class ItemRedShellComponent : public IItemComponent
{

    private:

        GameObject& player;
        float speed;
        float consTime;
        float decTime;
        unsigned int lastVector;
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
 

 
 
