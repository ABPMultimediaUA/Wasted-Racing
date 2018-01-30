#include "CameraRenderComponent.h"
#include "../../GameManager/RenderManager.h"
#include <iostream>

class MoveComponent;
class TerrainComponent;

//Initilizer
void CameraRenderComponent::init() {
    RenderManager::getInstance().getRenderFacade()->setCameraTarget(gameObject);
}

//Update   
void CameraRenderComponent::update(float dTime) {

    //Camera velocity
    const float camVel = 30;

    //Set oldDistance to currentDistance
    oldDistance = distance;
 
    //Get data for current position
    LAPAL::vec3f pos = getGameObject().getTransformData().position;
    float angleY = getGameObject().getTransformData().rotation.y;

    //Get current camera position
    LAPAL::vec3f currentPosition = LAPAL::vec3f(pos.x - distance * cos(angleY), pos.y + distance * 0.4, pos.z + distance * sin(angleY));

    //Data for next position
    auto moveCMP = getGameObject().getComponent<MoveComponent>();


    if(moveCMP != nullptr)
    {
        auto move = moveCMP.get()->getMovemententData();
        float radius = getGameObject().getComponent<CollisionComponent>().get()->getRadius();

        if(move.spin < 0)
            spinDir = 1;
        else if (move.spin > 0)
            spinDir = -1;

        //Get next camera position
        angleY += move.max_spin * 5 * spinDir;
        
        LAPAL::vec3f nextPlayerPos = LAPAL::vec3f( pos + move.velocity / move.vel * radius);
        if(move.vel == 0)
            nextPlayerPos = getGameObject().getTransformData().position;
        LAPAL::vec3f nextPosition = LAPAL::vec3f(nextPlayerPos.x - distance * cos(angleY), nextPlayerPos.y + distance * 0.4, nextPlayerPos.z + distance * sin(angleY));

        //====================================================================================================
        // CHECK COLLISION WITH TERRAIN
        //====================================================================================================
        auto terr = terrain.get()->getTerrain();

        //Check if we are out of front bounds
        if(!LAPAL::position2DLinePoint(terr.p1, terr.p2, nextPosition)){
            if( terrain.get()->getNext() == nullptr ) {   //If there isn't next plane, collision
                distance -= camVel * dTime;
                count = 0;
            }
            //Check if our center is still in the same terrain
            else if(!LAPAL::position2DLinePoint(terr.p1, terr.p2, currentPosition)){    //If not, change to next terrain
                terrain = terrain.get()->getNext();
            }
        }

        //Check if we are out of right bounds
        if(!LAPAL::position2DLinePoint(terr.p2, terr.p3, nextPosition)){
            if( terrain.get()->getRight() == nullptr ) {   //If there isn't next plane, collision
                distance -= camVel * dTime;
                count = 0;
            }
            //Check if our center is still in the same terrain
            else if(!LAPAL::position2DLinePoint(terr.p2, terr.p3, currentPosition)){    //If not, change to next terrain
                terrain = terrain.get()->getRight();
            }
        }
        
        //Check if we are out of back bounds
        if(!LAPAL::position2DLinePoint(terr.p3, terr.p4, nextPosition)){
            if( terrain.get()->getPrev() == nullptr ) {   //If there isn't next plane, collision
                distance -= camVel * dTime;
                count = 0;
            }
            //Check if our center is still in the same terrain
            else if(!LAPAL::position2DLinePoint(terr.p3, terr.p4, currentPosition)){  //If not, change to next terrain
                terrain = terrain.get()->getPrev();
            }
        }

        //Check if we are out of left bounds
        if(!LAPAL::position2DLinePoint(terr.p4, terr.p1, nextPosition)){
        if( terrain.get()->getLeft() == nullptr ) {   //If there isn't next plane, collision
                distance -= camVel * dTime;
                count = 0;
            }
            //Check if our center is still in the same terrain
            else if(!LAPAL::position2DLinePoint(terr.p4, terr.p1, currentPosition)){    //If not, change to next terrain
                terrain = terrain.get()->getLeft();
            }
        }

        ++count;

        if(count > 5 && distance < 30)
            distance += camVel * dTime / 2;

        if (distance < 5)
            distance = 5;

        if (distance > 30)
            distance = 30;
        
        if (move.spin == 0)
            distance = oldDistance;
    }

}

//Closer
void CameraRenderComponent::close() {

}

//Drawer
void CameraRenderComponent::draw() {
       
} 