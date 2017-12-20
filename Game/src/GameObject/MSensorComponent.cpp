#include "MSensorComponent.h"
#include <iostream>

//Constructors
MSensorComponent::MSensorComponent(GameObject& newGameObject) : 
ISensorComponent(newGameObject)
{
    angleInitial=newGameObject.getTransformData().rotation.y * 3.141592653589f / 180.f;
    angleVision=5.0 * 3.141592653589f / 180.f;
    sensorLeft = glm::vec3(cos(angleVision+angleInitial), 0.f, sin(angleVision+angleInitial));
    sensorRight = glm::vec3(cos(-angleVision+angleInitial), 0.f, sin(-angleVision+angleInitial));
}       

MSensorComponent::MSensorComponent(GameObject& newGameObject, float angV, float angI) :
ISensorComponent(newGameObject)
{
    angleInitial=angI;
    angleVision=angV * 3.141592653589f / 180.f;

    sensorLeft = glm::vec3(cos(angleVision+angleInitial), 0.f, sin(angleVision+angleInitial));
    sensorRight = glm::vec3(cos(-angleVision+angleInitial), 0.f, sin(-angleVision+angleInitial));
}

//Checks possible collisions with terrain and returns VObjects collisions
void MSensorComponent::updateMapCollisions()
{
    //---------------
    //INITIALIZATIONS
    //---------------

    //initial variables                                                      //Counter
    glm::vec3 position = this->getGameObject().getTransformData().position;  //Our position
    VObject::Pointer pvo;                                                    //VPointer included in the end result
    float a_back = 0.f, b_back = 0.f, a_end = 0.f, b_end = 0.f;              //initial A and B of the back points, and A and B of front points

    //update sensors
    sensorLeft = glm::vec3(cos(angleVision+angleInitial), 0.f, sin(angleVision+angleInitial));
    sensorRight = glm::vec3(cos(-angleVision+angleInitial), 0.f, sin(-angleVision+angleInitial));

    /* //TESTING
        std::cout<<"AnglI: "<<angleInitial<<", AnglV: "<<angleVision<<"\n";
        std::cout<<"Left: "<<sensorLeft.x<<","<<sensorLeft.z<<"\n";
        std::cout<<"Right: "<<sensorRight.x<<","<<sensorRight.z<<"\n";
    */

    //Clear seen objects
    seenObjects.clear();

    //Get terrain info and movement info
    auto moving = PhysicsManager::getInstance().getMovingCharacter(this->getGameObject().getId());
    auto moveC = moving.moveComponent;
    auto terrC = moving.terrainComponent;
    auto terrain = terrC->getTerrain();

    //-------------------
    //TERRAIN BACK POINTS
    //-------------------
    glm::vec3 point1, point2;
    switch(terrain.direction){
        case 0:
            point1 = terrain.p1;
            point2 = terrain.p4;
            break;

        case 1:
            point1 = terrain.p2;
            point2 = terrain.p1;
            break;

        case 2:
            point1 = terrain.p3;
            point2 = terrain.p2;
            break;

        case 3:
            point1 = terrain.p4;
            point2 = terrain.p3;
            break;
    }
    
    //Calculate first point
    calculateAB(point1, &a_back, &b_back);                            //Do the math
    pvo = std::make_shared<VObject>(point1, a_back, b_back, 1.f, 1);  //generate VObject with the data
    seenObjects.push_back(pvo);                                       //Add to seen objects

    //Calculate second point
    calculateAB(point2, &a_back, &b_back);                            //Do the math
    pvo = std::make_shared<VObject>(point2, a_back, b_back, 1.f, 1);  //generate VObject with the data
    seenObjects.push_back(pvo);                                       //Add to seen objects

    //-------------------
    //FRONT POINTS CHECK
    //-------------------
    //update sensors
    sensorLeft = glm::vec3(cos(angleVision+angleInitial), 0.f, sin(angleVision+angleInitial));
    sensorRight = glm::vec3(cos(-angleVision+angleInitial), 0.f, sin(-angleVision+angleInitial));

    //Collide with walls, and propagate if no collision
    glm::vec3 pointS1, pointS2;                 //End points of collision of the sensors
    bool contactL = false, contactR = false;    //Sentinels checking contact of one of the rays

    //Front
    if(terrC->getNext() == nullptr){
        float a = 0.f, b = 0.f;
        glm::vec3 vAB  = terrain.p3 - terrain.p2; //Vector de los puntos superiores
        
        //WITH SENSOR LEFT
        //MegaCalculus = (position + a*sensorLeft) + (p2 + b* vAB) = X
        if(sensorLeft.z * vAB.x != sensorLeft.x * vAB.z){
            b = ( (position.x - terrain.p2.x) * vAB.z - (position.z - terrain.p2.z) * vAB.x ) / (sensorLeft.z * vAB.x - sensorLeft.x * vAB.z);
        }
        if(vAB.x != 0){
            a = (position.x + sensorLeft.x * b - terrain.p2.x) / vAB.x;
        }

        //The vector must be inside the line (a > 0 && a <= 1), and different from the object (b > 0)
        if(a >= 0.f && b > 0.f && a <= 1){
            a_end       = b;                      //a_end => a * sensorLeft till point of cross, which it is b in this calculus
            contactL    = true;
            pointS1     = terrain.p2 + a * vAB;
        }

        //WITH SENSOR RIGHT
        a = 0.f;
        b = 0.f;
        //MegaCalculus = (position + a*sensorLeft) + (p2 + b* vAB) = X
        if(sensorRight.z * vAB.x != sensorRight.x * vAB.z){
            b = ( (position.x - terrain.p2.x) * vAB.z - (position.z - terrain.p2.z) * vAB.x ) / (sensorRight.z * vAB.x - sensorRight.x * vAB.z);
        }
        if(vAB.x != 0.f){
            a = (position.x + sensorRight.x * b - terrain.p2.x) / vAB.x;
        }

        //The vector must be inside the line (a > 0 && a <= 1), and different from the object (b > 0)
        if(a >= 0.f && b > 0.f && a <= 1.f){
            b_end       = b;                      //b_end => a * sensorRight till point of cross, which it is b in this calculus
            contactR    = true;
            pointS2     = terrain.p2 + a * vAB;
        }
    }

    //Right
    if(terrC->getRight() == nullptr){
        float a = 0.f, b = 0.f;
        glm::vec3 vAB = terrain.p4 - terrain.p3; //Vector de los puntos de la derecha
        
        //Check if the left sensor already collided before
        if(!contactL){
            //WITH SENSOR LEFT
            //MegaCalculus = (position + a*sensorLeft) + (p3 + b* vAB) = X
            if(sensorLeft.z * vAB.x != sensorLeft.x * vAB.z){
                b = ( (position.x - terrain.p3.x) * vAB.z - (position.z - terrain.p3.z) * vAB.x ) / (sensorLeft.z * vAB.x - sensorLeft.x * vAB.z);
            }
            if(vAB.x != 0){
                a = (position.x + sensorLeft.x * b - terrain.p3.x) / vAB.x;
            }

            //The vector must be inside the line (a > 0 && a <= 1), and different from the object (b > 0)
            if(a >= 0.f && b > 0.f && a <= 1){
                a_end       = b;                      //a_end => a * sensorLeft till point of cross, which it is b in this calculus
                contactL    = true;
                pointS1     = terrain.p3 + a * vAB;
            }
        }

        //Check if the right sensor already collided before
        if(!contactR){
            //WITH SENSOR RIGHT
            a = 0.f;
            b = 0.f;
            //MegaCalculus = (position + a*sensorLeft) + (p3 + b* vAB) = X
            if(sensorRight.z * vAB.x != sensorRight.x * vAB.z){
                b = ( (position.x - terrain.p3.x) * vAB.z - (position.z - terrain.p3.z) * vAB.x ) / (sensorRight.z * vAB.x - sensorRight.x * vAB.z);
            }
            if(vAB.x != 0.f){
                a = (position.x + sensorRight.x * b - terrain.p3.x) / vAB.x;
            }

            //The vector must be inside the line (a > 0 && a <= 1), and different from the object (b > 0)
            if(a >= 0.f && b > 0.f && a <= 1.f){
                b_end       = b;                      //b_end => a * sensorRight till point of cross, which it is b in this calculus
                contactR    = true;
                pointS2     = terrain.p3 + a * vAB;
            }
        }
    }

    //Down
    if(terrC->getPrev() == nullptr){

        float a = 0.f, b = 0.f;
        glm::vec3 vAB = terrain.p1 - terrain.p4; //Vector de los puntos de abajo
        
        //Check if the left sensor already collided before
        if(!contactL){
            //WITH SENSOR LEFT
            //MegaCalculus = (position + a*sensorLeft) + (p4 + b* vAB) = X
            if(sensorLeft.z * vAB.x != sensorLeft.x * vAB.z){
                b = ( (position.x - terrain.p4.x) * vAB.z - (position.z - terrain.p4.z) * vAB.x ) / (sensorLeft.z * vAB.x - sensorLeft.x * vAB.z);
            }
            if(vAB.x != 0){
                a = (position.x + sensorLeft.x * b - terrain.p4.x) / vAB.x;
            }

            ///_______________DOLOR DE TEST__________________
            std::cout<<"DOLORES. A: "<<a<<" , B: "<<b<<std::endl;
            ///______________________________________________

            //The vector must be inside the line (a > 0 && a <= 1), and different from the object (b > 0)
            if(a >= 0.f && b > 0.f && a <= 1.f){
                a_end       = b;                      //a_end => a * sensorLeft till point of cross, which it is b in this calculus
                contactL    = true;
                pointS1     = terrain.p4 + a * vAB;
            }
        }

        //Check if the right sensor already collided before
        if(!contactR){
            //WITH SENSOR RIGHT
            a = 0.f;
            b = 0.f;
            //MegaCalculus = (position + a*sensorLeft) + (p4 + b* vAB) = X
            if(sensorRight.z * vAB.x != sensorRight.x * vAB.z){
                b = ( (position.x - terrain.p4.x) * vAB.z - (position.z - terrain.p4.z) * vAB.x ) / (sensorRight.z * vAB.x - sensorRight.x * vAB.z);
            }
            if(vAB.x != 0.f){
                a = (position.x + sensorRight.x * b - terrain.p4.x) / vAB.x;
            }

            ///_______________DOLOR DE TEST__________________
            std::cout<<"DOLORES. A: "<<a<<" , B: "<<b<<std::endl;
            ///______________________________________________

            //The vector must be inside the line (a > 0 && a <= 1), and different from the object (b > 0)
            if(a >= 0.f && b > 0.f && a <= 1.f){
                b_end       = b;                      //b_end => a * sensorRight till point of cross, which it is b in this calculus
                contactR    = true;
                pointS2     = terrain.p4 + a * vAB;
            }
        }
    }

    //Left
    if(terrC->getLeft() == nullptr){
        float a = 0.f, b = 0.f;
        glm::vec3 vAB = terrain.p2 - terrain.p1; //Vector de los puntos de la izquierda
        
        //Check if the left sensor already collided before
        if(!contactL){
            //WITH SENSOR LEFT
            //MegaCalculus = (position + a*sensorLeft) + (p1 + b* vAB) = X
            if(sensorLeft.z * vAB.x != sensorLeft.x * vAB.z){
                b = ( (position.x - terrain.p1.x) * vAB.z - (position.z - terrain.p1.z) * vAB.x ) / (sensorLeft.z * vAB.x - sensorLeft.x * vAB.z);
            }
            if(vAB.x != 0){
                a = (position.x + sensorLeft.x * b - terrain.p1.x) / vAB.x;
            }

            //The vector must be inside the line (a > 0 && a <= 1), and different from the object (b > 0)
            if(a >= 0.f && b > 0.f && a <= 1){
                a_end       = b;                      //a_end => a * sensorLeft till point of cross, which it is b in this calculus
                contactL    = true;
                pointS1     = terrain.p1 + a * vAB;
            }
        }

        //Check if the right sensor already collided before
        if(!contactR){
            //WITH SENSOR RIGHT
            a = 0.f;
            b = 0.f;
            //MegaCalculus = (position + a*sensorLeft) + (p1 + b* vAB) = X
            if(sensorRight.z * vAB.x != sensorRight.x * vAB.z){
                b = ( (position.x - terrain.p1.x) * vAB.z - (position.z - terrain.p1.z) * vAB.x ) / (sensorRight.z * vAB.x - sensorRight.x * vAB.z);
            }
            if(vAB.x != 0.f){
                a = (position.x + sensorRight.x * b - terrain.p1.x) / vAB.x;
            }

            //The vector must be inside the line (a > 0 && a <= 1), and different from the object (b > 0)
            if(a >= 0.f && b > 0.f && a <= 1.f){
                b_end       = b;                      //b_end => a * sensorRight till point of cross, which it is b in this calculus
                contactR    = true;
                pointS2     = terrain.p1 + a * vAB;
            }
        }
    }

    if(contactL){   //if left sensor hit the wall
        //Calculate left point
        pvo = std::make_shared<VObject>(pointS1, a_end, 0.f, 1.f, 1);    //generate VObject with the data
        seenObjects.push_back(pvo);                                      //Add to seen objects
    }


    ///_______________DOLOR DE TEST__________________
    std::cout<<"DOLOR DE CORRECCIÓN. PUNTO 1: "<<pointS1.x<<","<<pointS1.z<<" --- "<<a_end<<std::endl;
    ///______________________________________________


    if(contactR){
        //Calculate right point
        pvo = std::make_shared<VObject>(pointS2, 0.f, b_end, 1.f, 1);    //generate VObject with the data
        seenObjects.push_back(pvo);                                      //Add to seen objects
    }


    ///_______________DOLOR DE TEST__________________
    std::cout<<"DOLOR DE CORRECCIÓN. PUNTO 2: "<<pointS2.x<<","<<pointS2.z<<" --- "<<b_end<<std::endl;
    ///______________________________________________


}

//Auxiliar function to calculate A and B of given objective
void MSensorComponent::calculateAB(glm::vec3& objective, float* a, float* b){
    *a = 0.f;
    *b = 0.f;

    glm::vec3 relativeP;

    //Do the math
    relativeP = objective - this->getGameObject().getTransformData().position;
    if(sensorRight.x*sensorLeft.z != sensorRight.z*sensorLeft.x) 
        *b = (relativeP.z * sensorLeft.x - relativeP.x*sensorLeft.z) /(sensorRight.z * sensorLeft.x - sensorRight.x*sensorLeft.z);
    if(sensorLeft.x != 0)
        *a = (relativeP.z - (*b) * sensorRight.z) / sensorLeft.z;
}

//Auxiliar function to calculate the point of collision with a vector given one of the sensors
//PARAMS: Terrain point 1, terrain point 2, is left = true / is right = false, *pointer boolean if the sensor is touching a surface
/*glm::vec3 MSensorComponent::calculateSensorCollision(glm::vec3& t1, glm::vec3& t2, bool isLeft, bool* contact){
        terrain.p3. terrain.p2, L o R, *contact, return glm ve3¡c3
}*/