#include "TResourceOBJ.h"

bool TResourceOBJ::loadResource()
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(name, aiProcess_JoinIdenticalVertices);

    if(scene)
    {
        for(int i = 0; i<scene->mNumMeshes; i++)
        {
            TResourceMesh* mesh = new TResourceMesh();
            aiMesh* m = scene->mMeshes[i];
            mesh->loadMesh(m);
            meshes.push_back(mesh);


        }
    }


}

void TResourceOBJ::draw()
{
    for(unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i]->draw();
    }

}
