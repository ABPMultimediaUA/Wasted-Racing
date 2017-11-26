#include "MoveComponent.h"
#include "GameObject.h"

//Initilizer
void MoveComponent::init() {

}

//Update
void MoveComponent::update(float dTime) {

    //Updates all linear variables 
    LAPAL::updateLinearVelocity(mData, dTime);

    //Updates all spin related variables
    LAPAL::updateSpin(mData, dTime);

    //Updates the velocity in a 2D world
    LAPAL::update2DVelocity(mData);

    //Add velocity to our game object
    getGameObject().getTransformData().position.x += mData.vel2d.x;
    getGameObject().getTransformData().position.z += mData.vel2d.z;

}

//Closer
void MoveComponent::close() {

}


//=======================================
// PHYSICS
//=======================================
void MoveComponent::changeImpulse(float n){
    mData.imp = n;
}

void MoveComponent::changeSpinIncrement(float n){
    mData.spin_inc = n;
}