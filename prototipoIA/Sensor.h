#include <irrlicht.h>
#include <glm/glm.hpp>
class Sensor{
     public:
        irr::core::vector3df position;
        double angleVision = 0;
        double angInitial = 0;
        float maxRadius = 0;
        double a = 0, b = 0;

        //Constructor
        Sensor(irr::core::vector3df p, double ang, float maxR, double angInit);
        
        //Destructor
        ~Sensor();

        //Detects collisions in field of vision
        bool detectFieldVision(irr::core::vector3df vel, irr::core::vector3df p);

        //updates Sensor position
        void updatePosition(irr::core::vector3df pos);

        //updates facing angle of Sensor
        void updateAngle(double angle);
 };