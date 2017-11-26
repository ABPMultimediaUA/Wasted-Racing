#pragma once

#include <glm/glm.hpp>

namespace LAPAL{

    //3d float point
    typedef glm::vec3 vec3f;

    //3d 4 point plane
    struct plane3f{
        vec3f p1;
        vec3f p2;
        vec3f p3;
        vec3f p4;
    };

    //Movement data
    struct movementData{
        float vel;
        float max_vel;
        float acc;
        float max_acc;
        float imp;
        float angle;
        float spin;
        float spin_inc;
        float max_spin;
        vec3f vel2d;
        vec3f force;
    };

}