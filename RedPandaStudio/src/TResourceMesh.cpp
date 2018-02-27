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
            
            nTriangles += nFaces;
            nVertex = mesh->mNumVertices;

            vertex = (float *)malloc(sizeof(float) * nVertex * 3);
            memcpy(&vertex[0], mesh->mVertices, 3 * sizeof(float) * nVertex);

            normals = (float *)malloc(sizeof(float) * nVertex * 3);
            memcpy(&normals[0], mesh->mNormals, 3 * sizeof(float) * nVertex);

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

        if(image.loadFromFile("Link/YoungLink_grp.png"))
        {
            std::cout << "LOADING" << std::endl;
        }
        sizeX = image.getSize().x;
        sizeY = image.getSize().y;

        std::cout << "X: " << sizeX << std::endl;
        std::cout << "Y: " << sizeY << std::endl;

        return true;
    }
    return false;
}

void TResourceMesh::draw()
{
    
    GLuint textureID;
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glEnable(GL_TEXTURE_2D);

    //glActiveTexture(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)sizeX, (GLsizei)sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());



    GLuint* vboHandles = (unsigned int *)malloc(sizeof(unsigned int) *4);
    glGenBuffers(4, vboHandles);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
    glBufferData(GL_ARRAY_BUFFER, nVertex*3*sizeof(float), vertex, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);
    glBufferData(GL_ARRAY_BUFFER, nVertex*3*sizeof(float), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[2]);
    glBufferData(GL_ARRAY_BUFFER, nVertex*2*sizeof(float), textures, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandles[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles*3*sizeof(unsigned int), vertexIndices, GL_STATIC_DRAW);

    //We order to draw here
    glDrawElements(GL_TRIANGLES, nTriangles*3, GL_UNSIGNED_INT, 0);


}