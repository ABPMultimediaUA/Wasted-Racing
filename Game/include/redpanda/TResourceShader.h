#pragma once

#include "TResource.h"
#include <GL/glew.h>

class TResourceShader : public TResource {
    public:
        TResourceShader(bool type){
            vertexShader = type;
        };
        ~TResourceShader(){};

        //Load the resource specified in the route provided
        bool loadResource();

        //Draws the shader (in this case, activates it)
        void draw();

        //Getter
        GLuint getShaderID() { return shaderID; }


    private:
        GLuint shaderID;
        bool   vertexShader;

};