#include <irrlicht.h>
#include <glm/glm.hpp>


class WayPoint{
private:
        class Node{
        private:
                double radius;
                irr::core::vector3df pos;

                Node next;
        public:
                double radius;
                Node next;
                
                Node();
                ~Node();
                Node getNext();
                Node setNext();
                double getRadius();
                irr::core::vector3df getPos();
                bool inside(irr::core::vector3df pos);
        }

public:
        Node startPoint;

        //Constructor
        WayPoint();

        //Destructor
        ~WayPoint();


};
