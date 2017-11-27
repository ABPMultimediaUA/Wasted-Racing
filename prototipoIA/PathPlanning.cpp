#include "PathPlanning.h"




 PathPlanning::PathPlanning()
 {
     listWay = new std::list<WayPoint*>;
 }

PathPlanning::~PathPlanning()
{
    delete nextWayPoint;
    delete startPoint;
    delete listWay;
}

void PathPlanning::addWayPoint(WayPoint* way)
{
    if (totalWayPoints == 0)
    {
        startPoint = way;
        nextWayPoint = way;
    }
    else
    {
        listWay->back()->setNext(way);
    }

    listWay->push_back(way);
    listWay->back()->setNext(startPoint);

    totalWayPoints++;
}

void PathPlanning::setMaxSpeed(double speed)
{
    maxSpeed = speed;
}

void PathPlanning::setFrame(double frame)
{
    frameDeltaTime = frame;
}

glm::vec3 PathPlanning::getNextPoint(glm::vec3 pos, glm::vec3 vel)
{
    if(nextWayPoint->inside(pos))
    {
        nextWayPoint = nextWayPoint->getNext();
    }

    glm::vec3 nextPos;

    double modSpeed = glm::sqrt(((vel.x / frameDeltaTime) * (vel.x / frameDeltaTime)) + 
                      ((vel.y / frameDeltaTime) * (vel.y / frameDeltaTime)) + ((vel.z / frameDeltaTime) * (vel.z / frameDeltaTime)));

    if(modSpeed <= 0)
    {
        nextPos = nextWayPoint->getPos(); 
    }
    else if((modSpeed > 0) && (modSpeed <= (maxSpeed * 0.5)))
    {
        nextPos.x = ((vel.x / frameDeltaTime) * (nextWayPoint->getNext()->getPos().x - nextWayPoint->getPos().x)) / (maxSpeed * 0.5);
        nextPos.y = ((vel.y / frameDeltaTime) * (nextWayPoint->getNext()->getPos().y - nextWayPoint->getPos().y)) / (maxSpeed * 0.5);
        nextPos.z = ((vel.z / frameDeltaTime) * (nextWayPoint->getNext()->getPos().z - nextWayPoint->getPos().z)) / (maxSpeed * 0.5);

        nextPos += nextWayPoint->getPos();
    }
    else if((modSpeed > (maxSpeed * 0.5)) && (modSpeed < maxSpeed))
    {
        nextPos.x = (((vel.x / frameDeltaTime) * (nextWayPoint->getNext()->getNext()->getPos().x - nextWayPoint->getNext()->getPos().x)) / (maxSpeed * 0.5)) -
                    (nextWayPoint->getNext()->getNext()->getPos().x - nextWayPoint->getNext()->getPos().x);
        nextPos.y = (((vel.y / frameDeltaTime) * (nextWayPoint->getNext()->getNext()->getPos().y - nextWayPoint->getNext()->getPos().y)) / (maxSpeed * 0.5)) -
                    (nextWayPoint->getNext()->getNext()->getPos().y - nextWayPoint->getNext()->getPos().y);
        nextPos.z = (((vel.z / frameDeltaTime) * (nextWayPoint->getNext()->getNext()->getPos().z - nextWayPoint->getNext()->getPos().z)) / (maxSpeed * 0.5)) -
                    (nextWayPoint->getNext()->getNext()->getPos().z - nextWayPoint->getNext()->getPos().z);

        nextPos += nextWayPoint->getNext()->getPos();
    }
    else if(modSpeed >= maxSpeed)
    {
        nextPos = nextWayPoint->getNext()->getNext()->getPos();
    }

    return nextPos;;
}