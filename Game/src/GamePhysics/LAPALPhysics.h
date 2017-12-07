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
    void elipticMovement(const LAPAL::movementData& mData, float radiusEx, float radiusIn, const float dTime);

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

    
    
    //-------DEPRECATED-----DEPRECATED------------------------------------------DEPRECATED--------------------DEPRECATED--------------------------------------------------------------------------------------------
    //---------------------------DEPRECATED---------------DEPRECATED-------------------------------DEPRECATED------------------------------------DEPRECATED---------------------------------------------------------
    //-------------DEPRECATED----------DEPRECATED----------------------DEPRECATED----------------DEPRECATED---------------DEPRECATED-----------------------------------------------------------------------------------
    //---DEPRECATED-----------------------DEPRECATED-----DEPRECATED-----------------DEPRECATED----------DEPRECATED--------------------DEPRECATED----------------------------------------------------------------------------------------
    //-----------------------------DEPRECATED-----------DEPRECATED-----------DEPRECATED--------------------------------DEPRECATED----------------------------------DEPRECATED-------------------------------------------------


    //Checks collision with the terrain
    bool checkTerrainCollision(const LAPAL::plane3f& terrain,const LAPAL::vec3f& position); //done

    //Returns normal of a 3d plane
    LAPAL::vec3f calculateNormal(const LAPAL::plane3f& plane); //done

    //Corrects position to the nearest one in the given plane
    void correctTerrainCollision(LAPAL::plane3f& terrain, LAPAL::vec3f& position); //done

    //--------------------------------------
    //-------------3D VOID------------------
    //--------------------------------------

    //Updates the gravity force W (gravity*mass)
    void updateGravityForce(LAPAL::movementData& mData, float& objMass, const float& gravity, LAPAL::plane3f& terrain, const LAPAL::vec3f& position); //done

    //Updates acceleration difference
    void updateAccDif(LAPAL::movementData& mData, LAPAL::plane3f& terrain, const float dTime);

    //Updates velocity difference
    void updateVelDif(LAPAL::movementData& mData, const float& dTime);

    //Calculates the total forces acting on the object
    void calculateNetForce(LAPAL::movementData& mData);

    //Updates the friction force based on the terrain
    void updateFrictionForce(LAPAL::movementData& mData, LAPAL::plane3f& terrain, float& objMass, const float& gravity, const LAPAL::vec3f& position); //done

    //Updates the velocity in a 2D world
    void update2DVelocity(LAPAL::movementData& mData);//done

    //Updates the velocity in a 3D world
    void update3DVelocity(LAPAL::movementData& mData);

    //Calculates the distance between a line defined by two points (l1,l2) and a point (p1)
    float distance2DLinePoint(const LAPAL::vec3f& l1, const LAPAL::vec3f& l2, const LAPAL::vec3f& p1);

    void updateFinalAcceleration(LAPAL::movementData& mData, const LAPAL::vec3f& position);

}