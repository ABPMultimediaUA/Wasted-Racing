#pragma once

#include "../GameObject/MoveComponent.h"
#include "../GameObject/CollisionComponent.h"
#include "../GameObject/TerrainComponent.h"
#include "../GameObject/RampComponent.h"
#include "../GameObject/ItemBoxComponent.h"
#include "../GameObject/ItemBananaComponent.h"
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

    //move component list getter
    std::vector<IComponent::Pointer>& getMoveComponentList() {
        return moveComponentList;
    }

    //move component list getter
    std::vector<IComponent::Pointer>& getCollisionComponentList() {
        return collisionComponentList;
    }

    //Component creators
    IComponent::Pointer createMoveComponent(GameObject& newGameObject, LAPAL::movementData newMData, LAPAL::plane3f newPlane, float newMass);
    IComponent::Pointer createTerrainComponent(GameObject& newGameObject, LAPAL::plane3f newPlane);
    IComponent::Pointer createCollisionComponent(GameObject& newGameObject, const float radius, const bool kinetic, const CollisionComponent::Type type);
    IComponent::Pointer createCollisionComponent(GameObject& newGameObject, const LAPAL::plane3f newPlane, const bool kinetic, const CollisionComponent::Type type);
    void                createMovingCharacter(IComponent::Pointer moveComponent, IComponent::Pointer terrainComponent, IComponent::Pointer collisionComponent);
    IComponent::Pointer createRampComponent(GameObject& newGameObject, const float speed, const float cTime, const float dTime);

private:

    void calculateObjectsCollision(std::shared_ptr<MoveComponent> , std::shared_ptr<CollisionComponent> );
    void calculateStaticCollision(std::shared_ptr<MoveComponent>);
    void calculateTerrainCollision(MovingCharacter&, std::shared_ptr<MoveComponent>, std::shared_ptr<TerrainComponent> , std::shared_ptr<CollisionComponent> );

    std::vector<IComponent::Pointer> moveComponentList;
    std::vector<IComponent::Pointer> collisionComponentList;
    std::vector<MovingCharacter>     movingCharacterList;   //A list of the moving characters that can collide with terrain

};