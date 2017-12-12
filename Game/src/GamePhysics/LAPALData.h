#pragma once

#include <glm/glm.hpp>

namespace LAPAL{

    //Universal constant
    const float gravity = 98.f;

    //3d float point
    typedef glm::vec3 vec3f;

    //3d 4 point plane
    struct plane3f{
        vec3f p1 = glm::vec3(0,0,0);
        vec3f p2 = glm::vec3(0,0,0);
        vec3f p3 = glm::vec3(0,0,0);
        vec3f p4 = glm::vec3(0,0,0);
        float fric = 0.0f; //friction coeficient !NOT FRICTION FORCE
        float rotX = 0.0f;     //rotation on X
        float rotZ = 0.0f;     //rotation on Z
    };

    //Movement data
    struct movementData{
        //conditionals
        bool jump = false;      //To know if the object is jumping
        bool mov  = false;      //To know if a movement related key is pressed
        bool spi  = false;      //To know if a spin related key is pressed
        
        //spin
        float angInc    = 0.0f;         //?
        float angle     = 0.0f;         //Angle faced by object
        float spin      = 0.0f;         //Speed of movement added to angle
        float spin_inc  = 0.0f;         //Increment of the spin
        float max_spin  = 0.0f;         //Maximum spin
        float brake_spin= 0.0f;         //Spin desaceleration by sheer time

        //Speed variables
        float vel       = 0.0f;         //Lineal speed
        float max_vel   = 0.0f;         //Maximum lineal speed
        float brake_vel = 0.0f;         //Natural braking speed
        float velY      = 0.0f;         //vertical speed

        float acc       = 0.0f;         //Linear acceleration
        float max_acc   = 0.0f;         //Maximum linear acceleration
        float dAcc      = 0.0f;         //Acceleration increment
        float brake_acc = 0.0f;         //Acceleration natural braking

        //Rotation in different axis
        float angX          = 0.0f;         //angle in X axis
        float angZ          = 0.0f;         //angle in Z axis
        float rotateX       = 0.0f;         //rotation speed towards X
        float rotateZ       = 0.0f;         //rotation speed towards X
        float rotate_inc    = 0.0f;         //rotation speed towards those angles
        float max_rotate    = 0.0f;         //maximum rotation speed

        //Movement vectors
        vec3f velocity  = glm::vec3(0,0,0); //base velocity in 3d
        vec3f finalAcc  = glm::vec3(0,0,0); //final acceleration
        vec3f accDif    = glm::vec3(0,0,0); //difference of acceleration

        //Force vectors
        vec3f netForce          = glm::vec3(0,0,0); 
        vec3f frictionForce     = glm::vec3(0,0,0); //All the forces must be in Newtons
        vec3f gravityForce      = glm::vec3(0,0,0);

    };

}