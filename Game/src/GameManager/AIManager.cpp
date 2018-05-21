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
AIManager::AIManager()
{
    //Initialize time clocks
    clock = new Clock();
    clock_scheduling = new Clock();
    clock->init();
    clock_scheduling->init();

}

AIManager& AIManager::getInstance() {
    static AIManager instance;
    return instance;
}

void AIManager::init() {
    //___>
    //changeAI = false;
    updateBattleBehaviour = false;
    
    //Scheduling data
    samplesBattle      = 0;
    samplesDriving     = 0;
    samplesLOD         = 0;
    averageTimeBattle  = 0.000005;
    averageTimeDriving = 0.0000075;
    averageTimeLOD     = 0.00000005;
    accumulatedTimeSchedule = 0;

    //Bind listeners
    EventManager::getInstance().addListener(EventListener {EventType::AIDrivingComponent_Create, addAIDrivingComponent});
    
    //No delete for the moment
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeleteAIDriving});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeleteAIBattle});

    itemLoD = false;

}


void AIManager::update(float dTime) {
    //Schedule
    accumulatedTimeSchedule = 0.0;

    //Updating battle AI
    for(unsigned int i=0; i<battleAI.size(); i++)
    {
        //<___
        //auto aiBattleComponent = std::dynamic_pointer_cast<AIBattleComponent>(battleAI[i]).get();
        //aiBattleComponent->update(dTime);

        //Create battle package processing
        AIEvent a;
        a.object    = battleAI[i];
        a.event     = AIEventType::UPDATE_BATTLE;
        a.timeStamp = clock->getInitTime();
        a.average   = averageTimeBattle;

        //Add to list
        AIQueue.push(a);

        //Count accumulated Time
        accumulatedTimeSchedule += averageTimeBattle;

        //___>
    }
    //get position of player to determine the distance to him (LOD)
    auto player = GlobalVariables::getInstance().getPlayer();
    auto posPlayer = player->getTransformData().position;

    //Update every AI
    for(unsigned int i=0; i<objectsAI.size(); ++i){

        auto aiDrivingComponent =  std::dynamic_pointer_cast<AIDrivingComponent>(objectsAI[i]).get();
        auto AIObject = aiDrivingComponent->getGameObject();
        //Update if the object is not an red shell or blue shell
        if(AIObject.getComponent<IItemComponent>() == nullptr)
        {
            auto posAI = AIObject.getTransformData().position; 


            auto distPlayerAI = (posPlayer.x - posAI.x) * (posPlayer.x - posAI.x) +
                                (posPlayer.y - posAI.y) * (posPlayer.y - posAI.y) +
                                (posPlayer.z - posAI.z) * (posPlayer.z - posAI.z);

            float distanceLoD = GlobalVariables::getInstance().getDistanceLoD();
                            
            //IF DISTANCE PLAYER-AI IS BIGER THAN DISTANCELOD, CALCULATE LOD
            if(distPlayerAI <= distanceLoD*distanceLoD || distanceLoD == 0)
            {
                //:::>Explain what
                if(AIObject.getComponent<CollisionComponent>()->getKinetic() == false)
                {
                    AIObject.getComponent<CollisionComponent>()->setKinetic(true);
                }

                
                    //<___
                    //updateDriving(aiDrivingComponent);

                    //Create battle package processing
                    AIEvent a;
                    a.object    = objectsAI[i];
                    a.event     = AIEventType::UPDATE_DRIVING_TURN;
                    a.timeStamp = clock->getInitTime();
                    a.average   = averageTimeDriving;

                    //Add to list
                    AIQueue.push(a);

                    //Count accumulated Time
                    accumulatedTimeSchedule += averageTimeDriving;
                    
                    //___>
            }
            else
            {
                //<___
                //calculateLoD(AIObject, dTime);

                //Create lod package processing
                AIEvent a;
                a.object    = objectsAI[i];
                a.event     = AIEventType::UPDATE_LOD;
                a.timeStamp = clock->getInitTime();
                a.average   = averageTimeLOD;

                //Add to list
                AIQueue.push(a);

                //Count accumulated Time
                accumulatedTimeSchedule += averageTimeLOD;
                    
                //___>
            }
        }
    }
}

