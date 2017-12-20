#include "ItemBlueShellComponent.h"



ItemBlueShellComponent::ItemBlueShellComponent(GameObject& newGameObject, GameObject& obj) : IItemComponent(newGameObject), player(obj)
{
    speed = 0.f;
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

    /*auto pos =getGameObject().getTransformData().position;

    float distaneActualWay = (listNodes[lastVector].get()->getTransformData().position.x - pos.x) * (listNodes[lastVector].get()->getTransformData().position.x - pos.x) +
						(listNodes[lastVector].get()->getTransformData().position.y - pos.y) * (listNodes[lastVector].get()->getTransformData().position.y - pos.y) +
						(listNodes[lastVector].get()->getTransformData().position.z - pos.z) * (listNodes[lastVector].get()->getTransformData().position.z - pos.z);
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
	}*/

    vSensorComponent->setAngleInitial(moveComponent->getMovemententData().angle);
    glm::vec3 objective = ScoreManager::getInstance().getPlayers()[0].get()->getGameObject().getTransformData().position; //= listNodes[lastVector]->getTransformData().position;
    float a=0,b=0;
    vSensorComponent->calculateAB(objective, &a, &b);
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
