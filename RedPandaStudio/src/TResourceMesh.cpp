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

    //Generates the two points needed for a parallel-to-edges bounding box
    generateBoundingBox();

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
    if((bbActivated && checkBoundingBox()) || !bbActivated)
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

        drawBoundingBox();
        /*
        if(texture!=NULL && textActive)
        {
            texture->endDraw();
        }
        */
    }
    else
    {
        std::cout << "Frustum culling working" << std::endl;
    }
}

void TResourceMesh::generateBoundingBox()
{
    //In first case, we initialize all the variables to the first vertex coordinates
    minX = vertex[0];
    minY = vertex[1];
    minZ = vertex[2];
    maxX = vertex[0];
    maxY = vertex[1];
    maxZ = vertex[2];

    //After that, we look for the highest and lowest values in each axis
    for(int i = 0; i < nVertex * 3; i+=3)
    {
        if(vertex[i] > maxX)
            maxX=vertex[i];
        if(vertex[i] < minX)
            minX=vertex[i];
        if(vertex[i+1] > maxY)
            maxY = vertex[i+1];
        if(vertex[i+1] < minY)
            minY = vertex[i+1];
        if(vertex[i+2] > maxZ)
            maxZ = vertex[i+2];
        if(vertex[i+2] < minZ)
            minZ = vertex[i+2];
    }

    //std::cout << "maxX: " << maxX << " minX: " << minX << std::endl;
    //std::cout << "maxY: " << maxY << " minY: " << minY << std::endl;
    //std::cout << "maxZ: " << maxZ << " minZ: " << minZ << std::endl;

    glm::vec3 size = glm::vec3(maxX-minX, maxY-minY, maxZ-minZ);
    glm::vec3 center = glm::vec3((minX+maxX)/2, (minY+maxY)/2, (minZ+maxZ)/2);
    bbTransform = glm::translate(glm::mat4(1), center) *  glm::scale(glm::mat4(1), size);
    
    //1x1x1 box centered on origin
    GLfloat boxVertices[] = {
    -0.5, -0.5, -0.5, 1.0,
     0.5, -0.5, -0.5, 1.0,
     0.5,  0.5, -0.5, 1.0,
    -0.5,  0.5, -0.5, 1.0,
    -0.5, -0.5,  0.5, 1.0,
     0.5, -0.5,  0.5, 1.0,
     0.5,  0.5,  0.5, 1.0,
    -0.5,  0.5,  0.5, 1.0,
  };

  glGenBuffers(1, &boxVBOVertices);
  glBindBuffer(GL_ARRAY_BUFFER, boxVBOVertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GLushort boxIndices[] = {
    0, 1, 2, 3,
    4, 5, 6, 7,
    0, 4, 1, 5, 2, 6, 3, 7
  };

  glGenBuffers(1, &boxIBOIndices);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxIBOIndices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxIndices), boxIndices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void TResourceMesh::drawBoundingBox()
{
    glm::mat4 m = TEntity::modelMatrix() * bbTransform;

    glUniformMatrix4fv(TEntity::getModelID(), 1, GL_FALSE, &m[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, boxVBOVertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxIBOIndices);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4*sizeof(GLushort)));
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8*sizeof(GLushort)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUniformMatrix4fv(TEntity::getModelID(), 1, GL_FALSE, &TEntity::modelMatrix()[0][0]);

}

bool TResourceMesh::checkBoundingBox()
{
    GLfloat boxVertices[] = {
    -0.5, -0.5, -0.5, 1.0,
     0.5, -0.5, -0.5, 1.0,
     0.5,  0.5, -0.5, 1.0,
    -0.5,  0.5, -0.5, 1.0,
    -0.5, -0.5,  0.5, 1.0,
     0.5, -0.5,  0.5, 1.0,
     0.5,  0.5,  0.5, 1.0,
    -0.5,  0.5,  0.5, 1.0,
  };

    glm::mat4 m = TEntity::projectionMatrix() * TEntity::viewMatrix() * TEntity::modelMatrix() * bbTransform;
    glm::vec4 p1 = m * glm::vec4(-0.5, -0.5, -0.5, 1.0);
    glm::vec4 p2 = m * glm::vec4(0.5, -0.5, -0.5, 1.0);
    glm::vec4 p3 = m * glm::vec4(0.5, 0.5, -0.5, 1.0);
    glm::vec4 p4 = m * glm::vec4(-0.5, 0.5, -0.5, 1.0);
    glm::vec4 p5 = m * glm::vec4(-0.5, -0.5, 0.5, 1.0);
    glm::vec4 p6 = m * glm::vec4(0.5, -0.5, 0.5, 1.0);
    glm::vec4 p7 = m * glm::vec4(0.5, 0.5, 0.5, 1.0);
    glm::vec4 p8 = m * glm::vec4(-0.5, 0.5, 0.5, 1.0);


    float leftX = -15.f;
    float rightX = 15.f;
    float topY = 15.f;
    float bottomY = -15.f;
    float nearZ = -1.f;
    float farZ = 500.f;

    //std::cout << p1.x << " " << p1.y << " " << p1.z << std::endl;
    if(p1.x >= leftX && p1.x <= rightX && p1.y >= bottomY && p1.y <=topY && p1.z >= nearZ && p1.z <=farZ)
    {
        return true;
    }
    if(p2.x >= leftX && p2.x <= rightX && p2.y >= bottomY && p2.y <=topY && p2.z >= nearZ && p2.z <=farZ)
    {
        return true;
    }
    if(p3.x >= leftX && p3.x <= rightX && p3.y >= bottomY && p3.y <=topY && p3.z >= nearZ && p3.z <=farZ)
    {
        return true;
    }
    if(p4.x >= leftX && p4.x <= rightX && p4.y >= bottomY && p4.y <=topY && p4.z >= nearZ && p4.z <=farZ)
    {
        return true;
    }
    if(p5.x >= leftX && p5.x <= rightX && p5.y >= bottomY && p5.y <=topY && p5.z >= nearZ && p5.z <=farZ)
    {
        return true;
    }
    if(p6.x >= leftX && p6.x <= rightX && p6.y >= bottomY && p6.y <=topY && p6.z >= nearZ && p6.z <=farZ)
    {
        return true;
    }
    if(p7.x >= leftX && p7.x <= rightX && p7.y >= bottomY && p7.y <=topY && p7.z >= nearZ && p7.z <=farZ)
    {
        return true;
    }
    if(p8.x >= leftX && p8.x <= rightX && p8.y >= bottomY && p8.y <=topY && p8.z >= nearZ && p8.z <=farZ)
    {
        return true;
    }
    




    return false;
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