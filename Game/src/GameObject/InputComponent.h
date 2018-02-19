#pragma once

#include "../GameEvent/EventManager.h"
#include "../GameManager/ItemManager.h"
#include "ItemComponent/ItemHolderComponent.h"
#include "GameObject.h"
#include "PhysicsComponent/MoveComponent.h"
#include <iostream>

class InputComponent : public IComponent {

public:

    //Constructor
	InputComponent(GameObject& newGameObject) : IComponent(newGameObject) {}

	//Destructor
	virtual ~InputComponent() {};

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();

private:

};