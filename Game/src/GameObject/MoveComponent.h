#pragma once

#include "IComponent.h"
#include "../GamePhysics/LAPALPhysics.h"

class MoveComponent : public IComponent {

public:

    //Constructor
	MoveComponent(GameObject& newGameObject, LAPAL::movementData newMData, LAPAL::plane3f newTerrain, float newMass) 
		: IComponent(newGameObject), mData(newMData), terrain(newTerrain), mass(newMass) {}

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
	LAPAL::movementData getMovemententData() 						{ 	return mData; 	}
	const float 		getMass()									{ 	return mass;  	}
	LAPAL::plane3f		getTerrain()								{ 	return terrain;   }
	void 				setMovementData(LAPAL::movementData data) 	{ 	mData = data;   	}
	void 				setMass(const float newMass)				{ 	mass = newMass; 	}
	void				setTerrain(LAPAL::plane3f p) 				{ 	terrain = p;		}


private:

    LAPAL::movementData mData;
	LAPAL::plane3f		terrain;
	float mass;

};