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
    //Clearing messages and leaving any possible function test message to occur
    system("clear");

    auto position = getGameObject().getTransformData().position;

    LAPAL::updateLinearVelocity(mData, dTime, terrain);
    LAPAL::updateSpin(mData, dTime);
    LAPAL::updateVelocity(mData, terrain);
    LAPAL::updateRotation(mData, terrain, dTime);
    LAPAL::correctYPosition(mData, dTime, terrain, position);


    //Collision deprecated calculus
    //LAPAL::correctTerrainCollision(terrain, position);

    //3D deprecated calculus
    //LAPAL::updateFrictionForce(mData, terrain, mass, 9.8, position);
    //LAPAL::updateGravityForce(mData, mass, 9.8, terrain, position);
    //LAPAL::calculateNetForce(mData);

    //LAPAL::updateAccDif(mData, terrain, dTime);
    //LAPAL::updateFinalAcceleration(mData, position);
    //LAPAL::update2DVelocity(mData);
    //LAPAL::update3DVelocity(mData, dTime);
    //LAPAL::updateVelDif(mData, dTime);

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
    
    ///*
    
    std::cout << "GIRO: "<<mData.angX<<","<<mData.angZ<<std::endl;
    std::cout << " POS X " << trans.position.x << " POS Z " << trans.position.z << std::endl;
    std::cout << " POS Y " << trans.position.y << std::endl;
    std::cout << " VEL X " << mData.velocity.x << " VEL Z " << mData.velocity.z << std::endl;
    std::cout << " INCR ANGLE " << mData.spin << std::endl;
    std::cout << " ANGULO GIRO " << mData.angle << std::endl;
    std::cout << " ANGULO GRADOS " << degreeAngle << std::endl;
    std::cout << " Aceleración " << mData.acc << std::endl;
    std::cout << " Velocidad " << mData.vel << std::endl;
    std::cout << " Gravity force on " << mData.gravityForce.y << std::endl;
    std::cout << "Terrain angles. X: " << terrain.rotX <<", Z: "<<terrain.rotZ << std::endl;

    if (mData.jump == false){
        std::cout << " No estoy saltando " << std::endl;
    }
    else{
        std::cout << " Sí estoy saltando " << std::endl;
    }
    
    //*/
     

    auto id = getGameObject().getId();
    RenderManager::getInstance().getRenderFacade()->updateObjectTransform(id, trans);

}

//Closer
void MoveComponent::close() {

}

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
