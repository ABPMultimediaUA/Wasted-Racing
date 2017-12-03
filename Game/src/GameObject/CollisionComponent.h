#pragma once

#include "IComponent.h"
#include "MoveComponent.h"
#include "../GamePhysics/LAPALPhysics.h"

class CollisionComponent : public IComponent {

public:

    //Constructor
	CollisionComponent(GameObject& newGameObject) : IComponent(newGameObject) {
        movementData = nullptr;
    }

	//Destructor
	virtual ~CollisionComponent() {};

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();

    //Setter
    void setMovementData(uint16_t);

private:

    std::shared_ptr<MoveComponent> movementData; //A collisionComponent contains a reference to a MoveComponent
                                                 //If this reference is set to nullptr, it means that the object that has the collision
                                                 //component is static, so it won't move but can  be collided


};