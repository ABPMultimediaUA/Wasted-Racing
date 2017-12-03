#include "MoveComponent.h"
#include "GameObject.h"
#include "../GameManager/RenderManager.h"
#include <iostream>

//Initilizer
void MoveComponent::init() {

}

//Update
void MoveComponent::update(float dTime) {

    LAPAL::plane3f terrain;
    terrain.p1 = (LAPAL::vec3f(-100,0,-100));
    terrain.p2 = (LAPAL::vec3f(100,0,100));
    terrain.p3 = (LAPAL::vec3f(100,0,-100));
    terrain.p4 = (LAPAL::vec3f(-100,0,100));
    terrain.fric = 0.2;
    terrain.incAngle = 0;
    terrain.rotAngle = 0;

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

    std::cout << mData.vel3d.x << " | " << mData.vel3d.z << std::endl;

    auto trans = getGameObject().getTransformData();
    trans.position += mData.vel3d;
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