#pragma once

#include <iostream>
#include "IRenderComponent.h"
#include "../PhysicsComponent/TerrainComponent.h"


class CameraRenderComponent : public IRenderComponent {

public:

    //Constructor
	CameraRenderComponent(GameObject& newGameObject);

	//Destructor
	virtual ~CameraRenderComponent() {};

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();

    //Drawer
	virtual void draw();

	//Getters
	std::shared_ptr<TerrainComponent> 	getTerrain() 		{	return terrain;		}
	float 								getDistance()		{	return distance;	}
	float 								getMaxDistance()	{	return maxDistance;	}
	float 								getOldDistance()	{	return oldDistance; }
	float								getHeight()			{	return height;		}
	float								getOldHeight()		{	return oldHeight;	}

	//Setters
	void setTerrain(std::shared_ptr<TerrainComponent> t) 	{	terrain = t; 		}
	void setDistance(float d) 								{ 	distance = d; 		}
	void setMaxDistance(float d) 							{ 	maxDistance = d;	}
	void setOldDistance(float d) 							{ 	oldDistance = d; 	}

private:

	std::shared_ptr<TerrainComponent> terrain;
	float distance;
	float maxDistance;
	float oldDistance;
	float height;
	float oldHeight;
	int count;
	int spinDir;

};