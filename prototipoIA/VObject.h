#include <irrlicht.h>
#include <glm/ext.hpp>

class VObject{
    private:
        glm::vec3 position;
        double a, b, radius;
        int type;

    public:

        //Constructors
        VObject();
        VObject(glm::vec3 p, double x, double y, double r, int t);

        //Destructor
        ~VObject();

        //Getters
        glm::vec3 getPosition();
        double getA();
        double getB();
        double getRadius();
        int getType();

        //Setters
        void setPosition(glm::vec3 p);
        void setA(double x);
        void setB(double y);
        void setRadius(double r);
        void setType(int t);
};