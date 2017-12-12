#pragma once

#include "IComponent.h"
#include "GameObject.h"
#include <glm/ext.hpp>

class WaypointComponent : public IComponent{
private:
        float radius;
        int level;
public:

        //Constructor
        WaypointComponent(GameObject& newGameObject, float rad, int lvl);

        //Destructor
        ~WaypointComponent();

        

        //Initilizer
	virtual void init() {}

	//Update
	virtual void update(float dTime) {}

	//Closer
	virtual void close() {}

        //Getters
        int getLevel();
        float getRadius();

        //Setters
        void setRadius(float rad);
        void setLevel(int lvl);

        //Functions
        //bool checkNext(glm::vec3 pos); //Check if we are on range with radius


};