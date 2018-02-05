#pragma once
#include "TEntity.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TTransform : public TEntity{

private:
    
    glm::mat4 matrix;

public:

    TTransform() {}
    ~TTransform() {}

    bool identity();
    bool transpose();

    bool translate(float x, float y, float z);
    bool rotate(float x, float y, float z, float w);

    bool inverse();

    glm::vec4 multiplyVector(glm::vec4 v);
    glm::mat4 multiplyMatrix(glm::mat4 m);

    //Setters
    void setMatrix(glm::mat4 m)         {       matrix = m;         }

    //Getters 
    glm::mat4 getMatrix()               {       return matrix;      }

    //Draw Methods
    void beginDraw() {}
    void endDraw() {}

};