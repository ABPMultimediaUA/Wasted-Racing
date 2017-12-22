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
}       

VSensorComponent::VSensorComponent(GameObject& newGameObject, float angV, float angI) :
ISensorComponent(newGameObject)
{
    angleInitial=angI;
    angleVision=angV * 3.141592653589f / 180.f;

    sensorLeft = glm::vec3(cos(angleVision+angleInitial), 0.f, sin(angleVision+angleInitial));
    sensorRight = glm::vec3(cos(-angleVision+angleInitial), 0.f, sin(-angleVision+angleInitial));

}

//Checks the objects seen and stores the ones seen in the seenObjects vector
void VSensorComponent::updateSeenObjects(std::vector<GameObject> objects)
{
    //initial variables
    size_t i;                                                                   //Counter
    VObject::Pointer pvo;                                                       //VPointer included in the end result
    float a = 0.f, b = 0.f;                                                         //initial a and b (left and right sensor, respectively)

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
        //auto ob = objects[i].get();                                                         //get object
        
        calculateAB(objects[i].getTransformData().position, a, b);       //Do the math

        //if both are inside the cone contained by A and B
        if(a > 0 && b > 0)  
        {
            pvo = std::make_shared<VObject>(objects[i].getTransformData().position, a, b, 1.f, 1); //generate VObject with the data
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
void VSensorComponent::calculateAB(glm::vec3 objective, float& a, float& b){
    glm::vec3 relativeP;
    
    //update sensors
    sensorLeft = glm::vec3(cos(angleVision+angleInitial), 0.f, sin(angleVision+angleInitial));
    sensorRight = glm::vec3(cos(-angleVision+angleInitial), 0.f, sin(-angleVision+angleInitial));

    //Do the math
    relativeP = objective - this->getGameObject().getTransformData().position;

    //Satan
    LAPAL::calculateAB(relativeP, sensorLeft, sensorRight, a, b);
}