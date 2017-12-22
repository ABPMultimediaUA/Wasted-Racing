#pragma once

#include "IComponent.h"
#include "../GameManager/WaypointManager.h"
#include "MoveComponent.h"
#include "ScoreComponent.h"

class PathPlanningComponent : public IComponent{
private:

    float seconds;
    float distLastWay;
    unsigned int lastVector;
    glm::vec3 nextPos;
        
public:

        //Constructor
    PathPlanningComponent(GameObject& newGameObject);

        //Destructor
    ~PathPlanningComponent() {};

        

        //Initilizer
	virtual void init() {}

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close() {}



    //Getters and setters
    void setSeconds(float sec)      {  seconds = sec;   };
    void setDistLastWay(GameObject::Pointer n, glm::vec3 pos);
    void setLastPosVector(int lvl);


    float getDistLastWay();
    int getLastPosVector();
    int getActualLevel();
    float getActualDistance();

    //Functions
    glm::vec3 getNextPoint();    

}; 
