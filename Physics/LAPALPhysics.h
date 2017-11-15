#pragma once

#include <LAPALData.h>

namespace LAPAL {

    //Updates all linear variables 
    void updateLinearVelocity(LAPAL::movementData& mData,const float dTime);

    //Sums up all forces
    void updateForces();

    //Updates and returns the gravity based on the terrain
    LAPAL::vec3f updateGravity();

    //Updates the friction based on the terrain
    LAPAL::vec3f updateFriction();

    //Checks collision with the terrain
    bool checkTerrainCollision(const LAPAL::plane3f& terrain,const LAPAL::vec3f& position);

    //Returns normal of a 3d plane
    LAPAL::vec3f calculateNormal(const LAPAL::plane3f& plane);

    //Corrects position to the nearest one in the given plane
    void correctTerrainCollision(const LAPAL::plane3f& terrain, LAPAL::vec3f& position);

    //Checks colision between circles
    bool checkCircleCircleCollision(const LAPAL::vec3f& pos1,const float& radius1, const LAPAL::vec3f& pos2,const float& radius2);

    //Assuming there's collision, changes velocity of every object after collision
    void calculateIneslaticCollision();


}