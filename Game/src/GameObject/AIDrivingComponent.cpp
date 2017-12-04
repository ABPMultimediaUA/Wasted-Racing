#include "AIDrivingComponent.h"
#include "WaypointComponent.h"
#include <memory>

AIDrivingComponent::AIDrivingComponent(GameObject& newGameObject) : IAIComponent(newGameObject)
 {
     listWay = new std::list<GameObject*>;
 }

AIDrivingComponent::~AIDrivingComponent()
{
    delete actualWayPoint;
    delete startPoint;
    delete listWay;
}

/*
void AIDrivingComponent::addWayPoint(GameObject* way)
{
    if (totalWayPoints == 0)
    {
        startPoint = way;
        actualWayPoint = way;
    }
    else
    {
        listWay->back()->setNext(way);
    }

    listWay->push_back(way);
    listWay->back()->setNext(startPoint);

    totalWayPoints++;
}

*/

void AIDrivingComponent::setMaxSpeed(float speed)
{
    maxSpeed = speed;
}

void AIDrivingComponent::setFrame(float frame)
{
    frameDeltaTime = frame;
}

void AIDrivingComponent::setSeconds(float sec)
{
    seconds = sec;
}

glm::vec3 AIDrivingComponent::getNextPoint(glm::vec3 pos, glm::vec3 vel, float modSpeed)
{
    glm::vec3 nextPos;

    float distance = (actualWayPoint->getTransformData().position.x - pos.x) * (actualWayPoint->getTransformData().position.x - pos.x) +
                     (actualWayPoint->getTransformData().position.y - pos.y) * (actualWayPoint->getTransformData().position.y - pos.y) +
                     (actualWayPoint->getTransformData().position.x - pos.z) * (actualWayPoint->getTransformData().position.z - pos.z);

    float tour = (modSpeed * seconds)*(modSpeed * seconds);

    if(tour <= distance)
    {
        nextPos = actualWayPoint->getTransformData().position;
    }
    else
    {
        /*
		tour -= distance;
        for(std::list<GameObject*>::iterator it = listWay->begin(); it != listWay->end() && tour >= actualWayPoint->getDistNextWays(); ++it){
            tour -= actualWayPoint->getDistNextWays();
            actualWayPoint = actualWayPoint->getNext();
        }
        nextPos = (tour/actualWayPoint->getDistNextWays()) * (actualWayPoint->getNext()->getPos() - actualWayPoint->getPos()) + actualWayPoint->getPos();
		*/
	}

	std::shared_ptr<WaypointComponent> wpComponent = actualWayPoint->getComponent<WaypointComponent>();
	//auto wpComponent = actualWayPoint->getComponent<WaypointComponent>();
	if( wpComponent != nullptr ){
    	if(wpComponent->checkNext(pos, nextPos))
    	{
			actualWayPoint = wpComponent->getNext();
		}
    }
	

    return nextPos;
}

