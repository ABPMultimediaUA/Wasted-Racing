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
    waypoints = WaypointManager::getInstance().getWaypoints();
    go = false;
    getGameObject().getComponent<MoveComponent>()->changeInvul(true);
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

    objective = enemy.get()->getGameObject().getTransformData().position; 
    /*glm::vec3 nextPos;

    auto distCover = (maxSpeed * maxSpeed) * dTime;
    glm::vec3 distination;


    float distancePlayer = (objective.x - pos.x) * (objective.x - pos.x) +
						(objective.y - pos.y) * (objective.y - pos.y) +
						(objective.z - pos.z) * (objective.z - pos.z);


    auto terrain = moveComponent->getTerrain();
    float yTerrain = LAPAL::calculateExpectedY(terrain, pos);

    unsigned int posVector = getGameObject().getComponent<PathPlanningComponent>()->getLastPosVector();

    if((distancePlayer <= 10000))
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
        
    }*/

    //Animation missile
    
    std::cout<<getGameObject().getId()<<" :1 x: "<<trans.position.x<<"\n";
    std::cout<<getGameObject().getId()<<" :1 y: "<<trans.position.y<<"\n";
    std::cout<<getGameObject().getId()<<" :1 z: "<<trans.position.z<<"\n";

    if(valueY < 60 && go == false)
    {
        valueY += 5;
        trans.position.y = trans.position.y + valueY;
    }
    else if(valueY == 60)
    {
        go = true;
    }

    if(go == true)
    {
        auto terrain = moveComponent->getTerrain();
        float yTerrain = LAPAL::calculateExpectedY(terrain, pos);
        trans.position = objective;
        auto length = enemy->getGameObject().getComponent<CollisionComponent>()->getLength();
        trans.position.y = yTerrain + length + valueY;
        if(valueY == 60)
            valueY = 20;
        else if(valueY > 0)
            valueY -= 1;

        if(valueY == 5)
        {
            EventData data;
            data.Id             = getGameObject().getId();
            data.Component      = enemy.get()->getGameObject().getComponent<MoveComponent>();
            data.CollComponent  = getGameObject().getComponent<CollisionComponent>();

            EventManager::getInstance().addEvent(Event {EventType::BlueShellComponent_Collision, data});
        }
    }
    //nextPos.y += valueY;

    //trans.position = objective;

    std::cout<<getGameObject().getId()<<" :2 x: "<<trans.position.x<<"\n";
    std::cout<<getGameObject().getId()<<" :2 y: "<<trans.position.y<<"\n";
    std::cout<<getGameObject().getId()<<" :2 z: "<<trans.position.z<<"\n";

        /*
            EventData data;
            data.Id             = getGameObject().getId();
            data.Component      = enemy.get()->getGameObject().getComponent<MoveComponent>();
            data.CollComponent  = getGameObject().getComponent<CollisionComponent>();

            EventManager::getInstance().addEvent(Event {EventType::BlueShellComponent_Collision, data});
            */
           
    getGameObject().setNewTransformData(trans);
    getGameObject().setOldTransformData(trans);
    getGameObject().setTransformData(trans);
    RenderManager::getInstance().getRenderFacade()->updateObjectTransform(getGameObject().getId(), trans);

}

void ItemBlueShellComponent::close()
{
    
} 
