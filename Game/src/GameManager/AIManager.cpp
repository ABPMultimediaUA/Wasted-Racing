#include "AIManager.h"
#include "../GameEvent/EventManager.h"
#include "../GameObject/ItemComponent/IItemComponent.h"
#include "../GameObject/AIComponent/MSensorComponent.h"
#include "../GameObject/AIComponent/VSensorComponent.h"
#include "../GameObject/AIComponent/AIBattleComponent.h"

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
    //___>
    //changeAI = false;
    updateBattleBehaviour = false;
    //<___
    distanceLoD = 0;

    //Bind listeners
    EventManager::getInstance().addListener(EventListener {EventType::AIDrivingComponent_Create, addAIDrivingComponent});
    
    //No delete for the moment
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeleteAIDriving});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeleteAIBattle});

}


void AIManager::update(float dTime) {
    //Update only the second AI
    //:::> Wtf? Why only the second one? makes no sense
    //<___
    //GameObject::Pointer AIObject = ObjectManager::getInstance().getObject(25001);
    //___>

    //Interchange updating direction or battle behaviour each frame
    if(updateBattleBehaviour == true)
    {
        for(unsigned int i=0; i<battleAI.size(); i++)
        {
            auto aiBattleComponent = std::dynamic_pointer_cast<AIBattleComponent>(battleAI[i]).get();
            aiBattleComponent->update(dTime);
        }
        //set flag to false
        updateBattleBehaviour = false;
    }
    else
    {
        //get position of player to determine the distance to him (LOD)
        auto player = GlobalVariables::getInstance().getPlayer();
        auto posPlayer = player->getTransformData().position;

        //Update every AI
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
                //:::>Explain what
                if(AIObject.getComponent<CollisionComponent>()->getKinetic() == false)
                {
                    AIObject.getComponent<CollisionComponent>()->setKinetic(true);
                }

                //Update if the object is not an red shell or blue shell
                if(AIObject.getComponent<IItemComponent>() == nullptr)
                {
                    updateDriving(aiDrivingComponent);
                }
            }
            else
            {
                calculateLoD(AIObject, dTime);
            }
        }
        //Set flag to true
        updateBattleBehaviour = true;
    }
}

void AIManager::close() {
    objectsAI.clear();
    battleAI.clear();
}


//COMPONENT CREATORS
IComponent::Pointer AIManager::createAIDrivingComponent(GameObject& newGameObject){
    //Make shared pointer of the AI Driving component
    IComponent::Pointer component = std::make_shared<AIDrivingComponent>(newGameObject);

    //Attach to object
    newGameObject.addComponent(component);

    //Send event of creation
    //:::> maybe it can be done without the event since it is only used in this manager
    //:::> needs scheduling to be useful
    EventData data;
    data.Component = component;
    EventManager::getInstance().addEvent(Event {EventType::AIDrivingComponent_Create, data});

    return component;
}

IComponent::Pointer AIManager::createAIBattleComponent(GameObject& newGameObject)
{
    //Make shared pointer of the AI Driving component
    IComponent::Pointer component = std::make_shared<AIBattleComponent>(newGameObject);

    //Attach to object
    newGameObject.addComponent(component);

    //:::> needs scheduling to be in event form

    //Push to battle AI list
    battleAI.push_back(component);

    //Initalize component
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

        moveComponent->changeSpin(turnValue);

        moveComponent->isMoving(true);
        moveComponent->changeAcc(speedValue);
    }
}



//==============================================
// LEVEL OF DETAIL
//==============================================

