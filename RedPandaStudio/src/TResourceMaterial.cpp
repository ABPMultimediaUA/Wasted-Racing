#include "TResourceMaterial.h"
#include <iostream>

bool TResourceMaterial::loadResource()
{
    return false;
}

bool TResourceMaterial::loadResource(aiMaterial* m)
{
    aiColor3D v;

    kd = (float *)malloc(sizeof(float) * 3);
    ka = (float *)malloc(sizeof(float) * 3);
    ks = (float *)malloc(sizeof(float) * 3);

    m->Get(AI_MATKEY_COLOR_DIFFUSE, v);
    kd[0]=v.r;
    kd[1]=v.g;
    kd[2]=v.b;
    
    std::cout << "KD: " << kd[0] << " - " << kd[1] << " - " << kd[2] << std::endl;

    m->Get(AI_MATKEY_COLOR_AMBIENT, v);
    ka[0]=v.r;
    ka[1]=v.g;
    ka[2]=v.b;
    

    m->Get(AI_MATKEY_COLOR_SPECULAR, v);
    ks[0]=v.r;
    ks[1]=v.g;
    ks[2]=v.b;

    m->Get(AI_MATKEY_SHININESS, ns);
    
}

void TResourceMaterial::draw()
{
    //We could optimice this by doing this call in the constructor
    GLuint diffuseID = glGetUniformLocation(TEntity::getProgramID(), "material.kd");
    GLuint ambientID = glGetUniformLocation(TEntity::getProgramID(), "material.ka");
    GLuint specularID = glGetUniformLocation(TEntity::getProgramID(), "material.ks");
    GLuint shininessID = glGetUniformLocation(TEntity::getProgramID(), "material.ns");

    glUniform3fv(diffuseID, 1, kd);
    glUniform3fv(ambientID, 1, ka);
    glUniform3fv(specularID, 1, ks);
    glUniform1f(shininessID, ns);
}