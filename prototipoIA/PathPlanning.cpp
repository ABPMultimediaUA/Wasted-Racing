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

void PathPlanning::setMaxSpeed(float speed)
{
    maxSpeed = speed;
}

void PathPlanning::setFrame(float frame)
{
    frameDeltaTime = frame;
}

glm::vec3 PathPlanning::getNextPoint(glm::vec3 pos, glm::vec3 vel, float modSpeed)
{
    if(nextWayPoint->inside(pos))
    {
        nextWayPoint = nextWayPoint->getNext();
    }
    if(nextWayPoint->getNext()->inside(pos))
    {
        nextWayPoint = nextWayPoint->getNext()->getNext();
    }
    if(nextWayPoint->getNext()->getNext()->inside(pos))
    {
        nextWayPoint = nextWayPoint->getNext()->getNext()->getNext();
    }

    glm::vec3 nextPos;

    if(modSpeed <= 0)
    {
        nextPos = nextWayPoint->getPos(); 
        std::cout<<"Way1: "<<nextWayPoint->getPos().z<<"\n";
    }
    else if((modSpeed > 0) && (modSpeed <= (maxSpeed * 0.5)))
    {

        nextPos.x = (modSpeed * (nextWayPoint->getNext()->getPos().x - nextWayPoint->getPos().x)) / (maxSpeed * 0.5);
        nextPos.y = (modSpeed * (nextWayPoint->getNext()->getPos().y - nextWayPoint->getPos().y)) / (maxSpeed * 0.5);
        nextPos.z = (modSpeed * (nextWayPoint->getNext()->getPos().z - nextWayPoint->getPos().z)) / (maxSpeed * 0.5);

        std::cout<<"Way3: "<<nextWayPoint->getNext()->getPos().z<<"\n";
        std::cout<<"Way2: "<<nextWayPoint->getPos().z<<"\n";

        nextPos += nextWayPoint->getPos();
    }
    else if((modSpeed > (maxSpeed * 0.5)) && (modSpeed < maxSpeed))
    {
        nextPos.x = ((modSpeed * (nextWayPoint->getNext()->getNext()->getPos().x - nextWayPoint->getNext()->getPos().x)) / (maxSpeed * 0.5)) -
                    (nextWayPoint->getNext()->getNext()->getPos().x - nextWayPoint->getNext()->getPos().x);
        nextPos.y = ((modSpeed * (nextWayPoint->getNext()->getNext()->getPos().y - nextWayPoint->getNext()->getPos().y)) / (maxSpeed * 0.5)) -
                    (nextWayPoint->getNext()->getNext()->getPos().y - nextWayPoint->getNext()->getPos().y);
        nextPos.z = ((modSpeed * (nextWayPoint->getNext()->getNext()->getPos().z - nextWayPoint->getNext()->getPos().z)) / (maxSpeed * 0.5)) -
                    (nextWayPoint->getNext()->getNext()->getPos().z - nextWayPoint->getNext()->getPos().z);

        std::cout<<"Way5: "<<nextWayPoint->getNext()->getNext()->getPos().z<<"\n";
        std::cout<<"Way4: "<<nextWayPoint->getNext()->getPos().z<<"\n";

        nextPos += nextWayPoint->getNext()->getPos();
    }
    else if(modSpeed >= maxSpeed)
    {
        nextPos = nextWayPoint->getNext()->getNext()->getPos();

        std::cout<<"Way6: "<<nextWayPoint->getNext()->getNext()->getPos().z<<"\n";
    }

    return nextPos;;
}