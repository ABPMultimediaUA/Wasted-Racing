#pragma once
//Basic includes
#include <memory>
#include <iostream>
#include <vector>
#include <queue>

#include "../GlobalVariables.h"
#include "../GameFacade/Clock.h"
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

    enum AIEventType {
        UPDATE_BATTLE,
        UPDATE_DRIVING_TURN,
        UPDATE_DRIVING_ACCELERATION,
        UPDATE_LOD
    }

    //Event to be updated by the scheduling
    struct AIEvent{
       IComponent::Pointer object; //Object to be updated;
       AIEventType event;          //Event type to be updated
       double average;             //Average time of the function for processing
       double timeStamp;           //When it was generated
    }


    //Constructor
    AIManager();

    //Destructor
    ~AIManager() {
        delete clock;
    }

    //Initialization
    void init();

    //Update
    void update(const float dTime);

    //Update
    void updateScheduling(const float dTime);

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
    std::vector<IComponent::Pointer> objectsAI; //AIDrivingComponents to be processed
    std::vector<IComponent::Pointer> battleAI;  //BattleBehaviour componentes to be processed

    //Processing queue
    std::queue<AIEvent> AIQueue; 
    double maxTimeSchedule;      //Maximum time for all processes   

    //___>
    //bool changeAI;
    bool updateBattleBehaviour;
    //<___

    //Clock
    Clock* clock;

    float distanceLoD;//////   PASAR A VARIABLE GLOBAL, ESTA EN AIMANAGER, WAYPOINTEMANAGER Y SENSORMANAGER
};