#include "WayPoint.h"


//////////Node//////////

WayPoint::Node::Node()
{

}

WayPoint::Node::~Node()
{

}

float WayPoint::Node::getRadius()
{
    return radius;
}

float WayPoint::Node::setRadius(float rad)
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

WayPoint::WayPoint(glm::vec3 pos, float rad)
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

bool WayPoint::checkNext(glm::vec3 pos, glm::vec3 nextPos)
{
    float distWay = (pos.x-startPoint->getPos().x)*(pos.x-startPoint->getPos().x) + 
                    (pos.z-startPoint->getPos().z)*(pos.z-startPoint->getPos().z) +
                    (pos.y-startPoint->getPos().y)*(pos.y-startPoint->getPos().y);

    float distNextPos = (pos.x-nextPos.x)*(pos.x-nextPos.x) + 
                        (pos.z-nextPos.z)*(pos.z-nextPos.z) +
                        (pos.y-nextPos.y)*(pos.y-nextPos.y);

        std::cout<<"distWay: "<<distWay<<"\n";
        std::cout<<"distNextPos: "<<distNextPos<<"\n";
    if((distWay <= (startPoint->getRadius() * startPoint->getRadius())) || distNextPos < distWay)
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
    distNextWay = ((startPoint->getPos().x - n->getPos().x) * (startPoint->getPos().x - n->getPos().x)) + 
                  ((startPoint->getPos().y - n->getPos().y) * (startPoint->getPos().y - n->getPos().y)) +
                  ((startPoint->getPos().z - n->getPos().z) * (startPoint->getPos().z - n->getPos().z));
}

float WayPoint::getDistNextWays()
{
    return distNextWay;
}