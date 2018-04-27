#pragma once

#include "TResource.h"
#include "TResourceMesh.h"
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

class TResourceOBJ : public TResource {
    public:
        TResourceOBJ() {};
        ~TResourceOBJ() {};

        //Load the resource specified in the route provided
        bool loadResource();

        //Draws the mesh
        void draw();

        std::vector<TResourceMesh*> getMeshes() {       return meshes;      }
        

    private:
        std::vector<TResourceMesh*> meshes;

};
