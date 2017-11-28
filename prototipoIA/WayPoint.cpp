#include "WayPoint.h"


//////////Node//////////

WayPoint::Node::Node()
{

}

WayPoint::Node::~Node()
{

}

double WayPoint::Node::getRadius()
{
    return radius;
}

double WayPoint::Node::setRadius(double rad)
{
    radius = rad;
}

glm::vec3 WayPoint::Node::getPos()
{
    return position;
}

glm::vec3 WayPoint::Node::setPos(glm::vec3 pos)
{
    position = pos;
}

////////WayPoint////////

WayPoint::WayPoint(glm::vec3 pos, double rad)
{
    startPoint = new Node();
    startPoint->setPos(pos);
    startPoint->setRadius(rad);
}
WayPoint::~WayPoint()
{
    delete next;
    delete startPoint;
}

glm::vec3 WayPoint::getPos()
{
    return startPoint->getPos();
}

bool WayPoint::inside(glm::vec3 pos)
{
    int distance = glm::sqrt((pos.x-startPoint->getPos().x)*(pos.x-startPoint->getPos().x) + 
                    (pos.z-startPoint->getPos().z)*(pos.z-startPoint->getPos().z) +
                    (pos.y-startPoint->getPos().y)*(pos.y-startPoint->getPos().y));

    if(distance <= startPoint->getRadius())
    {
        return true;
    }
    return false;
}

WayPoint* WayPoint::getNext()
{
    return next;
}

void WayPoint::setNext(WayPoint* n)
{
    next = n;
}