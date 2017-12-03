#pragma once

#include "../GameObject/MoveComponent.h"
#include <vector>

class PhysicsManager{

public: 

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

    IComponent::Pointer createMoveComponent(GameObject& newGameObject, LAPAL::movementData newMData, float newMass);

private:

    std::vector<IComponent::Pointer> moveComponentList;

};