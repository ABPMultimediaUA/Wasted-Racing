#pragma once
#include "TEntity.h"
#include <glm/glm.hpp>

class TTransform : public TEntity{

private:
    
    glm::mat4 matrix;

public:

    TTransform() {}
    ~TTransform() {}

    void identity();
    void transpose() {}

    void traslate(float x, float y, float z) {}
    void rotate(float x, float y, float z, float w) {}

    //Setters
    void setMatrix(glm::mat4 m)         {       matrix = m;         }

    //Getters 
    glm::mat4 getMatrix()               {       return matrix;      }

    //Draw Methods
    void beginDraw() {}
    void endDraw() {}

};