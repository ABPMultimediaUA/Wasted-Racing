#include "TResourceMesh.h"
#include <iostream>

unsigned int getAdjacentIndex(aiMesh* m, const unsigned int index1, const unsigned int index2, const unsigned int index3);

bool TResourceMesh::loadMesh(aiMesh* m)
{
    int nFaces = m->mNumFaces;
    nTriangles = nFaces;
    nVertex = m->mNumVertices;
    
    vertex = (float *)malloc(sizeof(float) * nVertex * 3);
    memcpy(&vertex[0], m->mVertices, 3 * sizeof(float) * nVertex);

    if(m->HasNormals())
    {
        normals = (float *)malloc(sizeof(float) * nVertex * 3);
        memcpy(&normals[0], m->mNormals, 3 * sizeof(float) * nVertex);
    }

    //We assume we are always working with triangles
    vertexIndices = (unsigned int *)malloc(sizeof(unsigned int) * nFaces * 6);
    unsigned int faceIndex = 0;

    for(int j = 0; j<nFaces; j++, faceIndex += 6)
    {
        vertexIndices[0+faceIndex] = m->mFaces[j].mIndices[0];
        vertexIndices[2+faceIndex] = m->mFaces[j].mIndices[1];
        vertexIndices[4+faceIndex] = m->mFaces[j].mIndices[2];
        vertexIndices[1+faceIndex] = getAdjacentIndex(m, vertexIndices[0+faceIndex], vertexIndices[2+faceIndex], vertexIndices[4+faceIndex]);
        vertexIndices[3+faceIndex] = getAdjacentIndex(m, vertexIndices[2+faceIndex], vertexIndices[4+faceIndex], vertexIndices[0+faceIndex]);
        vertexIndices[5+faceIndex] = getAdjacentIndex(m, vertexIndices[4+faceIndex], vertexIndices[0+faceIndex], vertexIndices[2+faceIndex]);
    }

    if(m->HasTextureCoords(0))
    {
        textures=(float *)malloc(sizeof(float)*2*nVertex);
        for(unsigned int k = 0; k<nVertex;k++)
        {
            textures[k*2] = m->mTextureCoords[0][k].x;
            textures[k*2+1] = m->mTextureCoords[0][k].y;
        }
    }

    //Generate an array of 4 buffer identifiers
    vboHandles = (unsigned int *)malloc(sizeof(unsigned int) *4);
    glGenBuffers(4, vboHandles);

/*
    std::cout << "nVertex: " << nVertex << std::endl;
    std::cout << "nTriangles: " << nTriangles << std::endl;

    for(int i = 0; i < nTriangles * 6; i++)
    {
        std::cout << i << ": " << vertexIndices[i] << std::endl;
    }

    for(int i = 0; i < nVertex * 3; i++)
    {
        std::cout << i << ": " << vertex[i] << std::endl;
    }
*/
    return true;

}

bool TResourceMesh::loadResource()
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(name, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);

    nTriangles = 0;

    if(scene)
    {
        for(unsigned int i = 0; i<scene->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[i];
            int nFaces = mesh->mNumFaces;
            //We get the vertex, normals and textures arrays and prepare them for OpenGL calls
            
            nTriangles += nFaces;
            nVertex = mesh->mNumVertices;

            vertex = (float *)malloc(sizeof(float) * nVertex * 3);
            memcpy(&vertex[0], mesh->mVertices, 3 * sizeof(float) * nVertex);

            if(mesh->HasNormals())
            {
                normals = (float *)malloc(sizeof(float) * nVertex * 3);
                memcpy(&normals[0], mesh->mNormals, 3 * sizeof(float) * nVertex);
            }

            //We assume we are always working with triangles
            vertexIndices = (unsigned int *)malloc(sizeof(unsigned int) * nFaces * 3);
            unsigned int faceIndex = 0;

            for(int j = 0; j<nFaces; j++)
            {
                memcpy(&vertexIndices[faceIndex], mesh->mFaces[j].mIndices, 3 * sizeof(unsigned int));
                faceIndex += 3;
            }
            if(mesh->HasTextureCoords(0))
            {
                textures=(float *)malloc(sizeof(float)*2*nVertex);
                for(unsigned int k = 0; k<nVertex;k++)
                {
                    textures[k*2] = mesh->mTextureCoords[0][k].x;
                    textures[k*2+1] = mesh->mTextureCoords[0][k].y;
                }
            }

        }
        return true;
    }
    return false;
}

void TResourceMesh::draw()
{
    
    //glEnable(GL_COLOR_MATERIAL);

    GLuint id = glGetUniformLocation(TEntity::getProgramID(), "textActive");
    glUniform1i(id, textActive);

    //First we draw the texture of our mesh
    if(texture!=NULL && textActive)
    {
        texture->draw();
    }
    
    if(material!=NULL)
    {
        material->draw();
    }

    //Bind and pass to OpenGL the first array (vertex coordinates)
    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
    glBufferData(GL_ARRAY_BUFFER, nVertex*3*sizeof(float), vertex, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    glEnableVertexAttribArray(0);
    
    //Bind and pass to OpenGL the second array (vertex normals)
    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);
    glBufferData(GL_ARRAY_BUFFER, nVertex*3*sizeof(float), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    glEnableVertexAttribArray(1);

    //Bind and pass to OpenGL the third array (vertex texture coordinates)
    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[2]);
    glBufferData(GL_ARRAY_BUFFER, nVertex*2*sizeof(float), textures, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    glEnableVertexAttribArray(2);

    //Bind and pass to OpenGL the fourth array (vertex indices)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandles[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles*6*sizeof(unsigned int), vertexIndices, GL_STATIC_DRAW);

    //We order to draw here
    glDrawElements(GL_TRIANGLES_ADJACENCY, nTriangles*6, GL_UNSIGNED_INT, 0);
/*
    if(texture!=NULL && textActive)
    {
        texture->endDraw();
    }
*/

}

//This functions looks for a specific adjacent vertex for the vertex indices. Due to the computational cost, this should be improved
//using the half-edge structure, to avoid a massive number of searchs through the mesh
unsigned int getAdjacentIndex(aiMesh* m, const unsigned int index1, const unsigned int index2, const unsigned int index3)
{
    for(unsigned int i=0; i < m->mNumFaces; i++)
    {
        for(int edge = 0; edge < 3; edge++)
        {
            unsigned int v1 = m->mFaces[i].mIndices[edge];
            unsigned int v2 = m->mFaces[i].mIndices[(edge+1)%3];
            unsigned int v3 = m->mFaces[i].mIndices[(edge+2)%3];

            if((v1 == index1 && v2 == index2) || (v1 == index2 && v2 == index1))
            {
                //std::cout << "Index1: " << index1 << " and index2: " << index2 << std::endl;
                //std::cout << "Entro aqui: " << v3 << " and index3: " << index3 << std::endl;
            }

            if(((v1 == index1 && v2 == index2) || (v1 == index2 && v2 == index1)) && (v3 != index3))
            {
                //std::cout << "Aqui tambien" << std::endl;
                return v3;
            }
        }
    }
    return 0;
}