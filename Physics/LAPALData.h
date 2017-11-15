#pragma once

namespace LAPAL{

    //3d float point
    struct vec3f{
        float x; //positive to the right
        float y; //positive to the top
        float z; //positive to you
    };

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
    };

}