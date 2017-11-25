#pragma once

#include "IRenderComponent.h"

class ObjectRenderComponent : public IRenderComponent {

public:

    //Constructor
	ObjectRenderComponent(GameObject& newGameObject) : IRenderComponent(newGameObject) {}

	//Destructor
	virtual ~ObjectRenderComponent() {};

	//Getter
	GameObject& getGameObject() { return gameObject; } 

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();

    //Drawer
	virtual void draw();


private:

};