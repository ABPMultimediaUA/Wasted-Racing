#pragma once

#include <irrlicht.h>
#include <glm/ext.hpp>

class VObject{
    private:
        glm::vec3 position;
        float a, b, radius;
        int type;

    public:

        //Constructors
        VObject();
        VObject(glm::vec3 p, float x, float y, float r, int t);

        //Destructor
        ~VObject();

        //Getters
        glm::vec3 getPosition();
        float getA();
        float getB();
        float getRadius();
        int getType();

        //Setters
        void setPosition(glm::vec3 p);
        void setA(float x);
        void setB(float y);
        void setRadius(float r);
        void setType(int t);
};
