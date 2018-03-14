#include "TResourceMesh.h"
#include <iostream>

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
    vertexIndices = (unsigned int *)malloc(sizeof(unsigned int) * nFaces * 3);
    unsigned int faceIndex = 0;

    for(int j = 0; j<nFaces; j++)
    {
        memcpy(&vertexIndices[faceIndex], m->mFaces[j].mIndices, 3 * sizeof(unsigned int));
        faceIndex += 3;
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

}

bool TResourceMesh::loadResource()
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(name, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

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
    
    glEnable(GL_COLOR_MATERIAL);
    GLfloat mat_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat mat_diffuse[] = { 0.8, 0.5, 0.1, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_specular);

    GLuint id = glGetUniformLocation(TEntity::getProgramID(), "textActive");
    glUniform1i(id, textActive);

    //First we draw the texture of our mesh
    if(texture!=NULL && textActive)
    {
        texture->draw();
    }
    
    //Generate an array of 4 buffer identifiers
    GLuint* vboHandles = (unsigned int *)malloc(sizeof(unsigned int) *4);
    glGenBuffers(4, vboHandles);

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles*3*sizeof(unsigned int), vertexIndices, GL_STATIC_DRAW);

    //We order to draw here
    glDrawElements(GL_TRIANGLES, nTriangles*3, GL_UNSIGNED_INT, 0);
/*
    if(texture!=NULL && textActive)
    {
        texture->endDraw();
    }
*/

}