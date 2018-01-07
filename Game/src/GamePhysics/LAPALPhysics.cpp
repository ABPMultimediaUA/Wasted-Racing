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

    //Aply friction
    if(!mData.mov) {
        if(mData.acc>0) {
            mData.acc -= (mData.brake_acc)*dTime;  
        }else {
            mData.acc += (mData.brake_acc)*dTime; 
        }
    }
    
    //If we aren't accelerating
    if(abs(mData.acc)<0.5  && !mData.mov)
        mData.acc = 0;

    //Check acceleration limits
    if(abs(mData.acc)>abs(mData.max_acc)){
        mData.acc = copysign(mData.max_acc, mData.acc);
    }

    //Update velocity
    mData.vel += mData.acc*dTime*(1-terrain.fric); 

    //Aply friction
    if(!mData.mov) {
        if(mData.vel>0) {
            mData.vel -= (mData.brake_vel + terrain.fric)*dTime;  
        }else {
            mData.vel += (mData.brake_vel + terrain.fric)*dTime; 
        }
    }

    //If we aren't accelerating
    if(abs(mData.vel)<0.5 && !mData.mov)
        mData.vel = 0;

    //Check velocity limits 
    if(abs(mData.vel)>abs(mData.max_vel*(1-terrain.fric))){
        mData.vel = copysign(mData.max_vel*(1-terrain.fric), mData.vel);
    }
    
    //Check if we are braking
    if(mData.braking){
        mData.vel = mData.vel * 0.99f;
        mData.acc = mData.acc * 0.99f;
    }
}

//Updates vector velocity
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
    //if spinning
    mData.spin += mData.spin_inc * dTime; //Spin depends on vel and spin_inc

    //if not
    if(!mData.spi) {
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

    if(!mData.drift){
        mData.angle += mData.spin;
    }
}

