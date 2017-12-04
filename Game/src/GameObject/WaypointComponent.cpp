#include "WaypointComponent.h"

////////////NODE/////////////

WaypointComponent::Node::Node()
{

}

WaypointComponent::Node::~Node()
{

}

float WaypointComponent::Node::getRadius()
{
    return radius;
}

void WaypointComponent::Node::setRadius(float rad)
{
    radius = rad;
}

glm::vec3 WaypointComponent::Node::getPos()
{
    return position;
}

void WaypointComponent::Node::setPos(glm::vec3 pos)
{
    position = pos;
}


///////////////////WaypointComponent//////////////////


WaypointComponent::WaypointComponent(GameObject& newGameObject, glm::vec3 pos, float rad) : IComponent(newGameObject)
{
    startPoint = new Node();
    startPoint->setPos(pos);
    startPoint->setRadius(rad);
}
WaypointComponent::~WaypointComponent()
{
    //delete next;
    //delete startPoint;
}

glm::vec3 WaypointComponent::getPos()
{
    return startPoint->getPos();
}

bool WaypointComponent::checkNext(glm::vec3 pos, glm::vec3 nextPos)
{
    float distWay = (pos.x-startPoint->getPos().x)*(pos.x-startPoint->getPos().x) + 
                    (pos.z-startPoint->getPos().z)*(pos.z-startPoint->getPos().z) +
                    (pos.y-startPoint->getPos().y)*(pos.y-startPoint->getPos().y);

    float distNextPos = (pos.x-nextPos.x)*(pos.x-nextPos.x) + 
                        (pos.z-nextPos.z)*(pos.z-nextPos.z) +
                        (pos.y-nextPos.y)*(pos.y-nextPos.y);

    if((distWay <= (startPoint->getRadius() * startPoint->getRadius())) || distNextPos < distWay)
    {
        return true;
    }
    return false;
}

GameObject* WaypointComponent::getNext()
{
    return next;
}

void WaypointComponent::setNext(GameObject* n)
{
    next = n;
    distNextWay = ((startPoint->getPos().x - n->getTransformData().position.x) * (startPoint->getPos().x - n->getTransformData().position.x)) + 
                  ((startPoint->getPos().y - n->getTransformData().position.y) * (startPoint->getPos().y - n->getTransformData().position.y)) +
                  ((startPoint->getPos().z - n->getTransformData().position.z) * (startPoint->getPos().z - n->getTransformData().position.z));
}

float WaypointComponent::getDistNextWays()
{
    return distNextWay;
}
