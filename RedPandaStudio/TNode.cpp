#include "TNode.h"



TNode::~TNode()
{
    delete father;
    delete entity;

    for(int i = 0; i < child.size(); i++)
    {
        delete child[i];
    }
    child.clear();
}


///////////////////////////////
/////       GETTERS       /////
///////////////////////////////

TNode* TNode::getFather()
{
    return father;
}

TEntity* TNode::getEntity()
{
    return entity;
}

vector<TNode*> TNode::getChild()
{
    return child;
}


///////////////////////////////
/////       METHODS       /////
///////////////////////////////

bool TNode::addChild(TNode* c)
{
    if(child.push_back(c))
    {
        return true;
    }
    return false;
}

bool TNode::removeChild(TNode* c)
{
    for(int i = 0; i < child.size(); i++)
    {
        if(c == child[i])
        {
            if(child.erase(child.begin()+i))
            {
                return true;
            }
        }
    }
    return false;
}

