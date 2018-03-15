#pragma once

#include <memory>
#include <iostream>
#include <vector>
#include "../GameObject/IComponent.h"
#include "../GameObject/GameObject.h"

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


private:
    std::vector<IComponent::Pointer> objectsAI;
    std::vector<IComponent::Pointer> battleAI;
    bool changeAI;
};