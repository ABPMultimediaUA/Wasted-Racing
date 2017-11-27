#pragma once

#include "LAPALData.h"

namespace LAPAL {

    //Updates all linear variables 
    void updateLinearVelocity(LAPAL::movementData& mData, const float dTime);

    //Updates spin velocity and spin
    void updateSpin(LAPAL::movementData& mData, const float dTime);

    //Sums up all forces
    void updateForces(); //not done

    //Updates and returns the gravity based on the terrain
    LAPAL::vec3f updateGravity(); //not done

    //Updates the friction based on the terrain
    LAPAL::vec3f updateFriction(); //not done

    //Checks collision with the terrain
    bool checkTerrainCollision(const LAPAL::plane3f& terrain,const LAPAL::vec3f& position); //not done

    //Returns normal of a 3d plane
    LAPAL::vec3f calculateNormal(const LAPAL::plane3f& plane); //not done

    //Corrects position to the nearest one in the given plane
    void correctTerrainCollision(const LAPAL::plane3f& terrain, LAPAL::vec3f& position); //not done

    //Checks colision between circles
    bool checkCircleCircleCollision(const LAPAL::vec3f& pos1,const float& radius1, const LAPAL::vec3f& pos2,const float& radius2); //done

    //Assuming there's collision, changes velocity of every object after collision
    void calculateElasticCollision(LAPAL::vec3f& vel1, float& mass1, LAPAL::vec3f& vel2, float& mass2);

    //Updates the velocity in a 2D world
    void update2DVelocity(LAPAL::movementData& mData);


}