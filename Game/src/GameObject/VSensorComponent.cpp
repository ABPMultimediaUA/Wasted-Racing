#include "VSensorComponent.h"
#include <iostream>

//Constructors
VSensorComponent::VSensorComponent(GameObject& newGameObject) : 
ISensorComponent(newGameObject)
{
    angleInitial=0.0;
    angleVision=55.0;
    sensorLeft = glm::vec3(sin(angleVision+angleInitial+270), 0.f, cos(angleVision+angleInitial));
    sensorRight = glm::vec3(sin(-angleVision+angleInitial+270), 0.f, cos(-angleVision+angleInitial));
}       

VSensorComponent::VSensorComponent(GameObject& newGameObject, float angV, float angI) :
ISensorComponent(newGameObject)
{
    angleInitial=angI;
    angleVision=angV;

    sensorLeft = glm::vec3(sin(angleVision+angleInitial), 0.f, cos(angleVision+angleInitial));
    sensorRight = glm::vec3(sin(-angleVision+angleInitial), 0.f, cos(-angleVision+angleInitial));
}

void VSensorComponent::updateSeenObjects(std::vector<GameObject::Pointer> objects)
{
    sensorLeft = glm::vec3(cos(angleVision+angleInitial+180), 0.f, sin(angleVision+angleInitial+180));
    sensorRight = glm::vec3(cos(-angleVision+angleInitial+180), 0.f, sin(-angleVision+angleInitial+180));
    size_t i;
    glm::vec3 relativeP;
    VObject::Pointer pvo;

    std::cout<<"AnglI: "<<angleInitial<<", AnglV: "<<angleVision<<"\n";
    std::cout<<"Left: "<<sensorLeft.x<<","<<sensorLeft.z<<"\n";
    std::cout<<"Right: "<<sensorRight.x<<","<<sensorRight.z<<"\n";

    seenObjects.clear();
    glm::vec3 position = this->getGameObject().getTransformData().position;
    float a = -1, b = -1;

    for(i=0; i<objects.size(); i++)
    {
        auto ob = objects[i].get();
        relativeP = ob->getTransformData().position - position;
        if(sensorRight.x*sensorLeft.z != sensorRight.z*sensorLeft.x) 
        {
            b = (relativeP.z * sensorLeft.x - relativeP.x*sensorLeft.z) /(sensorRight.z * sensorLeft.x - sensorRight.x*sensorLeft.z);
        }
        if(sensorLeft.x != 0){
            a = (relativeP.z - b * sensorRight.z) / sensorLeft.z;
        }

        if(a > 0 && b > 0)
        {
            pvo = std::make_shared<VObject>(ob->getTransformData().position, a, b, 1.f, 1);
            seenObjects.push_back(pvo);
        }
        std::cout<<"Adri dice que: "<<a<<","<<b<<std::endl;
        a=-1;
        b=-1;
    }
}
