#pragma once

#include "IComponent.h"
#include "../GamePhysics/LAPALPhysics.h"

class MoveComponent : public IComponent {

public:

    //Constructor
	MoveComponent(GameObject& newGameObject, LAPAL::movementData newMData, LAPAL::plane3f newTerrain, float newMass) 
		: IComponent(newGameObject), mData(newMData), terrain(newTerrain), mass(newMass) {
			
			auxData 				= newMData;
			constantAlteredTime		= 0.0f;		
			decrementalAlteredTime	= 0.0f;
			maxDecrementalAT		= 0.0f;			
		}

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
	//Keyboard functions
    void changeAccInc(float n);
    void changeSpinIncrement(float n);

	//Joystick functions
	void changeSpin(float n);
	void changeAcc(float n);

	//Checker functions
	void isMoving(bool m);
	void isJumping(bool j);
	void isSpinning(bool s);
	void isDrifting(bool d);
	void changeDir(int i);
	void isBraking(bool b);

	//Functions related with temporal data changes
	void changeMaxSpeedOverTime(float maxSpeed, float constTime, float decTime);
	void updateMaxSpeedOverTime(const float dTime);
	void updateJump(LAPAL::movementData& mData, glm::vec3& trans, LAPAL::plane3f t);

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

	//Data for movement changes
	float constantAlteredTime;		//Time during max_speed is constant
	float decrementalAlteredTime;	//Time during max_speed is reduced to its former value
	float maxDecrementalAT;			//Complete time interval whitin speed is reduced constantly
	LAPAL::movementData auxData;

};