#pragma once

#include <cstdint>
#include <list>
#include <glm/glm.hpp>
#include <memory>

#include "IComponent.h"

class GameObject {

public:

	struct TransformationData {
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

	//===========================================
	// BASIC FUNCTIONS
	//===========================================

	//Constructor
	GameObject(const u_int16_t newId, const TransformationData &newPos) 
		: id(newId), transformData(newPos)  {}

	//Destructor
	~GameObject() {}

	//GetID
	u_int16_t getId() { return id; }

	//Get position
	TransformationData& getTransformData(){
		return transformData;
	}

	//===========================================
	// SPECIFIC FUNCTIONS
	//===========================================

	//Init
	void init();

	//Update
	void update(float dTime);

	//Add component
	void addComponent(IComponent* component);

	//Get component
	template<typename Component>
	Component* GetComponent() {
		for (auto comp: components) {
			if (Component cmp = std::dynamic_pointer_cast<Component>(comp)) {
				return cmp;
			}
		}
		return nullptr;
	}


private:

	//ObjectID
	u_int16_t  id;

	//Object Transformation Data
	TransformationData transformData;

	//Object Components
	std::list<IComponent*> components;

};