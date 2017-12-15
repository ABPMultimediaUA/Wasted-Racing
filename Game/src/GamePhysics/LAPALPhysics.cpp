#include "LAPALPhysics.h"
#include <math.h>
#include <iostream>


//--------------------------------------
//-------------MOVEMENT-----------------
//--------------------------------------

//Updates all linear variables 
void LAPAL::updateLinearVelocity(LAPAL::movementData& mData, const float dTime, const LAPAL::plane3f& terrain) {

    //The approach is that we define a base acceleration for every object and it does not change when it reaches the max acceleration
    //then, we calculate how the different forces affect the acceleration and, therefore, the velocity

    //Initial check of destination: prevents acceleration from pushing same direction when chaning forces.
    if((mData.acc <0 && mData.dAcc>0) || (mData.acc >0 && mData.dAcc <0)){
        mData.acc = 0.f;
    }

    //BASE ACCELERATION
    mData.acc += mData.dAcc*dTime; //increment of acceleration * increment of time

    //If we aren't accelerating
    if(!mData.mov && mData.acc!=0) {
        if(abs(mData.acc)<0.1) {
            mData.acc = 0;
        
        //--------------Fictional friction
        }else{
            if(mData.acc>0){
                mData.acc -= (mData.brake_acc)*dTime;  
            }else{
                mData.acc += (mData.brake_acc)*dTime; 
            }
        }
    }

    //Check acceleration limits
    if(abs(mData.acc)>abs(mData.max_acc)){
        mData.acc = copysign(mData.max_acc, mData.acc);
    }

    //Update velocity
    mData.vel += mData.acc*dTime; 

    //If we aren't accelerating
    if(!mData.mov && mData.vel!=0) {
        if(abs(mData.vel)<0.1) {
            mData.vel= 0;

        //--------------Fictional friction
        }else{
            if(mData.vel>0){
                mData.vel -= (mData.brake_vel + terrain.fric)*dTime;
            }else{
                mData.vel += (mData.brake_vel + terrain.fric)*dTime;
            }  
        }
    }

    //Check velocity limits 
    if(abs(mData.vel)>abs(mData.max_vel)){
        mData.vel = copysign(mData.max_vel, mData.vel);
    }
    
}

void LAPAL::updateVelocity(LAPAL::movementData& mData, LAPAL::plane3f& terrain){
    mData.velocity.x = mData.vel*cos(mData.angle);
    mData.velocity.y = 0;
    mData.velocity.z = mData.vel*sin(mData.angle);

    //Applying reduction of speed based on plane angle
    if(!checkTerrain(terrain)){
        //If we are going upwards in X axis, then lower the speed
        if((mData.velocity.x > 0 && terrain.rotX > 0) || (mData.velocity.x < 0 && terrain.rotX < 0)){
            mData.velocity.x *= cos(terrain.rotX);
        }else{
            //if we are going downwards, raise it a bit
            mData.velocity.x *= (2-cos(terrain.rotX));
        }

        //If we are going upwards in Z axis, then lower the speed
        if((mData.velocity.z > 0 && terrain.rotZ > 0) || (mData.velocity.z < 0 && terrain.rotZ < 0)){
            mData.velocity.z *= cos(terrain.rotZ);
        }else{
            //if we are going downwards, raise it a bit
            mData.velocity.z *= (2-cos(terrain.rotZ));
        }
    }
}

//Updates all spin related variables
void LAPAL::updateSpin(LAPAL::movementData& mData, const float dTime){

    if(mData.spi) {

        mData.spin += mData.spin_inc * dTime; //Spin depends on vel and spin_inc

        if(abs(mData.spin)>abs(mData.max_spin)){
            mData.spin = copysign(mData.max_spin, mData.spin);
        }
    }
    else {
        if(abs(mData.spin) < 0.001) {
            mData.spin = 0;
        
        //Comment if no friction
        }else {
            mData.spin -= mData.spin*(mData.brake_spin);
        }
    }

    if(abs(mData.spin)>abs(mData.max_spin)){
        mData.spin = copysign(mData.max_spin, mData.spin);
    }
    mData.angle += mData.spin;
}

