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
void collideRamp(EventData eData);
void collideBanana(EventData eData);
void collideBlueShell(EventData eData);
void collideRedShell(EventData eData);
void collideItemBox(EventData eData);
void objectDeletedCollide(EventData eData);   
void objectDeletedMove(EventData eData);
void objectDeletedCharacter(EventData eData);

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
    EventManager::getInstance().addListener(EventListener {EventType::RampComponent_Collision, collideRamp});
    EventManager::getInstance().addListener(EventListener {EventType::ItemBoxComponent_Collision, collideItemBox});
    EventManager::getInstance().addListener(EventListener {EventType::BananaComponent_Collision, collideBanana});
    EventManager::getInstance().addListener(EventListener {EventType::BlueShellComponent_Collision, collideBlueShell});
    EventManager::getInstance().addListener(EventListener {EventType::RedShellComponent_Collision, collideRedShell});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeletedCollide});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeletedMove});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeletedCharacter});

}

void PhysicsManager::update(const float dTime) {

    for(unsigned int i=0; i<movingCharacterList.size(); ++i){

        //Get components in variables
        auto ourMove = movingCharacterList[i].moveComponent;
        auto ourTerr = movingCharacterList[i].terrainComponent;
        auto ourColl = movingCharacterList[i].collisionComponent;

        //==============================================================================
        // Move character
        //==============================================================================
        ourMove->update(dTime);
        
        //==============================================================================
        // Check collisions with other objects
        //==============================================================================
        calculateObjectsCollision(ourMove, ourColl, dTime);

        //==============================================================================
        // Check collisions with terrain limits and terrain change
        //==============================================================================
        calculateTerrainCollision(movingCharacterList[i], ourMove, ourTerr, ourColl, dTime);
        
    }

}

void PhysicsManager::close() {
    moveComponentList.clear();
    collisionComponentList.clear();
    movingCharacterList.clear();  
}


//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

void PhysicsManager::calculateObjectsCollision(std::shared_ptr<MoveComponent> move, std::shared_ptr<CollisionComponent> coll, const float dTime) {

    CollisionComponent* ourColl = coll.get();

    for(unsigned int j=0; j<collisionComponentList.size(); ++j) {

        std::shared_ptr<CollisionComponent> hColl = std::dynamic_pointer_cast<CollisionComponent>(collisionComponentList[j]);
        CollisionComponent* hisColl = hColl.get();
        if( hisColl != ourColl ) { //If the collider is different to the one of ourselves

            bool collision;
            
            if(hisColl->getShape() == CollisionComponent::Shape::Circle) {
                collision = LAPAL::checkCircleCircleCollision(  ourColl->getGameObject().getTransformData().position, ourColl->getRadius(), 
                                                                hisColl->getGameObject().getTransformData().position, hisColl->getRadius());
            } 
            else if(hisColl->getShape() == CollisionComponent::Shape::Rectangle) {
                collision = LAPAL::checkCircleRectangleCollision(   hisColl->getRectangle(), 
                                                                    ourColl->getGameObject().getTransformData().position, ourColl->getRadius());
            }

            if(collision && hisColl->getKinetic()){

                //Get other object move component
                auto hisMove = hisColl->getGameObject().getComponent<MoveComponent>();

                if(hisMove == nullptr) {    //If the object doesn't have move component, it's static
                        
                    calculateStaticCollision(move, dTime);

                }
                else {  //The object is not static
                        //***** CODE FOR COLLISIONS WHERE BOTH OBJECTS ARE MOVING *****//
                        calculateStaticCollision(move, dTime); //This is not right!!!
                        calculateStaticCollision(hisMove, dTime); //This is not right!!!
                }
            }
            else if(collision && !hisColl->getKinetic()){

                if(hisColl->getType() == CollisionComponent::Type::Ramp)
                {
                    EventData data;
                    data.Component      = std::static_pointer_cast<IComponent>(move);
                    data.CollComponent  = std::static_pointer_cast<IComponent>(hColl);

                    EventManager::getInstance().addEvent(Event {EventType::RampComponent_Collision, data});
                }
                else if(hisColl->getType() == CollisionComponent::Type::Banana)
                {
                    EventData data;
                    data.Component      = std::static_pointer_cast<IComponent>(move);
                    data.CollComponent  = std::static_pointer_cast<IComponent>(hColl);

                    EventManager::getInstance().addEvent(Event {EventType::BananaComponent_Collision, data});
                }
                else if(hisColl->getType() == CollisionComponent::Type::ItemBox)
                {
                    EventData data;
                    data.Component      = std::static_pointer_cast<IComponent>(move);
                    data.CollComponent  = std::static_pointer_cast<IComponent>(hColl);

                    EventManager::getInstance().addEvent(Event {EventType::ItemBoxComponent_Collision, data});
                }
                else if(hisColl->getType() == CollisionComponent::Type::BlueShell && coll == ScoreManager::getInstance().getPlayers()[0].get()->getGameObject().getComponent<CollisionComponent>())
                {
                    EventData data;
                    data.Component      = std::static_pointer_cast<IComponent>(move);
                    data.CollComponent  = std::static_pointer_cast<IComponent>(hColl);

                    EventManager::getInstance().addEvent(Event {EventType::BlueShellComponent_Collision, data});
                }
                else if(hisColl->getType() == CollisionComponent::Type::RedShell)
                {
                    EventData data;
                    data.Component      = std::static_pointer_cast<IComponent>(move);
                    data.CollComponent  = std::static_pointer_cast<IComponent>(hColl);

                    EventManager::getInstance().addEvent(Event {EventType::RedShellComponent_Collision, data});
                }
            }
        }
    }
}

