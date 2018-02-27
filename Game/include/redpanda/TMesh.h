#pragma once

#include <iostream>
#include "TEntity.h"
#include "TResource.h"


class TMesh : public TEntity{

private:
    TResource* mesh;

public:

    TMesh() {}
    ~TMesh() {}

    //Draw Methods
    void beginDraw();
    void endDraw() {}



    //////////////////////////////////////////////////////////////////////
    /////////////// GETTERS & SETTERS
    //////////////////////////////////////////////////////////////////////

    TResource* getMesh()                                 {   return mesh;    };
    void  setMesh(TResource* m)                          {   mesh=m;         };
};