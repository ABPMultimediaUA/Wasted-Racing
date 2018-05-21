#include "TResourceAnimation.h"
#include <iostream>

std::vector<std::string> TResourceAnimation::split(const std::string& s, const char& c) {
	std::string buff{""};
	std::vector<std::string> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

TResourceAnimation::~TResourceAnimation()
{
    for(unsigned int i = 0; i < objs.size(); i++)
    {
        delete objs[i];
    }
    objs.clear();
}

bool TResourceAnimation::loadResource()
{

    for(int i = 1; i < frames; i++)
    {
        std::string route = name;
        if(i < 10)
        {
            route += "00";
        }
        else if(i < 100)
        {
            route += "0";
        }
            route += std::to_string(i) + ".obj";
            TResourceOBJ* obj = new TResourceOBJ();
            obj->setName(route.c_str());
            obj->loadOnlyMeshes();
            objs.push_back(obj);
    }

    populateTextures();
    

    if(objs.size() > 0)
    {
        return true;
    }
    return false;
}

void TResourceAnimation::populateTextures()
{
    std::string s = name;
    s+= "000.obj";

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(s, 0);

    if(scene)
    {
        std::vector<std::string> v = split(s, '/');
        std::string route;

        if(v[0].compare("home") == 0)
            route = "/";

        for(unsigned int i = 0; i < v.size()-1; i++)
        {
            route+=v[i] + "/";
        }
        for(unsigned int i = 1; i<scene->mNumMaterials; i++)
        {
            
            TResourceMaterial* mat = new TResourceMaterial();
            mat->loadResource(scene->mMaterials[i]);
            for(unsigned int j = 0; j < objs.size(); j++)
            {
                objs[j]->setMaterial(i-1, mat);
            }
        }
    }

}

void TResourceAnimation::draw()
{
    if(texture != NULL)
    {
        texture->draw();
    }
    objs[0]->draw();
}

void TResourceAnimation::draw(unsigned int i)
{
    if(i >= 0 && i < objs.size())
    {
        if(texture != NULL)
        {
            texture->draw();
        }
        else
        {
            std::cout << "Texture is null" << std::endl;
        }
        objs[i]->setTexture(0, texture);
        objs[i]->draw();
    }
}