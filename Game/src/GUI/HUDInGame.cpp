#pragma once

#include "HUDInGame.h"

void HUDInGame::addSprite(glm::vec2 pos, glm::vec2 siz, bool vis, bool act, const std::string& file, int id){

    GUISprite* sprite = new GUISprite(pos, siz, vis, act, file, id);
    addGUIElement(sprite);

}

void HUDInGame::addText(glm::vec2 pos, glm::vec2 siz, bool vis, bool act, const std::string& txt, const std::string& color, int id){

    GUIText* text = new GUIText(glm::vec2 pos, glm::vec2 siz, bool vis, bool act, const std::string& txt, const std::string& color, int id);
    addGUIElement(text);

}

void HUDInGame::addButton(glm::vec2 pos, glm::vec2 siz, bool vis, bool act, const std::string& txt, const std::string& color, int id){

    GUIText* button = new GUIText(glm::vec2 pos, glm::vec2 siz, bool vis, bool act, const std::string& txt, const std::string& color, int id);
    addGUIElement(button);

}


void HUDInGame::init(){

    //CHANGE THE POSITION RELATED TO THE WINDOW SIZE
    empty = addSprite(glm::vec2(50, 50), glm::vec2(25, 25), true, false, "media/img/emptyBox.png", 500);
    banana = addSprite(glm::vec2(50, 50), glm::vec2(25, 25), false, false, "media/img/banana.png", 500);
    star = addSprite(glm::vec2(50, 50), glm::vec2(25, 25), false, false, "media/img/star.png", 500);
    blueshell = addSprite(glm::vec2(50, 50), glm::vec2(25, 25), false, false, "media/img/blueshell.png", 500);
    redshell = addSprite(glm::vec2(50, 50), glm::vec2(25, 25), false, false, "media/img/redshell.png", 500);
    mushroom = addSprite(glm::vec2(50, 50), glm::vec2(25, 25), false, false, "media/img/mushroom.png", 500);


    laptxt = addSprite(glm::vec2(50, 250), glm::vec2(50, 25), true, false, "media/img/lap.png", 500);
    lapnum1 = addSprite(glm::vec2(50, 250), glm::vec2(25, 25), true, false, "media/img/lap1.png", 500);//the sprite will be 1/3, 2/3, 3/3 etc
    lapnum1 = addSprite(glm::vec2(50, 250), glm::vec2(25, 25), true, false, "media/img/lap1.png", 500);
    lapnum1 = addSprite(glm::vec2(50, 250), glm::vec2(25, 25), true, false, "media/img/lap1.png", 500);

    postxt = addSprite(glm::vec2(250, 50), glm::vec2(55, 25), true, false, "media/img/position.png", 500);
    posnum1 = addSprite(glm::vec2(250, 50), glm::vec2(25, 25), true, false, "media/img/1.png", 500); 
    posnum2 = addSprite(glm::vec2(250, 50), glm::vec2(25, 25), true, false, "media/img/2.png", 500); 
    posnum3 = addSprite(glm::vec2(250, 50), glm::vec2(25, 25), true, false, "media/img/3.png", 500); 
    posnum4 = addSprite(glm::vec2(250, 50), glm::vec2(25, 25), true, false, "media/img/4.png", 500); 

}

GUIElement* HUDInGame::getElement(int id){
    
    for(int i = 0; i<guiElements.size(); i++){
        if(guiElements.[i] == id){
            return guiElements[i];
        }
    }
}

void HUDInGame::removeGUIElement(int id){

    for(int i = 0; i<guiElements.size(); i++){
        if(guiElements.[i] == id){
            delete guiElements[i];
            guiElements.erase(guiElements.begin()+i);
            return;
        }
    }
}

void HUDInGame::updateItem(){

    int itemID = ObjectManager::getInstance().getObject(50).get()->getComponent<ItemHolderComponent>().get()->getItemType();
 
    switch(itemID){

        case -1: //EMPTY
                empty->setVisible(true);
                redshell->setVisible(false);
                blueshell->setVisible(false);
                banana->setVisible(false);
                mushroom->setVisible(false);
                star->setVisible(false);
                 break;

        case 0: //RED SHELL
                empty->setVisible(false);
                redshell->setVisible(true);
                blueshell->setVisible(false);
                banana->setVisible(false);
                mushroom->setVisible(false);
                star->setVisible(false);
                 break;

        case 1: //BLUE SHELL
                empty->setVisible(false);
                redshell->setVisible(false);
                blueshell->setVisible(true);
                banana->setVisible(false);
                mushroom->setVisible(false);
                star->setVisible(false);
                 break;

        case 2: //BANANA
                empty->setVisible(false);
                redshell->setVisible(false);
                blueshell->setVisible(false);
                banana->setVisible(true);
                mushroom->setVisible(false);
                star->setVisible(false);
                 break;

        case 3: //MUSHROOM
                empty->setVisible(false);
                redshell->setVisible(false);
                blueshell->setVisible(false);
                banana->setVisible(false);
                mushroom->setVisible(true);
                star->setVisible(false);
                 break;

        case 4: //STAR
                empty->setVisible(false);
                redshell->setVisible(false);
                blueshell->setVisible(false);
                banana->setVisible(false);
                mushroom->setVisible(false);
                star->setVisible(true);
                 break;
    }


}

void HUDInGame::updateLap(){

    int lap = ObjectManager::getInstance().getObject(50).get()->getComponent<ScoreComponent>().get()->getLap();

    switch(lap){
        case 1: 
            lap1->setVisible(true);
            lap2->setVisible(false);
            lap3->setVisible(false);
            break;
        case 2:
            lap1->setVisible(false);
            lap2->setVisible(true);
            lap3->setVisible(false);
            break;
        case 3:
            lap1->setVisible(false);
            lap2->setVisible(false);
            lap3->setVisible(true);
            break;
    }

}

void HUDInGame::updatePos(){

    int position = ObjectManager::getInstance().getObject(50).get()->getComponent<ScoreComponent>().get()->getPosition();
  
    switch(position){
        case 1:
            posnum1->setVisible(true);
            posnum2->setVisible(false);
            posnum3->setVisible(false);
            posnum4->setVisible(false);
            break;
        case 2:
            posnum1->setVisible(false);
            posnum2->setVisible(true);
            posnum3->setVisible(false);
            posnum4->setVisible(false);
            break;
        case 3:
            posnum1->setVisible(false);
            posnum2->setVisible(false);
            posnum3->setVisible(true);
            posnum4->setVisible(false);
            break;
        case 4:
            posnum1->setVisible(false);
            posnum2->setVisible(false);
            posnum3->setVisible(false);
            posnum4->setVisible(true);
            break;
    }
}