//Updates rotation of the vehicle to match the terrain's. Do it smoothly.
void LAPAL::updateRotation(LAPAL::movementData& mData, LAPAL::plane3f& terrain, const float dTime){

    //Increasing rotation axis in mData until it matches the terrain. Incremental turn.
    //X axis.
    if(glm::abs(mData.angX - terrain.rotX)<0.05f){
        mData.angX = terrain.rotX;
        mData.rotateX = 0.f;
    }else{
        //increment rotate speed
        mData.rotateX += mData.rotate_inc*dTime*50;

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
    if(glm::abs(mData.angZ - terrain.rotZ)<0.05f){
        mData.angZ = terrain.rotZ;
        mData.rotateZ = 0.f;
    }else{
        //increment rotate speed
        mData.rotateZ += mData.rotate_inc*dTime*50;

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

//Function that moves the vehicle elliptically given its internal radius ratio rotation
void LAPAL::updateEllipticMovement( LAPAL::movementData& mData, const float dTime){
    //Check if it is turning somewhere
    if(mData.spin_inc != 0){
        //Check if drifting is pressed
        if(mData.drift){ 
            //Initial variables
        
            //if true, drift going left and NPC was right
            if(mData.driftDir == 1.0f){
                //if spin is going in the same direction as when it began (right)
                if(mData.spin_inc < 0){
                    mData.angle  -= dTime*2;
                }

                //Update velocity with 90º vector
                mData.velocity.x += mData.vel*cos(mData.angle + mData.driftIncrement);
                mData.velocity.z += mData.vel*sin(mData.angle + mData.driftIncrement);
            }
            //if true drift is going right and NPC was left
            if(mData.driftDir == -1.f){
                //if spin is positive, it is going in the same direction as when it began ( left)
                if(mData.spin_inc > 0){
                    mData.angle          += dTime*2;
                }

                //Update velocity with 90º vector
                mData.velocity.x += mData.vel*cos(mData.angle - mData.driftIncrement);
                mData.velocity.z += mData.vel*sin(mData.angle - mData.driftIncrement);
            }
        }else{
            mData.driftDir       = 0.f;
        }
    }
}

//Updates the deviation in velocity caused by a collision
void LAPAL::updateCollisionMovement ( LAPAL::movementData& mData, const float dTime) {

    if(abs(mData.colVel.x) > 0.1 || abs(mData.colVel.z) > 0.1) {

        mData.velocity.x -= mData.colVel.x;
        mData.velocity.z -= mData.colVel.z;

        mData.colVel.x -= mData.colVel.x*dTime;
        mData.colVel.z -= mData.colVel.z*dTime;

    }

}


//--------------------------------------
//-------------COLLISIONS---------------
//--------------------------------------

//Checks 2D collision between circles
bool LAPAL::checkCircleCircleCollision(const LAPAL::vec3f& pos1,const float& radius1, const LAPAL::vec3f& pos2,const float& radius2) {
    if ( ( pos2.x-pos1.x ) * ( pos2.x-pos1.x )  + ( pos2.z-pos1.z ) * ( pos2.z-pos1.z ) < ( radius1 + radius2 ) * ( radius1 + radius2) ) // = (x² + z²)<tRadius²
        if( abs(pos2.y-pos1.y) < (radius1 + radius2) )
            return true;
    return false;
} 


//Assuming there's collision, changes velocity of every object after collision
void LAPAL::calculateElasticCollision(LAPAL::vec3f& vel1, float& mass1, LAPAL::vec3f& vel2, float& mass2) {

    //We apply the physical inelastic collision formula

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

//Checks if terrain is horizontal o it's a pendent 
bool LAPAL::checkTerrain(const LAPAL::plane3f& terrain){

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
        terrain.rotZ = 0.f;
        terrain.rotX = 0.f;
    }else{
        //check which points are at different heights. We only need to check two pairs.
        if(terrain.p1.x != terrain.p2.x){
            terrain.rotZ = glm::atan( (terrain.p2.y - terrain.p1.y) / glm::abs(terrain.p2.x-terrain.p1.x) );
        }else{
            terrain.rotZ = glm::atan( (terrain.p3.y - terrain.p2.y) / glm::abs(terrain.p3.x-terrain.p2.x) );
        }  
        
        if(terrain.p1.z != terrain.p2.z){
            terrain.rotX = glm::atan( (terrain.p2.y - terrain.p1.y) / glm::abs(terrain.p2.z-terrain.p1.z) );
        }else{
            terrain.rotX = glm::atan( (terrain.p3.y - terrain.p2.y) / glm::abs(terrain.p3.z-terrain.p2.z) );
        }
    } 
}

//Calculates expected Y for the object given its position
float LAPAL::calculateExpectedY(const LAPAL::plane3f& terrain, const LAPAL::vec3f& position ){
    if(checkTerrain(terrain)){
        return terrain.p1.y;
    }else{
        //Scalars that determine the position Y of the object given the vectors needed to compose its position in X and Z
        float a = 0.f, b = 0.f;

        //Using auxiliar function to calculate them
        calculateTerrainAB(terrain, position, a, b);

        //Returns the Y value correspondent
        return a * (terrain.p3 - terrain.p1).y + b  * (terrain.p4 - terrain.p2).y + terrain.p1.y;
    }
}

//Corrects position of the object on Y
void LAPAL::correctYPosition(LAPAL::movementData& mData, const float dTime, LAPAL::plane3f& terrain, LAPAL::vec3f& position){
    float y = calculateExpectedY(terrain, position);
    
    if(!mData.asc){
        //check if we are not touching the ground
        if(position.y>y){
             
            //update vertical speed with gravity
            mData.velY += gravity*dTime*mData.velY*0.05;
        
            //update falling position
            position.y -= mData.velY* dTime;
            if(position.y < y)  //Correct position if we get under it
                position.y = y; 
        
            //if terrain is plane, reset gravity
            if(position.y == y)
                mData.velY = gravity;
        
            //if gravity is too high, cap gravity
            if(mData.velY > gravity * 20)
                mData.velY = gravity * 20;
        
        }else{
            position.y = y;
        }
    }
    mData.jump = false;
}

//--------------------------------------
//---------------MATHS------------------
//--------------------------------------

//Calculates values A and B which are the scalars that multiply vector 1 and 2 to compose the point C in 2D (X-Z plane) inside the terrain givenç
void LAPAL::calculateAB(const LAPAL::vec3f& vecC, const LAPAL::vec3f& vec1, const LAPAL::vec3f& vec2,  float& a, float& b){
    double a_x = vec2.x;
    double a_z = vec2.z;
    double b_x = vec1.x;
    double b_z = vec1.z;
    double c_x = vecC.x;
    double c_z = vecC.z;
    double aux_a = 0;
    double aux_b = 0;
    
 
    //Composing vectors a * vec1 +  b * vec2 = vecC
    if(b_x * a_z != b_z * a_x){
        aux_a = (c_z * a_x - c_x * a_z) / (b_z * a_x - b_x * a_z);
    }
    if(a_z != 0.0){
        aux_b = (c_z - aux_a * b_z) / a_z;
    }
    if(a_x != 0.0){
        aux_b = (c_x - aux_a * b_x) / a_x;
    }

    a = aux_a;
    b = aux_b;
}


//Calculates values A and B in given terrain, using Cross vectors (from p1 to p3 and from p2 to p4 OR down-left to up-right and up-left to down-right)
void LAPAL::calculateTerrainAB(const LAPAL::plane3f& terrain, const LAPAL::vec3f& position, float& a, float& b){

    //Vector that will compose the position inside the terrain
    glm::vec3 vec_a     =   terrain.p3 - terrain.p1;
    glm::vec3 vec_b     =   terrain.p4 - terrain.p2;
    glm::vec3 relativeP = position - terrain.p1;

    //Composing the scalars
    calculateAB(relativeP, vec_a, vec_b, a, b);
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

    if( abs(LAPAL::calculateExpectedY(terrain, position)-position.y)-radius > 1)
        return false;

    return true;

}