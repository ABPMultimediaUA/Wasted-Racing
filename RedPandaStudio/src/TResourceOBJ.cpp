#include "TResourceOBJ.h"
#include <string>

std::vector<std::string> split(const std::string& s, const char& c) {
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

bool TResourceOBJ::loadResource()
{
    Assimp::Importer importer;
    //First we attempt to load the obj
    const aiScene* scene = importer.ReadFile(name, aiProcess_JoinIdenticalVertices);

    //If loaded succesfully, we proceed to get all his data
    if(scene)
    {
        //For each mesh in the obj, we create a new resourceMesh and storage the mesh data on it
        for(int i = 0; i<scene->mNumMeshes; i++)
        {
            TResourceMesh* mesh = new TResourceMesh();
            aiMesh* m = scene->mMeshes[i];
            mesh->loadMesh(m);
            meshes.push_back(mesh);
        }

        //We get the directory path to load properly the textures
        std::string s(name);
        std::vector<std::string> v = split(s, '/');
        std::string route;
        for(int i = 0; i < v.size()-1; i++)
        {
            route+=v[i] + "/";
        }

        //We proceed to get all the materials and textures
        for(int i = 0; i<scene->mNumMaterials; i++)
        {
            aiString path;
            TResourceTexture* texture = new TResourceTexture();
            //If the material has a diffuse texture, we get his path
            if(scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
            {
                //First we combine the path we just got with the directory path of the obj, and then we just load the texture
                std::string completePath = route + path.data;
                texture->setName(completePath.c_str());
                texture->loadResource();
            }
            textures.push_back(texture);
        }


        return true;
    }
    return false;
}

void TResourceOBJ::draw()
{
    //The textures, materials and meshes are loaded, suposedly, in a way that they should just correspond, so we draw one of each
    for(unsigned int i = 0; i < meshes.size(); i++)
    {
        //textures[i]->draw();
        meshes[i]->draw();
    }

}