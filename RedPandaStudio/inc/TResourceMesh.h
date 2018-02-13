#pragma once

#include "TResource.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class TResourceMesh : public TResource {
    public:
        TResourceMesh() {};
        ~TResourceMesh() {};

        //Load the resource specified in the route provided
        bool loadResource();

        //Draws the mesh
        void draw();

    private:
        //GLint vs GLfloat
        GLint* vertex, normals, textures;
        //What's the difference??
        GLfloat* triangleVertex, triangleNormals, triangleText;
        long nTriangles;


};