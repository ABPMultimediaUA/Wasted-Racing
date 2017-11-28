#pragma once

#include "IRenderComponent.h"

class ObjectRenderComponent : public IRenderComponent {

public:

	enum Shape {
		Cube,
		Sphere,
		Mesh
	};

    //Constructor
	ObjectRenderComponent(GameObject& newGameObject, Shape newShape) : IRenderComponent(newGameObject), objectShape(newShape) {}

	//Destructor
	virtual ~ObjectRenderComponent() {};

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();

    //Drawer
	virtual void draw();

	Shape getObjectShape() {
		return objectShape;
	}


private:

	Shape objectShape;

};