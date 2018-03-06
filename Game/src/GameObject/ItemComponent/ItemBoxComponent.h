#pragma once

#include "../../GameManager/RenderManager.h"
#include "../ScoreComponent.h"

class ItemBoxComponent : public IComponent{

    public:

        //Constructor
        ItemBoxComponent(GameObject& newGameObject);

        //Destructor
        virtual ~ItemBoxComponent() {};

        //Initializer
        virtual void init();

        //Closer 
        virtual void close();

        //Update
        virtual void update(float dTime);


        int getActive()             {       return active;      }

    
    //==========================================
    // BOX RELATED FUNCTIONS
    //==========================================
    void deactivateBox();
    void asignItem(GameObject& obj);

    private:
        int active = 1;
        float boxTime;
        float actTime;

};