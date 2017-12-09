#include "VSensorComponent.h"

//Constructors
VSensorComponent::VSensorComponent(GameObject& newGameObject) : 
ISensorComponent(newGameObject)
{
    maxRadius=0.0;
    angleInitial=0.0;
    angleVision=0.0;
}       

VSensorComponent::VSensorComponent(GameObject& newGameObject, float angV, float maxR, float angI) :
ISensorComponent(newGameObject)
{
    maxRadius=maxR;
    angleInitial=angI;
    angleVision=angV;
}