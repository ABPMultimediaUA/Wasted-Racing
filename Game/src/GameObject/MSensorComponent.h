#pragma once

#include "ISensorComponent.h"
#include "VObject.h"
#include <glm/ext.hpp>
#include <vector>
#include "GameObject.h"
#include "../GameManager/PhysicsManager.h"
#include <memory>

class MSensorComponent : public ISensorComponent{
    public:

        //Constructors
        MSensorComponent(GameObject& newGameObject);
        MSensorComponent(GameObject& newGameObject, float angV, float angI);
        
        //Destructor
        virtual ~MSensorComponent() {};

        //Initialize
        virtual void init(){};

        //Close
        virtual void close(){};

        //update
        virtual void update(float dTime){};

        //Checks the objects receives and stores the ones seen in the seenObjects vector
        void updateSeenObjects();

        //Auxiliar function to calculate A and B of given objective
        void calculateAB(glm::vec3& objective, float* a, float* b);

        //Getters and setters
        std::vector<VObject::Pointer> getSeenObjects()           {   return seenObjects;    };
        glm::vec3 getSensorLeft()                                {   return sensorLeft;     };
        glm::vec3 getSensorRight()                               {   return sensorRight;    };
        float getAngleInitial()                                  {   return angleInitial;   };
        float getAngleVision()                                   {   return angleVision;    };

        void setAngleInitial(float a)                            {   angleInitial = a;      };
        void setAngleVision(float a)                             {   angleVision = a;       };

    private:
        //Angle of the sensor
        float angleInitial;
        //Angle of vision
        float angleVision;

        //Vectors that define the limit of the sensors
        glm::vec3 sensorLeft, sensorRight;

        //Objects being seen
        std::vector<VObject::Pointer> seenObjects;


};