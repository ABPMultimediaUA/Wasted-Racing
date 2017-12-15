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

    auto position = getGameObject().getTransformData().position;

    updateMaxSpeedOverTime(dTime); //Update speed difference over time (for powerups)

    LAPAL::updateLinearVelocity(mData, dTime, terrain);
    LAPAL::updateSpin(mData, dTime);
    LAPAL::updateVelocity(mData, terrain);
    LAPAL::updateRotation(mData, terrain, dTime);
    LAPAL::correctYPosition(mData, dTime, terrain, position);

    auto trans = getGameObject().getTransformData();

    //Change position
    //trans.position += mData.vel3d;
    trans.position = position;
    trans.position += mData.velocity*dTime;

    //Change rotation
    float degreeAngle = (mData.angle*180)/M_PI;
    float degreeX = (mData.angX*180.f)/M_PI;
    float degreeZ = (mData.angZ*180.f)/M_PI;
    trans.rotation.y = -degreeAngle;
    trans.rotation.z = degreeX;
    trans.rotation.x = degreeZ;
    getGameObject().setTransformData(trans);

    auto id = getGameObject().getId();
    RenderManager::getInstance().getRenderFacade()->updateObjectTransform(id, trans);
    
    ///*===========================================================================================
    // DEBUG
    system("clear");
    if(id == 8){
    //std::cout << " GIRO: "<<mData.angX<<","<<mData.angZ<<std::endl;
    std::cout << " POS X " << trans.position.x << " POS Z " << trans.position.z << std::endl;
    //std::cout << " POS Y " << trans.position.y << std::endl;
    std::cout << " VEL X " << mData.velocity.x << " VEL Z " << mData.velocity.z << std::endl;
    std::cout << " INCR ANGLE " << mData.spin << std::endl;
    std::cout << " ANGULO GIRO " << mData.angle << std::endl;
    //std::cout << " ANGULO GRADOS " << degreeAngle << std::endl;
    //std::cout << " Aceleración " << mData.acc << std::endl;
    //std::cout << " Velocidad " << mData.vel << std::endl;
    //std::cout << " Gravity force on " << mData.gravityForce.y << std::endl;
    //std::cout << " Terrain angles. X: " << terrain.rotX <<", Z: "<<terrain.rotZ << std::endl;

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
    mData.angle += mData.max_spin * n;
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
        mData.vel = mData.max_vel;
        constantAlteredTime -= dTime;
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
