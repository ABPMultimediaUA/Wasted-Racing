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
        WayPoint* nextWayPoint;
        WayPoint* lastWayPoint;
                
        float maxSpeed;

        float frameDeltaTime;

        float seconds;

        float tour;

        float distTourWay = 0;

    public:
        PathPlanning();

        ~PathPlanning();

        void addWayPoint(WayPoint* way);

        void setMaxSpeed(float speed);

        void setFrame(float frame);

        void setSeconds(float sec);

        glm::vec3 getNextPoint(glm::vec3 vel, glm::vec3 pos, float modSpeed);
       
};