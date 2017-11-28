#include <irrlicht.h>
#include <vector>
#include "VObject.h"
#include <glm/glm.hpp>

#pragma once

using namespace irr;

class FuzzyLogic{
    public:

        //Makes NPC turn a fixated angle based on data
        static float girar(std::vector<VObject*>, glm::vec3 waypoint, float distance, float a, float b, float maxR);
        
        //Makes NPC accelerate or brake based on data
        static float acelerar_frenar(std::vector<VObject*>, glm::vec3 waypoint, float a, float b, float maxR);

        //Inferes the fuzzy value in a line with the type given
        static float inferL(float value, float limit1, float limit2, int type);

        //Inferes the fuzzy value in a triangular function given the parameters
        static float inferT(float value, float limit1, float limit2, float limit3);

        //Inferes the fuzzy value in a trapeizodal function
        static float inferT2(float value, float limit1, float limit2, float limit3, float limit4);

        //Inferes the fuzzy value with a circular inference
        static float inferC(float value, float limit1, float limit2, float limit3);


        //Constructor
        FuzzyLogic();

        //Destructor
        ~FuzzyLogic();
};

