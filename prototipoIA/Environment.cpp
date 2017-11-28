#include "Environment.h"

//Constructor
Environment::Environment()
{
    size=10;
    list=new irr::scene::ISceneNode*[size];
}

Environment::~Environment()
{
    delete[] list;
}

irr::scene::ISceneNode** Environment::getList()
{
    return list;
}

int Environment::getSize()
{
    return size;
}

void Environment::addNode(irr::scene::ISceneNode* n)
{
    bool found=false;
    int i=0;

    for(i=0; i<size&&found==false; i++)
    {
        if(list[i]==NULL)
        {
            found=true;
            list[i]=n;
        }
    }
}