#include "VSensorComponent.h"
#include <iostream>

//Constructors
VSensorComponent::VSensorComponent(GameObject& newGameObject) : 
ISensorComponent(newGameObject)
{
    angleInitial=newGameObject.getTransformData().rotation.y * 3.141592653589f / 180.f;
    angleVision=55.0 * 3.141592653589f / 180.f;
    sensorLeft = glm::vec3(cos(angleVision+angleInitial), 0.f, sin(angleVision+angleInitial));
    sensorRight = glm::vec3(cos(-angleVision+angleInitial), 0.f, sin(-angleVision+angleInitial));
    //sensorLeft = glm::vec3(1+cos(angleInitial),0,1+sin(angleInitial));
    //sensorRight = glm::vec3(1+cos(angleInitial),0,-1+sin(angleInitial));
}       

VSensorComponent::VSensorComponent(GameObject& newGameObject, float angV, float angI) :
ISensorComponent(newGameObject)
{
    angleInitial=angI;
    angleVision=angV;

    sensorLeft = glm::vec3(cos(angleVision+angleInitial), 0.f, sin(angleVision+angleInitial));
    sensorRight = glm::vec3(cos(-angleVision+angleInitial), 0.f, sin(-angleVision+angleInitial));
    //sensorLeft = glm::vec3(1+cos(angleInitial),0,1+sin(angleInitial));
    //sensorRight = glm::vec3(1+cos(angleInitial),0,-1+sin(angleInitial));
}

void VSensorComponent::updateSeenObjects(std::vector<GameObject::Pointer> objects)
{
    //initial variables
    size_t i;                                                                   //Counter
    glm::vec3 relativeP;                                                        //Relative position of objects to us
    glm::vec3 position = this->getGameObject().getTransformData().position;     //Our position
    VObject::Pointer pvo;                                                       //VPointer included in the end result
    float a = 0, b = 0;                                                         //initial a and b

    //update sensors
    sensorLeft = glm::vec3(cos(angleVision+angleInitial), 0.f, sin(angleVision+angleInitial));
    sensorRight = glm::vec3(cos(-angleVision+angleInitial), 0.f, sin(-angleVision+angleInitial));

    //sensorLeft = glm::vec3(1+cos(angleInitial),0,1+sin(angleInitial));
    //sensorRight = glm::vec3(1+cos(angleInitial),0,-1+sin(angleInitial));
    
    /* //TESTING
        std::cout<<"AnglI: "<<angleInitial<<", AnglV: "<<angleVision<<"\n";
        std::cout<<"Left: "<<sensorLeft.x<<","<<sensorLeft.z<<"\n";
        std::cout<<"Right: "<<sensorRight.x<<","<<sensorRight.z<<"\n";
    */

    //Clear seen objects
    seenObjects.clear();

    //Iterate through all available objects
    for(i=0; i<objects.size(); i++)
    {
        auto ob = objects[i].get();                             //get object
        relativeP = ob->getTransformData().position - position; //Obtain relative position to us (object - our) 
        
        //A and B calculus
        if(sensorRight.x*sensorLeft.z != sensorRight.z*sensorLeft.x) 
        {
            b = (relativeP.z * sensorLeft.x - relativeP.x*sensorLeft.z) /(sensorRight.z * sensorLeft.x - sensorRight.x*sensorLeft.z);
        }
        if(sensorLeft.x != 0){
            a = (relativeP.z - b * sensorRight.z) / sensorLeft.z;
        }

        //if both are inside the cone
        if(a > 0 && b > 0)  
        {
            pvo = std::make_shared<VObject>(ob->getTransformData().position, a, b, 1.f, 1); //generate VObject with the data
            seenObjects.push_back(pvo);                                                     //Add to seen objects
        }
       
        /* //TESTING
        
        std::cout<<"Adri dice que: "<<a<<","<<b<<std::endl;
        */
        
        //clean A and B for the next object
        a = 0;
        b = 0;
    }
}

//Auxiliar function to calculate A and B of given objective
void VSensorComponent::calculateAB(glm::vec3& objective, float* a, float* b){
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