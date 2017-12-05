#pragma once

#include "IComponent.h"
#include "MoveComponent.h"
#include "../GamePhysics/LAPALPhysics.h"

class CollisionComponent : public IComponent {

public:

    //Constructor
	CollisionComponent(GameObject& newGameObject, const float newRadius) : IComponent(newGameObject), radius(newRadius) {}

	//Destructor
	virtual ~CollisionComponent() {};

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();

	//Radius getter
	const float getRadius() { return radius; }

private:

    const float radius;


};