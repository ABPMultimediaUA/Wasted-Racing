#pragma once

#include "IComponent.h"
#include "../GamePhysics/LAPALPhysics.h"

class MoveComponent : public IComponent {

public:

    //Constructor
	MoveComponent(GameObject& newGameObject, LAPAL::movementData newMData, float newMass) : IComponent(newGameObject), mData(newMData), mass(newMass) {}

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
    void changeAccInc(float n);
    void changeSpinIncrement(float n);
	void isMoving(bool m);
	void changeDir(int i);
	void changeAngleInc(float i);
	void isJumping(bool j);


private:

    LAPAL::movementData mData;
	float mass;

};