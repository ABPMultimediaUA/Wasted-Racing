#pragma once

#include "TResource.h"
#include <GL/glew.h>
#include <SFML/Graphics.hpp>

class TResourceTexture : public TResource {
    public:
        TResourceTexture() {};
        ~TResourceTexture();

        //Load the resource specified in the route provided (stored in the father class)
        bool loadResource();

        //Draws the texture
        void draw();
        
        //Deactivates the current texture
        void endDraw();


    private:
        //SFML Class to store our texture
        sf::Image texture;
        //Width and height of the texture
        int sizeX, sizeY;
        //OpenGL ID of the texture
        GLuint textureID;

};