//Updates rotation of the vehicle to match the terrain's. Do it smoothly.
void LAPAL::updateRotation(LAPAL::movementData& mData, LAPAL::plane3f& terrain, const float dTime){

    //Increasing rotation axis in mData until it matches the terrain. Incremental turn.
    //X axis.
    if(glm::abs(mData.angX - terrain.rotX)<0.01f){
        mData.angX = terrain.rotX;
        mData.rotateX = 0.f;
    }else{
        //increment rotate speed
        mData.rotateX += mData.rotate_inc*dTime;

        if(abs(mData.rotateX)>abs(mData.max_rotate)){
            mData.rotateX = copysign(mData.max_rotate, mData.rotateX);
        }

        //if rotation is higher or lower
        if(mData.angX < terrain.rotX){
            mData.angX += mData.rotateX*dTime;
        }else{
            mData.angX -= mData.rotateX*dTime;
        }
    }

    //Z axis
    if(glm::abs(mData.angZ - terrain.rotZ)<0.001f){
        mData.angZ = terrain.rotZ;
        mData.rotateZ = 0.f;
    }else{
        //increment rotate speed
        mData.rotateZ += mData.rotate_inc*dTime;

        if(abs(mData.rotateZ)>abs(mData.max_rotate)){
            mData.rotateZ = copysign(mData.max_rotate, mData.rotateZ);
        }

        //if rotation is higher or lower
        if(mData.angZ < terrain.rotZ){
            mData.angZ += mData.rotateZ*dTime;
        }else{
            mData.angZ -= mData.rotateZ*dTime;
        }
    }
}

//function that moves the vehicle elliptically given its internal radius ratio rotation
void LAPAL::updateEllipticMovement( LAPAL::movementData& mData, const float dTime){
    //Check if drifting is pressed
    if(mData.drift){
        //Initial variables
        float driftIncrement = glm::abs( mData.spin / mData.max_spin );

        //Initial declarations
        if(mData.driftDir){
            
            mData.velocity.x += mData.vel*cos(mData.angle-1.57079632679f) * driftIncrement;
            mData.velocity.z += mData.vel*sin(mData.angle-1.57079632679f) * driftIncrement;
        }else{
            mData.velocity.x += mData.vel*cos(mData.angle+1.57079632679f) * driftIncrement;
            mData.velocity.z += mData.vel*sin(mData.angle+1.57079632679f) * driftIncrement;
        }
    }
}

//--------------------------------------
//-------------COLLISIONS---------------
//--------------------------------------

