#include "AIManager.h"
#include <rapidxml/rapidxml.hpp>


//==============================================
// DELEGATES DECLARATIONS
//==============================================

void addAIDrivingComponent(EventData data);
void objectDeleteAIDriving(EventData data);
void objectDeleteAIBattle(EventData data);


//==============================================
// AI MANAGER FUNCTIONS
//==============================================

AIManager& AIManager::getInstance() {
    static AIManager instance;
    return instance;
}

void AIManager::init() {
    changeAI = false;
    distanceLoD = 100;
    //Bind listeners
    EventManager::getInstance().addListener(EventListener {EventType::AIDrivingComponent_Create, addAIDrivingComponent});
    //No delete by the moment
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeleteAIDriving});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeleteAIBattle});

}


void AIManager::update(float dTime) {
    //Update of all behaviour trees

GameObject::Pointer AIObject = ObjectManager::getInstance().getObject(25001);

    if(changeAI == true)
    {
        for(unsigned int i=0; i<battleAI.size(); i++)
        {
            auto aiBattleComponent = std::dynamic_pointer_cast<AIBattleComponent>(battleAI[i]).get();
            aiBattleComponent->update(0.02f);
        }
        changeAI = false;
    }
    else if (changeAI == false)
    {
        auto player = InputManager::getInstance().getComponent().get()->getGameObject();
        auto posPlayer = player.getTransformData().position;


        for(unsigned int i=0; i<objectsAI.size(); ++i){

            auto aiDrivingComponent =  std::dynamic_pointer_cast<AIDrivingComponent>(objectsAI[i]).get();
            auto AIObject = aiDrivingComponent->getGameObject();
            auto posAI = AIObject.getTransformData().position; 


            auto distPlayerAI = (posPlayer.x - posAI.x) * (posPlayer.x - posAI.x) +
                                (posPlayer.y - posAI.y) * (posPlayer.y - posAI.y) +
                                (posPlayer.z - posAI.z) * (posPlayer.z - posAI.z);

                            
            //IF DISTANCE PLAYER-AI IS BIGER THAN DISTANCELOD, CALCULATE LOD
            if(distPlayerAI <= distanceLoD*distanceLoD)
            {
                updateDriving(aiDrivingComponent);
            }
            else
            {
                calculateLoD(AIObject, dTime);
            }
        }
        changeAI = true;
    }
}

void AIManager::close() {
    objectsAI.clear();
    battleAI.clear();
}


//COMPONENT CREATORS
IComponent::Pointer AIManager::createAIDrivingComponent(GameObject& newGameObject){

    IComponent::Pointer component = std::make_shared<AIDrivingComponent>(newGameObject);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    EventManager::getInstance().addEvent(Event {EventType::AIDrivingComponent_Create, data});

    return component;
}

IComponent::Pointer AIManager::createAIBattleComponent(GameObject& newGameObject)
{
    IComponent::Pointer component = std::make_shared<AIBattleComponent>(newGameObject);

    newGameObject.addComponent(component);

    battleAI.push_back(component);

   std::dynamic_pointer_cast<AIBattleComponent>(component).get()->init();


    return component;
}


//==============================================
// DELEGATES
//==============================================
void addAIDrivingComponent(EventData data) {
    AIManager::getInstance().getAIDrivingComponentList().push_back(data.Component);
    data.Component.get()->init();
}

void objectDeleteAIDriving(EventData eData) {

    auto& aIDrivingComponentList = AIManager::getInstance().getAIDrivingComponentList();

    for(unsigned int i = 0; i<aIDrivingComponentList.size(); ++i) {
        if(eData.Id == aIDrivingComponentList[i].get()->getGameObject().getId()) {
            aIDrivingComponentList.erase(aIDrivingComponentList.begin() + i);
            return;
        }
    }
}

void objectDeleteAIBattle(EventData eData) {

    auto& aIBattleComponentList = AIManager::getInstance().getAIBattleComponentList();

    for(unsigned int i = 0; i<aIBattleComponentList.size(); ++i) {
        if(eData.Id == aIBattleComponentList[i].get()->getGameObject().getId()) {
            aIBattleComponentList.erase(aIBattleComponentList.begin() + i);
            return;
        }
    }
}


//==============================================
// UPDATE DRIVING
//==============================================

void AIManager::updateDriving(AIDrivingComponent* aiDrivingComponent)
{
    //Get components needed for the movement
    auto moveComponent = aiDrivingComponent->getGameObject().getComponent<MoveComponent>().get();
    auto vSensorComponent = aiDrivingComponent->getGameObject().getComponent<VSensorComponent>().get();
    auto mSensorComponent = aiDrivingComponent->getGameObject().getComponent<MSensorComponent>().get();
    auto pathPlanningComponent = aiDrivingComponent->getGameObject().getComponent<PathPlanningComponent>().get();
    auto iItemComponent = aiDrivingComponent->getGameObject().getComponent<IItemComponent>().get();

    if(aiDrivingComponent && moveComponent && vSensorComponent && mSensorComponent && iItemComponent == nullptr){
        //get all objects that are seen to the visual sensor
        std::vector<VObject::Pointer> seenObjects  = vSensorComponent->getSeenObjects();
        std::vector<VObject::Pointer> mapCollisions = mSensorComponent->getMapCollisions();
        

        //Set angle of the sensors to the NPC one
        vSensorComponent->setAngleInitial(moveComponent->getMovemententData().angle);
        mSensorComponent->setAngleInitial(moveComponent->getMovemententData().angle);
        
        //Get next waypoint
        pathPlanningComponent->setSeconds(1);
        glm::vec3 objective = pathPlanningComponent->getNextPoint();


        //Update A and B of the objective
        float a = 0.f,b = 0.f;
        vSensorComponent->calculateAB(objective, a, b);

        auto myPos = aiDrivingComponent->getGameObject();

        float turnValue = aiDrivingComponent->girar(myPos, seenObjects, mapCollisions, objective, a, b);
        float speedValue = aiDrivingComponent->acelerar_frenar(myPos, seenObjects, turnValue, moveComponent->getMovemententData().vel, a, b); 
        //----------------------------------

        //Send signal of movement
        //Turn

        moveComponent->changeSpin(turnValue );

        moveComponent->isMoving(true);
        moveComponent->changeAcc(speedValue);
    }
}



//==============================================
// LEVEL OF DETAIL
//==============================================

void AIManager::calculateLoD(GameObject AI, float dTime)
{
    GameObject::Pointer AIObject = ObjectManager::getInstance().getObject(AI.getId());
    auto trans = AIObject->getTransformData();

    auto maxSpeed = AIObject->getComponent<MoveComponent>()->getMovemententData().max_vel;

    auto distCover = (maxSpeed * maxSpeed) * dTime;

    trans.position.x = 0;
    trans.position.y = 0;
    trans.position.z = 0;
    
    trans.rotation.y += 1*M_PI/180;

    AIObject->setNewTransformData(trans);
    RenderManager::getInstance().getRenderFacade()->updateObjectTransform(AIObject.get()->getId(), trans);
}