void AIManager::updateScheduling(float dTime, float loopTime) {
    //Iterate through the processes
    double timeCounter = 0.0;
    /*std::cout<<"---------------------"<<std::endl;
    std::cout<<"Scheduling queue size before: "<<AIQueue.size()<<std::endl;*/
    while(!AIQueue.empty())
    {
        //Time checker
        if(timeCounter > (accumulatedTimeSchedule)/ (loopTime/dTime) ) //loopTime / dTime = number of frames between updates. Acc / frame = time of processing assignated that frame.
            break;

        //Initialize time passing
        clock_scheduling->restart();

        //Take object
        AIEvent a = AIQueue.front();

        //Different cases
        switch(a.event)
        {
            case AIEventType::UPDATE_BATTLE:
            {
                //->Timing
                clock->restart();
                
                //Launch effect
                auto aiBattleComponent = std::dynamic_pointer_cast<AIBattleComponent>(a.object).get();
                aiBattleComponent->update(dTime);

                //<-Timing
                averageTimeBattle = (samplesBattle > 500) ? averageTimeBattle 
                                    : (averageTimeBattle * samplesBattle + clock->getElapsedTime()) / (samplesBattle+1); 
                                    //( (n-1) * previous_media + new_value ) / n = new_media

                samplesBattle++;
                
                break;
            }
            case AIEventType::UPDATE_DRIVING_TURN:
            {
                //->Timing
                clock->restart();

                //Launch effect
                auto aiDrivingComponent = std::dynamic_pointer_cast<AIDrivingComponent>(a.object).get();
                updateDriving(aiDrivingComponent);
                updateAISpeed();

                //<-Timing
                averageTimeDriving = (samplesDriving > 500) ? averageTimeDriving 
                                     : (averageTimeDriving * samplesDriving + clock->getElapsedTime()) / (samplesDriving+1); 
                                     //( (n-1) * previous_media + new_value ) / n = new_media

                samplesDriving++;

                break;
            }
            case AIEventType::UPDATE_LOD:
            {
                //->Timing
                clock->restart();

                //Get variables
                auto aiDrivingComponent2 =  std::dynamic_pointer_cast<AIDrivingComponent>(a.object).get();
                auto AIObject = aiDrivingComponent2->getGameObject();
                double timePassed = clock->getInitTime() - a.timeStamp + dTime; //Actual interval of time (dTime + time passed since creation of event)

                float maxDTime = GlobalVariables::getInstance().getMaxDTime();
                if(timePassed > maxDTime)
                {
                    timePassed = maxDTime;
                }

                //Launch effect
                calculateLoD(AIObject, timePassed);

                //<-Timing
                averageTimeLOD = (samplesLOD > 500) ? averageTimeLOD 
                                : (averageTimeLOD * samplesLOD + clock->getElapsedTime()) / (samplesLOD+1); 
                                //( (n-1) * previous_media + new_value ) / n = new_media
                samplesLOD++;
                
                break;
            }
            case AIEventType::UPDATE_DRIVING_ACCELERATION:
            {
                break;
            }
            default: break;
        }

        //Release 
        AIQueue.pop();

        //End time passing;
        timeCounter += clock_scheduling->getElapsedTime();
    }

    /*std::cout<<"---------------------"<<std::endl;
    std::cout<<"Scheduling queue size after: "<<AIQueue.size()<<std::endl;
    std::cout<<"Scheduling time accumulated: "<<accumulatedTimeSchedule<<std::endl;
    std::cout<<"Scheduling parameters:  "<<dTime <<std::endl;
    std::cout<<"Scheduling time spent:  "<<timeCounter <<std::endl;
    std::cout<<"---------------------"<<std::endl;*/

    //WE SHOW THE DIFFERENCES
    /*
    system("clear");
    std::cout<<"Average Time for battle: "<<averageTimeBattle<<" with "<<battleAI.size()<<std::endl;
    std::cout<<"Average Time for DRIVING: "<<averageTimeDriving<<" with "<<objectsAI.size()<<std::endl;
    std::cout<<"Average Time for LOD: "<<averageTimeLOD<<" with "<<objectsAI.size()<<std::endl;
    */
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
    
    if(aiDrivingComponent && moveComponent && vSensorComponent && mSensorComponent){
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
    
    GameObject::Pointer AIObject = ObjectManager::getInstance().getObject(AI.getId());
    auto trans = AIObject->getTransformData();
    AIObject->getComponent<CollisionComponent>()->setKinetic(false);

    auto maxSpeed = AIObject->getComponent<MoveComponent>()->getMovemententData().max_vel;
    AIObject->getComponent<MoveComponent>()->changeVel(0);

/////////////////////
    auto player = GlobalVariables::getInstance().getPlayer();
    int scorePlayer = player->getComponent<ScoreComponent>()->getPosition();

    int scoreAI = AIObject->getComponent<ScoreComponent>()->getPosition();
    float distCover;
    if(scoreAI < scorePlayer)
    {
        distCover = (maxSpeed * maxSpeed)*0.35 * dTime;
    }
    else if(scoreAI > scorePlayer)
    {
        distCover = (maxSpeed * maxSpeed)*0.6 * dTime;
    }
////////////////////////
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
    /*if(posVector%5 == 0 && itemLoD == false)
    {
        srand (time(NULL));
        auto itemHolder = AIObject->getComponent<ItemHolderComponent>();

        if(itemHolder->getItemType() == -1){
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
            itemLoD = true;
        }
    }
    else if (itemLoD == true)
    {
        itemLoD = false;
    }*/
    //:::> Fix this
    /////////////////////////////////////////////////////////////////////////
    ///////     AJUSTAR EL BEHAVIOUR THREE A QUE SE USE SIEMPRE EL ITEM
    /////////////////////////////////////////////////////////////////////////
}

void AIManager::updateAISpeed()
{
    auto player = GlobalVariables::getInstance().getPlayer();
    int scorePlayer = player->getComponent<ScoreComponent>()->getPosition();

    for(int i = 0; i < objectsAI.size(); i++)
    {
        if(objectsAI[i]->getGameObject().getComponent<ScoreComponent>() != nullptr)
        {
            int scoreAI = objectsAI[i]->getGameObject().getComponent<ScoreComponent>()->getPosition();
            auto move = objectsAI[i]->getGameObject().getComponent<MoveComponent>();
            float real_max_vel = move->getMovemententData().real_max_vel;
            
            if(move->getMovemententData().boost == false)
            {
                move->changeMaxVel(real_max_vel);

                if(scoreAI < scorePlayer)
                {
                    move->changeMaxVel(real_max_vel*0.9);
                }
                else if(scoreAI > scorePlayer)
                {
                    move->changeMaxVel(real_max_vel*1.1);
                }
            }
        }
    }
}