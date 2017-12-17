#pragma once

#include "IComponent.h" 
#include "GameObject.h"

class ItemBoxComponent : public IComponent{

    public:

        //Constructor
        ItemBoxComponent(GameObject& newGameObject) : IComponent(newGameObject){}

        //Destructor
        virtual ~ItemBoxComponent() {};

        //Initializer
        virtual void init();

        //Closer 
        virtual void close();

        //Update
        virtual void update();

    
    //==========================================
    // BOX RELATED FUNCTIONS
    //==========================================
    void checkCollision();
    void deactivateBox();
    void activateBox();

};