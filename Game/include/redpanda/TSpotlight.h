#pragma once
#include "TLight.h"

class TSpotlight : public TLight{
    public:
        TSpotlight(){};
        TSpotlight(glm::vec3 in, glm::vec3 dir, float cut)
        {
            intensity = glm::vec4(in, 1.0);
            direction = dir;
            cutoff = cut;
        }
        ~TSpotlight(){};

        /////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////// GETTERS && SETTERS ////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        glm::vec3 getDirection()            {   return direction;       }
        float getCutoff()                   {   return cutoff;          }
        void setDirection(glm::vec3 d)      {   direction = d;          }
        void setCutoff(float c)             {   cutoff = c;             }

    private:
        glm::vec3 direction;
        float cutoff;
};