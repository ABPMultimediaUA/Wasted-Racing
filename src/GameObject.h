#pragma once

#include <string>
#include <list>

#include "IComponent.h"

class GameObject {

public:

	//Constructor
	GameObject(const std::string &newId, const core::vector3df &newPos) 
		: id(newId), pos(newPos)  {}

	//Destructor
	~GameObject() {}

	//GetID
	std::string GetID() { return id; }

	//Init
	void Init();

	//Update
	void Update(float dTime);

	//Add component
	void AddComponent(IComponent& component);

	//Get component
	IComponent& GetComponent();


private:

	std::string		id;
	core::vector3df pos;

	std::list<IComponent&> components;

}