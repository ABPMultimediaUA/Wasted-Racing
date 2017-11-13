#pragma once

#include "GameObject.h"	//we need to say this class exists in order to store info about it

class IComponent {

public:

	//Constructor
	IComponent() {}

	//Destructor
	virtual ~IComponent() = 0;

	//Get
	//GameObject* GetGameObject() { return &gameObject; } 

	//Init
	virtual void VInit() = 0;

	//Update
	virtual void VUpdate(float dTime) = 0;


protected:
	
	//GameObject gameObject; //we store the reference to our game Object


};