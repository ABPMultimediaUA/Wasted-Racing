#pragma once

#include <string>
#include <list>
#include <glm/glm.hpp>

#include "IComponent.h"


class GameObject {

public:

	//Constructor
	GameObject(const std::string &newId, const glm::vec3 &newPos) 
		: id(newId), pos(newPos)  {}

	//Destructor
	~GameObject() {}

	//GetID
	std::string getId() { return id; }

	//Get position
	glm::vec3 getPosition(){
		return pos;
	}

	//Init
	void init();

	//Update
	void update(float dTime);

	//Add component
	void addComponent(IComponent& component);

	//Get component
	IComponent* getComponent();


private:

	std::string		id;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 sca;

	std::list<IComponent*> components;

};