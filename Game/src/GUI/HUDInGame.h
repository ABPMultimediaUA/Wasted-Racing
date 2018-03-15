#pragma once

#include "IGUI.h"
#include "../GameManager/ObjectManager.h"

class HUDInGame : public IGUI{

    public:

        HUDInGame();

        ~HUDInGame();

        //Initializer
        void init();

        //Updater
        void update(float dTime);

        //Closer
        void close();

        //Item update
        void updateItem();

        //Lap update
        void updateLap();

        //Position update
        void updatePos();

    private:

        //ITEMS
        GUISprite* empty;
        GUISprite* banana;
        GUISprite* star;
        GUISprite* blueshell;
        GUISprite* redshell;
        GUISprite* mushroom;

        //LAP
        GUISprite* laptxt;
        GUISprite* lapnum1;
        GUISprite* lapnum2;
        GUISprite* lapnum3;

        //POSITION
        GUISprite* postxt;
        GUISprite* posnum1;
        GUISprite* posnum2;
        GUISprite* posnum3;
        GUISprite* posnum4;
}