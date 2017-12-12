#pragma once

#include "IComponent.h"
#include "MoveComponent.h"
#include "../GamePhysics/LAPALPhysics.h"

class CollisionComponent : public IComponent {

public:

	enum Shape {
		Circle,
		Rectangle
	};

	enum Type {
		Default,
		Ramp,
		ItemBox
	};

    //Constructor
	CollisionComponent(GameObject& newGameObject, float newRadius, bool isKinetic, Type newType) : IComponent(newGameObject), 
						radius(newRadius), kinetic(isKinetic), type(newType) { shape = Shape::Circle; }
	CollisionComponent(GameObject& newGameObject, LAPAL::plane3f newRectangle, bool isKinetic, Type newType) : IComponent(newGameObject), 
						rectangle(newRectangle), kinetic(isKinetic), type(newType) { shape = Shape::Rectangle; }

	//Destructor
	virtual ~CollisionComponent() {};

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();

	//Getters
	const float 			getRadius() 	{ return radius; 	}
	const LAPAL::plane3f 	getRectangle() 	{ return rectangle; }
	const Shape 			getShape()		{ return shape;		}
	const Type 				getType()		{ return type;		}
	const bool 				getKinetic()	{ return kinetic; 	}



private:

    float 			radius;
	LAPAL::plane3f 	rectangle;
	Shape 			shape;
	bool 			kinetic;
	Type			type;


};