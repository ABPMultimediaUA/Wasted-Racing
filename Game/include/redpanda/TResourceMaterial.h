#pragma once

#include "TResource.h"
<<<<<<< HEAD
=======
#include "TEntity.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <assimp/scene.h>
>>>>>>> 9417c0d1639e6067b4e72d4c551e3d94986b82ce

class TResourceMaterial : public TResource {
    public:
        TResourceMaterial() {};
        ~TResourceMaterial() {};

<<<<<<< HEAD
=======
        //Load the material from an already loaded material provided in assimp format
        bool loadResource(aiMaterial* m);

>>>>>>> 9417c0d1639e6067b4e72d4c551e3d94986b82ce
        //Load the resource specified in the route provided
        bool loadResource();

        //Draws the material
        void draw();

    private:
<<<<<<< HEAD
=======
        //Diffuse, ambient and specular component of the material
        GLfloat* kd, *ka, *ks;

        //Glossiness of the material (specular exponent)
        GLfloat ns=0.0f;
>>>>>>> 9417c0d1639e6067b4e72d4c551e3d94986b82ce

};