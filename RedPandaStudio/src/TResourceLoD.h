#pragma once

#include "TResource.h"
#include "TResourceOBJ.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class TResourceLoD : public TResource {
    public:
        TResourceLoD(){};
        ~TResourceLoD(){};
        

        void insertObj(int lvl, TResourceOBJ* obj);

        //Load the resource specified in the route provided
        virtual bool loadResource() {};

        //Draws the shader (in this case, activates it)
        virtual void draw() {};
        //////////////////////////////////////////////////////////////////////
        /////////////////////////// GETTERS && SETTERS ///////////////////////
        //////////////////////////////////////////////////////////////////////
        
        //Return an obj by lvl of lod
        TResourceOBJ* getResourceObj(int lvl);

        std::map<int,TResourceOBJ*> getMap()                {       return objs;        }


    private:
    
        //Array of Objs
        std::map<int,TResourceOBJ*> objs;

};