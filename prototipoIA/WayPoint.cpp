#include "WayPoint.h"


//////////Node//////////

WayPoint::Node::Node(glm::vec3 pos, float rad, int lvl)
{
    position = pos;
    radius = rad;
    level = lvl;
    nextNodes = new std::list<WayPoint::Node*>;
}

WayPoint::Node::~Node()
{
    delete nextNodes;
}

float WayPoint::Node::getRadius()
{
    return radius;
}

void WayPoint::Node::setRadius(float rad)
{
    radius = rad;
}

glm::vec3 WayPoint::Node::getPos()
{
    return position;
}

void WayPoint::Node::setPos(glm::vec3 pos)
{
    position = pos;
}

int WayPoint::Node::getLevel()
{
    return level;
}

void WayPoint::Node::setLevel(int lvl)
{
    level = lvl;
}

void WayPoint::Node::addNextNodes(WayPoint::Node* n)
{
    nextNodes->push_back(n);
}

std::list<WayPoint::Node*> WayPoint::Node::getNextNodes()
{
    return *nextNodes;
}

bool WayPoint::Node::checkNext(glm::vec3 pos)
{
    float distWay = (pos.x-position.x)*(pos.x-position.x) + 
                    (pos.z-position.z)*(pos.z-position.z) +
                    (pos.y-position.y)*(pos.y-position.y);


    if(distWay <= (radius * radius))
    {
        return true;
    }
    return false;
}

void WayPoint::Node::clearListNext()
{
    nextNodes->clear();
}

int WayPoint::Node::getAccess()
{
    return access;
}
void WayPoint::Node::maxAcces()
{
    access++;
}
////////WayPoint////////

WayPoint::WayPoint(glm::vec3 pos, float rad)
{
    startPoint = new Node(pos, rad, 0);
    lastNode = startPoint;
    listSubNodes = new std::list<Node*>;
    listSubNodes->push_back(startPoint);
    startPoint->addNextNodes(startPoint);
    auxNode = new Node(glm::vec3(0.f,0.f,0.f), 0.f, 0);
}
WayPoint::~WayPoint()
{
    delete next;
    delete startPoint;
    delete auxNode;
//check how remove list
    delete listSubNodes;
}

glm::vec3 WayPoint::getPos()
{
    return startPoint->getPos();
}

WayPoint::Node* WayPoint::getNode()
{
    return startPoint;
}

bool WayPoint::checkNext(glm::vec3 pos, glm::vec3 nextPos)
{
    float distWay = (pos.x-startPoint->getPos().x)*(pos.x-startPoint->getPos().x) + 
                    (pos.z-startPoint->getPos().z)*(pos.z-startPoint->getPos().z) +
                    (pos.y-startPoint->getPos().y)*(pos.y-startPoint->getPos().y);

    float distNextPos = (pos.x-nextPos.x)*(pos.x-nextPos.x) + 
                        (pos.z-nextPos.z)*(pos.z-nextPos.z) +
                        (pos.y-nextPos.y)*(pos.y-nextPos.y);

    if(distWay <= (startPoint->getRadius() * startPoint->getRadius()) || distNextPos < distWay)
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
    //listSubNodes->push_back(next->getNode());
}

float WayPoint::getDistNextWays()
{
    return distNextWay;
}

void WayPoint::addSubNodes(glm::vec3 pos, float rad, int lvl)
{
    Node* node = new Node(pos, rad, lvl); 
    for(std::list<WayPoint::Node*>::iterator it = listSubNodes->begin(); it != listSubNodes->end(); ++it){
        auxNode = *it;
        if(auxNode->getAccess() == 0)
        {
            auxNode->clearListNext();
            auxNode->maxAcces();
        }
        if(auxNode->getLevel() == 0)
        {
            node->addNextNodes(auxNode);
        }
    }

    listSubNodes->push_back(node);

    for(std::list<WayPoint::Node*>::iterator it = listSubNodes->begin(); it != listSubNodes->end(); ++it){
        auxNode = *it;
        if(auxNode->getLevel() == lvl-1)
        {
            if(auxNode->getAccess() == 0)
            {
                auxNode->clearListNext();
                auxNode->maxAcces();
            }
            auxNode->addNextNodes(node);
        }
    }

}

std::list<WayPoint::Node*> WayPoint::getSubNodes()
{
    return *listSubNodes;
}

WayPoint::Node *WayPoint::getLastNode()
{
    return lastNode;
}

void WayPoint::setLastNode(Node* n)
{
    lastNode = n;
}

glm::vec3 WayPoint::nextNode()
{
    return lastNode->getPos();
}

void WayPoint::setDistLastNode(Node *n, glm::vec3 pos)
{
    distLastNode = (n->getPos().x - pos.x) * (n->getPos().x - pos.x) +
                (n->getPos().y - pos.y) * (n->getPos().y - pos.y) +
                (n->getPos().z - pos.z) * (n->getPos().z - pos.z);
}

float WayPoint::getDistLastNode()
{
    return distLastNode;
}

void WayPoint::setLastLevel(int lvl)
{
    lastLevel = lvl;
}

int WayPoint::getLastLevel()
{
    return lastLevel;
}