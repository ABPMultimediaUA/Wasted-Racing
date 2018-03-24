#pragma once

#include "TResource.h"
#include <GL/glew.h>

class TResourceShader : public TResource {
    public:
<<<<<<< HEAD
        TResourceShader(){};
=======
        TResourceShader(bool type){
            vertexShader = type;
        };
>>>>>>> 9417c0d1639e6067b4e72d4c551e3d94986b82ce
        ~TResourceShader(){};

        //Load the resource specified in the route provided
        bool loadResource();

        //Draws the shader (in this case, activates it)
        void draw();

<<<<<<< HEAD

    private:
        GLuint shaderID;
=======
        //Getter
        GLuint getShaderID() { return shaderID; }


    private:
        GLuint shaderID;
        bool   vertexShader;
>>>>>>> 9417c0d1639e6067b4e72d4c551e3d94986b82ce

};