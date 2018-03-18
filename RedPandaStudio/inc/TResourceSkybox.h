#pragma once

#include "TEntity.h"
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <vector>

//////////////////////////////////////////POSITION CORRESPONDENCY/////////////////////////////////////////
//This class loads a set of six pictures and draws the skybox. The order of the pictures is the following:
// 0 -> Top
// 1 -> Bottom
// 2 -> Left
// 3 -> Right
// 4 -> Front
// 5 -> Back

//This could be improved with an enumerator

class TResourceSkybox {
    public:
        TResourceSkybox();
        ~TResourceSkybox() {};

        //Loads the texture given in the route and stores it in the position specified (check position correspondency for more info)
        bool loadResource(char* route, int i);

        //Draws the skybox
        void draw();

        /////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////// GETTERS && SETTERS ////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////
        void setView(GLuint v)              { view=v;   }

    private:
        //Vector of the 6 textures that compose the skybox, in the order showed in position correspondency
        std::vector<sf::Image*> textures;

        //Identifier of the texture of the skybox
        GLuint texture;

        //Identifier of the viewMatrix of the shader
        GLuint view;

};