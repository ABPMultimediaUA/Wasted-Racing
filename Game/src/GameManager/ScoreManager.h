#pragma once

#include "../GameObject/ScoreComponent.h"
#include <vector>
#include <memory>
#include <cstdint>

class ScoreManager {
    public:
        //Constructor
        ScoreManager();

        //Destructor
        ~ScoreManager();

        //Initializer
        void init();

        //Update : This method  updates the position of every player
        void update();

        //Close
        void close();

        //Getters
        std::vector<ScoreComponent::Pointer> getPlayers() {return players;}
        int getMaxLaps() {return maxLaps;}

        //Setters
        void setMaxLaps(int i) {maxLaps=i;}


    private:
        std::vector<ScoreComponent::Pointer> players;
        int maxLaps;
};