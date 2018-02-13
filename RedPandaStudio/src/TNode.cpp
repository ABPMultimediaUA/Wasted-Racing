#include "TNode.h"

TNode::TNode()
{
    father = nullptr;
    entity = nullptr;
}


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

TNode* TNode::searchChild(unsigned int c)
{
    if(c > 0 && c < child.size()-1)
    {
        return nullptr;
    }
    return child[c];
}

///////////////////////////////
/////        DRAW         /////
///////////////////////////////

void TNode::draw()
{
        
    if(entity != nullptr)
    {
        entity->beginDraw();
    }
    for(unsigned int i = 0; i < child.size(); i++)
    {
        child[i]->draw();
    }
    if(entity != nullptr)
    {
        entity->endDraw();
    }
}