//Checks 2D collision between circles
bool LAPAL::checkCircleCircleCollision(const LAPAL::vec3f& pos1,const float& radius1, const LAPAL::vec3f& pos2,const float& radius2) {
    if ( ( pos2.x-pos1.x ) * ( pos2.x-pos1.x )  + ( pos2.z-pos1.z ) * ( pos2.z-pos1.z ) < ( radius1 + radius2 ) * ( radius1 + radius2) ) // = (x² + z²)<tRadius²
        return true;
    return false;
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

bool LAPAL::checkTerrain(LAPAL::plane3f& terrain){

    bool state=false;

   if(terrain.p1.y == terrain.p2.y && terrain.p1.y == terrain.p3.y && terrain.p1.y == terrain.p4.y){ 
        state=true; //the plane is horizontal
    }

    return state;
}

//Calculates rotation in X and Z of the plane, assigning the value to rotZ and rotX
void LAPAL::calculateRotationsXZ(LAPAL::plane3f& terrain){
    //check if terrain is horizontal or not
    if(checkTerrain(terrain)){
        terrain.rotX = 0.f;
        terrain.rotZ = 0.f;
    }else{
        //check which points are at different heights. We only need to check two pairs.
        if(terrain.p1.x != terrain.p2.x){
            //angle = acos(cc / h) acos
            terrain.rotX = glm::atan( (terrain.p2.y - terrain.p1.y) / glm::abs(terrain.p2.x-terrain.p1.x) );
        }else{
            terrain.rotX = glm::atan( (terrain.p3.y - terrain.p2.y) / glm::abs(terrain.p3.x-terrain.p2.x) );
        }  
        
        if(terrain.p1.z != terrain.p2.z){
            terrain.rotZ = glm::atan( (terrain.p2.y - terrain.p1.y) / glm::abs(terrain.p2.z-terrain.p1.z) );
        }else{
            terrain.rotZ = glm::atan( (terrain.p3.y - terrain.p2.y) / glm::abs(terrain.p3.z-terrain.p2.z) );
        }
    } 
}

//Calculates expected Y for the object given its position
float LAPAL::calculateExpectedY(LAPAL::plane3f& terrain, LAPAL::vec3f& position ){
    if(checkTerrain(terrain)){
        return terrain.p1.y;
    }else{
        //Scalars that determine the position Y of the object given the vectors needed to compose its position in X and Z
        float a, b;

        //Using auxiliar function to calculate them
        calculateConstantAB(terrain, position, &a, &b);

        //Returns the Y value correspondent
        return a * (terrain.p3 - terrain.p1).y + b  * (terrain.p4 - terrain.p2).y + terrain.p1.y;
    }
}

//Corrects position of the object on Y
void LAPAL::correctYPosition(LAPAL::movementData& mData, const float dTime, LAPAL::plane3f& terrain, LAPAL::vec3f& position){
    float y = calculateExpectedY(terrain, position);
    
    //check if we are not touching the ground
    if(position.y>y){
        
        //update vertical speed with gravity
        mData.velY += gravity*dTime*dTime;

        //Maximum velocity
        if(mData.velY > gravity){
            mData.velY = gravity;
        }

        //update falling position
        position.y -= mData.velY* dTime;


        if(position.y <= y){
            position.y = y;
            mData.velY = 10.f;
        }
    }else{
        position.y = y;
    }
}

//Calculates values A and B which are the scalars that multiply vector A and B to compose the point C in 2D (X-Z plane) inside the terrain given
void LAPAL::calculateConstantAB(LAPAL::plane3f& terrain, LAPAL::vec3f& position, float* a, float* b){
    *a = 0.f;
    *b = 0.f;

    //Vector that will compose the position inside the terrain
    glm::vec3 vec_a = terrain.p3 - terrain.p1;
    glm::vec3 vec_b = terrain.p4 - terrain.p2;
    glm::vec3 relativeP = position - terrain.p1;

    //Composing the scalars
    if(vec_a.x * vec_b.z != vec_a.z * vec_b.x) 
        *a = (relativeP.x * vec_b.z - relativeP.z * vec_b.x) /(vec_a.x * vec_b.z - vec_a.z * vec_b.x);
    if(vec_b.z != 0)
        *b = (relativeP.z - (*a) * vec_a.z) / vec_b.z;

}

//Calculates the distance between a line defined by two points (l1,l2) and a point (p1)
float LAPAL::distance2DLinePoint(const LAPAL::vec3f& l1, const LAPAL::vec3f& l2, const LAPAL::vec3f& p1) {

    LAPAL::vec3f lineVec = l2-l1;
    LAPAL::vec3f circVec = l2-p1;

    float lineVecMod = sqrt(lineVec.x*lineVec.x + lineVec.z*lineVec.z);
    float circVecMod = sqrt(circVec.x*circVec.x + circVec.z*circVec.z);

    float vectorCos = (lineVec.x*circVec.x + lineVec.z*circVec.z)/(lineVecMod*circVecMod);

    float distance = vectorCos*circVecMod;

    return distance;

}

//Calculates if a circle is inside a rectangle
bool LAPAL::checkCircleRectangleCollision(const LAPAL::plane3f& terrain, const LAPAL::vec3f& position, const float radius) {

    float distance;

    distance = LAPAL::distance2DLinePoint(terrain.p1, terrain.p2, position);
    if( distance+radius < 0 ) return false;

    distance = LAPAL::distance2DLinePoint(terrain.p3, terrain.p4, position);
    if( distance+radius < 0 ) return false;

    distance = LAPAL::distance2DLinePoint(terrain.p2, terrain.p3, position);
    if( distance+radius < 0 ) return false;

    distance = LAPAL::distance2DLinePoint(terrain.p4, terrain.p1, position);
    if( distance+radius < 0 ) return false;

    return true;

}