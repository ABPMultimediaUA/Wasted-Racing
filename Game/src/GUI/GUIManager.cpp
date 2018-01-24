#pragma once

#include "GUIManager.h"

void GUIManager::drawGUI(sf::RenderWindow* window, const std::string name){

    for(int i = 0; i < GUIManager::guiList.size; i++){
        if(GUIManager::guiList[i].getName() == name){
            GUIManager::guiList[i].draw(window);
        }
    }
}

void GUIManager::drawAllGUIs(sf::RenderWindow* window){

     for(int i = 0; i < GUIManager::guiList.size; i++){
            GUIManager::guiList[i].draw(window);
    }
}

void GUIManager::update(){

  for(int i = 0; i < GUIManager::guiList.size; i++){
            GUIManager::guiList[i].update();
    }
}
