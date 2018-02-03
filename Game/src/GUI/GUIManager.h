#pragma once

#include "SFML/Graphics.hpp"
#include <vector>
#include "IGUI.h"

class GUIManager {

    public:

        //Constructor
        GUIManager() {}

        //Destructor
        ~GUIManager() {}

        //Initializer
        void init();

        //Closer
        void close();

        //Updater for all GUI
        void update();

        //Static class getter
        static GUIManager& getInstance(){
            static GUIManager instance;
            return instance;
        };

        //Add GUI
        void addGUI(IGUI *gui){
            guiList.push_back(gui);
        };

        //Draw one GUI
        void drawGUI(sf::RenderWindow* window, const std::string name);

        //Draw all GUIS
        void drawAllGUIs(sf::RenderWindow* window);


    private:

        std::vector<IGUI*> guiList;
};