#pragma once
//Basic includes
#include <memory>
#include <iostream>
#include <vector>
#include "../GlobalVariables.h"
#include "../GameObject/IComponent.h"
#include "../GameObject/GameObject.h"
#include "../GameEvent/EventManager.h"

//Inner components includes
#include "../GameObject/AIComponent/AIDrivingComponent.h"
#include "../GameObject/AIComponent/AIBattleComponent.h"
#include "../GameObject/AIComponent/VSensorComponent.h"
#include "../GameObject/AIComponent/MSensorComponent.h"
#include "../GameObject/AIComponent/PathPlanningComponent.h"
#include "../GameObject/AIComponent/VObject.h"

//Extra includes
#include "../GameObject/ItemComponent/IItemComponent.h"
#include "../GameObject/PhysicsComponent/MoveComponent.h"

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
    //:::> Explain what are the variables for if the name is not self explaining
    std::vector<IComponent::Pointer> objectsAI;
    std::vector<IComponent::Pointer> battleAI;

    //___>
    //bool changeAI;
    bool updateBattleBehaviour;
    //<___

    float distanceLoD;//////   PASAR A VARIABLE GLOBAL, ESTA EN AIMANAGER, WAYPOINTEMANAGER Y SENSORMANAGER
};