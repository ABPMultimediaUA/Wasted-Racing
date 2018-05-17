#pragma once

#include "MoveComponent.h"

class CollisionComponent : public IComponent {

public:

	enum Shape {
		Circle,
		Rectangle
	};

	enum Type {
		Default,
		Ramp,
		ItemBox,
		Trap,
		BlueShell,
		RedShell,
		StartLine
	};

    //Constructor
	CollisionComponent(GameObject& newGameObject, float newRadius, float newLength, bool isKinetic, Type newType) : IComponent(newGameObject), 
						radius(newRadius), length(newLength), kinetic(isKinetic), type(newType), collisionOn(true) { shape = Shape::Circle; }
	CollisionComponent(GameObject& newGameObject, LAPAL::plane3f newRectangle, float newLength, bool isKinetic, Type newType) : IComponent(newGameObject), 
						rectangle(newRectangle), length(newLength), kinetic(isKinetic), type(newType), collisionOn(true) { shape = Shape::Rectangle; }

	//Destructor
	virtual ~CollisionComponent() {};

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();

	//Getters		
	const float 			getRadius() 			{ return radius; 		}
	const LAPAL::plane3f 	getRectangle() 			{ return rectangle; 	}
	const float 			getLength() 			{ return length; 		}
	const Shape 			getShape()				{ return shape;			}
	const Type 				getType()				{ return type;			}
	const bool 				getKinetic()			{ return kinetic; 		}
	const bool 				getCollisionOn()		{ return collisionOn; 	}

	//Setters
	void 				setKinetic(bool k)				{ 	kinetic = k; 	}
	void 				setCollisionOn(bool c)			{ 	collisionOn = c; 	}



private:

    float 			radius = -1.f;
	LAPAL::plane3f 	rectangle;
	float 			length;
	Shape 			shape;
	bool 			kinetic;
	Type			type;
	bool 			collisionOn;

};