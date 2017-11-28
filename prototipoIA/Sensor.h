#pragma once

#include <irrlicht.h>
#include <iostream>
#include <vector>
#include <glm/ext.hpp>
#include "VObject.h"
class Sensor{
     public:
        glm::vec3 position;
        float angleVision;
        float angleInitial;
        float maxRadius;
        float a, b;

        //Constructor
        Sensor(glm::vec3 p, float ang, float maxR, float angInit);
        Sensor();
        
        //Destructor
        ~Sensor();

        //Detects collisions in field of vision
        bool detectFieldVision(glm::vec3 vel, glm::vec3 p);

        //Detects all the objects in the field of vision
        std::vector<VObject*> getAllFieldVisionObjects(irr::scene::ISceneNode** list, int size);

        //updates Sensor position
        void updatePosition(glm::vec3 pos);

        //updates facing angle of Sensor
        void updateAngle(float angle);

        irr::core::vector3df getSensorLeft();
        irr::core::vector3df getSensorRight();
 };
