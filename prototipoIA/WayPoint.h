#pragma once

#include <irrlicht.h>
#include <glm/ext.hpp>

class WayPoint{
private:
        class Node{
        private:
                double radius;
                glm::vec3 position;
        public:                
                Node();
                ~Node();
                double getRadius();
                double setRadius(double rad);
                glm::vec3 getPos();
                glm::vec3 setPos(glm::vec3 pos);
        };
public:
        Node* startPoint;

        WayPoint* next;

        //Constructor
        WayPoint(glm::vec3 pos, double rad);

        //Destructor
        ~WayPoint();

        glm::vec3 getPos();

        bool inside(glm::vec3 pos);
        
        WayPoint* getNext();
        
        void setNext(WayPoint* n);


};
