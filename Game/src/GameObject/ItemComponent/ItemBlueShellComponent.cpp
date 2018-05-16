#include "ItemBlueShellComponent.h"



ItemBlueShellComponent::ItemBlueShellComponent(GameObject& newGameObject, GameObject& obj, IItemComponent::InstanceType m) : IItemComponent(newGameObject), player(obj), mode(m)
{
    speed = 1.f;
    consTime = 0.1f;
    decTime = 1.f;
}

ItemBlueShellComponent::~ItemBlueShellComponent()
{

}

void ItemBlueShellComponent::init(float actualVector)
{
    lastVector = player.getComponent<PathPlanningComponent>()->getLastPosVector();
    enemy = ScoreManager::getInstance().getPlayers()[0];
    valueY = 0;
    getGameObject().getComponent<PathPlanningComponent>()->setLastPosVector(actualVector);
}

//:::> This ALL should be in AIManager since it does calculations using all components
void ItemBlueShellComponent::update(float dTime)
{
    //Return if object was created as a copy of a remote online object
    if(mode == IItemComponent::InstanceType::REMOTE)
        return;


 /*   auto trans = getGameObject().getTransformData();
    auto pos = trans.position;
    
    //<___

    //:::>Explain all of this code pls
    auto listNodes = WaypointManager::getInstance().getWaypoints();
    auto vSensorComponent = getGameObject().getComponent<VSensorComponent>().get();
    auto moveComponent = getGameObject().getComponent<MoveComponent>().get();
    auto aiDrivingComponent = getGameObject().getComponent<AIDrivingComponent>().get();
    getGameObject().getComponent<CollisionComponent>()->setKinetic(false);

    auto posWay = listNodes[lastVector].get()->getTransformData().position;

    float distaneActualWay = (posWay.x - pos.x) * (posWay.x - pos.x) +
						(posWay.y - pos.y) * (posWay.y - pos.y) +
						(posWay.z - pos.z) * (posWay.z - pos.z);
	float radius = listNodes[lastVector].get()->getComponent<WaypointComponent>()->getRadius();
	
    if((distaneActualWay <= (radius*radius)/2) || (posWay.x - pos.x < radius && posWay.z - pos.z < radius))
    {
        if(lastVector < listNodes.size()-1)
        {
            lastVector++;
        }
        else if(lastVector == listNodes.size()-1)
        {
            lastVector = 0;
        }
    }
	
    
    vSensorComponent->setAngleInitial(moveComponent->getMovemententData().angle);

    objective = enemy.get()->getGameObject().getTransformData().position; 

    float distancePlayer = (objective.x - pos.x) * (objective.x - pos.x) +
						(objective.y - pos.y) * (objective.y - pos.y) +
						(objective.z - pos.z) * (objective.z - pos.z);
	

    unsigned int posVectorEnemy = enemy.get()->getGameObject().getComponent<PathPlanningComponent>()->getLastPosVector();
    if(distaneActualWay < distancePlayer || lastVector < posVectorEnemy)
    {
        objective = posWay;
    }
    
    float a = 0.f,b = 0.f;
    vSensorComponent->calculateAB(objective, a, b);
    std::vector<VObject::Pointer> seenObjects;
    //DECIDE 
    float turnValue = aiDrivingComponent->girar(getGameObject(), seenObjects, seenObjects, objective, a, b);

    moveComponent->changeSpin(turnValue);

    //Accelerate and brake
    moveComponent->isMoving(true);
    moveComponent->changeAcc(1);

*/






    auto trans = getGameObject().getTransformData();
    auto pos = trans.position;
    auto maxSpeed = getGameObject().getComponent<MoveComponent>()->getMovemententData().max_vel;
    getGameObject().getComponent<MoveComponent>()->changeVel(0);

    auto distCover = (maxSpeed * maxSpeed) * 0.25 * dTime;
    glm::vec3 distination;

    objective = enemy.get()->getGameObject().getTransformData().position; 

    float distancePlayer = (objective.x - pos.x) * (objective.x - pos.x) +
						(objective.y - pos.y) * (objective.y - pos.y) +
						(objective.z - pos.z) * (objective.z - pos.z);

    glm::vec3 nextPos;

    if((distancePlayer < 1000) /*|| (objective.x - pos.x < 120 && objective.z - pos.z < 120)*/)
    {
        /*getGameObject().getComponent<MoveComponent>()->changeVel(maxSpeed);
        objective.y += 10; 
        distination = objective;
        nextPos = distination;*/
    }
    else
    {
        auto waypoints = WaypointManager::getInstance().getWaypoints();

        unsigned int posVector = getGameObject().getComponent<PathPlanningComponent>()->getLastPosVector();

        float distaneActualWay = (waypoints[posVector].get()->getTransformData().position.x - trans.position.x) * (waypoints[posVector].get()->getTransformData().position.x - trans.position.x) +
                                (waypoints[posVector].get()->getTransformData().position.y - trans.position.y) * (waypoints[posVector].get()->getTransformData().position.y - trans.position.y) +
                                (waypoints[posVector].get()->getTransformData().position.z - trans.position.z) * (waypoints[posVector].get()->getTransformData().position.z - trans.position.z);

        if(distaneActualWay <= maxSpeed*10)
        {
            if(posVector < waypoints.size()-1)
            {
                posVector++;
                getGameObject().getComponent<PathPlanningComponent>()->setLastPosVector(posVector);
            }
            else if(posVector == waypoints.size()-1)
            {
                getGameObject().getComponent<PathPlanningComponent>()->setLastPosVector(0);
            }
            posVector = getGameObject().getComponent<PathPlanningComponent>()->getLastPosVector();
            distaneActualWay = (waypoints[posVector].get()->getTransformData().position.x - trans.position.x) * (waypoints[posVector].get()->getTransformData().position.x - trans.position.x) +
                                (waypoints[posVector].get()->getTransformData().position.y - trans.position.y) * (waypoints[posVector].get()->getTransformData().position.y - trans.position.y) +
                                (waypoints[posVector].get()->getTransformData().position.z - trans.position.z) * (waypoints[posVector].get()->getTransformData().position.z - trans.position.z);
        }
        
        distination = waypoints[posVector].get()->getTransformData().position;
        nextPos = ((distCover/5000) * (distination - trans.position)) + trans.position;
    }
    

    
    trans.position = nextPos;


    getGameObject().setNewTransformData(trans);
    RenderManager::getInstance().getRenderFacade()->updateObjectTransform(getGameObject().getId(), trans);












/*
   
    //Y animation
    auto terrain = moveComponent->getTerrain();
    float yTerrain = LAPAL::calculateExpectedY(terrain, pos);
    if(distancePlayer > 120*120)
    {
        if(valueY < 20.f) 
        {
            valueY += 1;
            trans.position.y = yTerrain + valueY;
        }
        else
        {
            trans.position.y = yTerrain + valueY;
        }
    }
    else
    {
        if(valueY > 0.f) 
        {
            valueY -= 10;
            trans.position.y = yTerrain + valueY;
        }
    }
    getGameObject().setNewTransformData(trans);
    RenderManager::getInstance().getRenderFacade()->updateObjectTransform(getGameObject().getId(), trans);*/
}

void ItemBlueShellComponent::close()
{
    
} 
