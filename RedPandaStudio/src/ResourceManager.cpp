#include "ResourceManager.h"

TResource* ResourceManager::getResource(char* n)
{
    unsigned int i;
    TResource* res;
    bool found = false;

    for(i=0; i<resources.size() && found == false; i++)
    {
        if(strcmp(n, resources[i]->getName()) == 0)
        {
            found = true;
            res=resources[i];
        }
    }
    if(found == false)
    {
        
    }

    return res;
}