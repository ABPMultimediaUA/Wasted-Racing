#include "ResourceManager.h"

TResourceMesh* ResourceManager::getResourceMesh(const char* n)
{
    unsigned int i;
    TResourceMesh* res=NULL;
    bool found = false;

    //First we look in memory loaded resources
    for(i=0; i<meshes.size() && found == false; i++)
    {
        if(strcmp(n, meshes[i]->getName()) == 0)
        {
            found = true;
            res=meshes[i];
        }
    }
    //If it's not loaded, we create a new resource and try to load it
    if(found == false)
    {
        res = new TResourceMesh();

        res->setName(n);
        if(res->loadResource())
        {
            meshes.push_back(res);
        }
    }
    
    return res;
}

TResourceMaterial* ResourceManager::getResourceMaterial(const char* n)
{
    unsigned int i;
    TResourceMaterial* res=NULL;
    bool found = false;

    //First we look in memory loaded resources
    for(i=0; i<materials.size() && found == false; i++)
    {
        if(strcmp(n, materials[i]->getName()) == 0)
        {
            found = true;
            res=materials[i];
        }
    }
    //If it's not loaded, we create a new resource and try to load it
    if(found == false)
    {
        res = new TResourceMaterial();

        res->setName(n);
        if(res->loadResource())
        {
            materials.push_back(res);
        }
    }
    
    return res;
}


TResourceTexture* ResourceManager::getResourceTexture(const char* n)
{
    unsigned int i;
    TResourceTexture* res=NULL;
    bool found = false;

    //First we look in memory loaded resources
    for(i=0; i<textures.size() && found == false; i++)
    {
        if(strcmp(n, textures[i]->getName()) == 0)
        {
            found = true;
            res=textures[i];
        }
    }
    //If it's not loaded, we create a new resource and try to load it
    if(found == false)
    {
        res = new TResourceTexture();

        res->setName(n);
        if(res->loadResource())
        {
            textures.push_back(res);
        }
    }
    
    return res;
}

TResourceShader* ResourceManager::getResourceShader(const char* n, GLenum e)
{
    unsigned int i;
    TResourceShader* res=NULL;
    bool found = false;

    //First we look in memory loaded resources
    for(i=0; i<shaders.size() && found == false; i++)
    {
        if(strcmp(n, shaders[i]->getName()) == 0)
        {
            found = true;
            res=shaders[i];
        }
    }
    //If it's not loaded, we create a new resource and try to load it
    if(found == false)
    {
        res = new TResourceShader();

        res->setName(n);
        res->setShaderType(e);
        if(res->loadResource())
        {
            shaders.push_back(res);
        }
    }
    
    return res;
}

TResourceOBJ* ResourceManager::getResourceOBJ(const char* n)
{
    unsigned int i;
    TResourceOBJ* res=NULL;
    bool found = false;

    //First we look in memory loaded resources
    for(i=0; i<objs.size() && found == false; i++)
    {
        if(strcmp(n, objs[i]->getName()) == 0)
        {
            found = true;
            res=objs[i];
        }
    }
    //If it's not loaded, we create a new resource and try to load it
    if(found == false)
    {
        res = new TResourceOBJ();

        res->setName(n);
        if(res->loadResource())
        {
            objs.push_back(res);
        }
    }
    
    return res;
}

TResourceAnimation* ResourceManager::getResourceAnimation(const char* n, int frames)
{
    unsigned int i;
    TResourceAnimation* res=NULL;
    bool found = false;

    //First we look in memory loaded resources
    for(i=0; i<animations.size() && found == false; i++)
    {
        if(strcmp(n, animations[i]->getName()) == 0)
        {
            found = true;
            res=animations[i];
        }
    }
    //If it's not loaded, we create a new resource and try to load it
    if(found == false)
    {
        res = new TResourceAnimation();
        
        res->setFrames(frames);
        res->setName(n);
        if(res->loadResource())
        {
            animations.push_back(res);
        }
    }
    
    return res;
}


TResourceLoD* ResourceManager::getResourceLoD(const char* n)
{
    unsigned int i;
    TResourceLoD* res=NULL;
    bool found = false;

    //First we look in memory loaded resources
    for(i=0; i<lods.size() && found == false; i++)
    {
        if(strcmp(n, lods[i]->getName()) == 0)
        {
            found = true;
            res=lods[i];
        }
    }
    //If it's not loaded, we create a new resource and try to load it
    if(found == false)
    {
        res = new TResourceLoD();
        TResourceOBJ* rObj = ResourceManager::getResourceOBJ(n);

        res->setName(n);
        
        res->insertObj(0,rObj);
        lods.push_back(res);
        
    }
    
    return res;
}