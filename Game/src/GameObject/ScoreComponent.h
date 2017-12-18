#pragma once

#include "IComponent.h"

class ScoreComponent : public IComponent{
    public:
        //Constructor
        ScoreComponent(GameObject& newGameObject);

        //Destructor
        ~ScoreComponent() {}

        //Initilizer
        virtual void init() {};

        //Update
        virtual void update(float dTime) {};

        //Closer
        virtual void close() {};

        //Returns a score based on lap and waypoint on the lap, in a way that being on the first waypoint of a higher lap
        //has more score than being in the highest waypoint of a previous lap
        //Maximum number of waypoints == 300 -> If need, change it in the function
        int getScore();

        //Getters
        int getLap() {return lap;}
        int getPosition() {return position;}

        //Setters
        void setLap(int i) {lap=i;}
        void setPosition(int i) {position=i;}
    private:
        int lap;
        int position;
};