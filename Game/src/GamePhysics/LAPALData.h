#pragma once

#include <glm/glm.hpp>

namespace LAPAL{

    //Universal constant
    const float gravity = 30.f;

    //3d float point
    typedef glm::vec3 vec3f;

    //3d 4 point plane
    struct plane3f{
        vec3f p1 = glm::vec3(0,0,0);
        vec3f p2 = glm::vec3(0,0,0);
        vec3f p3 = glm::vec3(0,0,0);
        vec3f p4 = glm::vec3(0,0,0);
        float fric = 0.0f;      //Friction coeficient !NOT FRICTION FORCE
        float rotX = 0.0f;      //Rotation on X
        float rotZ = 0.0f;      //Rotation on Z
        uint16_t direction = 0; //Direction of preferred movement (0 up, 1 right, 2 down, 3 left)
    };

    //Movement data
    struct movementData{
        //conditionals
        bool jump = false;      //To know if the object is jumping
        bool mov  = false;      //To know if a movement related key is pressed
        bool spi  = false;      //To know if a spin related key is pressed
        bool asc = false;       //To know that the object is ascending
        bool drift = false;     //To know if object is drifting or not
        bool braking = false;   //To know if brake key is pressed
        bool boost = false;     //To know if velocity/acceletarion has been abnormally increased

        //spin
        float angle             = 0.0f;         //Angle faced by object
        float spin              = 0.0f;         //Speed of movement added to angle
        float spin_inc          = 0.0f;         //Increment of the spin
        float max_spin          = 0.0f;         //Maximum spin
        float brake_spin        = 0.0f;         //Spin desaceleration by sheer time

        //Speed variables
        float vel               = 0.0f;         //Lineal speed
        float max_vel           = 0.0f;         //Maximum lineal speed
        float brake_vel         = 0.0f;         //Natural braking speed
        float velY              = 0.0f;         //vertical speed

        float acc               = 0.0f;         //Linear acceleration
        float max_acc           = 0.0f;         //Maximum linear acceleration
        float dAcc              = 0.0f;         //Acceleration increment
        float brake_acc         = 0.0f;         //Acceleration natural braking

        //Rotation in different axis
        float angX              = 0.0f;         //angle in X axis
        float angZ              = 0.0f;         //angle in Z axis
        float rotateX           = 0.0f;         //rotation speed towards X
        float rotateZ           = 0.0f;         //rotation speed towards X
        float rotate_inc        = 0.0f;         //rotation speed towards those angles
        float max_rotate        = 0.0f;         //maximum rotation speed

        //Drifting variables
        float driftDir          = 0.f;                      //Direction of drifting
        float driftIncrement    = 0.785398163397f;          //Initial increment of drifting angle

        //Movement vectors
        vec3f velocity          = glm::vec3(0,0,0);         //Base velocity in 3d

        //Jump variables
        float posY = 0.0;      //Relative position in y when the jump starts

        //Collision variables
        vec3f colVel            = glm::vec3(0,0,0);         //Velocity of the object after a collision  

        //Character type
        int player = -1;         //Type of the character we are playing with
        bool isPlayer = false;   //To distinguish between player and AI

    };

}