#include "WaypointComponent.h"


//////////Node//////////

WaypointComponent::Node::Node(glm::vec3 pos, float rad, int lvl)
{
    position = pos;
    radius = rad;
    level = lvl;
    nextNodes = new std::list<WaypointComponent::Node*>;
}

WaypointComponent::Node::~Node()
{
    delete nextNodes;
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

int WaypointComponent::Node::getLevel()
{
    return level;
}

void WaypointComponent::Node::setLevel(int lvl)
{
    level = lvl;
}

void WaypointComponent::Node::addNextNodes(WaypointComponent::Node* n)
{
    nextNodes->push_back(n);
}

std::list<WaypointComponent::Node*> WaypointComponent::Node::getNextNodes()
{
    return *nextNodes;
}

bool WaypointComponent::Node::checkNext(glm::vec3 pos)
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

void WaypointComponent::Node::clearListNext()
{
    nextNodes->clear();
}

int WaypointComponent::Node::getAccess()
{
    return access;
}
void WaypointComponent::Node::maxAcces()
{
    access++;
}
////////WayPoint////////

WaypointComponent::WaypointComponent(GameObject& newGameObject, glm::vec3 pos, float rad) : IComponent(newGameObject)
{
    startPoint = new Node(pos, rad, 0);
    lastNode = startPoint;
    listSubNodes = new std::list<Node*>;
    listSubNodes->push_back(startPoint);
    startPoint->addNextNodes(startPoint);
    auxNode = new Node(glm::vec3(0.f,0.f,0.f), 0.f, 0);
}
WaypointComponent::~WaypointComponent()
{
    delete next;
    delete startPoint;
    delete auxNode;
//check how remove list
    delete listSubNodes;
}

glm::vec3 WaypointComponent::getPos()
{
    return startPoint->getPos();
}

WaypointComponent::Node* WaypointComponent::getNode()
{
    return startPoint;
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
    //listSubNodes->push_back(next->getNode());
}

float WaypointComponent::getDistNextWays()
{
    return distNextWay;
}

void WaypointComponent::addSubNodes(glm::vec3 pos, float rad, int lvl)
{
    Node* node = new Node(pos, rad, lvl); 
    for(std::list<WaypointComponent::Node*>::iterator it = listSubNodes->begin(); it != listSubNodes->end(); ++it){
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

    for(std::list<WaypointComponent::Node*>::iterator it = listSubNodes->begin(); it != listSubNodes->end(); ++it){
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

std::list<WaypointComponent::Node*> WaypointComponent::getSubNodes()
{
    return *listSubNodes;
}

WaypointComponent::Node *WaypointComponent::getLastNode()
{
    return lastNode;
}

void WaypointComponent::setLastNode(Node* n)
{
    lastNode = n;
}

glm::vec3 WaypointComponent::nextNode()
{
    return lastNode->getPos();
}

void WaypointComponent::setDistLastNode(Node *n, glm::vec3 pos)
{
    distLastNode = (n->getPos().x - pos.x) * (n->getPos().x - pos.x) +
                (n->getPos().y - pos.y) * (n->getPos().y - pos.y) +
                (n->getPos().z - pos.z) * (n->getPos().z - pos.z);
}

float WaypointComponent::getDistLastNode()
{
    return distLastNode;
}

void WaypointComponent::setLastLevel(int lvl)
{
    lastLevel = lvl;
}

int WaypointComponent::getLastLevel()
{
    return lastLevel;
}
