#include "VSensorComponent.h"

//Constructors
VSensorComponent::VSensorComponent(GameObject& newGameObject) : 
ISensorComponent(newGameObject)
{
    angleInitial=0.0;
    angleVision=0.0;
    glm::vec3 sensorLeft(sin(-angleVision-angleInitial), 0.f, cos(-angleVision-angleInitial));
    glm::vec3 sensorRight(sin(angleVision-angleInitial), 0.f, cos(angleVision-angleInitial));
}       

VSensorComponent::VSensorComponent(GameObject& newGameObject, float angV, float angI) :
ISensorComponent(newGameObject)
{
    angleInitial=angI;
    angleVision=angV;
}

void VSensorComponent::updateSeenObjects(std::vector<GameObject::Pointer> objects)
{
    size_t i;
    glm::vec3 relativeP;
    VObject::Pointer pvo;

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
        a=-1;
        b=-1;
    }
}
