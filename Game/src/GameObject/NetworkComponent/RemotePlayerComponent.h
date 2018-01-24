#pragma once

#include "../IComponent.h"
#include "../GameObject.h"

class RemotePlayerComponent : public IComponent{
    public:
        //Define ::Pointer
        typedef std::shared_ptr<RemotePlayerComponent> Pointer;

        //Constructor
        RemotePlayerComponent(GameObject& newGameObject);

        //Destructor
        ~RemotePlayerComponent() {}

        //Initilizer
        virtual void init() {};

        //Update
        virtual void update(float dTime) {};

        //Closer
        virtual void close() {};

    private:
    
};