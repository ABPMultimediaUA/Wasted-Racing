#pragma once

#include <vector>
#include "TEntity.h"

class TNode{

public:

    TNode(TNode* f) : father(f) {}
    TNode(TNode* f, TEntity* e) : father(f), entity(e) {}

    ~TNode();

    //Setters
    bool setEntity(TEntity* e);

    //Getters
    TNode* getFather();
    TEntity* getEntity();
    std::vector<TNode*> getChild();

    //Methods
    bool addChild(TNode* c);
    bool removeChild(TNode* c);

    void draw();

private:

    TNode* father;
    TEntity* entity;
    std::vector<TNode*> child;
        
};