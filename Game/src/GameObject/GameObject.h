#pragma once

#include <string>
#include <list>

#include <irrlicht.h>

#include "IComponent.h"

class GameObject {

public:

	//Constructor
	GameObject(const std::string &newId, const irr::core::vector3df &newPos) 
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
	irr::core::vector3df pos;
	irr::core::vector3df rot;
	irr::core::vector3df sca;

	//std::list<IComponent> components;

};