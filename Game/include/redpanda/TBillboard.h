#pragma once

#include "TEntity.h"
#include "TResourceTexture.h"
#include <iostream>

class TBillboard : public TEntity {
    public:
        TBillboard() {};
        TBillboard(TResourceTexture* t, glm::vec3 p) 
        {
            texture = t;
            position = p;
            glGenBuffers(1, &vertexBuffer);
        }
        ~TBillboard() {};

        //Draws the billboard
        void beginDraw();

        void endDraw() {};


    //////////////////////////////////////////////////////////////////////
    ////////////////////// GETTERS & SETTERS /////////////////////////////
    //////////////////////////////////////////////////////////////////////
        TResourceTexture* getTexture()                      { return texture;       };
        glm::vec3 getPosition()                             { return position;      };
        void setTexture(TResourceTexture* text)             { texture = text;       };
        void setPosition(glm::vec3 p)                       { position = p;         };
    private:
        //Texture to be drawn in the billboard
        TResourceTexture*  texture = nullptr;
        //Position in world coordinates of the billboard
        glm::vec3 position;

        //OpenGL identifier of the vertex buffer (vec3 position of the billboard low edge center in world coordinates)
        GLuint vertexBuffer;
};