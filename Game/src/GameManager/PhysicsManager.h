#pragma once

#include "../GameObject/PhysicsComponent/MoveComponent.h"
#include "../GameObject/PhysicsComponent/CollisionComponent.h"
#include "../GameObject/PhysicsComponent/TerrainComponent.h"
#include "../GameObject/PhysicsComponent/RampComponent.h"
#include "../GameObject/ItemComponent/ItemBoxComponent.h"
#include "../GameObject/ItemComponent/ItemBananaComponent.h"
#include "../GameObject/ItemComponent/ItemBlueShellComponent.h"
#include "../GameObject/ItemComponent/ItemRedShellComponent.h"
#include "../GameObject/ItemComponent/ItemStarComponent.h"
#include <vector>

class PhysicsManager{

public: 

    struct MovingCharacter {
        std::shared_ptr<MoveComponent> moveComponent;
        std::shared_ptr<TerrainComponent> terrainComponent;
        std::shared_ptr<CollisionComponent> collisionComponent;

        void setTerrain(std::shared_ptr<TerrainComponent> t){ terrainComponent = t; };
    };

    //Constructor
    PhysicsManager() {}

    //Destructor
    ~PhysicsManager() {}

    //Initialization
    void init();

    //Update
    void update(const float dTime);

    //Shutdown
    void close();


    //Static class getter
    static PhysicsManager& getInstance();
    std::vector<IComponent::Pointer>& getMoveComponentList()       {    return moveComponentList;        } //Move component list getter
    std::vector<IComponent::Pointer>& getCollisionComponentList()  {    return collisionComponentList;   } //Collision component list getter
    std::vector<MovingCharacter>& getMovingCharacterList()     {    return movingCharacterList;      }     //MovingCharacter component list getter
    
    MovingCharacter getMovingCharacter(uint16_t id){      //Get given moving character by object id from list
        MovingCharacter mChar;
        for(unsigned int i=0; i < movingCharacterList.size(); ++i){
            if(movingCharacterList[i].moveComponent->getGameObject().getId() == id){
                return movingCharacterList[i];
            }
        }
        return mChar;
    }
    //Setters
    void setCollisionComponentList(std::vector<IComponent::Pointer>& collision)            {    collisionComponentList = collision; }

    //Component creators
    IComponent::Pointer createMoveComponent(GameObject& newGameObject, LAPAL::movementData newMData, LAPAL::plane3f newPlane, float newMass);
    IComponent::Pointer createTerrainComponent(GameObject& newGameObject, LAPAL::plane3f newPlane);
    IComponent::Pointer createCollisionComponent(GameObject& newGameObject, const float radius, const bool kinetic, const CollisionComponent::Type type);
    IComponent::Pointer createCollisionComponent(GameObject& newGameObject, const LAPAL::plane3f newPlane, const bool kinetic, const CollisionComponent::Type type);
    void                createMovingCharacter(IComponent::Pointer moveComponent, IComponent::Pointer terrainComponent, IComponent::Pointer collisionComponent);
    IComponent::Pointer createRampComponent(GameObject& newGameObject, const float speed, const float cTime, const float dTime);

private:

    void calculateObjectsCollision(std::shared_ptr<MoveComponent> , std::shared_ptr<CollisionComponent>, const float );
    void calculateStaticCollision(std::shared_ptr<MoveComponent>, const float);
    void calculateTerrainCollision(MovingCharacter&, std::shared_ptr<MoveComponent>, std::shared_ptr<TerrainComponent> , std::shared_ptr<CollisionComponent>, const float );
    void checkCollisionShellTerrain(GameObject& obj);

    std::vector<IComponent::Pointer> moveComponentList;
    std::vector<IComponent::Pointer> collisionComponentList;
    std::vector<MovingCharacter>     movingCharacterList;   //A list of the moving characters that can collide with terrain

};