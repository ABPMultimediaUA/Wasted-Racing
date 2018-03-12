#pragma once

#include "TEntity.h"
#include "TResource.h"
#include <GL/glew.h>


class TMesh : public TEntity{

private:
    TResource* mesh;
    TResource* material;

public:

    TMesh() {}
    ~TMesh() {}

    //Draw Methods
    void beginDraw();
    void endDraw() {}



    //////////////////////////////////////////////////////////////////////
    /////////////// GETTERS & SETTERS
    //////////////////////////////////////////////////////////////////////

    TResource* getMesh()                                      {   return mesh;      };
    TResource* getMaterial()                                  {   return material;  };

    void  setMesh(TResource* m)                               {   mesh=m;           };
    void  setMaterial(TResource* m)                           {   material=m;       };
};