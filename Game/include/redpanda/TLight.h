#pragma once
#include "TEntity.h"
#include <iostream>

class TLight : public TEntity{

private:
    glm::vec4 intensity = glm::vec4(1.0,1.0,1.0,1.0);

public:

    TLight() {}
    TLight(glm::vec3 in) {
        intensity = glm::vec4(in, 1.0);
    }
    ~TLight() {}

    //Getter
    glm::vec4 getIntensity() {  return intensity;   }

    //Draw Methods
    void beginDraw();
    void endDraw();

};