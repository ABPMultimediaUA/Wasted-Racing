#pragma once

#include "TResource.h"
#include "TResourceOBJ.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class TMeshLoD : public TResource {
    public:
        TMeshLoD(){};
        ~TMeshLoD(){};
        

        

        //////////////////////////////////////////////////////////////////////
        /////////////////////////// GETTERS && SETTERS ///////////////////////
        //////////////////////////////////////////////////////////////////////
        

    private:
    
        //Array of Objs
        std::vector<TResourceOBJ*> objs;

};