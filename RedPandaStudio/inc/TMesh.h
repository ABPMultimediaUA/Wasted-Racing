#pragma once

#include <iostream>
#include "TEntity.h"
#include "TResourceMesh.h"


class TMesh : public TEntity{

private:
    TResourceMesh* mesh;

public:

    TMesh() {}
    ~TMesh() {}

    //Draw Methods
    void beginDraw();
    void endDraw() {}



    //////////////////////////////////////////////////////////////////////
    /////////////// GETTERS & SETTERS
    //////////////////////////////////////////////////////////////////////

    TResourceMesh* getMesh()                                 {   return mesh;    };
    void  setMesh(TResourceMesh* m)                          {   mesh=m;         };
};