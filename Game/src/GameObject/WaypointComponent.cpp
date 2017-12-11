#include "WaypointComponent.h"



//==============================================
//Contructor and Destructor
//==============================================

WaypointComponent::WaypointComponent(GameObject& newGameObject, float rad, int lvl) : IComponent(newGameObject)
{
    radius = rad;
    level = lvl;
}

WaypointComponent::~WaypointComponent()
{

}

//Functions
/*
bool WaypointComponent::checkNext(glm::vec3 pos)
{
    float distWay = (pos.x-position.x)*(pos.x-position.x) + 
                    (pos.z-position.z)*(pos.z-position.z) +
                    (pos.y-position.y)*(pos.y-position.y);


    if(distWay <= (radius * radius))
    {
        return true;
    }
    return false;
}*/
