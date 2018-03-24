#pragma once

#include <string>
#include "IRenderComponent.h"

class ObjectRenderComponent : public IRenderComponent {

public:

	enum Shape {
		Cube,
		Sphere,
		Mesh,
		Cone,
		Plane,
		Arrow,
		Cylinder,
		Portion,
		Pyramid
	};

    //Constructor
	ObjectRenderComponent(GameObject& newGameObject, Shape newShape, const char* newStr) : IRenderComponent(newGameObject), objectShape(newShape) {
		//If type is mesh, search it in mesh folder
		if(newShape == Shape::Mesh){

			std::string folder = "";
			int i = 0;

			while(newStr[i]!='.'){
				folder += newStr[i];
				i++;
			}

			mesh = "media/mesh/" + folder + "/" + std::string(newStr);
		}
		//If not in img folder
		else{
			img = "media/img/" + std::string(newStr);
		}

	}

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

	std::string getMesh() {
		return mesh;
	}

	std::string getImg() {
		return img;
	}

private:

	Shape objectShape;
	std::string mesh;
	std::string img;

};