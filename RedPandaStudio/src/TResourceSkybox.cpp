#include "TResourceSkybox.h"

TResourceSkybox::TResourceSkybox()
{
    for(int i = 0; i < 6; i++)
    {
        sf::Image* img = new sf::Image();
        textures.push_back(img);
    }
    glGenTextures(1, &texture);
}

bool TResourceSkybox::loadResource(char* route, int i)
{
    if(i>= 0 && i<6)
    {
        if(textures[i]->loadFromFile(route))
        {
            return true;
        }
    }
    return false;
}

void TResourceSkybox::draw()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    for(int i = 0; i<6; i++)
    {
        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, textures[i]->getSize().x, textures[i]->getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, textures[i]->getPixelsPtr());
    }
}