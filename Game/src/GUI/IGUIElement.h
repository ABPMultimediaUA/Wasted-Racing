#pragma once

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include "../GameObject.h"

class IGUIComponent {

    public:

        //==============================================================
        // Class Related functions
        //==============================================================

        enum ElementType {

            sprite,
            text,
            button
            
        };

        IGUIComponent();

        virtual ~IGUIComponent(){};

        //Initializer
        virtual void init() = 0;

        //Updater
        virtual void update(float dTime) = 0;

        //Closer
        virtual void close() = 0;

        //ID Setter
        virtual void setId(int id){
            this->id = id;
        };

        //ID Getter
        virtual int getId(){
            return id;
        };

        //Position Setter 
        virtual void setPos(float x, float y) = 0;

        //Position Getter
        virtual glm::vec2 getPos(){
            return pos;
        };

        //Size Setter
        virtual void setSize(float w, float h) = 0;

        //Size Getter
        virtual glm::vec2 getSize(){
            return siz;
        };

        //Visible setter
        virtual void setVisible(bool vis){
            visible = vis;
        };

        //Visible getter
        virtual bool getVisible(){
            return visible;
        };

        //Activation setter (for buttons and things like that)
        virtual void setActivated(bool act){
            activated = act;
        };

        //Activation getter
        virtual bool getActivated(){
            return activated;
        };

        //For drawing only 1 element
        virtual void draw(sf::RenderWindow* window) = 0;

    private:

        //ID of the element
        int id;

        //Position and size of the element
        glm::vec2 pos;
        glm::vec2 siz;

        //State bools
        bool visible;
        bool activated;

};