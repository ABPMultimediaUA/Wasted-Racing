#include "LAPALPhysics.h"
#include <math.h>

//Updates all linear variables 
void LAPAL::updateLinearVelocity(LAPAL::movementData& mData,const float dTime) {

    //Update acceleration
    mData.acc += mData.imp*dTime;

    //Check acceleration limits
    if(abs(mData.acc)>abs(mData.max_acc)){
        mData.acc = copysign(mData.max_acc, mData.acc);
    }

    //Update velocity
    mData.vel += mData.acc*dTime; 

    //Check velocity limits 
    if(abs(mData.vel)>abs(mData.max_vel)){
        mData.vel = copysign(mData.max_vel, mData.vel);
    }

}

//Checks 2D collision between circles
bool LAPAL::checkCircleCircleCollision(const LAPAL::vec3f& pos1,const float& radius1, const LAPAL::vec3f& pos2,const float& radius2){
    if ( sqrt( ( pos2.x-pos1.x ) * ( pos2.x-pos1.x )  + ( pos2.z-pos1.z ) * ( pos2.z-pos1.z ) ) < ( radius1 + radius2 ) ) // square(x^2+z^2) < total radius
        return true;
    return false;
} 

//Updates all spin related variables
void LAPAL::updateSpin(LAPAL::movementData& mData, const float dTime){

    mData.spin += mData.vel*mData.spin_inc*dTime; //Spin depends on vel and spin_inc

    if(abs(mData.spin)>abs(mData.max_spin)){
        mData.spin = copysign(mData.max_spin, mData.spin);
    }

    mData.angle += mData.spin;

}

//Updates the velocity in a 2D world
void LAPAL::update2DVelocity(LAPAL::movementData& mData) {
    mData.vel2d.x = mData.vel*cos(mData.angle);
    mData.vel2d.z = mData.vel*sin(mData.angle);
}

//Assuming there's collision, changes velocity of every object after collision
void LAPAL::calculateElasticCollision(LAPAL::vec3f& vel1, float& mass1, LAPAL::vec3f& vel2, float& mass2) {

    float mT = mass1 + mass2;

    float m1i_1 = (mass1 - mass2)/mT;
    float m2i_1 = (mass2 + mass2)/mT;
    
    float m1i_2 = (mass1 + mass1)/mT;
    float m2i_2 = (mass2 - mass1)/mT;

    vel1.x = vel1.x*m1i_1 + vel2.x*m2i_1;
    vel2.x = vel1.x*m1i_2 + vel2.x*m2i_2;

    vel1.z = vel1.z*m1i_1 + vel2.z*m2i_1;
    vel2.z = vel1.z*m1i_2 + vel2.z*m2i_2;

}