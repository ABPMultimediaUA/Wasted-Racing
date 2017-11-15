#include <iostream>
#include <LAPALPhysics.h>

int main(){

    std::cout << "Warming up..." << std::endl;
    std::cout << std::endl;

    LAPAL::movementData mData = {0, 50, 0, 5, 1};

    std::cout << "Linear Velocity Test 0" << std::endl;
    for(int i=0; i<15; i++){
        LAPAL::updateLinearVelocity(mData, 1);

        std::cout << mData.vel << " " << mData.acc << std::endl;
    }
    std::cout << std::endl;

    LAPAL::movementData mData1 = {0, 50, 0, 5, -1};

    std::cout << "Linear Velocity Test 1" << std::endl;
    for(int i=0; i<15; i++){
        LAPAL::updateLinearVelocity(mData1, 1);

        std::cout << mData1.vel << " " << mData1.acc << std::endl;
    }
    std::cout << std::endl;

    LAPAL::movementData mData2 = {0, 50, 0, 5, 1};

    std::cout << "Linear Velocity Test 2" << std::endl;
    for(int i=0; i<25; i++){

        //impulse changes from 1 to -1 at 7
        if(i==7)
            mData2.imp = -1;

        LAPAL::updateLinearVelocity(mData2, 1);

        std::cout << mData2.vel << " " << mData2.acc << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Circle Collision Test 0" << std::endl;
    LAPAL::vec3f obj1 = {0,0,0};
    LAPAL::vec3f obj2 = {0,0,1};
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


    


    return 0;
}