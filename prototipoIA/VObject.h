#include <glm/glm.hpp>

class VObject{
    private:
        glm::vec3 position;
        float a = 0, b = 0, radius = 0;
        int type = -1;

    public:
        //Constructor
        VObject(glm::vec3 pos, float A, float B, float r, int t);

        //Destructor
        ~VObject();

        //get position
        glm::vec3 getPosition();

        //get a
        float getA();

        //get b
        float getB();

        //get radius
        float getRadius();

        //get type
        int getType();
};
