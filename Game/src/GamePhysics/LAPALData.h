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
        float fric; //friction coeficient !NOT FRICTION FORCE
        float incAngle; //inclination angle of the plane 
        float rotAngle; //rotation of the plane
    };

    //Movement data
    struct movementData{
        bool mov;
        int dir;
        bool jump;
        float angInc;
        float vel;
        float max_vel;
        float acc;
        float max_acc;
        float dAcc; //acceleration increment
        float angle;
        float spin;
        float spin_inc;
        float max_spin;
        vec3f vel2d; //base velocity without minding forces 
        vec3f vel3d;
        vec3f netForce;
        vec3f frictionForce; //All the forces must be in Newtons
        vec3f gravityForce;
        vec3f velDif; //difference of velocity
        vec3f accDif; //difference of acceleration

    };

}