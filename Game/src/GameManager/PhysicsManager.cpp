#include "PhysicsManager.h"
#include "../GameObject/GameObject.h"
#include "../GameEvent/EventManager.h"
#include <memory>
#include <iostream>

//==============================================
// DELEGATES DECLARATIONS
//==============================================
void addMoveComponent(EventData eData); 
void addCollisionComponent(EventData eData); 

//==============================================
// PHYSICS MANAGER FUNCTIONS
//============================================== 
PhysicsManager& PhysicsManager::getInstance() {
    static PhysicsManager instance;
    return instance;
}

void PhysicsManager::init() {

    //Bind listeners
    EventManager::getInstance().addListener(EventListener {EventType::MoveComponent_Create, addMoveComponent});
    EventManager::getInstance().addListener(EventListener {EventType::CollisionComponent_Create, addCollisionComponent});

}

void PhysicsManager::update(const float dTime) {

    for(unsigned int i=0; i<movingCharacterList.size(); ++i){

        //Get components in variables
        auto ourMove = movingCharacterList.at(i).moveComponent.get();
        auto ourTerr = movingCharacterList.at(i).terrainComponent.get();
        auto ourColl = movingCharacterList.at(i).collisionComponent.get();

        //==============================================================================
        // Move character
        //==============================================================================
        ourMove->update(dTime);
        
        //==============================================================================
        // Check collisions with character
        //==============================================================================
        for(unsigned int j=0; j<collisionComponentList.size(); ++j) {
            CollisionComponent* hisColl = std::dynamic_pointer_cast<CollisionComponent>(collisionComponentList.at(j)).get();
            if( hisColl != ourColl ) { //If the collider is different to the one of ourselves

                bool collision = LAPAL::checkCircleCircleCollision(  ourColl->getGameObject().getTransformData().position, ourColl->getRadius(), 
                                                    hisColl->getGameObject().getTransformData().position, hisColl->getRadius());

                if(collision){

                    //Get other object move component
                    auto hisMove = hisColl->getGameObject().getComponent<MoveComponent>();

                    if(hisMove == nullptr) {    //If the object doesn't have move component, it's static
                        
                        calculateStaticCollision(ourMove);

                    }
                    else {  //The object is not static
                        //***** CODE FOR COLLISIONS WHERE BOTH OBJECTS ARE MOVING *****//
                    }
                }
            }
        }

        //==============================================================================
        // Check collisions with terrain
        //==============================================================================
        auto terrain    = ourMove->getTerrain();
        auto ourMData   = ourMove->getGameObject().getTransformData();
        float radius    = ourColl->getRadius();
        float distance  = 0;

        //Check if we are out of front bounds
        distance = LAPAL::distance2DLinePoint(terrain.p1, terrain.p2, ourMData.position);
        if( distance-radius < 0 ) {
            if( ourTerr->getNext() == nullptr ) {   //If there isn't next plane, collision
                calculateStaticCollision(ourMove);
            }
            else if (distance < 0 ) {
                ourMove->setTerrain(ourTerr->getNext()->getTerrain()); //Set new terrain
                movingCharacterList.at(i).terrainComponent = std::shared_ptr<TerrainComponent>(ourTerr->getNext()); //Set new terrain component
            }
        }
        //Check if we are out of right bounds
        distance = LAPAL::distance2DLinePoint(terrain.p2, terrain.p3, ourMData.position);
        if( distance-radius < 0 ) {
            if( ourTerr->getRight() == nullptr ) {   //If there isn't next plane, collision
                calculateStaticCollision(ourMove);
            }
            else if (distance < 0 ) {
                ourMove->setTerrain(ourTerr->getRight()->getTerrain()); //Set new terrain
                movingCharacterList.at(i).terrainComponent = std::shared_ptr<TerrainComponent>(ourTerr->getRight()); //Set new terrain component
            }
        }
        //Check if we are out of back bounds
        distance = LAPAL::distance2DLinePoint(terrain.p3, terrain.p4, ourMData.position);
        if( distance-radius < 0 ) {
            if( ourTerr->getPrev() == nullptr ) {   //If there isn't next plane, collision
                calculateStaticCollision(ourMove);
            }
            else if (distance < 0 ) {
                ourMove->setTerrain(ourTerr->getPrev()->getTerrain()); //Set new terrain
                movingCharacterList.at(i).terrainComponent = std::shared_ptr<TerrainComponent>(ourTerr->getPrev()); //Set new terrain component
            }
        }
        //Check if we are out of left bounds
        distance = LAPAL::distance2DLinePoint(terrain.p4, terrain.p1, ourMData.position);
        if( distance-radius < 0 ) {
            if( ourTerr->getLeft() == nullptr ) {   //If there isn't next plane, collision
                calculateStaticCollision(ourMove);
            }
            else if (distance < 0 ) {
                ourMove->setTerrain(ourTerr->getLeft()->getTerrain()); //Set new terrain
                movingCharacterList.at(i).terrainComponent = std::shared_ptr<TerrainComponent>(ourTerr->getLeft()); //Set new terrain component
            }
        }
    }
}

