#include <irrlicht.h>
#include <glm/glm.hpp>

using namespace irr;

class FuzzyLogic{
    public:

        //Makes NPC turn a fixated angle based on data
        static double girar(int distance, double a, double b, float maxR);
        
        //Makes NPC accelerate or brake based on data
        static int acelerar_frenar(int distance);

        //Constructor
        FuzzyLogic();

        //Destructor
        ~FuzzyLogic();
};

