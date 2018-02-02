#pragma once
#include "TEntity.h"

class TNode{

    private:

        TNode* father;
        TEntity* entity;
        vector<TNode*> child;

    public:

        TNode(TNode* f) : father(f);
        TNode(TNode* f, TEntity* e) : father(f), entity(e);

        ~TNode();

        //Setters
        bool setEntity(TEntity* e);

        //Getters
        TNode* getFather();
        TEntity* getEntity();
        vector<TNode*> getChild();

        //Methods
        bool addChild(TNode* c);
        bool removeChild(TNode* c);

        void draw();

}