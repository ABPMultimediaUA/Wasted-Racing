#pragma once

#include "IComponent.h" 
#include "GameObject.h"

class ItemBoxComponent : public IComponent{

    public:

        //Constructor
        ItemBoxComponent(GameObject& newGameObject, float dTime);

        //Destructor
        virtual ~ItemBoxComponent() {};

        //Initializer
        virtual void init();

        //Closer 
        virtual void close();

        //Update
        virtual void update(float dTime);

    
    //==========================================
    // BOX RELATED FUNCTIONS
    //==========================================
    void deactivateBox();
    void asignItem(GameObject& obj);

    private:
        
        float boxTime;
        float actTime;

};