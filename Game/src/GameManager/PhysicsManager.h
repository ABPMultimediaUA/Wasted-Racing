#pragma once

#include "../GameObject/MoveComponent.h"
#include "../GameObject/CollisionComponent.h"
#include "../GameObject/TerrainComponent.h"
#include <vector>

class PhysicsManager{

public: 

    struct MovingCharacter {
        std::shared_ptr<MoveComponent> moveComponent;
        std::shared_ptr<TerrainComponent> terrainComponent;
        std::shared_ptr<CollisionComponent> collisionComponent;
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
    IComponent::Pointer createCollisionComponent(GameObject& newGameObject, const float radius);
    void                createMovingCharacter(IComponent::Pointer moveComponent, IComponent::Pointer terrainComponent, IComponent::Pointer collisionComponent);

private:

    void calculateStaticCollision(MoveComponent* );

    std::vector<IComponent::Pointer> moveComponentList;
    std::vector<IComponent::Pointer> collisionComponentList;
    std::vector<MovingCharacter>     movingCharacterList;   //A list of the moving characters that can collide with terrain

};