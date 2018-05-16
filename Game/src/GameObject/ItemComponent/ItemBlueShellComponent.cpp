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


    auto trans = getGameObject().getTransformData();
    auto pos = trans.position;
    auto maxSpeed = getGameObject().getComponent<MoveComponent>()->getMovemententData().max_vel;
    getGameObject().getComponent<MoveComponent>()->changeVel(0);

    auto moveComponent = getGameObject().getComponent<MoveComponent>().get();
    
    auto distCover = (maxSpeed * maxSpeed) * dTime;
    glm::vec3 distination;

    objective = enemy.get()->getGameObject().getTransformData().position; 

    float distancePlayer = (objective.x - pos.x) * (objective.x - pos.x) +
						(objective.y - pos.y) * (objective.y - pos.y) +
						(objective.z - pos.z) * (objective.z - pos.z);

    glm::vec3 nextPos;

    auto terrain = moveComponent->getTerrain();
    float yTerrain = LAPAL::calculateExpectedY(terrain, pos);

    if((distancePlayer <= 10000) /*|| (objective.x - pos.x < 7 && objective.z - pos.z < 7)*/)
    {   //When the distance enemy-missile is to small, we change the position of missile to enemy and event collision

        if(distancePlayer <= 100)
        {

            valueY = 0;
            nextPos = distination;

            EventData data;
            data.Id             = getGameObject().getId();
            data.Component      = enemy.get()->getGameObject().getComponent<MoveComponent>();
            data.CollComponent  = getGameObject().getComponent<CollisionComponent>();

            EventManager::getInstance().addEvent(Event {EventType::BlueShellComponent_Collision, data});
        }
        else
        {
            valueY = 0;
            distination = objective;
            nextPos = distination;

            nextPos = ((distCover/4000) * (distination - trans.position)) + trans.position;
        }
    }
    else
    {

        auto waypoints = WaypointManager::getInstance().getWaypoints();

        unsigned int posVector = getGameObject().getComponent<PathPlanningComponent>()->getLastPosVector();

        float distaneActualWay = (waypoints[posVector].get()->getTransformData().position.x - trans.position.x) * (waypoints[posVector].get()->getTransformData().position.x - trans.position.x) +
                                (waypoints[posVector].get()->getTransformData().position.y - trans.position.y) * (waypoints[posVector].get()->getTransformData().position.y - trans.position.y) +
                                (waypoints[posVector].get()->getTransformData().position.z - trans.position.z) * (waypoints[posVector].get()->getTransformData().position.z - trans.position.z);

	    float radius = waypoints[lastVector].get()->getComponent<WaypointComponent>()->getRadius();
	
        if((distaneActualWay <= maxSpeed) || (waypoints[posVector].get()->getTransformData().position.x - pos.x == 0 && waypoints[posVector].get()->getTransformData().position.z - pos.z == 0))
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
        }
        
        distination = waypoints[posVector].get()->getTransformData().position;
        nextPos = ((distCover/8000) * (distination - trans.position)) + trans.position;
        
    }

    //Animation missile
    if(distancePlayer <= 4000 && distancePlayer >= 500)
    {
        if(valueY > 0)
            valueY -= 5;
    }
    else if(distancePlayer >= 4000)
    {
        valueY = 40;
    }

    nextPos.y += valueY;
    trans.position = nextPos;

    getGameObject().setNewTransformData(trans);
    RenderManager::getInstance().getRenderFacade()->updateObjectTransform(getGameObject().getId(), trans);

}

void ItemBlueShellComponent::close()
{
    
} 
