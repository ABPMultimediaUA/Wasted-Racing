#pragma once

#include <glm/glm.hpp>

namespace LAPAL{

    //Universal constant
    const float gravity = 98.f;

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
        float rotX;     //rotation on X
        float rotZ;     //rotation on Z
    };

    //Movement data
    struct movementData{
        //conditionals
        bool jump;  //To know if the object is jumping
        bool mov;   //To know if a movement related key is pressed
        bool spi;   //To know if a spin related key is pressed
        
        //spin
        float angInc;       //?
        float angle;        //Angle faced by object
        float spin;         //Speed of movement added to angle
        float spin_inc;     //Increment of the spin
        float max_spin;     //Maximum spin
        float brake_spin;   //Spin desaceleration by sheer time

        //Speed variables
        float vel;          //Lineal speed
        float max_vel;      //Maximum lineal speed
        float brake_vel;    //Natural braking speed
        float velY;         //vertical speed

        float acc;          //Linear acceleration
        float max_acc;      //Maximum linear acceleration
        float dAcc;         //Acceleration increment
        float brake_acc;    //Acceleration natural braking

        //Rotation in different axis
        float angX;         //angle in X axis
        float angZ;         //angle in Z axis
        float rotateX;       //rotation speed towards X
        float rotateZ;       //rotation speed towards X
        float rotate_inc;   //rotation speed towards those angles
        float max_rotate;   //maximum rotation speed

        //Movement vectors
        vec3f velocity; //base velocity in 3d
        vec3f finalAcc; //final acceleration
        vec3f accDif; //difference of acceleration

        //Force vectors
        vec3f netForce; 
        vec3f frictionForce; //All the forces must be in Newtons
        vec3f gravityForce;

        //vec3f vel2d; //base velocity without minding forces 
        //vec3f vel3d; //3d velocity

       // vec3f velDif; //difference of velocity


    };

}