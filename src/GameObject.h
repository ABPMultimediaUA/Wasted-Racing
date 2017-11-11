#pragma once

#include <string>
#include <vector>

#include "IComponent.h"

class GameObject {

public:

	//Constructor
	GameObject(const std::string &newId) : id(newId) {}

	//Destructor
	~GameObject() {}

	//Init
	void Init();

	//Update
	void Update(float fTime);

	//GetID
	std::string getID() { return id; }

	//Add component
	void AddComponent(IComponent& component);

	//Get component
	IComponent& GetComponent();


private:

	std::string		id;
	core::vector3df pos;

	std::vector<IComponent&> components;

}