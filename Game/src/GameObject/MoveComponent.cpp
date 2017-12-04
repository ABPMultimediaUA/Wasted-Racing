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

    LAPAL::updateLinearVelocity(mData, dTime);
    LAPAL::updateSpin(mData, dTime);
    LAPAL::update2DVelocity(mData);
    LAPAL::updateFrictionForce(mData, terrain, mass, 9.8, position);
    LAPAL::updateGravityForce(mData, mass, 9.8, terrain, position);
    LAPAL::calculateNetForce(mData);
    LAPAL::updateAccDif(mData, mass);
    LAPAL::updateVelDif(mData, dTime);
    LAPAL::update3DVelocity(mData);
    LAPAL::correctTerrainCollision(terrain, position);
    
   
    auto trans = getGameObject().getTransformData();

    //Change position
    trans.position += mData.vel3d;

    //Change rotation
    float degreeAngle = (mData.angle*180)/M_PI;
    trans.rotation.y = -degreeAngle;
    
    getGameObject().setTransformData(trans);
    
    ///*
    system("clear");
    std::cout << " POS X " << trans.position.x << " POS Z " << trans.position.z << std::endl;
    std::cout << " POS Y " << trans.position.y << std::endl;
    std::cout << " VEL X " << mData.vel3d.x << " VEL Z " << mData.vel3d.z << std::endl;
    std::cout << " INCR ANGLE " << mData.spin << std::endl;
    std::cout << " ANGULO GIRO " << mData.angle << std::endl;
    std::cout << " ANGULO GRADOS " << degreeAngle << std::endl;
    std::cout << " Aceleración " << mData.acc << std::endl;
    std::cout << " Velocidad " << mData.vel << std::endl;
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

void MoveComponent::isSpinning(bool s){
    mData.spi = s;
}