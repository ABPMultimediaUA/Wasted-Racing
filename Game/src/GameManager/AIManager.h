#pragma once


#include "InputManager.h"
#include "../GameObject/AIComponent/AIDrivingComponent.h"
#include "../GameObject/AIComponent/AIBattleComponent.h"
#include "../GameObject/AIComponent/VSensorComponent.h"
#include "../GameObject/AIComponent/MSensorComponent.h"
#include "../GameObject/AIComponent/PathPlanningComponent.h"
#include "../GameObject/AIComponent/VObject.h"
#include "../GameObject/ItemComponent/IItemComponent.h"
#include "../GameObject/PhysicsComponent/MoveComponent.h"
#include "../GameObject/GameObject.h"
#include "../GameEvent/EventManager.h"
#include <memory>
#include <iostream>
#include <vector>
#include "../GameObject/ItemComponent/IItemComponent.h"
#include "../GameObject/AIComponent/MSensorComponent.h"
#include "../GameObject/AIComponent/AIBattleComponent.h"
#include "../GlobalVariables.h"

class AIManager{

public: 

    //Constructor
    AIManager() {}

    //Destructor
    ~AIManager() {}

    //Initialization
    void init();

    //Update
    void update(const float dTime);

    //Shutdown
    void close();

    std::vector<IComponent::Pointer>& getAIDrivingComponentList() {
        return objectsAI;
    }

    std::vector<IComponent::Pointer>& getAIBattleComponentList()
    {
        return battleAI;
    }

    //Static class getter
    static AIManager& getInstance();


    //Component creators
    IComponent::Pointer createAIDrivingComponent(GameObject& newGameObject);
    IComponent::Pointer createAIBattleComponent(GameObject& newGameObject);

    //Update AI driving
    void updateDriving(AIDrivingComponent* aiDrivingComponent);

    //Level of Detail
    void calculateLoD(GameObject AI, float dTime);

private:
    std::vector<IComponent::Pointer> objectsAI;
    std::vector<IComponent::Pointer> battleAI;
    bool changeAI;
};