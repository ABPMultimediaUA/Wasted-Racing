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
    double sR_x = sensorRight.x;
    double sR_z = sensorRight.z;
    double sL_x = sensorLeft.x;
    double sL_z = sensorLeft.z;
    double r_x = relativeP.x;
    double r_z = relativeP.z;
    double aux_a = 0;
    double aux_b = 0;
    //
    if(sR_x * sL_z != sR_z * sL_x){
        aux_a = (r_z * sL_x - r_x * sL_z) / (sR_z * sL_x - sR_x * sL_z);
    }
    if(sL_z != 0){
        aux_b = (r_z - aux_a * sR_z) / sL_z;
    }

    a = (float) aux_a;
    b = (float) aux_b;
}