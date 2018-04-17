#pragma once

#include <string>
#include <iostream>
#include "IRenderComponent.h"

class ObjectRenderComponent : public IRenderComponent {

public:

	enum Shape {
		Cube,
		Sphere,
		Mesh,
		Circuit,
		Cone,
		Plane,
		Arrow,
		Cylinder,
		Portion,
		Pyramid,
		Skybox
	};

    //Constructor
	ObjectRenderComponent(GameObject& newGameObject, Shape newShape, const char* newStr) : IRenderComponent(newGameObject), objectShape(newShape) {

		if(newShape == Shape::Mesh){

			folder = "";
			int i = 0;

			while(newStr[i]!='.'){
				folder += newStr[i];
				i++;
			}

			name = std::string(newStr);
			mesh = "media/mesh/" + folder + "/" + std::string(newStr);
		}
		else{
			name = std::string(newStr);
			img = "media/img/" + std::string(newStr);
		}

	}

	ObjectRenderComponent(GameObject& newGameObject, Shape newShape) : IRenderComponent(newGameObject), objectShape(newShape) {};

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

	std::string getFolder() {
		return folder;
	}

	std::string getName() {
		return name;
	}

	void setMesh(const char* m)	{
		mesh = std::string(m);
	}

private:

	Shape objectShape;
	std::string mesh;
	std::string img;
	std::string name;
	std::string folder;

};