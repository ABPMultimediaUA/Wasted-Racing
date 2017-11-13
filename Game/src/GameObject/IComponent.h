#pragma once

class GameObject;

class IComponent {

public:

	//Constructor
	IComponent(GameObject& newGameObject) : gameObject(newGameObject) {}

	//Destructor
	virtual ~IComponent() = 0;

	//Getter
	GameObject& getGameObject() { return gameObject; } 

	//Initilizer
	virtual void init() = 0;

	//Update
	virtual void update(float dTime) = 0;


protected:
	
	GameObject& gameObject; //we store the reference to our game Object

};