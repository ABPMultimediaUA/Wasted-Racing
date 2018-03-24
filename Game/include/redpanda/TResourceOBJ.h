#pragma once

#include "TResource.h"
#include "TResourceMesh.h"
<<<<<<< HEAD
#include "TResourceMaterial.h"
=======
>>>>>>> 9417c0d1639e6067b4e72d4c551e3d94986b82ce
#include <GL/glew.h>
#include <GL/glut.h>
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


    private:
        std::vector<TResourceMesh*> meshes;

};