void PhysicsManager::calculateStaticCollision(std::shared_ptr<MoveComponent> move, const float dTime) {

    MoveComponent* ourMove = move.get(); 

    //float ourMass = ourMove->getMass();
    //float hisMass = 5;

    auto ourMData = ourMove->getMovemententData();

    //LAPAL::vec3f ourVel = ourMData.velocity;
    //LAPAL::vec3f hisVel = glm::vec3(0,0,0);

    //Calculate new velocity after collision
    //LAPAL::calculateElasticCollision(ourVel, ourMass, hisVel, hisMass);
    //ourMData.velocity = ourVel;

    ////Calculate new velocity module
    //float newVel    = -sqrt(ourVel.x*ourVel.x + ourVel.z*ourVel.z);
    //if(ourMData.vel < 0)
    //    newVel = -newVel;

    ourMData.vel    = -0.1;

    //-----_TESTING_------
    //ourMData.spin = -ourMData.spin;

    //Set new movement
    ourMove->setMovementData(ourMData);
    auto tData = ourMove->getGameObject().getTransformData();

    tData.position.x -= ourMData.velocity.x*dTime;
    tData.position.z -= ourMData.velocity.z*dTime;

    ourMove->getGameObject().setTransformData(tData);
}


///--------------MEJORAS--------------------
/*
    >Comprobar colisioens con la Y y que rebote en caso de estar por
    debajo de la Y del siguiente terreno a cierto nivel (para saltos)
    >Añadir botón o mecánica de recuperación en caso de caída

*/
///-----------------------------------------
void PhysicsManager::calculateTerrainCollision(MovingCharacter& movingChar, std::shared_ptr<MoveComponent> move, std::shared_ptr<TerrainComponent> terr, std::shared_ptr<CollisionComponent> coll, const float dTime) {

        MoveComponent* ourMove = move.get();
        TerrainComponent* ourTerr = terr.get();
        CollisionComponent* ourColl = coll.get();
    

        auto terrain    = ourMove->getTerrain();
        auto ourMData   = ourMove->getGameObject().getTransformData();
        auto ourMovementData = ourMove ->getMovemententData();
        float radius    = ourColl->getRadius();
        
        //Composition values of the point inside the terrain
        float a = 0.f, b = 0.f;
        glm::vec3 nextPosition;

        //Future position:
        if(ourMovementData.vel != 0){
            nextPosition = glm::vec3(
                                ourMData.position.x + ourMovementData.velocity.x / ourMovementData.vel * radius,
                                ourMData.position.y + ourMovementData.velocity.y / ourMovementData.vel * radius,
                                ourMData.position.z + ourMovementData.velocity.z / ourMovementData.vel * radius
                                );
        }else{
            nextPosition = ourMData.position;
        }
        //Calculate A and B for the object radius in the direction of its movement
        LAPAL::calculateTerrainAB(terrain, nextPosition, a, b);


        //Check if we are out of front bounds
        if(a>0 && b<0 && glm::abs(a)+glm::abs(b)>=1){
            if( ourTerr->getNext() == nullptr ) {   //If there isn't next plane, collision
                calculateStaticCollision(move, dTime);
                return;
            }else{
                //Check if we are inside the next terrain. If not, still collide.
                LAPAL::calculateTerrainAB(ourTerr->getNext()->getTerrain(), nextPosition, a, b);
                if(a+b >= 0 && abs(a)+abs(b)<=1){
                    //Inside the next terrain
                    ourMove->setTerrain(ourTerr->getNext()->getTerrain()); //Set new terrain
                    movingChar.terrainComponent = ourTerr->getNext(); //Set new terrain component
                }else{
                    calculateStaticCollision(move, dTime);
                }
            }
        }

        //Check if we are out of right bounds
        if(a>0 && b>0 && glm::abs(a)+glm::abs(b)>=1){
            if( ourTerr->getRight() == nullptr ) {   //If there isn't next plane, collision
                calculateStaticCollision(move, dTime);
                return;
            }
            else{
                //Check if we are inside the next terrain. If not, still collide.
                LAPAL::calculateTerrainAB(ourTerr->getRight()->getTerrain(), nextPosition, a, b);
                if(a+b >= 0 && abs(a)+abs(b)<=1){
                    //Inside the next terrain
                    ourMove->setTerrain(ourTerr->getRight()->getTerrain()); //Set new terrain
                    movingChar.terrainComponent = ourTerr->getRight(); //Set new terrain component
                }else{
                    calculateStaticCollision(move, dTime);
                }
            }
        }
        
        //Check if we are out of back bounds
        if(a>0 && b>0 && a - b<=0){
            if( ourTerr->getPrev() == nullptr ) {   //If there isn't next plane, collision
                calculateStaticCollision(move, dTime);
                return;
            }
            else{
                //Check if we are inside the next terrain. If not, still collide.
                LAPAL::calculateTerrainAB(ourTerr->getPrev()->getTerrain(), nextPosition, a, b);
                if(a+b >= 0 && abs(a)+abs(b)<=1){
                    //Inside the next terrain
                    ourMove->setTerrain(ourTerr->getPrev()->getTerrain()); //Set new terrain
                    movingChar.terrainComponent = ourTerr->getPrev(); //Set new terrain component
                }else{
                    calculateStaticCollision(move, dTime);
                }
                
            }
        }

        //Check if we are out of left bounds
        if(a>0 && b<0 && a+b<=0){
           if( ourTerr->getLeft() == nullptr ) {   //If there isn't next plane, collision
                calculateStaticCollision(move, dTime);
                return;
            }
            else{
                //Check if we are inside the next terrain. If not, still collide.
                LAPAL::calculateTerrainAB(ourTerr->getLeft()->getTerrain(), nextPosition, a, b);
                if(a+b >= 0 && abs(a)+abs(b)<=1){
                    //Inside the next terrain
                    ourMove->setTerrain(ourTerr->getLeft()->getTerrain()); //Set new terrain
                    movingChar.terrainComponent = ourTerr->getLeft(); //Set new terrain component
                }else{
                    calculateStaticCollision(move, dTime);
                }
            }
        }
}


