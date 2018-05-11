#include "CameraRenderComponent.h"
#include "../../GameManager/PhysicsManager.h"

CameraRenderComponent::CameraRenderComponent(GameObject& newGameObject) : IRenderComponent(newGameObject) {
		distance = 20;
        minDistanceCP = 20;
		maxDistance = 40;
		oldDistance = distance;
		terrain = PhysicsManager::getInstance().getTerrainFromPos(newGameObject.getTransformData().position);
		count = 0;
		spinDir = 1;
        cameraMaxVel = this->getGameObject().getComponent<MoveComponent>()->getMovemententData().max_vel;
}

//Initilizer
void CameraRenderComponent::init() {

}

//Update   
void CameraRenderComponent::update(float dTime) {

    //Actual velocity
    float vel = this->getGameObject().getComponent<MoveComponent>()->getMovemententData().vel;

    //calculate distance camera-player
    float sumDistanceCP = (maxDistance - minDistanceCP) * (vel/cameraMaxVel);

    //Camera velocity
    const float camVel = 8;

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
        
        if(count > 5 && distance < sumDistanceCP + minDistanceCP)
            distance += camVel * dTime / 2;

        if (distance < 15)
            distance = 15;

        if (distance > sumDistanceCP + minDistanceCP){
            if(vel <= 1)
            {
                distance = distance-1;
                if(distance < minDistanceCP)
                {
                    distance = minDistanceCP;
                }
            }
            else
            {
                distance = sumDistanceCP + minDistanceCP;
            }
            
        }
        //if (move.spin == 0)
            //distance = oldDistance;
        std::cout << distance << std::endl;
    }

    //Update camera
    for(int i = 4; i > 0; i--) {
        oldHeight[i] = oldHeight[i-1];
        height[i] = height[i-1];
    }
    
    oldHeight[0] = height[0];
    height[0] = LAPAL::calculateExpectedY(terrain.get()->getTerrain(), currentPosition);
    
    moldHeight = (oldHeight[0] + oldHeight[1] + oldHeight[2] + oldHeight[3] + oldHeight[4])/5;
    mheight = (height[0] + height[1] + height[2] + height[3] + height[4])/5;

}

//Closer
void CameraRenderComponent::close() {

}

//Drawer
void CameraRenderComponent::draw() {
       
} 