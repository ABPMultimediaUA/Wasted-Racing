#pragma once

#include "IItemComponent.h"
#include "../../GameManager/ScoreManager.h"
#include "../AIComponent/VSensorComponent.h"

class ItemRedShellComponent : public IItemComponent
{


    public:

        //Constructor
        ItemRedShellComponent(GameObject& newGameObject, GameObject& obj);

        //Destructor
        ~ItemRedShellComponent();

        //Initializer
        void init();

        //Update
        void update(float dTime);

        //Close function
        void close();

        //==============================================================
        // GETTERS & SETTERS
        //==============================================================
        float getSpeed()        {       return speed;       };
        float getConsTime()     {       return consTime;    };
        float getDecTime()      {       return decTime;     };

    private:
        //==============================================================
        // PRIVATE DATA
        //==============================================================
        GameObject& player;             //Victim player
        float speed;                    //Speed of the red shell
        float consTime;                 //----no idea
        float decTime;                  //----no idea
        unsigned int lastVector;        //----no idea
        int myPos;                      //----no idea
        glm::vec3 objective;            //vec3 indicating the position to which to go
        ScoreComponent::Pointer enemy;  //Pointer to the score of the victim player

};
 

 
 
