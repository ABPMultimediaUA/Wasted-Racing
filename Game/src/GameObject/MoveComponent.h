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
	void isSpinning(bool s);

	//Mass and Data getters and setters
	LAPAL::movementData getMovemententData() 						{ return mData; 	}
	const float 		getMass()									{ return mass;  	}
	void 				setMovementData(LAPAL::movementData data) 	{ mData = data;   	}
	void 				setMass(const float newMass)				{ mass = newMass; 	}


private:

    LAPAL::movementData mData;
	float mass;

};