void PhysicsManager::calculateStaticCollision(MoveComponent* ourMove) {

    float ourMass = ourMove->getMass();
    float hisMass = 2;

    auto ourMData = ourMove->getMovemententData();

    LAPAL::vec3f ourVel = ourMData.vel2d;
    LAPAL::vec3f hisVel = glm::vec3(0,0,0);

    //Calculate new velocity after collision
    LAPAL::calculateElasticCollision(ourVel, ourMass, hisVel, hisMass);
    ourMData.vel2d = ourVel;

    //Calculate new velocity module
    float newVel    = -sqrt(ourVel.x*ourVel.x + ourVel.z*ourVel.z);
    ourMData.vel    = newVel;

    //Set new movement
    ourMove->setMovementData(ourMData);
    auto tData = ourMove->getGameObject().getTransformData();
    tData.position += ourVel+ourVel;
    ourMove->getGameObject().setTransformData(tData);
}

void PhysicsManager::close() {

}

IComponent::Pointer PhysicsManager::createMoveComponent(GameObject& newGameObject, LAPAL::movementData newMData, LAPAL::plane3f newPlane, float newMass) {

    IComponent::Pointer component = std::make_shared<MoveComponent>(newGameObject, newMData, newPlane, newMass);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    EventManager::getInstance().addEvent(Event {EventType::MoveComponent_Create, data});

    return component;
}

IComponent::Pointer PhysicsManager::createTerrainComponent(GameObject& newGameObject, LAPAL::plane3f newPlane) {

    IComponent::Pointer component = std::make_shared<TerrainComponent>(newGameObject, newPlane);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    EventManager::getInstance().addEvent(Event {EventType::TerrainComponent_Create, data});

    return component;
}


IComponent::Pointer PhysicsManager::createCollisionComponent(GameObject& newGameObject, const float radius) {

    IComponent::Pointer component = std::make_shared<CollisionComponent>(newGameObject, radius);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    EventManager::getInstance().addEvent(Event {EventType::CollisionComponent_Create, data});

    return component;
}

//Create and add a new updateable character to the movingCharacterList
void PhysicsManager::createMovingCharacter(IComponent::Pointer moveComponent, IComponent::Pointer terrainComponent, IComponent::Pointer collisionComponent) {

    MovingCharacter mChar;

    mChar.moveComponent = std::dynamic_pointer_cast<MoveComponent>(moveComponent);
    mChar.terrainComponent = std::dynamic_pointer_cast<TerrainComponent>(terrainComponent);
    mChar.collisionComponent = std::dynamic_pointer_cast<CollisionComponent>(collisionComponent);

    movingCharacterList.push_back(mChar);

}

//==============================================
// DELEGATES
//============================================== 
void addMoveComponent(EventData data) {
    PhysicsManager::getInstance().getMoveComponentList().push_back(data.Component);
    data.Component.get()->init();
}

void addCollisionComponent(EventData data) {
    PhysicsManager::getInstance().getCollisionComponentList().push_back(data.Component);
    data.Component.get()->init();
}