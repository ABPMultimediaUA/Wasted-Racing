#include "MoveComponent.h"
#include "../GameObject.h"
#include "../../GameManager/RenderManager.h"
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
    LAPAL::updateCollisionMovement(mData, dTime);       //Update deviation in velocity after a collision

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
    //auto id = getGameObject().getId();
    //RenderManager::getInstance().getRenderFacade()->updateObjectTransform(id, trans);
    
    //===========================================================================================
    // DEBUG
    if(getGameObject().getId() == 25000){
        system("clear");
        //std::cout << " GIRO: "<<mData.angX<<","<<mData.angZ<<std::endl;
        std::cout << " POS X " << trans.position.x << " POS Z " << trans.position.z << std::endl;
        //std::cout << " ANG X " << trans.rotation.x << " ANG Y " << trans.rotation.y << " ANG Z " << trans.rotation.z << std::endl;
        //std::cout << " POS Y " << trans.position.y << std::endl;
        std::cout << " VEL X " << mData.velocity.x << " VEL Z " << mData.velocity.z << std::endl;
        std::cout << " INCR ANGLE: " << mData.spin_inc << std::endl;
        std::cout << " ANGULO VELOCIDAD: " << mData.spin << std::endl;
        std::cout << " ANGULO GIRO: " << mData.angle << std::endl;
        std::cout << " Velocidad " << mData.vel << std::endl;
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
    mData.dAcc     = n;
}

void MoveComponent::changeSpinIncrement(float n) {
    //Reset spin speed in case of change of direction
    mData.spin_inc = n;
}

void MoveComponent::isMoving(bool m){
    mData.mov      = m;
}

void MoveComponent::isJumping(bool j){
    mData.jump = j;
}
void MoveComponent::isSpinning(bool s){
    mData.spi      = s;
}

void MoveComponent::changeSpin(float n) {
    mData.spin     = mData.max_spin * n;
}

void MoveComponent::changeAcc(float a){
    mData.acc      = mData.max_acc  * a;
}

void MoveComponent::isDrifting(bool d){
    mData.drift    = d;
    mData.driftDir = (d && mData.spin_inc < 0) ? 1.f : -1.f ; //if it is drifting activation, change direction of drift to the speed one. (negative spin = right turn)
}

void MoveComponent::isBraking(bool b) {
    mData.braking  = b;
}

//=================================================
//Functions related with temporal data changes
//=================================================

//Activate temporal speed change
void MoveComponent::changeMaxSpeedOverTime(float maxSpeed, float constTime, float decTime) {

    if(mData.max_vel != maxSpeed){
        auxData.max_vel         = mData.max_vel;
        mData.max_vel           = maxSpeed;      
    }
    constantAlteredTime     = constTime;
    decrementalAlteredTime  = decTime;
    maxDecrementalAT        = decTime;
    mData.boost             = true;

}

//Update and interpolate temporal speed change
void MoveComponent::updateMaxSpeedOverTime(const float dTime) {

    if(mData.boost) {
        if(constantAlteredTime > 0) {
            //While time is constant, velocity is constant and maximum
            mData.vel = mData.max_vel;
            constantAlteredTime -= dTime;
        }
        else if (decrementalAlteredTime > 0) {
            //Calculate velocity decrease depending on dTime
            float vel_diff = mData.max_vel - auxData.max_vel;
            float vel      = (dTime*vel_diff)/maxDecrementalAT;

            mData.vel     -= vel; 

            decrementalAlteredTime -= dTime;

            if(decrementalAlteredTime < 0) {
                mData.max_vel = auxData.max_vel;
                mData.boost   = false;
            }
                
        }
    }
    else {
        constantAlteredTime = 0;
        decrementalAlteredTime = 0;
        mData.max_vel = auxData.max_vel; 
    }
}

//Control and update jump
void MoveComponent::updateJump(LAPAL::movementData& mData, glm::vec3& pos, LAPAL::plane3f t){

    float maxJump = 15.0;
    float velJump = 50.0;

    if(mData.jump == true){
        if(abs(LAPAL::calculateExpectedY(t, pos)-pos.y) < 1) { 
            mData.posY = pos.y + maxJump;
            mData.asc = true;
        }
    }
    if(mData.asc == true){
        if(pos.y < mData.posY){
            mData.velocity.y = (velJump*(mData.posY-pos.y)/maxJump)+10; //Interpolate velocity 
                                                                            //(the higher we get, the slower we go up)
        }
        else{
            mData.asc = false;
            mData.velocity.y = 0;
        }
    }
}