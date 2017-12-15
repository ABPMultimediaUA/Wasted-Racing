#pragma once

#include "../GameObject/AIDrivingComponent.h"
#include "../GameObject/MoveComponent.h"
#include "../GameObject/VSensorComponent.h"
#include "../GameObject/VObject.h"
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

    //Static class getter
    static AIManager& getInstance();


    //Component creators
    IComponent::Pointer createAIDrivingComponent(GameObject& newGameObject);

private:
    std::vector<IComponent::Pointer> objectsAI;
};