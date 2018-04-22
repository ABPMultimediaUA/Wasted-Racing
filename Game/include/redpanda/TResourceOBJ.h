#pragma once

#include "TResource.h"
#include "TResourceMesh.h"
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

        //Only loads the meshes in the OBJ provided
        bool loadOnlyMeshes();

        //Sets the provided texture in the mesh placed in position i, if possible
        void setTexture(int i, TResourceTexture* t);

        //Sets the provided material in the mesh placed in position i, if possible
        void setMaterial(int i, TResourceMaterial* m);

        //Load the resource specified in the route provided
        bool loadResource();

        //Draws the mesh
        void draw();


    private:
        std::vector<TResourceMesh*> meshes;

        //Auxiliar function to split strings
        std::vector<std::string> split(const std::string& s, const char& c);
};