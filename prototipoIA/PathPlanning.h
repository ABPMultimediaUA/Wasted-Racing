#pragma once

#include <irrlicht.h>
#include <glm/ext.hpp>
#include "WayPoint.h"
#include <list> 
#include <iostream>

class PathPlanning{
    private:
       
        std::list<WayPoint*> *listWay;

        WayPoint* startPoint;
        WayPoint* actualWayPoint;

        int totalWayPoints = 0;
        
        float maxSpeed;

        float frameDeltaTime;

        float seconds;

    public:
        PathPlanning();

        ~PathPlanning();

        void addWayPoint(WayPoint* way);

        void setMaxSpeed(float speed);

        void setFrame(float frame);

        void setSeconds(float sec);

        glm::vec3 getNextPoint(glm::vec3 vel, glm::vec3 pos, float modSpeed);

       
};