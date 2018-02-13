#include "TResourceMesh.h"

bool TResourceMesh::loadResource()
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(name, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

    if(scene)
    {
        for(int i = 0; i<scene->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[i];
            vertex = (int*)&mesh->mVertices[0][0];
            normals = mesh->mNormals[0][0];
            textures = mesh->mTextureCoords[0][0][0];
            nTriangles += mesh->mNumFaces;

        }
        return true;
    }
    return false;
}

void TResourceMesh::draw()
{
    glDrawArrays(GL_TRIANGLES, vertex[0], nTriangles);
}