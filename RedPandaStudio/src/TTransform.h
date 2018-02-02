#pragma once
#include "TEntity.h"
#include <glm/glm.hpp>

class TTransform : public TEntity{

public:
    
    glm::mat4 matrix;

private:

    TTransform() {}
    ~TTransform() {}

    void identity();
    void setMatrix(glm::mat4 m)         {       matrix = m;     }
    void transpose() {}

    void traslate(float x, float y, float z) {}
    void rotate(float x, float y, float z, float w) {}

    void beginDraw() {}
    void endDraw() {}

};