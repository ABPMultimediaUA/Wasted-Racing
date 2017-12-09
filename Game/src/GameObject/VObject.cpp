#include "VObject.h"


//Constructors
VObject::VObject()
{
    a=0;
    b=0;
    radius=0;
    type=0;
}

VObject::VObject(glm::vec3 p, float x, float y, float r, int t)
{
    position=p;
    a=x;
    b=y;
    radius=r;
    type=t;
}

//Destructor
VObject::~VObject()
{

}

//Getters
glm::vec3 VObject::getPosition()
{
    return position;
}

float VObject::getA()
{
    return a;
}

float VObject::getB()
{
    return b;
}

float VObject::getRadius()
{
    return radius;
}

int VObject::getType()
{
    return type;
}
      
//Setters
void VObject::setPosition(glm::vec3 p)
{
    position=p;
}

void VObject::setA(float x)
{
    a=x;
}

void VObject::setB(float y)
{
    b=y;
}

void VObject::setRadius(float r)
{
    radius=r;
}

void VObject::setType(int t)
{
    type=t;
}