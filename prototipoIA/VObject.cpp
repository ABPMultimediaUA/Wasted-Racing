#include "VObject.h" 

//Constructor
VObject::VObject(glm::vec3 pos, float A, float B, float r, int t){
    position = pos;
    a = A;
    b = B;
    radius = r;
    type = t;
}

//Destructor
VObject::~VObject(){

}

//get position
glm::vec3 VObject::getPosition(){
    return position;
}

//get a
float VObject::getA(){
    return a;
}

//get b
float VObject::getB(){
    return b;
}

//get radius
float VObject::getRadius(){
    return radius;
}

//get type
int VObject::getType(){
    return type;
}