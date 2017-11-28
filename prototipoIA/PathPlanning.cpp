#include "PathPlanning.h"




 PathPlanning::PathPlanning()
 {
     listWay = new std::list<WayPoint*>;
 }

PathPlanning::~PathPlanning()
{
    delete actualWayPoint;
    delete startPoint;
    delete listWay;
}

void PathPlanning::addWayPoint(WayPoint* way)
{
    if (totalWayPoints == 0)
    {
        startPoint = way;
        actualWayPoint = way;
    }
    else
    {
        listWay->back()->setNext(way);
    }

    listWay->push_back(way);
    listWay->back()->setNext(startPoint);

    totalWayPoints++;
}

void PathPlanning::setMaxSpeed(float speed)
{
    maxSpeed = speed;
}

void PathPlanning::setFrame(float frame)
{
    frameDeltaTime = frame;
}

void PathPlanning::setSeconds(float sec)
{
    seconds = sec;
}

glm::vec3 PathPlanning::getNextPoint(glm::vec3 pos, glm::vec3 vel, float modSpeed)
{
    glm::vec3 nextPos;

    float distance = (actualWayPoint->getPos().x - pos.x) * (actualWayPoint->getPos().x - pos.x) +
                     (actualWayPoint->getPos().y - pos.y) * (actualWayPoint->getPos().y - pos.y) +
                     (actualWayPoint->getPos().z - pos.z) * (actualWayPoint->getPos().z - pos.z);

    float tour = (modSpeed * seconds)*(modSpeed * seconds);

    if(tour <= distance)
    {
        nextPos = actualWayPoint->getPos();
    }
    else
    {
        tour -= distance;
        for(std::list<WayPoint*>::iterator it = listWay->begin(); it != listWay->end() && tour >= actualWayPoint->getDistNextWays(); ++it){
            tour -= actualWayPoint->getDistNextWays();
            actualWayPoint = actualWayPoint->getNext();
        }
        nextPos = (tour/actualWayPoint->getDistNextWays()) * (actualWayPoint->getNext()->getPos() - actualWayPoint->getPos()) + actualWayPoint->getPos();
    }

    if(actualWayPoint->checkNext(pos, nextPos))
    {
        actualWayPoint = actualWayPoint->getNext();
    }

    return nextPos;
}