#include "TResourceOBJ.h"
#include <string>
#include <iostream>

std::vector<std::string> TResourceOBJ::split(const std::string& s, const char& c) {
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

bool TResourceOBJ::loadOnlyMeshes()
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(name, aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);

    if(scene)
    {
        for(unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            TResourceMesh* mesh = new TResourceMesh();
            aiMesh* m = scene->mMeshes[i];
            mesh->loadMesh(m);
            meshes.push_back(mesh);
        }

        generateBoundingBox();
        
        return true;
    }
    return false;
}

void TResourceOBJ::setTexture(int i, TResourceTexture* t)
{
    if(i>=0 && i<meshes.size())
    {
        meshes[i]->setTextActive(true);
        meshes[i]->setTexture(t);
    }
}


void TResourceOBJ::setMaterial(int i, TResourceMaterial* m)
{
    if(i >= 0 && i < meshes.size())
    {
        meshes[i]->setMaterial(m);
    }
}

bool TResourceOBJ::loadResource()
{
    Assimp::Importer importer;
    //First we attempt to load the obj
    const aiScene* scene = importer.ReadFile(name, aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);


    //If loaded succesfully, we proceed to get all his data
    if(scene)
    {
        //For each mesh in the obj, we create a new resourceMesh and storage the mesh data on it
        for(unsigned int i = 0; i<scene->mNumMeshes; i++)
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

        if(v[0].compare("home") == 0)
            route = "/";

        for(unsigned int i = 0; i < v.size()-1; i++)
        {
            route+=v[i] + "/";
        }

        //We proceed to get all the materials and textures
        for(unsigned int i = 1; i<scene->mNumMaterials; i++)
        {
            TResourceMaterial* mat = new TResourceMaterial();
            mat->loadResource(scene->mMaterials[i]);
            meshes[i-1]->setMaterial(mat);
            aiString path;
            //If the material has a diffuse texture, we get his path
            if(scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
            {
                TResourceTexture* texture = new TResourceTexture();
                
                //First we combine the path we just got with the directory path of the obj, and then we just load the texture
                std::string completePath = route + path.data;
                texture->setName(completePath.c_str());
                texture->loadResource();
                meshes[i-1]->setTexture(texture);
                meshes[i-1]->setTextActive(true);
            }
        }

        generateBoundingBox();

        return true;
    }
    return false;
}

void TResourceOBJ::draw()
{
    if((bbActivated && checkBoundingBox()) || !bbActivated)
    {
        //The textures, materials and meshes are loaded, suposedly, in a way that they should just correspond, so we draw one of each
        for(unsigned int i = 0; i < meshes.size(); i++)
        {
            meshes[i]->draw();
        }

        drawBoundingBox();
    }
}

void TResourceOBJ::generateBoundingBox()
{

    //First we initialize the variables of the bounding box
    maxX = meshes[0]->getMaxX();
    minX = meshes[0]->getMinX();
    maxY = meshes[0]->getMaxY();
    minY = meshes[0]->getMinY();
    maxZ = meshes[0]->getMaxZ();
    minZ = meshes[0]->getMinZ();

    for(int i = 1; i < meshes.size(); i++)
    {
        if(meshes[i]->getMaxX() > maxX)
            maxX = meshes[i]->getMaxX();
        if(meshes[i]->getMinX() < minX)
            minX = meshes[i]->getMinX();
        if(meshes[i]->getMaxY() > maxY)
            maxY = meshes[i]->getMaxY();
        if(meshes[i]->getMinY() < minY)
            minY = meshes[i]->getMinY();
        if(meshes[i]->getMaxZ() > maxZ)
            maxZ = meshes[i]->getMaxZ();
        if(meshes[i]->getMinZ() < minZ)
            minZ = meshes[i]->getMinZ();
    }

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

void TResourceOBJ::drawBoundingBox()
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

bool TResourceOBJ::checkBoundingBox()
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


    float leftX = -11.f;
    float rightX = 11.f;
    float topY = 11.f;
    float bottomY = -11.f;
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