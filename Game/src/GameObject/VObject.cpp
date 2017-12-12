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