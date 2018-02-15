#include "TResourceMesh.h"
#include <iostream>

bool TResourceMesh::loadResource()
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(name, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

    if(scene)
    {
        std::cout << "Succesfully loaded" << std::endl;
        for(unsigned int i = 0; i<scene->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[i];
            int nFaces = mesh->mNumFaces;
            //We get the vertex, normals and textures arrays and prepare them for OpenGL calls
            vertex = &mesh->mVertices[0][0];
            normals = mesh->mNormals[0][0];
            textures = mesh->mTextureCoords[0][0][0];
            nTriangles += nFaces;
            nVertex = mesh->mNumVertices;

            //We assume we are always working with triangles
            vertexIndices = (unsigned int *)malloc(sizeof(unsigned int) * nFaces * 3);
            unsigned int faceIndex = 0;

            for(unsigned int j = 0; j<nFaces; j++)
            {
                memcpy(&vertexIndices[faceIndex], mesh->mFaces[j].mIndices, 3 * sizeof(unsigned int));
                faceIndex += 3;
            }

        }
        return true;
    }
    return false;
}

void TResourceMesh::draw()
{
    GLuint* vboHandles = (unsigned int *)malloc(sizeof(unsigned int) *2);
    glGenBuffers(2, vboHandles);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
    glBufferData(GL_ARRAY_BUFFER, nVertex*3*sizeof(float), vertex, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandles[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles*3*sizeof(unsigned int), vertexIndices, GL_STATIC_DRAW);


    //We order to draw here
    glDrawElements(GL_TRIANGLES, nTriangles, GL_UNSIGNED_INT, 0);


}