#pragma once
#include "TEntity.h"
#include <iostream>

class TLight : public TEntity{

private:
    float intensity = 1.0f;

public:

    TLight() {}
    ~TLight() {}

    //Draw Methods
    void beginDraw();
    void endDraw() {}

};