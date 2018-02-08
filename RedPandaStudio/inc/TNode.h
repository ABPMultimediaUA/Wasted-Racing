#pragma once

#include <vector>
#include "TEntity.h"

class TNode{

public:

    TNode(TNode* f) : father(f) {}
    TNode(TNode* f, TEntity* e) : father(f), entity(e) {}

    ~TNode();

    //Setters
    bool setEntity(TEntity* e)                  {       e = entity;     
                                                        return true;        }

    //Getters
    TNode* getFather()                          {       return father;      }
    TEntity* getEntity()                        {       return entity;      }
    std::vector<TNode*> getChild()              {       return child;       }

    //Methods
    bool addChild(TNode* c);
    bool removeChild(TNode* c);
    TNode* searchChild(unsigned int c);

    //Draw tree
    void draw();

private:

    TNode* father;
    TEntity* entity;
    std::vector<TNode*> child;
        
};