#pragma once

#include "ISensorComponent.h"
#include "VObject.h"
#include <glm/ext.hpp>
#include <vector>

class VSensorComponent : public ISensorComponent{
    public:

        //Constructors
        VSensorComponent(GameObject& newGameObject);
        VSensorComponent(GameObject& newGameObject, float angV, float maxR, float angI);
        
        //Destructor
        virtual ~VSensorComponent() {};

        //Getters
        VObject** getSeenObjects();
        glm::vec3 getSensorLeft();
        glm::vec3 getSensorRight();

        //Checks the objects receives and stores the ones seen in the seenObjects vector
        void updateSeenObjects(GameObject** objects, int s);



    private:
        float maxRadius;
        float angleInitial;
        float angleVision;
        glm::vec3 sensorLeft, sensorRight;

        std::vector<VObject*> seenObjects;


};