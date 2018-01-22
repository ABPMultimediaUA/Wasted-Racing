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
    float a_back = 0.f, b_back = 0.f;              //initial A and B of the back points, and A and B of front points

    //update sensors
    sensorLeft = glm::vec3(cos(angleVision+angleInitial), 0.f, sin(angleVision+angleInitial));
    sensorRight = glm::vec3(cos(-angleVision+angleInitial), 0.f, sin(-angleVision+angleInitial));


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
    calculateABTerrainBack(point1, a_back, b_back);                            //Do the math
    pvo = std::make_shared<VObject>(point1, a_back, b_back, 1.f, 1, 1.f);  //generate VObject with the data
    seenObjects.push_back(pvo);                                       //Add to seen objects

    //Calculate second point
    calculateABTerrainBack(point2, a_back, b_back);                            //Do the math
    pvo = std::make_shared<VObject>(point2, a_back, b_back, 1.f, 1, 1.f);  //generate VObject with the data
    seenObjects.push_back(pvo);                                       //Add to seen objects

    //-------------------
    //FRONT POINTS CHECK
    //-------------------
    //Collide with walls, and propagate if no collision
    glm::vec3 pointS1, pointS2;                 //End points of collision of the sensors
    bool contactL = false, contactR = false;    //Sentinels checking contact of one of the rays

    //Front
    if(terrC->getNext() == nullptr){
        float a = 0.f, b = 0.f;
        glm::vec3 vAB           = - (terrain.p3 - terrain.p2);  //Vector de los puntos superiores
        glm::vec3 relativePoint = terrain.p2 - position;        //Vector from position to p2

        //MegaCalculus = (position + a*sensorLeft) - (p2 + b* vAB) = 0 -> position + a*sensorLeft - p2 - b* vAB = 0 
                        // -> a * sensorLeft + b * (-vAB) = p2 - position;
        
        //WITH SENSOR LEFT
        LAPAL::calculateAB(relativePoint, sensorLeft, vAB,  a, b);

        //The vector must be inside the line (a > 0 && a <= 1), and different from the object (b > 0)
        if(a > 0.f && b >= 0.f && b <= 1.f){
            contactL    = true;
            pointS1     = terrain.p2 - b * vAB;
        }

        //WITH SENSOR RIGHT
        LAPAL::calculateAB(relativePoint, sensorRight, vAB,  a, b);

        //The vector must be inside the line (a > 0 && a <= 1), and different from the object (b > 0)
        if(a > 0.f && b >= 0.f && b <= 1.f){
            contactR    = true;
            pointS2     = terrain.p2 - b * vAB;
        }
    }

    //Right
    if(terrC->getRight() == nullptr){
        float a = 0.f, b = 0.f;
        glm::vec3 vAB           = - (terrain.p4 - terrain.p3);  //Vector de los puntos de la derecha
        glm::vec3 relativePoint = terrain.p3 - position;        //Vector from position to p3

        //Check if the left sensor already collided before
        if(!contactL){
            //WITH SENSOR LEFT
            LAPAL::calculateAB(relativePoint, sensorLeft, vAB, a, b);

            //The vector must be inside the line (a > 0 && a <= 1), and different from the object (b > 0)
            if(a > 0.f && b >= 0.f && b <= 1){
                contactL    = true;
                pointS1     = terrain.p3 - b * vAB;
            }
        }

        //Check if the right sensor already collided before
        if(!contactR){
            //WITH SENSOR RIGHT
            LAPAL::calculateAB(relativePoint, sensorRight, vAB,  a, b);

            //The vector must be inside the line (a > 0 && a <= 1), and different from the object (b > 0)
            if(a > 0.f && b >= 0.f && b <= 1.f){
                contactR    = true;
                pointS2     = terrain.p3 - b * vAB;
            }
        }
    }

    //Down
    if(terrC->getPrev() == nullptr){

        float a = 0.f, b = 0.f;
        glm::vec3 vAB           = - (terrain.p1 - terrain.p4);  //Vector de los puntos de abajo
        glm::vec3 relativePoint = terrain.p4 - position;        //Vector from position to p4

        //Check if the left sensor already collided before
        if(!contactL){
            //WITH SENSOR LEFT
            LAPAL::calculateAB(relativePoint, sensorLeft, vAB,  a, b);

            //The vector must be inside the line (a > 0 && a <= 1), and different from the object (b > 0)
            if(a > 0.f && b >= 0.f && b <= 1.f){
                contactL    = true;
                pointS1     = terrain.p4 - b * vAB;
            }
        }

        //Check if the right sensor already collided before
        if(!contactR){
            //WITH SENSOR RIGHT
            LAPAL::calculateAB(relativePoint, sensorRight, vAB,  a, b);

            //The vector must be inside the line (a > 0 && a <= 1), and different from the object (b > 0)
            if(a > 0.f && b >= 0.f && b <= 1.f){
                contactR    = true;
                pointS2     = terrain.p4 - b * vAB;
            }
        }
    }

    //Left
    if(terrC->getLeft() == nullptr){
        float a = 0.f, b = 0.f;
        glm::vec3 vAB           = -(terrain.p2 - terrain.p1);   //Vector de los puntos de la izquierda
        glm::vec3 relativePoint = terrain.p1 - position;        //Vector from position to p4

        //Check if the left sensor already collided before
        if(!contactL){
            //WITH SENSOR LEFT
            LAPAL::calculateAB(relativePoint, sensorLeft, vAB,  a, b);

            //The vector must be inside the line (a > 0 && a <= 1), and different from the object (b > 0)
            if(a > 0.f && b >= 0.f && a <= 1){
                contactL    = true;
                pointS1     = terrain.p1 - b * vAB;
            }
        }

        //Check if the right sensor already collided before
        if(!contactR){
            //WITH SENSOR RIGHT
            LAPAL::calculateAB(relativePoint, sensorRight, vAB,  a, b);

            //The vector must be inside the line (a > 0 && a <= 1), and different from the object (b > 0)
            if(a > 0.f && b >= 0.f && a <= 1.f){
                contactR    = true;
                pointS2     = terrain.p1 - b * vAB;
            }
        }
    }

    if(contactL){   //if left sensor hit the wall
        //Calculate A and B with the sensor of VSensor
        float a_final = 0.f, b_final = 0.f;
        auto vsensor = this->getGameObject().getComponent<VSensorComponent>().get();
        LAPAL::calculateAB(pointS1 - position, vsensor->getSensorLeft(), vsensor->getSensorRight(), a_final, b_final);

        //Calculate left point
        pvo = std::make_shared<VObject>(pointS1, a_final, b_final, 1.f, 1, 1.f);    //generate VObject with the data
        seenObjects.push_back(pvo);                                      //Add to seen objects
    }


    if(contactR){
        //Calculate A and B with the sensors of VSensor
        float a_final2 = 0.f, b_final2 = 0.f;
        auto vsensor = this->getGameObject().getComponent<VSensorComponent>().get();
        LAPAL::calculateAB(pointS2 - position, vsensor->getSensorLeft(), vsensor->getSensorRight(), a_final2, b_final2);

        //Calculate right point
        pvo = std::make_shared<VObject>(pointS2, a_final2, b_final2, 1.f, 1, 1.f);    //generate VObject with the data
        seenObjects.push_back(pvo);                                      //Add to seen objects
    }



}

//Auxiliar function to calculate A and B of given objective
void MSensorComponent::calculateABTerrainBack(glm::vec3& objective, float& a, float& b){
    //relative point to us
    glm::vec3 relativeP = objective - this->getGameObject().getTransformData().position;

    //get vectors of vsensor for standarization
    auto vsensor = this->getGameObject().getComponent<VSensorComponent>().get();

    //calculation in physics of point A and B
    LAPAL::calculateAB(relativeP, vsensor->getSensorLeft(), vsensor->getSensorRight(), a, b);

}

//Auxiliar function to calculate the point of collision with a vector given one of the sensors
//PARAMS: Terrain point 1, terrain point 2, is left = true / is right = false, *pointer boolean if the sensor is touching a surface
/*glm::vec3 MSensorComponent::calculateSensorCollision(glm::vec3& t1, glm::vec3& t2, bool isLeft, bool* contact){
        terrain.p3. terrain.p2, L o R, *contact, return glm ve3¡c3
}*/