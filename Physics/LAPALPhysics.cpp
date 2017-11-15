#include <LAPALPhysics.h>
#include <math.h>

//Updates all linear variables 
void LAPAL::updateLinearVelocity(LAPAL::movementData& mData,const float dTime) {

    //Update acceleration
    mData.acc += mData.imp*dTime;

    //Check acceleration limits
    if(abs(mData.acc)>abs(mData.max_acc)){
        mData.acc = mData.max_acc;
        mData.acc = copysign(mData.acc, mData.imp);
    }

    //Update velocity
    mData.vel += mData.acc*dTime; 

    //Check velocity limits
    if(abs(mData.vel)>abs(mData.max_vel)){
        mData.vel = mData.max_vel;
        mData.vel = copysign(mData.vel, mData.imp);
    }

}

bool LAPAL::checkCircleCircleCollision(const LAPAL::vec3f& pos1,const float& radius1, const LAPAL::vec3f& pos2,const float& radius2){
    if ( sqrt( ( pos2.x-pos1.x ) * ( pos2.x-pos1.x )  + ( pos2.z-pos1.z ) * ( pos2.z-pos1.z ) ) < ( radius1 + radius2 ) ) // square(x^2+z^2) < total radius
        return true;
    return false;
}