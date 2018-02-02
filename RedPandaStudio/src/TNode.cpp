#include "TNode.h"

TNode::~TNode()
{
    delete father;
    delete entity;

    for(unsigned int i = 0; i < child.size(); i++)
    {
        delete child[i];
    }
    child.clear();
}

///////////////////////////////
/////       SETTERS       /////
///////////////////////////////

bool TNode::setEntity(TEntity* e)
{
    e = entity;
    return true;
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

std::vector<TNode*> TNode::getChild()
{
    return child;
}


///////////////////////////////
/////       METHODS       /////
///////////////////////////////

bool TNode::addChild(TNode* c)
{
    //Push_back es una funcion void, siempre insertará el valor que se le pasa
    //Y en caso de que no lo consiga, lanzará una excepción 'bad_alloc' parando la ejecución
    child.push_back(c);
    return true;
}

bool TNode::removeChild(TNode* c)
{
    for(unsigned int i = 0; i < child.size(); i++)
    {
        if(c == child[i])
        {
            //Erase devuelve un iterator al elemento que sigue al borrado o una excepción si se intenta
            //borrar algo fuera de rango
            child.erase(child.begin()+i);
            return true;
        }
    }
    return false;
}

void TNode::draw()
{
    entity->beginDraw();
    for(unsigned int i = 0; i < child.size(); i++)
    {
        child[i]->draw();
    }
    entity->endDraw();
}