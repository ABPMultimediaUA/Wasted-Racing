#include <irrlicht.h>
#include "VObject.h"
#include <glm/glm.hpp>

using namespace irr;

class FuzzyLogic{
    public:

        //Makes NPC turn a fixated angle based on data
        static float girar(VObject[] collisions, glm::vec3 waypoint, int distance, double a, double b, float maxR);
        
        //Makes NPC accelerate or brake based on data
        static float acelerar_frenar(int distance);

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

