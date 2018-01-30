#pragma once

#include "GUISprite.h"


void GUISprite::GUISprite(glm::vec2 pos, glm::vec2 siz, bool vis, bool act, const std::string& file, int id){

    this->id = id;
    visible = vis;
    activate = act;
    this->pos = pos;
    this->siz = size;
    this->setTexture(file);
    sprite.setPosition(pos.x, pos.y);

}

void GUISprite::setPosition(float x, float y){

    pos = glm::vec2(x, y);
    sprite.setPosition(x, y);

}

void GUISprite::setTexture(std::string& fileName){
    
    texture = fileName;
    sf::Texture txt;
    txt.loadFromFile(texture);
    sprite.setTexture(txt);
    siz = glm::vec2(sprite.getLocalBounds().width, sprite.getLocalBounds().height);
    sprite.setTextureRect(sf::IntRect(0,0,siz.x, siz.y));

}

void GUISprite::draw(sf::RenderWindow* window){

    if(visible){
        window->draw(sprite);
    }

}

void GUISprite::setScale(float w, float h){

    glm::vec2 sca(w/siz.x, h/siz.y);
    sprite.setScale(sca.x, sca.y);
    
}
