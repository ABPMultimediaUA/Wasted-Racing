#include "PathPlanning.h"




 PathPlanning::PathPlanning()
 {
     listWay = new std::list<WayPoint*>;
 }

PathPlanning::~PathPlanning()
{
    delete lastWayPoint;
    delete nextWayPoint;
    delete startPoint;
    delete listWay;
}

void PathPlanning::addWayPoint(WayPoint* way)
{
    if (listWay->size() == 0)
    {
        startPoint = way;
        nextWayPoint = way;
        lastWayPoint = way;
    }
    else if(listWay->size() == 1)
    {
        nextWayPoint = way;
        listWay->back()->setNext(way);
    }
    else
    {
        listWay->back()->setNext(way);
    }

    listWay->push_back(way);
    listWay->back()->setNext(startPoint);

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

    tour = (modSpeed * seconds) * (modSpeed * seconds);
    float auxTour = tour;
    float distNode;
    std::list<WayPoint::Node*> listNodes = lastWayPoint->getSubNodes();

    int lvl;
    WayPoint::Node *aux;
    for(std::list<WayPoint::Node*>::iterator it = listNodes.begin(); it != listNodes.end(); ++it)
    {   
        lvl = lastWayPoint->getLastLevel();
        aux = *it;
        if(aux->getLevel() >= lvl)
        {
            lastWayPoint->setDistLastNode(lastWayPoint->getLastNode(), pos);
            distNode = (aux->getPos().x - pos.x) * (aux->getPos().x - pos.x) +
                    (aux->getPos().y - pos.y) * (aux->getPos().y - pos.y) +
                    (aux->getPos().z - pos.z) * (aux->getPos().z - pos.z);

            if(lvl == aux->getLevel())
            {
                if(lastWayPoint->getDistLastNode() == -1 || distNode < lastWayPoint->getDistLastNode())
                {
                    lastWayPoint->setLastNode(aux);
                    lastWayPoint->setLastLevel(aux->getLevel());
                }
            }
            else if((lvl+1) == aux->getLevel())
            {
                if(tour-lastWayPoint->getDistLastNode() < 0)
                {
                    nextPos = ((tour/lastWayPoint->getDistLastNode()) * (lastWayPoint->getLastNode()->getPos() - pos)) + pos;
                    
                    return nextPos;
                }
                else
                {
                    if(distNode <= tour)
                    {
                        lastWayPoint->setLastNode(aux);
                        lastWayPoint->setLastLevel(aux->getLevel());
                    }                        
                }
            }
        }
        
    }
        std::list<WayPoint::Node*> subListNodes = lastWayPoint->getLastNode()->getNextNodes();
        WayPoint::Node* nNode = lastWayPoint->getLastNode();
        float distanceNextNode  = -1;

        for(std::list<WayPoint::Node*>::iterator it = subListNodes.begin(); it != subListNodes.end(); ++it)
        {  
            aux = *it;
            distNode = (aux->getPos().x - pos.x) * (aux->getPos().x - pos.x) +
                        (aux->getPos().y - pos.y) * (aux->getPos().y - pos.y) +
                        (aux->getPos().z - pos.z) * (aux->getPos().z - pos.z);
            if(distanceNextNode == -1 || distanceNextNode > distNode)
            {
                distanceNextNode = distNode;
                lastWayPoint->setLastNode(aux);
                lastWayPoint->setLastLevel(aux->getLevel());
            }

        }

        distNode = (pos.x - nNode->getPos().x) * (pos.x - nNode->getPos().x) +
                (pos.y - nNode->getPos().y) * (pos.y - nNode->getPos().y) +
                (pos.z - nNode->getPos().z) * (pos.z - nNode->getPos().z);
        
        float dist = ( pos.x - lastWayPoint->getLastNode()->getPos().x) * ( pos.x - lastWayPoint->getLastNode()->getPos().x) +
                ( pos.y - lastWayPoint->getLastNode()->getPos().y) * ( pos.y - lastWayPoint->getLastNode()->getPos().y) +
                (pos.z - lastWayPoint->getLastNode()->getPos().z) * ( pos.z - lastWayPoint->getLastNode()->getPos().z);
        tour -= distNode;

        nextPos = ((tour/dist) * (lastWayPoint->getLastNode()->getPos() - nNode->getPos()) + nNode->getPos());
        
    return nextPos;
}