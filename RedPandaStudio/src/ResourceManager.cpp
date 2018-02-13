#include "ResourceManager.h"

TResource* ResourceManager::getResource(char* n)
{
    unsigned int i;
    TResource* res;
    bool found = false;

    //First we look in memory loaded resources
    for(i=0; i<resources.size() && found == false; i++)
    {
        if(strcmp(n, resources[i]->getName()) == 0)
        {
            found = true;
            res=resources[i];
        }
    }
    //If it's not loaded, we create a new resource and try to load it
    if(found == false)
    {
        res->setName(n);
        if(res->loadResource())
        {
            resources.push_back(res);
        }
    }

    return res;
}