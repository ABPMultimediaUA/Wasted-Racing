#include "PathPlanningComponent.h"
#include <iostream>
 

PathPlanningComponent::PathPlanningComponent(GameObject& newGameObject) : IComponent(newGameObject) 
{
	seconds = 1;
    distLastWay = -1;
    lastVector = 0;
}

void PathPlanningComponent::update(float dTime)
{

	auto pos = this->getGameObject().getTransformData().position;
	auto modSpeed = this->getGameObject().getComponent<MoveComponent>()->getMovemententData().vel;

	auto wpManager = &WaypointManager::getInstance();

    std::vector<GameObject::Pointer> listNodes = wpManager->getWaypoints(); 

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
			int vuelta = getGameObject().getComponent<ScoreComponent>()->getLap();
			getGameObject().getComponent<ScoreComponent>()->setLap(vuelta + 1);
			lastVector = 0;
		}
	}

    float tour = (modSpeed * seconds) * (modSpeed * seconds);
	int posVector;
	int lastPosVector = lastVector;
	float distanceNextNode;
    float distNode;
	float dist;

    int lvl;

    for (size_t i = lastPosVector; i < listNodes.size(); i++)
	{
		/*if(listNodes[listNodes.size()-2]->getComponent<WaypointComponent>()->getLevel() == listNodes[lastPosVector]->getComponent<WaypointComponent>()->getLevel()
		 && i == listNodes.size()-1)
		{
			setLastPosVector(0);
			//i = 0;
		}
		else
		{*/
			lvl = listNodes[lastPosVector].get()->getComponent<WaypointComponent>()->getLevel();
			if(listNodes[i].get()->getComponent<WaypointComponent>()->getLevel() >= lvl)
			{
				setDistLastWay(listNodes[lastPosVector], pos);
				distNode = (listNodes[i].get()->getTransformData().position.x - pos.x) * (listNodes[i].get()->getTransformData().position.x - pos.x) +
						(listNodes[i].get()->getTransformData().position.y - pos.y) * (listNodes[i].get()->getTransformData().position.y - pos.y) +
						(listNodes[i].get()->getTransformData().position.z - pos.z) * (listNodes[i].get()->getTransformData().position.z - pos.z);

				
				if((lvl+1) == listNodes[i].get()->getComponent<WaypointComponent>()->getLevel())
				{
					if(tour-distLastWay < 0)
					{
						//nextPos = ((tour/distLastWay) * (listNodes[lastPosVector].get()->getTransformData().position - pos)) + pos;
						nextPos = listNodes[lastPosVector].get()->getTransformData().position;
						return;
					}
					else
					{
						if(distNode <= tour)
						{
							lastPosVector = i;
						}                        
					}
				}
			}
		//}
	}
		posVector = lastPosVector;
        distanceNextNode  = -1;

        for(size_t i = lastPosVector; i < listNodes.size(); i++)
        {  
			/*if(listNodes[listNodes.size()-2]->getComponent<WaypointComponent>()->getLevel() == listNodes[lastPosVector]->getComponent<WaypointComponent>()->getLevel()
			&& i == listNodes.size()-1)
			{
				setLastPosVector(0);
				//i = 0;
			}
			else
			{*/
				if(listNodes[i]->getComponent<WaypointComponent>()->getLevel() == listNodes[lastPosVector]->getComponent<WaypointComponent>()->getLevel()+1)
				{
					distNode = (listNodes[i].get()->getTransformData().position.x - pos.x) * (listNodes[i].get()->getTransformData().position.x - pos.x) +
							(listNodes[i].get()->getTransformData().position.y - pos.y) * (listNodes[i].get()->getTransformData().position.y - pos.y) +
							(listNodes[i].get()->getTransformData().position.z - pos.z) * (listNodes[i].get()->getTransformData().position.z - pos.z);

					if(distanceNextNode == -1)
					{
						distanceNextNode = distNode;
						lastPosVector = i;
					}
				}
			//}
        }

        distNode = (pos.x - listNodes[posVector].get()->getTransformData().position.x) * (pos.x - listNodes[posVector]->getTransformData().position.x) +
                (pos.y - listNodes[posVector].get()->getTransformData().position.y) * (pos.y - listNodes[posVector]->getTransformData().position.y) +
                (pos.z - listNodes[posVector].get()->getTransformData().position.z) * (pos.z - listNodes[posVector]->getTransformData().position.z);
        
		dist = ( pos.x - listNodes[lastPosVector].get()->getTransformData().position.x) * ( pos.x - listNodes[lastPosVector].get()->getTransformData().position.x) +
				( pos.y - listNodes[lastPosVector].get()->getTransformData().position.y) * ( pos.y - listNodes[lastPosVector].get()->getTransformData().position.y) +
				(pos.z - listNodes[lastPosVector].get()->getTransformData().position.z) * ( pos.z - listNodes[lastPosVector].get()->getTransformData().position.z);
		
		tour -= distNode;
		

        nextPos = ((tour/dist) * (listNodes[lastPosVector].get()->getTransformData().position - listNodes[posVector].get()->getTransformData().position) + listNodes[posVector].get()->getTransformData().position);
		
        
    return;
}

glm::vec3 PathPlanningComponent::getNextPoint()
{
	return nextPos;
}


//==============================================
//Getters
//==============================================

float PathPlanningComponent::getDistLastWay()
{
    return distLastWay;
}

int PathPlanningComponent::getLastPosVector()
{
    return lastVector;
}

int PathPlanningComponent::getActualLevel()
{
	auto listNodes = WaypointManager::getInstance().getWaypoints();
	return listNodes[lastVector].get()->getComponent<WaypointComponent>()->getLevel();
}

float PathPlanningComponent::getActualDistance()
{
	glm::vec3 pos = getGameObject().getTransformData().position;
	auto listNodes = WaypointManager::getInstance().getWaypoints();

	float distanceActualWay = (listNodes[lastVector].get()->getTransformData().position.x - pos.x) * (listNodes[lastVector].get()->getTransformData().position.x - pos.x) +
						(listNodes[lastVector].get()->getTransformData().position.y - pos.y) * (listNodes[lastVector].get()->getTransformData().position.y - pos.y) +
						(listNodes[lastVector].get()->getTransformData().position.z - pos.z) * (listNodes[lastVector].get()->getTransformData().position.z - pos.z);

	return distanceActualWay;

}
//==============================================
//Setters
//==============================================

void PathPlanningComponent::setDistLastWay(GameObject::Pointer n, glm::vec3 pos)
{
    distLastWay = (n.get()->getTransformData().position.x - pos.x) * (n.get()->getTransformData().position.x - pos.x) +
                (n.get()->getTransformData().position.y - pos.y) * (n.get()->getTransformData().position.y - pos.y) +
                (n.get()->getTransformData().position.z - pos.z) * (n.get()->getTransformData().position.z - pos.z);
}

void PathPlanningComponent::setLastPosVector(int lvl)
{
    lastVector = lvl;
}
