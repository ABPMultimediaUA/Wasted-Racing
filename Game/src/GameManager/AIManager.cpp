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
    distanceLoD = 0;
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
            aiBattleComponent->update(dTime);
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
            if(distPlayerAI <= distanceLoD*distanceLoD || distanceLoD == 0)
            {
                if(AIObject.getComponent<CollisionComponent>()->getKinetic() == false)
                {
                    AIObject.getComponent<CollisionComponent>()->setKinetic(true);
                }
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
    AIObject->getComponent<CollisionComponent>()->setKinetic(false);

    auto maxSpeed = AIObject->getComponent<MoveComponent>()->getMovemententData().max_vel;
    AIObject->getComponent<MoveComponent>()->changeVel(0);

    auto distCover = (maxSpeed * maxSpeed) * dTime;

    auto waypoints = WaypointManager::getInstance().getWaypoints();

    unsigned int posVector = AIObject->getComponent<PathPlanningComponent>()->getLastPosVector();

    float distaneActualWay = (waypoints[posVector].get()->getTransformData().position.x - trans.position.x) * (waypoints[posVector].get()->getTransformData().position.x - trans.position.x) +
						     (waypoints[posVector].get()->getTransformData().position.y - trans.position.y) * (waypoints[posVector].get()->getTransformData().position.y - trans.position.y) +
						     (waypoints[posVector].get()->getTransformData().position.z - trans.position.z) * (waypoints[posVector].get()->getTransformData().position.z - trans.position.z);

    glm::vec3 nextPos;

    if(distaneActualWay <= maxSpeed*10)
    {
        if(posVector < waypoints.size()-1)
        {
            posVector++;
            AIObject->getComponent<PathPlanningComponent>()->setLastPosVector(posVector);
        }
        else if(posVector == waypoints.size()-1)
        {
            AIObject->getComponent<StartLineComponent>()->setActive(true);
            AIObject->getComponent<PathPlanningComponent>()->setLastPosVector(0);

        }
        posVector = AIObject->getComponent<PathPlanningComponent>()->getLastPosVector();
        distaneActualWay = (waypoints[posVector].get()->getTransformData().position.x - trans.position.x) * (waypoints[posVector].get()->getTransformData().position.x - trans.position.x) +
                            (waypoints[posVector].get()->getTransformData().position.y - trans.position.y) * (waypoints[posVector].get()->getTransformData().position.y - trans.position.y) +
                            (waypoints[posVector].get()->getTransformData().position.z - trans.position.z) * (waypoints[posVector].get()->getTransformData().position.z - trans.position.z);
    }
    /*if(posVector == 0)
    {
        distaneActualWay = (waypoints[posVector].get()->getTransformData().position.x - waypoints[waypoints.size()-1].get()->getTransformData().position.x) * (waypoints[posVector].get()->getTransformData().position.x - waypoints[waypoints.size()-1].get()->getTransformData().position.x) +
                           (waypoints[posVector].get()->getTransformData().position.y - waypoints[waypoints.size()-1].get()->getTransformData().position.y) * (waypoints[posVector].get()->getTransformData().position.y - waypoints[waypoints.size()-1].get()->getTransformData().position.y) +
                           (waypoints[posVector].get()->getTransformData().position.z - waypoints[waypoints.size()-1].get()->getTransformData().position.z) * (waypoints[posVector].get()->getTransformData().position.z - waypoints[waypoints.size()-1].get()->getTransformData().position.z);
        nextPos = ((distCover/distaneActualWay) * (waypoints[posVector].get()->getTransformData().position - trans.position)) + trans.position;
            
    }
    else if(posVector > 0 && posVector < waypoints.size()-1)
    {
        distaneActualWay = (waypoints[posVector].get()->getTransformData().position.x - waypoints[posVector-1].get()->getTransformData().position.x) * (waypoints[posVector].get()->getTransformData().position.x - waypoints[posVector-1].get()->getTransformData().position.x) +
                           (waypoints[posVector].get()->getTransformData().position.y - waypoints[posVector-1].get()->getTransformData().position.y) * (waypoints[posVector].get()->getTransformData().position.y - waypoints[posVector-1].get()->getTransformData().position.y) +
                           (waypoints[posVector].get()->getTransformData().position.z - waypoints[posVector-1].get()->getTransformData().position.z) * (waypoints[posVector].get()->getTransformData().position.z - waypoints[posVector-1].get()->getTransformData().position.z);
        nextPos = ((distCover/distaneActualWay) * (waypoints[posVector].get()->getTransformData().position - trans.position)) + trans.position;
    }
    else if(posVector == waypoints.size()-1)
    {
        distaneActualWay = (waypoints[0].get()->getTransformData().position.x - waypoints[posVector].get()->getTransformData().position.x) * (waypoints[0].get()->getTransformData().position.x - waypoints[posVector].get()->getTransformData().position.x) +
                           (waypoints[0].get()->getTransformData().position.y - waypoints[posVector].get()->getTransformData().position.y) * (waypoints[0].get()->getTransformData().position.y - waypoints[posVector].get()->getTransformData().position.y) +
                           (waypoints[0].get()->getTransformData().position.z - waypoints[posVector].get()->getTransformData().position.z) * (waypoints[0].get()->getTransformData().position.z - waypoints[posVector].get()->getTransformData().position.z);
        nextPos = ((distCover/distaneActualWay) * (waypoints[0].get()->getTransformData().position - trans.position)) + trans.position;
    }*/
    nextPos = ((distCover/distaneActualWay) * (waypoints[posVector].get()->getTransformData().position - trans.position)) + trans.position;
    
    trans.position = nextPos;
    
    AIObject->setNewTransformData(trans);
    RenderManager::getInstance().getRenderFacade()->updateObjectTransform(AI.getId(), trans);

    ////////////////////////////////////
    /////    ASSIGN RANDOM ITEM    /////
    //////////////////////////////////// 
    if(posVector%2 == 1)
    {
        auto itemHolder = AIObject->getComponent<ItemHolderComponent>();

        if(itemHolder->getItemType() == -1){
            srand (time(NULL));
            int random;
            if(AIObject->getComponent<ScoreComponent>()->getPosition() == 1)
            {
                random = rand() % 3 + 2;
            }
            else
            {
                random = rand() % 5;
            }

            itemHolder->setItemType(random);
        }
    }
    /////////////////////////////////////////////////////////////////////////
    ///////     AJUSTAR EL BEHAVIOUR THREE A QUE SE USE SIEMPRE EL ITEM
    /////////////////////////////////////////////////////////////////////////
}