#pragma once

#include <irrlicht.h>
#include <glm/ext.hpp>
#include <iostream>

class WayPoint{
private:
        class Node{
        private:
                float radius;
                glm::vec3 position;
        public:                
                Node();
                ~Node();
                float getRadius();
                float setRadius(float rad);
                glm::vec3 getPos();
                glm::vec3 setPos(glm::vec3 pos);
        };
public:
        Node* startPoint;

        WayPoint* next;

        float distNextWay = 0;

        //Constructor
        WayPoint(glm::vec3 pos, float rad);

        //Destructor
        ~WayPoint();

        glm::vec3 getPos();

        bool checkNext(glm::vec3 pos, glm::vec3 nextPos);
        
        WayPoint* getNext();
        
        void setNext(WayPoint* n);

        float getDistNextWays();

};
