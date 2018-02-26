#pragma once

#include "TEntity.h"
#include "TResource.h"
#include <GL/glew.h>


class TMesh : public TEntity{

private:
    TResource* mesh;
    TResource* material;
    TResource* texture;

    bool textActive;

public:

    TMesh() { textActive = false; }
    ~TMesh() {}

    //Draw Methods
    void beginDraw();
    void endDraw() {}



    //////////////////////////////////////////////////////////////////////
    /////////////// GETTERS & SETTERS
    //////////////////////////////////////////////////////////////////////

    TResource* getMesh()                                      {   return mesh;      };
    TResource* getMaterial()                                  {   return material;  };
    TResource* getTexture()                                   {   return texture;   };
    bool getTextActive()                                      {   return textActive;};

    void  setMesh(TResource* m)                               {   mesh=m;           };
    void  setMaterial(TResource* m)                           {   material=m;       };
    void  setTexture(TResource* m)                            {   texture=m;        };
    void  setTextActive(bool b)                               {   textActive=b;     };
};