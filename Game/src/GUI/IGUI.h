#pragma once

#include <SFML/Graphics.hpp>
#include "IGUIElement.h"
#include "GUISprite.h"
#include "GUIText.h"
#include "GUIButton.h"
#include <glm/glm.hpp>

class IGUI {

    public:

        //Constructor
        IGUI() {}

        //Destructor
        virtual ~IGUI() {}

        //Initializer
        virtual void init() = 0;

        //Closer
        virtual void close() = 0;

        //Updater for all GUI Elements
        virtual void update() = 0;

        //Add Sprite
        virtual void addSprite(glm::vec2 pos, glm::vec2 siz, bool vis, bool act, const std::string& file, int id) = 0;

        //Add Text
        virtual void addText(glm::vec2 pos, glm::vec2 siz, bool vis, bool act, const std::string& txt, const std::string& color, int id) = 0;

        //Add Button
        virtual void addButton(glm::vec2 pos, glm::vec2 siz, bool vis, bool act, const std::string& txt, const std::string& color, int id) = 0;

        //Checkbox ????

        //Add element
        virtual void addGUIElement(IGUIElement* elm){
            guiElements.push_back(elm);
        };

        //Clears all elements from the GUI
        virtual void clearGUI() = 0;

        //Clears one element from GUI
        virtual void removeGUIElement(int id) = 0;

        //Element getter
        virtual IGUIElement* getElement(int id) = 0;

        //Name Getter
        virtual std::string getName(){
            return name;
        };

        //Draw all elements
        virtual void draw(sf::RenderWindow* window) = 0;

    private:

        //Vector of all the GUI Elements
        std::vector<IGUIElement*> guiElements;

        //GUI Name
        std::string name;

};