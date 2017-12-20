#include "MoveComponent.h"
#include "GameObject.h"
#include "../GameManager/RenderManager.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>

//Initilizer
void MoveComponent::init() {
}

//Update
void MoveComponent::update(float dTime) {

    //get transform data
    auto position = getGameObject().getTransformData().position;

    //Update all movement conditions and visuals
    updateMaxSpeedOverTime(dTime); //Update speed difference over time (for powerups)
    LAPAL::updateRotation(mData, terrain, dTime);       //Updates rotation of object (axis and visual)

    //Update horizontal movement
    LAPAL::updateLinearVelocity(mData, dTime, terrain); //Update linear velocity
    LAPAL::updateSpin(mData, dTime);                    //Update spin (turning)
    LAPAL::updateVelocity(mData, terrain);              //Update velocity (vector)
    LAPAL::updateEllipticMovement(mData, dTime);        //Update elliptic movement (if object is drifting)

    //Correct vertical movement
    updateJump(mData, position, terrain);
    LAPAL::correctYPosition(mData, dTime, terrain, position);

    //Get transform data
    auto trans = getGameObject().getTransformData(); 
    
    //Change position
    trans.position = position;
    trans.position += mData.velocity*dTime;

    //Change rotation
    trans.rotation.y = -mData.angle;
    trans.rotation.z = mData.angZ;
    trans.rotation.x = mData.angX;


    //Set final transform of position
    getGameObject().setTransformData(trans);

    auto id = getGameObject().getId();
    RenderManager::getInstance().getRenderFacade()->updateObjectTransform(id, trans);
    
    ///*===========================================================================================
    // DEBUG
    if(id == 55){
        system("clear");
        std::cout << " GIRO: "<<mData.angX<<","<<mData.angZ<<std::endl;
        std::cout << " POS X " << trans.position.x << " POS Z " << trans.position.z << std::endl;
        std::cout << " ANG X " << trans.rotation.x << " ANG Y " << trans.rotation.y << " ANG Z " << trans.rotation.z << std::endl;
        //std::cout << " POS Y " << trans.position.y << std::endl;
        std::cout << " VEL X " << mData.velocity.x << " VEL Z " << mData.velocity.z << std::endl;
        std::cout << " INCR ANGLE " << mData.spin << std::endl;
        std::cout << " ANGULO GIRO " << mData.angle << std::endl;
        //std::cout << " ANGULO GRADOS " << degreeAngle << std::endl;
        //std::cout << " Aceleración " << mData.acc << std::endl;
        std::cout << " Velocidad " << mData.vel << std::endl;
        //std::cout << " Gravity force on " << mData.gravityForce.y << std::endl;
        //std::cout << " Terrain angles. X: " << terrain.rotX <<", Z: "<<terrain.rotZ << std::endl;
        //std::cout << " VEL Y " << mData.velY << std::endl;


        /*if (mData.jump == false){
            std::cout << " No estoy saltando " << std::endl;
        }
        else{
            std::cout << " Sí estoy saltando " << std::endl;
        }*/
    }

    //=========================================================================================*/
}

//Closer
void MoveComponent::close() {

}

//Physics related functions
void MoveComponent::changeAccInc(float n) {
    mData.dAcc = n;
}

void MoveComponent::changeSpinIncrement(float n) {
    mData.spin_inc = n;
}

void MoveComponent::isMoving(bool m){
    mData.mov = m;
}

void MoveComponent::changeAngleInc(float i){
    mData.angInc = i;
}

void MoveComponent::isJumping(bool j){
        mData.jump = j;
}
void MoveComponent::isSpinning(bool s){
    mData.spi = s;
}

void MoveComponent::changeSpin(float n) {
    mData.spin = mData.max_spin * n;
}

void MoveComponent::changeAcc(float a){
    mData.acc  = mData.max_acc  * a;
}

void MoveComponent::isDrifting(bool d){
    mData.drift = d;
    
    //Change directions
    if(d && mData.spin<0){
        mData.driftDir=false;
    }else{
        mData.driftDir=true;
    }
}

void MoveComponent::isBraking(bool b) {
    mData.braking = b;
}


//Functions related with temporal data changes
void MoveComponent::changeMaxSpeedOverTime(float maxSpeed, float constTime, float decTime) {

    if(mData.max_vel != maxSpeed){
 
        auxData.max_vel         = mData.max_vel;
        mData.max_vel           = maxSpeed;

        constantAlteredTime     = constTime;
        decrementalAlteredTime  = decTime;
        maxDecrementalAT        = decTime;
    }
}

void MoveComponent::updateMaxSpeedOverTime(const float dTime) {

    if(constantAlteredTime > 0) {
        //While time is constant, velocity is constant and maximum
        if(mData.vel < 0) {
            constantAlteredTime = -1;
        }
        else {
            mData.vel = mData.max_vel;
            constantAlteredTime -= dTime;
        }
    }
    else if (decrementalAlteredTime > 0) {
        //Calculate velocity decrease depending on dTime
        float vel_diff = mData.max_vel - auxData.max_vel;
        float vel      = (dTime*vel_diff)/maxDecrementalAT;

        mData.vel     -= vel; 

        decrementalAlteredTime -= dTime;

        if(decrementalAlteredTime < 0)
            mData.max_vel = auxData.max_vel;
    }

}

void MoveComponent::updateJump(LAPAL::movementData& mData, glm::vec3& pos, LAPAL::plane3f t){

    if(mData.jump == true){
        if(abs(LAPAL::calculateExpectedY(t, pos)-pos.y) < 1) { 
            mData.posY = pos.y +15;
            mData.asc = true;
        }
    }
    if(mData.asc == true){
        if(pos.y < mData.posY){
            mData.velocity.y = 50;
        }
        else{
            mData.asc = false;
            mData.velocity.y = 0;
        }
    }
}