void AIManager::calculateLoD(GameObject AI, float dTime)
{

    auto trans = AI.getTransformData();
    AI.getComponent<CollisionComponent>()->setKinetic(false);
    //std::cout<<AIObject->getComponent<CollisionComponent>()->getKinetic()<<"\n";

    auto maxSpeed = AI.getComponent<MoveComponent>()->getMovemententData().max_vel;
    AI.getComponent<MoveComponent>()->changeVel(maxSpeed);

    auto distCover = (maxSpeed * maxSpeed) * dTime;

    auto waypoints = WaypointManager::getInstance().getWaypoints();

    unsigned int posVector = AI.getComponent<PathPlanningComponent>()->getLastPosVector();

    float distaneActualWay = (waypoints[posVector].get()->getTransformData().position.x - trans.position.x) * (waypoints[posVector].get()->getTransformData().position.x - trans.position.x) +
						     (waypoints[posVector].get()->getTransformData().position.y - trans.position.y) * (waypoints[posVector].get()->getTransformData().position.y - trans.position.y) +
						     (waypoints[posVector].get()->getTransformData().position.z - trans.position.z) * (waypoints[posVector].get()->getTransformData().position.z - trans.position.z);

    glm::vec3 nextPos;

    if(distaneActualWay <= maxSpeed*10)
    {
        if(posVector < waypoints.size()-1)
        {
            posVector++;
            AI.getComponent<PathPlanningComponent>()->setLastPosVector(posVector);
        }
        else if(posVector == waypoints.size()-1)
        {
            AI.getComponent<StartLineComponent>()->setActive(true);
            AI.getComponent<PathPlanningComponent>()->setLastPosVector(0);

        }
        posVector = AI.getComponent<PathPlanningComponent>()->getLastPosVector();
        distaneActualWay = (waypoints[posVector].get()->getTransformData().position.x - trans.position.x) * (waypoints[posVector].get()->getTransformData().position.x - trans.position.x) +
                            (waypoints[posVector].get()->getTransformData().position.y - trans.position.y) * (waypoints[posVector].get()->getTransformData().position.y - trans.position.y) +
                            (waypoints[posVector].get()->getTransformData().position.z - trans.position.z) * (waypoints[posVector].get()->getTransformData().position.z - trans.position.z);
    }
    /*if(posVector == 0)
    {
        distaneActualWay = (waypoints[posVector].get()->getTransformData().position.x - waypoints[waypoints.size()-1].get()->getTransformData().position.x) * (waypoints[posVector].get()->getTransformData().position.x - waypoints[waypoints.size()-1].get()->getTransformData().position.x) +
                           (waypoints[posVector].get()->getTransformData().position.y - waypoints[waypoints.size()-1].get()->getTransformData().position.y) * (waypoints[posVector].get()->getTransformData().position.y - waypoints[waypoints.size()-1].get()->getTransformData().position.y) +
                           (waypoints[posVector].get()->getTransformData().position.z - waypoints[waypoints.size()-1].get()->getTransformData().position.z) * (waypoints[posVector].get()->getTransformData().position.z - waypoints[waypoints.size()-1].get()->getTransformData().position.z);
        //nextPos = ((distCover/distaneActualWay) * (waypoints[posVector].get()->getTransformData().position - waypoints[waypoints.size()-1].get()->getTransformData().position)) + trans.position;
            
    }
    else if(posVector > 0 && posVector < waypoints.size()-1)
    {
        distaneActualWay = (waypoints[posVector].get()->getTransformData().position.x - waypoints[posVector-1].get()->getTransformData().position.x) * (waypoints[posVector].get()->getTransformData().position.x - waypoints[posVector-1].get()->getTransformData().position.x) +
                           (waypoints[posVector].get()->getTransformData().position.y - waypoints[posVector-1].get()->getTransformData().position.y) * (waypoints[posVector].get()->getTransformData().position.y - waypoints[posVector-1].get()->getTransformData().position.y) +
                           (waypoints[posVector].get()->getTransformData().position.z - waypoints[posVector-1].get()->getTransformData().position.z) * (waypoints[posVector].get()->getTransformData().position.z - waypoints[posVector-1].get()->getTransformData().position.z);
        //nextPos = ((distCover/distaneActualWay) * (waypoints[posVector].get()->getTransformData().position - waypoints[posVector-1].get()->getTransformData().position)) + trans.position;
    }
    else if(posVector == waypoints.size()-1)
    {
        distaneActualWay = (waypoints[0].get()->getTransformData().position.x - waypoints[posVector].get()->getTransformData().position.x) * (waypoints[0].get()->getTransformData().position.x - waypoints[posVector].get()->getTransformData().position.x) +
                           (waypoints[0].get()->getTransformData().position.y - waypoints[posVector].get()->getTransformData().position.y) * (waypoints[0].get()->getTransformData().position.y - waypoints[posVector].get()->getTransformData().position.y) +
                           (waypoints[0].get()->getTransformData().position.z - waypoints[posVector].get()->getTransformData().position.z) * (waypoints[0].get()->getTransformData().position.z - waypoints[posVector].get()->getTransformData().position.z);
        //nextPos = ((distCover/distaneActualWay) * (waypoints[0].get()->getTransformData().position - waypoints[posVector].get()->getTransformData().position)) + trans.position;
    }*/
    nextPos = ((distCover/distaneActualWay) * (waypoints[posVector].get()->getTransformData().position - trans.position)) + trans.position;
    /*auto aux = (distCover/distaneActualWay) * (waypoints[posVector].get()->getTransformData().position - trans.position);
    auto mod2 =  (aux.x * aux.x) +
                (aux.y * aux.y) +
                (aux.z * aux.z);
    auto mod =  (nextPos.x * nextPos.x) +
                (nextPos.y * nextPos.y) +
                (nextPos.z * nextPos.z);
    std::cout<<"/////////////////////"<<"\n";
    std::cout<<mod2<<"\n";
    std::cout<<aux.x<<"\n";
    std::cout<<aux.y<<"\n";
    std::cout<<aux.z<<"\n";
    std::cout<<mod<<"\n";
    std::cout<<nextPos.x<<"\n";
    std::cout<<nextPos.y<<"\n";
    std::cout<<nextPos.z<<"\n";*/
    //nextPos = waypoints[posVector].get()->getTransformData().position;
    trans.position = nextPos;
    
    AI.setNewTransformData(trans);
    //AI->setOldTransformData(trans);
    //AI->setTransformData(trans);
    RenderManager::getInstance().getRenderFacade()->updateObjectTransform(AI.getId(), trans);

    ////////////////////////////////////
    /////    ASSIGN RANDOM ITEM    /////
    //////////////////////////////////// 
    if(posVector%2 == 1)
    {
        auto itemHolder = AI.getComponent<ItemHolderComponent>();

        if(itemHolder->getItemType() == -1){
            srand (time(NULL));
            int random;
            if(AI.getComponent<ScoreComponent>()->getPosition() == 1)
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
    //:::> Fix this
    /////////////////////////////////////////////////////////////////////////
    ///////     AJUSTAR EL BEHAVIOUR THREE A QUE SE USE SIEMPRE EL ITEM
    /////////////////////////////////////////////////////////////////////////
}