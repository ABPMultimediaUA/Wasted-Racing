#pragma once

#include <irrlicht.h>
#include <iostream>
#include <glm/ext.hpp>
#include "VObject.h"
class Sensor{
     public:
        glm::vec3 position;
        double angleVision;
        double angleInitial;
        float maxRadius;
        double a, b;

        //Constructor
        Sensor(glm::vec3 p, double ang, float maxR, double angInit);
        Sensor();
        
        //Destructor
        ~Sensor();

        //Detects collisions in field of vision
        bool detectFieldVision(glm::vec3 vel, glm::vec3 p);

        //Detects all the objects in the field of vision
        VObject** getAllFieldVisionObjects(irr::scene::ISceneNode** list, int size);

        //updates Sensor position
        void updatePosition(glm::vec3 pos);

        //updates facing angle of Sensor
        void updateAngle(double angle);

        irr::core::vector3df getSensorLeft();
        irr::core::vector3df getSensorRight();
 };
