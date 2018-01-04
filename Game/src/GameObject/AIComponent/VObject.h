#pragma once

#include <glm/ext.hpp>
#include <memory>

class VObject{
    private:
        glm::vec3 position;
        float a, b, radius;
        int type;

    public:

        //Define shared pointer type
        typedef std::shared_ptr<VObject> Pointer;

        //Constructors
        VObject();
        VObject(glm::vec3 p, float x, float y, float r, int t);

        //Destructor
        ~VObject();

        //Getters
        glm::vec3 getPosition()     {   return position;    };
        float getA()                {   return a;           };
        float getB()                {   return b;           };
        float getRadius()           {   return radius;      };
        int getType()               {   return type;        };

        //Setters
        void setPosition(glm::vec3 p)   {   position = p;   };
        void setA(float x)              {   a = x;          };
        void setB(float y)              {   b = y;          };
        void setRadius(float r)         {   radius = r;     };
        void setType(int t)             {   type = t;       };
};
