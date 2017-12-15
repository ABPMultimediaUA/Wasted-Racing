#pragma once

#include "LAPALData.h"

namespace LAPAL {

    //--------------------------------------
    //-------------MOVEMENT-----------------
    //--------------------------------------
    //Updates all linear variables 
    void updateLinearVelocity(LAPAL::movementData& mData, const float dTime, const LAPAL::plane3f& terrain);//done

    //Updates spin velocity and spin
    void updateSpin(LAPAL::movementData& mData, const float dTime);//done

    //Updates vector velocity
    void updateVelocity(LAPAL::movementData& mData, LAPAL::plane3f& terrain);

    //Updates object rotation based on terrain
    void updateRotation(LAPAL::movementData& mData, LAPAL::plane3f& terrain, const float dTime);

    //function that moves the vehicle elliptically given its internal radius ratio rotation
    void updateEllipticMovement( LAPAL::movementData& mData, const float dTime);

    //--------------------------------------
    //-------------COLLISIONS---------------
    //--------------------------------------
    //Checks colision between circles
    bool checkCircleCircleCollision(const LAPAL::vec3f& pos1,const float& radius1, const LAPAL::vec3f& pos2,const float& radius2); //done

    //Assuming there's collision, changes velocity of every object after collision
    void calculateElasticCollision(LAPAL::vec3f& vel1, float& mass1, LAPAL::vec3f& vel2, float& mass2); //done

    //Checks if terrain is horizontal o it's a pendent
    bool checkTerrain(LAPAL::plane3f& terrain); //done

    //Calculates rotation in X and Z of the plane, assigning the value to rotZ and rotX
    void calculateRotationsXZ(LAPAL::plane3f& terrain);

    //Calculates expected Y for the object given its position
    float calculateExpectedY(LAPAL::plane3f& terrain, LAPAL::vec3f& position );

    //Corrects position of the object on Y
    void correctYPosition(LAPAL::movementData& mData, const float dTime, LAPAL::plane3f& terrain, LAPAL::vec3f& position);

    //Calculates values A and B which are the scalars that multiply vector A and B to compose the point C in 2D (X-Z plane) inside the terrain given
    void calculateConstantAB(LAPAL::plane3f& terrain, LAPAL::vec3f& position, float* a, float* b);

    //Calculates the distance between a line defined by two points (l1,l2) and a point (p1)
    float distance2DLinePoint(const LAPAL::vec3f& l1, const LAPAL::vec3f& l2, const LAPAL::vec3f& p1);

    //Calculates if a circle is inside a rectangle
    bool checkCircleRectangleCollision(const LAPAL::plane3f& terrain, const LAPAL::vec3f& position, const float radius);

}