#pragma once

#include "../IComponent.h"
#include "../GameObject.h"

class DebugNetworkComponent : public IComponent{
    public:
        //Define ::Pointer
        typedef std::shared_ptr<DebugNetworkComponent> Pointer;

        //Constructor
        DebugNetworkComponent(GameObject& newGameObject);

        //Destructor
        ~DebugNetworkComponent() {};

        //Initilizer
        virtual void init() {};

        //Update
        virtual void update(float dTime) {};

        //Closer
        virtual void close() {};

        //==============================================================
        // Getters and setters
        //==============================================================

};
