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

    
    if(nextWayPoint->checkNext(pos, nextWayPoint->getPos()))
    {
        lastWayPoint = nextWayPoint;
        nextWayPoint = nextWayPoint->getNext();
    }

    float distance = (nextWayPoint->getPos().x - pos.x) * (nextWayPoint->getPos().x - pos.x) +
                     (nextWayPoint->getPos().y - pos.y) * (nextWayPoint->getPos().y - pos.y) +
                     (nextWayPoint->getPos().z - pos.z) * (nextWayPoint->getPos().z - pos.z);


    tour = (modSpeed * seconds) * (modSpeed * seconds);
    float auxTour = tour;
    float distNode;
    std::list<WayPoint::Node*> listNodes = lastWayPoint->getSubNodes();
    //////////////////////Iteration 3/////////////////////////
    
    
        int lvl;
        WayPoint::Node *aux;

        std::cout<<"LASTNODE: "<<lastWayPoint->getLastNode()->getPos().x<<"\n";
        std::cout<<"LASTNODE: "<<lastWayPoint->getLastLevel()<<"\n";
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
                        std::cout<<"3: "<<"\n";
                        lastWayPoint->setLastNode(aux);
                        lastWayPoint->setLastLevel(aux->getLevel());
                    }
                }
                else if((lvl+1) == aux->getLevel())
                {
                        std::cout<<"AQUIIIIIIIIIIIII: "<<lastWayPoint->getDistLastNode()<<"\n";
                        std::cout<<"AQUIIII: "<<tour<<"\n";
                    if(tour-lastWayPoint->getDistLastNode() < 0)
                    {
                        std::cout<<"1: "<<"\n";
                        nextPos = ((tour/lastWayPoint->getDistLastNode()) * (lastWayPoint->getLastNode()->getPos() - pos)) + pos;
                        //lastWayPoint->setLastNode(aux);
                        //lastWayPoint->setLastLevel(aux->getLevel());
                            //lastWayPoint->setDistLastNode(-1.f);
                        return nextPos;
                    }
                    else
                    {
                        std::cout<<"2: "<<"\n";
                        //nextPos = lastWayPoint->getLastNode();
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

        std::cout<<"TOUR: "<<tour<<"\n";
        std::cout<<"DIST: "<<dist<<"\n";
        std::cout<<"distNode: "<<distNode<<"\n";

        nextPos = ((tour/dist) * (lastWayPoint->getLastNode()->getPos() - nNode->getPos()) + nNode->getPos());
        
    
        


    //////////////////////Iteration 2/////////////////////////

/*
    if(tour <= distance)
    {
        nextPos = nextWayPoint->getPos();
    }
    else
    {
        tour -= distance;
        for(std::list<WayPoint*>::iterator it = listWay->begin(); it != listWay->end() && tour >= nextWayPoint->getDistNextWays(); ++it){
            tour -= nextWayPoint->getDistNextWays();
            nextWayPoint = nextWayPoint->getNext();
        }
        nextPos = (tour/nextWayPoint->getDistNextWays()) * (nextWayPoint->getNext()->getPos() - nextWayPoint->getPos()) + nextWayPoint->getPos();
    }

    if(nextWayPoint->checkNext(pos, nextPos))
    {
        lastWayPoint = nextWayPoint;
        nextWayPoint = nextWayPoint->getNext();
    }
*/
    return nextPos;
}

void PathPlanning::checkLists()
{
    WayPoint::Node *aux;
    int i, j;
    i=0;
    j=0;
    std::list<WayPoint::Node*> list = lastWayPoint->getSubNodes();
    std::list<WayPoint::Node*> list2;
    for(std::list<WayPoint::Node*>::iterator it = list.begin(); it != list.end(); ++it)
    {   
        i++;
        aux = *it;
        std::cout<<i<<": "<<aux->getPos().x<<"\n";
        j=0;
        list2 = aux->getNextNodes();
        for(std::list<WayPoint::Node*>::iterator it = list2.begin(); it != list2.end(); ++it)
        {  
            j++;
            aux = *it;
            std::cout<<j<<": "<<aux->getPos().x<<"\n";
        }

    }
}