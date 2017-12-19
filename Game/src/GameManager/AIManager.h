#pragma once

#include "../GameObject/AIDrivingComponent.h"
#include "../GameObject/AIBattleComponent.h"
#include "../GameObject/MoveComponent.h"
#include "../GameObject/VSensorComponent.h"
#include "../GameObject/MSensorComponent.h"
#include "../GameObject/PathPlanningComponent.h"
#include "../GameObject/VObject.h"
#include "../GameObject/GameObject.h"
#include "../GameEvent/EventManager.h"
#include <memory>
#include <iostream>
#include <vector>

class AIManager{

public: 

    //Constructor
    AIManager() {}

    //Destructor
    ~AIManager() {}

    //Initialization
    void init();

    //Update
    void update();

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

private:
    std::vector<IComponent::Pointer> objectsAI;
    std::vector<IComponent::Pointer> battleAI;
};