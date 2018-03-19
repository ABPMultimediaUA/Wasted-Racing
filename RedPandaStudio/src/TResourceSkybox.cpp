#include "TResourceSkybox.h"
#include <iostream>

TResourceSkybox::TResourceSkybox()
{
    for(int i = 0; i < 6; i++)
    {
        sf::Image* img = new sf::Image();
        textures.push_back(img);
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

bool TResourceSkybox::loadResource(char* route, int i)
{
    if(i>= 0 && i<6)
    {
        if(textures[i]->loadFromFile(route))
        {
            std::cout << "Width: " << textures[i]->getSize().x << " Height: " << textures[i]->getSize().y << std::endl;
            //glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, textures[i]->getSize().x, textures[i]->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[i]->getPixelsPtr());
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)512, (GLsizei)512, 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[i]->getPixelsPtr());
            return true;
        }
    }
    return false;
}

void TResourceSkybox::draw()
{
    glUniformMatrix4fv(view, 1, GL_FALSE, &TEntity::viewMatrix()[0][0]);

    glDisable(GL_DEPTH_TEST);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

bool TResourceSkybox::initSkybox()
{
    glTexStorage2D(GL_TEXTURE_CUBE_MAP, 0, GL_RGBA8, textures[0]->getSize().x, textures[0]->getSize().y);

    for(int i = 0; i < 6; i++)
    {
        std::cout << "Width: " << textures[i]->getSize().x << " Height: " << textures[i]->getSize().y << std::endl;
        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, textures[i]->getSize().x, textures[i]->getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, textures[i]->getPixelsPtr());
    }

    GLint answer;
    glGetTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_RESIDENT, &answer);
    if(answer == 0)
    {
        std::cout << "YOYA" << std::endl;
    }
    else
    {
        std::cout << "WTFACK?? " << std::endl;
    }
}