#pragma once

#include <IGUIElement.h>

class GUISprite : public IGUIElement{

    public:

        GUISprite(glm::vec2 pos, glm::vec2 siz, bool vis, bool act, const std::string& file, int id);

        ~GUISprite();


        //Initializer
        void init();

        //Updater
        void update(float dTime);

        //Closer
        void close();

        //For drawing only 1 element
        virtual void draw(sf::RenderWindow* window) = 0;

        //Scale setter
        void setScale(float w, float h);

        //Texture setter
        void setTexture(const std::string& file);

        //Set sprite position
        void setPosition(float x, float y);

    
    private:

        sf::Sprite sprite;
        //Image filename
        sf::Texture texture;

}