//==============================================================================
// COMPONENT CREATOR FUNCTIONS
//==============================================================================

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


IComponent::Pointer PhysicsManager::createCollisionComponent(GameObject& newGameObject, const float radius, const bool kinetic, const CollisionComponent::Type type) {

    IComponent::Pointer component = std::make_shared<CollisionComponent>(newGameObject, radius, kinetic, type);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    EventManager::getInstance().addEvent(Event {EventType::CollisionComponent_Create, data});

    return component;
}

IComponent::Pointer PhysicsManager::createCollisionComponent(GameObject& newGameObject, const LAPAL::plane3f plane, const bool kinetic, const CollisionComponent::Type type) {

    IComponent::Pointer component = std::make_shared<CollisionComponent>(newGameObject, plane, kinetic, type);

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

IComponent::Pointer PhysicsManager::createRampComponent(GameObject& newGameObject, const float speed, const float cTime, const float dTime) {

    IComponent::Pointer component = std::make_shared<RampComponent>(newGameObject, speed, cTime, dTime);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    EventManager::getInstance().addEvent(Event {EventType::RampComponent_Create, data});

    return component;
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

void collideRamp(EventData eData) {

    auto move = std::static_pointer_cast<MoveComponent>(eData.Component);
    auto coll = std::static_pointer_cast<CollisionComponent>(eData.CollComponent);

    auto ramp = coll->getGameObject().getComponent<RampComponent>();

    if(ramp != nullptr) {
        move->changeMaxSpeedOverTime(ramp.get()->getSpeed(), ramp.get()->getConstTime(), ramp.get()->getDecTime());
    }
}

void collideBanana(EventData eData) {
    auto move = std::static_pointer_cast<MoveComponent>(eData.Component);
    auto coll = std::static_pointer_cast<CollisionComponent>(eData.CollComponent);

    auto banana = coll->getGameObject().getComponent<ItemBananaComponent>();

    if(banana != nullptr) {
        move->changeMaxSpeedOverTime(banana.get()->getSpeed(), banana.get()->getConsTime(), banana.get()->getDecTime());

        EventData data;
        data.Id = banana->getGameObject().getId();

        EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});
    }
}
void collideBlueShell(EventData eData) {
    auto move = std::static_pointer_cast<MoveComponent>(eData.Component);
    auto coll = std::static_pointer_cast<CollisionComponent>(eData.CollComponent);

    auto shell = coll->getGameObject().getComponent<ItemBlueShellComponent>();

    if(shell != nullptr) {
        move->changeMaxSpeedOverTime(shell.get()->getSpeed(), shell.get()->getConsTime(), shell.get()->getDecTime());

        EventData data;
        data.Id = shell->getGameObject().getId();

        EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});
    }
}

