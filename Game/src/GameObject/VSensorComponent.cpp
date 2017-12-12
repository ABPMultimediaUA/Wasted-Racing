#include "VSensorComponent.h"

//Constructors
VSensorComponent::VSensorComponent(GameObject& newGameObject) : 
ISensorComponent(newGameObject)
{
    angleInitial=0.0;
    angleVision=0.0;
}       

VSensorComponent::VSensorComponent(GameObject& newGameObject, float angV, float angI) :
ISensorComponent(newGameObject)
{
    angleInitial=angI;
    angleVision=angV;
}