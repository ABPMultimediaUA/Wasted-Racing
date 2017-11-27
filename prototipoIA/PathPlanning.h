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

        int totalWayPoints = 0;
        
        double maxSpeed;

        double frameDeltaTime;

    public:
        PathPlanning();

        ~PathPlanning();

        void addWayPoint(WayPoint* way);

        void setMaxSpeed(double speed);

        void setFrame(double frame);

        glm::vec3 getNextPoint(glm::vec3 vel, glm::vec3 pos);

       
};