void collideRedShell(EventData eData) {
    auto move = std::static_pointer_cast<MoveComponent>(eData.Component);
    auto coll = std::static_pointer_cast<CollisionComponent>(eData.CollComponent);

    auto shell = coll->getGameObject().getComponent<ItemRedShellComponent>();

    if(shell != nullptr) {
        move->changeMaxSpeedOverTime(shell.get()->getSpeed(), shell.get()->getConsTime(), shell.get()->getDecTime());

        EventData data;
        data.Id = shell->getGameObject().getId();

        EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});
    }
}

//Collide Item Box
void collideItemBox(EventData eData){

    auto move = std::static_pointer_cast<MoveComponent>(eData.Component);
    auto coll = std::static_pointer_cast<CollisionComponent>(eData.CollComponent);

    auto itemBox = coll->getGameObject().getComponent<ItemBoxComponent>();

    auto obj = move->getGameObject();

    if(itemBox != nullptr){
       if(move->getGameObject().getComponent<IItemComponent>() == nullptr && coll->getGameObject().getTransformData().scale.x != 0 && coll->getGameObject().getTransformData().scale.y != 0 && coll->getGameObject().getTransformData().scale.z != 0){
           itemBox->asignItem(obj);
           itemBox->deactivateBox();
       }
    }
}

void objectDeletedCollide(EventData eData) {

    auto& collisionComponentList = PhysicsManager::getInstance().getCollisionComponentList();

    for(unsigned int i = 0; i<collisionComponentList.size(); ++i) {
        if(eData.Id == collisionComponentList.at(i).get()->getGameObject().getId()) {
            collisionComponentList.erase(collisionComponentList.begin() + i);
            return;
        }
    }
}

void objectDeletedMove(EventData eData) {

    auto& moveComponentList = PhysicsManager::getInstance().getMoveComponentList();

    for(unsigned int i = 0; i<moveComponentList.size(); ++i) {
        if(eData.Id == moveComponentList.at(i).get()->getGameObject().getId()) {
            moveComponentList.erase(moveComponentList.begin() + i);
            return;
        }
    }
}


void objectDeletedCharacter(EventData eData) {

    auto& movingCharacterList = PhysicsManager::getInstance().getMovingCharacterList();

    for(unsigned int i = 0; i<movingCharacterList.size(); ++i) {
        if(eData.Id == movingCharacterList[i].moveComponent->getGameObject().getId()) {
            movingCharacterList.erase(movingCharacterList.begin() + i);
            return;
        }
    }
}
