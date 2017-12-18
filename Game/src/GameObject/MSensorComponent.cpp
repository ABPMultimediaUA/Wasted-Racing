#include "MSensorComponent.h"
#include <iostream>

//Constructors
MSensorComponent::MSensorComponent(GameObject& newGameObject) : 
ISensorComponent(newGameObject)
{
    angleInitial=newGameObject.getTransformData().rotation.y * 3.141592653589f / 180.f;
    angleVision=5.0 * 3.141592653589f / 180.f;
    sensorLeft = glm::vec3(cos(angleVision+angleInitial), 0.f, sin(angleVision+angleInitial));
    sensorRight = glm::vec3(cos(-angleVision+angleInitial), 0.f, sin(-angleVision+angleInitial));
}       

MSensorComponent::MSensorComponent(GameObject& newGameObject, float angV, float angI) :
ISensorComponent(newGameObject)
{
    angleInitial=angI;
    angleVision=angV * 3.141592653589f / 180.f;

    sensorLeft = glm::vec3(cos(angleVision+angleInitial), 0.f, sin(angleVision+angleInitial));
    sensorRight = glm::vec3(cos(-angleVision+angleInitial), 0.f, sin(-angleVision+angleInitial));
}

void MSensorComponent::updateSeenObjects()
{
    //---------------
    //INITIALIZATIONS
    //---------------

    //initial variables
    size_t i;                                                                   //Counter
    glm::vec3 relativeP;                                                        //Relative position of objects to us
    glm::vec3 position = this->getGameObject().getTransformData().position;     //Our position
    VObject::Pointer pvo;                                                       //VPointer included in the end result
    float a = 0, b = 0;                                                         //initial a and b

    /* //TESTING
        std::cout<<"AnglI: "<<angleInitial<<", AnglV: "<<angleVision<<"\n";
        std::cout<<"Left: "<<sensorLeft.x<<","<<sensorLeft.z<<"\n";
        std::cout<<"Right: "<<sensorRight.x<<","<<sensorRight.z<<"\n";
    */

    //Clear seen objects
    seenObjects.clear();

    //Get terrain info and movement info
    /*auto moving = PhysicsManager::getMovingCharacter(this->getGameObject().getId());
    auto moving.moveComponent;
    auto moving.terrainComponent;

    //-------------------
    //TERRAIN BACK POINTS
    //-------------------

    

    //Check sensor collisions
    //update sensors
    sensorLeft = glm::vec3(cos(angleVision+angleInitial), 0.f, sin(angleVision+angleInitial));
    sensorRight = glm::vec3(cos(-angleVision+angleInitial), 0.f, sin(-angleVision+angleInitial));*/
    
    //-------------------
    //FRONT POINTS CHECK
    //-------------------
}

//Auxiliar function to calculate A and B of given objective
void MSensorComponent::calculateAB(glm::vec3& objective, float* a, float* b){
    glm::vec3 relativeP;
    
    //update sensors
    sensorLeft = glm::vec3(cos(angleVision+angleInitial), 0.f, sin(angleVision+angleInitial));
    sensorRight = glm::vec3(cos(-angleVision+angleInitial), 0.f, sin(-angleVision+angleInitial));

    //Do the math
    relativeP = objective - this->getGameObject().getTransformData().position;
    if(sensorRight.x*sensorLeft.z != sensorRight.z*sensorLeft.x) 
        *b = (relativeP.z * sensorLeft.x - relativeP.x*sensorLeft.z) /(sensorRight.z * sensorLeft.x - sensorRight.x*sensorLeft.z);
    if(sensorLeft.x != 0)
        *a = (relativeP.z - (*b) * sensorRight.z) / sensorLeft.z;
}