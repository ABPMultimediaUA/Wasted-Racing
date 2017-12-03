#pragma once

#include "LAPALData.h"

namespace LAPAL {

    //Updates all linear variables 
    void updateLinearVelocity(LAPAL::movementData& mData, const float dTime);//done

    //Updates spin velocity and spin
   // void updateSpin(LAPAL::movementData& mData, const float dTime);//done

    //Updates the gravity force W (gravity*mass)
    void updateGravityForce(LAPAL::movementData& mData, float& objMass, const float& gravity, LAPAL::plane3f& terrain, const LAPAL::vec3f& position); //done

    //Updates acceleration difference
    void updateAccDif(LAPAL::movementData& mData, float& objMass);

    //Updates velocity difference
    void updateVelDif(LAPAL::movementData& mData, const float& dTime);

    //Calculates the total forces acting on the object
    void calculateNetForce(LAPAL::movementData& mData);

    //Updates the friction force based on the terrain
    void updateFrictionForce(LAPAL::movementData& mData, LAPAL::plane3f& terrain, float& objMass, const float& gravity, const LAPAL::vec3f& position); //done

    //Checks collision with the terrain
    bool checkTerrainCollision(const LAPAL::plane3f& terrain,const LAPAL::vec3f& position); //done

    //Returns normal of a 3d plane
    LAPAL::vec3f calculateNormal(const LAPAL::plane3f& plane); //done

    //Corrects position to the nearest one in the given plane
    void correctTerrainCollision(LAPAL::plane3f& terrain, LAPAL::vec3f& position); //done

    //Checks colision between circles
    bool checkCircleCircleCollision(const LAPAL::vec3f& pos1,const float& radius1, const LAPAL::vec3f& pos2,const float& radius2); //done

    //Assuming there's collision, changes velocity of every object after collision
    void calculateElasticCollision(LAPAL::vec3f& vel1, float& mass1, LAPAL::vec3f& vel2, float& mass2); //done

    //Updates the velocity in a 2D world
    void update2DVelocity(LAPAL::movementData& mData);//done

    //Updates the velocity in a 3D world
    void update3DVelocity(LAPAL::movementData& mData);

    //Checks if terrain is horizontal o it's a pendent
    bool checkTerrain(LAPAL::plane3f& terrain); //done

}