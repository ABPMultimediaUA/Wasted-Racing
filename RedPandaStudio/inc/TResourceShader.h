#pragma once

#include "TResource.h"
#include <GL/glew.h>

class TResourceShader : public TResource {
    public:
        TResourceShader(){};
        ~TResourceShader(){};

        //Load the resource specified in the route provided
        bool loadResource();

        //Draws the shader (in this case, activates it)
        void draw();


    private:
        GLuint shaderID;

};