#include "ItemBlueShellComponent.h"



ItemBlueShellComponent::ItemBlueShellComponent(GameObject& newGameObject, GameObject& obj) : IItemComponent(newGameObject), player(obj)
{
    speed = 1.f;
    consTime = 0.1f;
    decTime = 1.f;
}

ItemBlueShellComponent::~ItemBlueShellComponent()
{

}

void ItemBlueShellComponent::init()
{
    lastVector = player.getComponent<PathPlanningComponent>()->getLastPosVector();
}

void ItemBlueShellComponent::update(float dTime)
{
    //auto obj;// = ScoreManager::getInstance()->getPlayers()[0];
    auto listNodes = WaypointManager::getInstance().getWaypoints();
    auto vSensorComponent = getGameObject().getComponent<VSensorComponent>().get();
    auto moveComponent = getGameObject().getComponent<MoveComponent>().get();
    auto aiDrivingComponent = getGameObject().getComponent<AIDrivingComponent>().get();

    auto pos =getGameObject().getTransformData().position;

    auto posWay = listNodes[lastVector].get()->getTransformData().position;

    float distaneActualWay = (posWay.x - pos.x) * (posWay.x - pos.x) +
						(posWay.y - pos.y) * (posWay.y - pos.y) +
						(posWay.z - pos.z) * (posWay.z - pos.z);
	float radius = listNodes[lastVector].get()->getComponent<WaypointComponent>()->getRadius();
	if(distaneActualWay <= radius*radius)
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
    glm::vec3 objective = ScoreManager::getInstance().getPlayers()[0].get()->getGameObject().getTransformData().position; //= listNodes[lastVector]->getTransformData().position;

    float distancePlayer = (objective.x - pos.x) * (objective.x - pos.x) +
						(objective.y - pos.y) * (objective.y - pos.y) +
						(objective.z - pos.z) * (objective.z - pos.z);
	
    if(distaneActualWay < distancePlayer)
    {
        objective = posWay;
    }

    float a = 0.f,b = 0.f;
    vSensorComponent->calculateAB(objective, a, b);
    std::vector<VObject::Pointer> seenObjects;
    //DECIDE 
    float turnValue = aiDrivingComponent->girar(seenObjects, objective, a, b);
    float speedValue = aiDrivingComponent->acelerar_frenar(seenObjects, turnValue, vSensorComponent->getAngleInitial(), a, b);

    moveComponent->changeSpin(turnValue);

    //Accelerate and brake
    moveComponent->isMoving(true);
    moveComponent->changeAcc(1);
   

}

void ItemBlueShellComponent::close()
{
    
} 
