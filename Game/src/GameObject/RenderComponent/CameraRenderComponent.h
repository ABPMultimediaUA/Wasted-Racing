#pragma once

#include "IRenderComponent.h"

class CameraRenderComponent : public IRenderComponent {

public:

    //Constructor
	CameraRenderComponent(GameObject& newGameObject) : IRenderComponent(newGameObject) {}

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

private:

};