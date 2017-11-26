#pragma once

#include "IComponent.h"
#include "../GamePhysics/LAPALPhysics.h"

class MoveComponent : public IComponent {

public:

    //Constructor
	MoveComponent(GameObject& newGameObject, LAPAL::movementData newMData) : IComponent(newGameObject), mData(newMData) {}

	//Destructor
	virtual ~MoveComponent() {};

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();

    //==========================================
    // PHYSICS RELATED FUNCTIONS
    //==========================================
    void changeImpulse(float n);
    void changeSpinIncrement(float n);

private:

    LAPAL::movementData mData;

};