//Decides where to turn and in which grade (with a percentage of 0 to 1)
float AIDrivingComponent::girar(std::vector<VObject*> array, glm::vec3 waypoint, float distance, float a, float b, float maxR)
{	
	//APROXIMATION:
	int aproximation = 2;

	//final turn decision
	float decision = 0.0f;

	//Markers
	float INIT_GIRO = 0.0f;
	float END_GIRO = 100;
	float INIT_NONE = 80;
	float END_NONE = 120;

	float INIT_A = 0.0f;
	float END_A = maxR;
	float INIT_B = 0.0f;
	float END_B = maxR;

	//Pertenencys
	float no_turn_pertenency = 0.0f;
	float turn_pertenency = 0.0f;
	float a_pertenency = 0.0f;
	float b_pertenency = 0.0f;
	float aWP = 0.0f;
	float bWP = 0.0f;

	//auxiliar values
	float totalA = 0.0f;
	float totalB = 0.0f;


	switch(aproximation){
		//One collision, no waypoints
		case 0:
			//Turn or not to turn
			turn_pertenency = inferL(distance, INIT_GIRO, END_GIRO, 1);
			no_turn_pertenency = inferL(distance, INIT_NONE, END_NONE, 0);

			//USE A AND B AS TURN REGULATORS
			//We use the circular aproximation, since the turn must be sharper the closer to 0 the value is, and soften the turn the further away
			//from the center

			//Left turn
			a_pertenency = inferL(a, INIT_A, END_A, 2);
			
			//Right turn
			b_pertenency = inferL(b, INIT_B, END_B, 2);

			//Defuzzyfier
			if(no_turn_pertenency>turn_pertenency)
			{
				decision=0;
			}
			else
			{
				if(a>b){
					decision=-turn_pertenency*a_pertenency;
				}else{
					decision=turn_pertenency*b_pertenency;
				}
			}
			break;

		//Various collisions, a waypoint. All Collisions' A-B equal 50%, Waypoint 50% too. Linear approach.
		case 1:
			//------------Analize waypoint
			//USE A AND B AS TURN REGULATORS
			//We use the circular aproximation, since the turn must be sharper the closer to 0 the value is, and soften the turn the further away
			//from the center

			//Left and right turn in waypoint
			if(a <= 0 || b<=0){
				if(a<b){
					aWP = 1;
				}else{
					bWP = 1;
				}
			}else if(a>b){
				bWP = inferL(b/a, 0, 1, 1);
			}else{
				aWP = inferL(a/b, 0, 1, 1);
			}
			//------------Analize all obstacles
			for(unsigned i = 0; i<array.size(); i++){
				a_pertenency += inferL(array.at(i)->getA(), INIT_A, END_A, 0)/array.size();
				b_pertenency += inferL(array.at(i)->getB(), INIT_B, END_B, 0)/array.size();
			}

			if(aWP>bWP){
				decision = (-aWP + a_pertenency) * 0.5f;
			}else{
				decision = (bWP - b_pertenency) * 0.5f;
			}
			break;

		//Various collisions, a waypoint. All Collisions' A-B equal 50%, Waypoint 50% too. Circular approach.
		case 2:
			//------------Analize waypoint
			//USE A AND B AS TURN REGULATORS
			//We use the circular aproximation, since the turn must be sharper the closer to 0 the value is, and soften the turn the further away
			//from the center

			//Left and right turn in waypoint
			if(a <= 0 || b<=0){
				if(a<b){
					aWP = 1;
				}else{
					bWP = 1;
				}
			}else if(a>b){
				bWP = inferL(b/a, 0, 1, 1);
			}else{
				aWP = inferL(a/b, 0, 1, 1);
			}

			//------------Analize all obstacles
			for(unsigned i = 0; i<array.size(); i++){
				a_pertenency += inferL(array.at(i)->getA(), INIT_A, END_A, 2)/array.size();
				b_pertenency += inferL(array.at(i)->getB(), INIT_B, END_B, 2)/array.size();
			}
			
			if(aWP>bWP){
				decision = (-aWP + a_pertenency);
			}else{
				decision = (bWP - b_pertenency);
			}
			break;
		
		//Various collisions, a waypoint. Priority list based on distance to object. Waypoint also in the list.
		case 3:

			//USE A AND B AS TURN REGULATORS
			//We use the circular aproximation, since the turn must be sharper the closer to 0 the value is, and soften the turn the further away
			//from the center
			
			//------------Analize waypoint
			//Left and right turn in waypoint
			if(a <= 0 || b<=0){
				if(a<b){
					aWP = 1;
				}else{
					bWP = 1;
				}
			}else if(a>b){
				bWP = inferL(b/a, 0, 1, 1);
			}else{
				aWP = inferL(a/b, 0, 1, 1);
			}
			
			//------------Analize all obstacles

			for(unsigned i = 0; i<array.size(); i++){
				a_pertenency += inferL(array.at(i)->getA(), INIT_A, END_A, 1);
				b_pertenency += inferL(array.at(i)->getB(), INIT_B, END_B, 1);
			}

			//------------final decision
			if(totalA>totalB){
				if(aWP+a_pertenency!=0.0f){
					decision = (-aWP+a_pertenency)/(aWP+a_pertenency);
				}else{
					decision = 0;
				}
					
			}else{
				if(bWP+b_pertenency!=0.0f){
					decision = (bWP-b_pertenency)/(bWP+b_pertenency);
				}else{
					decision = 0;
				}
			}

			break;
	}
	return decision;

}

float AIDrivingComponent::acelerar_frenar(std::vector<VObject*> array, glm::vec3 waypoint, float a, float b, float maxR)
{
	//final turn decision
	float decision = 0.0f;

	return decision;
}

//Inferes the fuzzy value in a line with the type given
float AIDrivingComponent::inferL(float value, float limit1, float limit2, int type){
	//Switching the type
	switch(type){
		//Lineal case
		case 0:
			if(value>limit2)
			{
				return 1;
			}
			else if(value<limit1)
			{
				return 0;
			}
			else
			{
				return (value/(limit2-limit1));
			}
			break;

		//Inverse line case
		case 1:
			if(value>limit2)
			{
				return 0;
			}
			else if(value<limit1)
			{
				return 1;
			}
			else
			{
				return (1-value/(limit2-limit1));
			}
			break;

		//Circular line case
		case 2:
			if(value>limit2)
			{
				return 1;
			}
			else if(value<limit1)
			{
				return 0;
			}
			else
			{
				
				float cos=value/(limit2-limit1);
				return sqrt(1-cos*cos);
			}

			break;
		//Inverse circular line case
		case 3:
			if(value>limit2)
			{
				return 0;
			}
			else if(value<limit1)
			{
				return 1;
			}
			else
			{
				
				float cos=1-value/(limit2-limit1);
				return sqrt(1-cos*cos);
			}

			break;

	}
	return -1;
}

//Inferes the fuzzy value in a triangular function given the parameters
float AIDrivingComponent::inferT(float value, float limit1, float limit2, float limit3){
	if(value<limit1){
		return 0;
	}else if(value>limit3){
		return 0;
	}else if(limit1<=value && value<limit2){
		return value/(limit2-limit1);
	}else if(limit2<=value && value<=limit3){
		return 1-value/(limit3-limit2);
	}
	return -1;
}

//Inferes the fuzzy value in a trapeizodal function
float AIDrivingComponent::inferT2(float value, float limit1, float limit2, float limit3, float limit4){
	if(value<limit1){
		return 0;
	}else if(value>limit4){
		return 0;
	}else if(limit1<=value && value<limit2){
		return value/(limit2-limit1);
	}else if(limit2<=value && value<limit3){
		return 1;
	}else if(limit3<=value && value<=limit4){
		return 1-value/(limit4-limit3);
	}
	return -1;
}

//Inferes the fuzzy value with a circular inference
float AIDrivingComponent::inferC(float value, float limit1, float limit2, float limit3){
		if(value<limit1){
			return 0;
		}else if(value>limit3){
			return 0;
		}else if(limit1<=value && value<limit2){
			float cos=value/(limit2-limit1);
			return sqrt(1-cos*cos);
		}else if(limit2<=value && value<=limit3){
			float cos=1-value/(limit2-limit1);
			return sqrt(1-cos*cos);
		}
	return -1;
}