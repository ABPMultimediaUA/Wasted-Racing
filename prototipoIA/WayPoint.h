#include <irrlicht.h>
#include <glm/ext.hpp>

class WayPoint{
private:
        class Node{
        private:
                double radius;
                glm::vec3 position;

                Node* next;
        public:                
                Node();
                ~Node();
                Node getNext();
                Node setNext();
                double getRadius();
                irr::core::vector3df getPos();
                bool inside(irr::core::vector3df pos);
        };
public:
        Node* startPoint;

        //Constructor
        WayPoint(glm::vec3);

        //Destructor
        ~WayPoint();


};
