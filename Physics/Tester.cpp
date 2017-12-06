#include <iostream>
#include <LAPALPhysics.h>

int main(){

    std::cout << "Warming up..." << std::endl;
    std::cout << std::endl;

    float mass = 1;

    LAPAL::movementData mData = {0, 50, 0, 5, 1};

  /*  std::cout << "Linear Velocity Test 0" << std::endl;
    for(int i=0; i<15; i++){
        LAPAL::updateLinearVelocity(mData, 1, mass);

        std::cout << mData.vel << " " << mData.acc << std::endl;
    }
    std::cout << std::endl; 

    LAPAL::movementData mData1 = {0, 50, 0, 5, -1};

    std::cout << "Linear Velocity Test 1" << std::endl;
    for(int i=0; i<15; i++){
        LAPAL::updateLinearVelocity(mData1, 1, mass);

        std::cout << mData1.vel << " " << mData1.acc << std::endl;
    }
    std::cout << std::endl;

    LAPAL::movementData mData2 = {0, 50, 0, 5, 1};

    std::cout << "Linear Velocity Test 2" << std::endl;
    for(int i=0; i<25; i++){
 
        //impulse changes from 1 to -1 at 7
        if(i==7)
            mData2.imp = -1;

        LAPAL::updateLinearVelocity(mData2, 1, mass);

        std::cout << mData2.vel << " " << mData2.acc << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Circle Collision Test 0" << std::endl;
    LAPAL::vec3f obj1(0,0,0);
    LAPAL::vec3f obj2(0,0,1);
    float r1 = 0.5;
    float r2 = 0.25;

    std::cout << LAPAL::checkCircleCircleCollision(obj1, r1, obj2, r2) << std::endl; //should be false
    r2 = 0.5;
    std::cout << LAPAL::checkCircleCircleCollision(obj1, r1, obj2, r2) << std::endl; //should be false
    r2 = 0.51;
    std::cout << LAPAL::checkCircleCircleCollision(obj1, r1, obj2, r2) << std::endl; //should be true
    obj2.z = -1; 
    std::cout << LAPAL::checkCircleCircleCollision(obj1, r1, obj2, r2) << std::endl; //should be true 
    obj2.x = -1;
    std::cout << LAPAL::checkCircleCircleCollision(obj1, r1, obj2, r2) << std::endl; //should be false
    r1 = 0.91;
    std::cout << LAPAL::checkCircleCircleCollision(obj1, r1, obj2, r2) << std::endl; //should be true
    std::cout << std::endl;

    std::cout << "Spin and Angle Test 0" << std::endl;
    LAPAL::movementData mData3 = {0, 5, 0, 1, 0.01};
    mData3.spin_inc = 0.01; mData3.max_spin = 1; mData3.angle = 0;

    for(int i=0; i<30; i++){
 
        //impulse changes from 1 to -1 at 15
        if(i==15)
            mData3.spin_inc = -0.1;

        LAPAL::updateLinearVelocity(mData3, 1, mass);
        LAPAL::updateSpin(mData3, 1);
        LAPAL::update2DVelocity(mData3);

        std::cout << mData3.angle << " " << mData3.spin << " | " << mData3.vel3d.x << " " << mData3.vel3d.z << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Elastic Collision Test 0" << std::endl;
    LAPAL::vec3f vel1(1, 0, 1);
    LAPAL::vec3f vel2(0, 0, 0);
    float mass1 = 0.1;
    float mass2 = 1000;
    LAPAL::calculateElasticCollision(vel1, mass1, vel2, mass2);
    std::cout << vel1.x << " " << vel1.z << std::endl;
    vel1.z = 0; 
    LAPAL::calculateElasticCollision(vel1, mass1, vel2, mass2);
    std::cout << vel1.x << " " << vel1.z << std::endl;
    mass2 = 1; vel1.z = 3; vel1.x = 3;
    LAPAL::calculateElasticCollision(vel1, mass1, vel2, mass2);
    std::cout << vel1.x << " " << vel1.z << std::endl;             */
    
    std::cout << "3D PHYSICS TEST" << std::endl;
    std::cout << "Normal of the plane test" << std::endl;

    LAPAL::plane3f terrain;
    LAPAL::vec3f point1(0, 5, 0);
    LAPAL::vec3f point2(5, 5, 0);
    LAPAL::vec3f point3(5, 5, 5);
    LAPAL::vec3f point4(0, 5, 5);
    
    terrain.p1 = point1;
    terrain.p2 = point2;
    terrain.p3 = point3;
    terrain.p4 = point4;

    LAPAL::plane3f terrain2;
    LAPAL::vec3f point12(0, 5, 0);
    LAPAL::vec3f point22(5, 5, 0);
    LAPAL::vec3f point32(5, 7, 5);
    LAPAL::vec3f point42(0, 7, 5);
    
    terrain2.p1 = point12;
    terrain2.p2 = point22;
    terrain2.p3 = point32;
    terrain2.p4 = point42;

    LAPAL::vec3f normal = LAPAL::calculateNormal(terrain);
    std::cout << "Normal:" << std::endl;
    std::cout << normal.x << std::endl;
    std::cout << normal.y << std::endl;
    std::cout << normal.z << std::endl;

    std::cout << "Check terrain collision test" << std::endl;

    LAPAL::vec3f obj1(2, 5, 2);
    LAPAL::vec3f obj2(2, 50, 2);
    bool coll;
    coll = LAPAL::checkTerrainCollision(terrain, obj1);
    std::cout << coll << std::endl;
    coll = LAPAL::checkTerrainCollision(terrain, obj2);
    std::cout << coll << std::endl;

    std::cout << "Correct terrain collision test" << std::endl;
    LAPAL::correctTerrainCollision(terrain2, obj2);
    std::cout << obj2.x << std::endl;
    std::cout << obj2.y << std::endl;
    std::cout << obj2.z << std::endl;

    std::cout << "Check terrain test" << std::endl;

    bool terr;
    terr = LAPAL::checkTerrain(terrain);
    std::cout << terr << std::endl;
    terr = LAPAL::checkTerrain(terrain2);
    std::cout << terr << std::endl;

    std::cout << "Update friction force test" << std::endl;

    terrain.fric= 0.2;
    LAPAL::updateFrictionForce(mData, terrain, mass, 9.8, obj1);
    std::cout << mData.frictionForce.x << std::endl;
    std::cout << mData.frictionForce.y << std::endl;
    std::cout << mData.frictionForce.z << std::endl;


   

    return 0;
}