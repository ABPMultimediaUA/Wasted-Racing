#include "TTransform.h"

bool TTransform::identity()
{
    matrix = glm::mat4(1.0);
    return true;
}

bool TTransform::transpose()
{
    matrix = glm::transpose(matrix);
    return true;
}

bool TTransform::translate(float x, float y, float z)
{
    matrix = glm::translate(matrix, glm::vec3(x,y,z));
    return true;
} 

bool TTransform::rotate(float x, float y, float z, float w)
{
    glm::mat4 rotateY = glm::rotate(matrix, w, glm::vec3(0.0f, y, 0.0f));
    glm::mat4 rotateX = glm::rotate(rotateY, w, glm::vec3(x, 0.0f, 0.0f));
    glm::mat4 rotateZ = glm::rotate(rotateX, w, glm::vec3(0.0f, 0.0f, z));
    matrix = rotateZ;
    return true;
} 

bool TTransform::inverse()
{
    matrix = glm::inverse(matrix);
    return true;
}

glm::vec4 TTransform::multiplyVector(glm::vec4 v)
{
    return (matrix * v);
} 

glm::mat4 TTransform::multiplyMatrix(glm::mat4 m)
{
    return (matrix * m);
}