#include "VObject.h"


//Constructors
VObject::VObject()
{
    a=0;
    b=0;
    radius=0;
    type=0;
}

VObject::VObject(glm::vec3 p, double x, double y, double r, int t)
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

double VObject::getA()
{
    return a;
}

double VObject::getB()
{
    return b;
}

double VObject::getRadius()
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

void VObject::setA(double x)
{
    a=x;
}

void VObject::setB(double y)
{
    b=y;
}

void VObject::setRadius(double r)
{
    radius=r;
}

void VObject::setType(int t)
{
    type=t;
}