#include "FuzzyLogic.h"
#include <iostream>

//Decides where to turn and in which grade (with a percentage of 0 to 1)
float FuzzyLogic::girar(std::vector<VObject*> array, glm::vec3 waypoint, float distance, float a, float b, float maxR)
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
		//Various collisions, a waypoint. All Collisions' A-B equal 50%, Waypoint 50% too. Linear approach.
		case 0:
			//------------Analize waypoint
			//USE A AND B AS TURN REGULATORS
			//We use the circular aproximation, since the turn must be sharper the closer to 0 the value is, and soften the turn the further away
			//from the center

			//Left and right turn in waypoint
			std::cout<<"WayPoint direction: "<<b<<","<<a<<"\n";
			if(a <= 0 || b<=0){
				if(a<b){
					aWP = 1;
				}else{
					bWP = 1;
				}
			}else if(a>b){
				bWP = 1-b/a;
			}else{
				aWP = 1-a/b;
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
		case 1:
			//------------Analize waypoint
			//USE A AND B AS TURN REGULATORS
			//We use the circular aproximation, since the turn must be sharper the closer to 0 the value is, and soften the turn the further away
			//from the center

			//Left and right turn in waypoint
			std::cout<<"WayPoint direction: "<<b<<","<<a<<"\n";
			if(a <= 0 || b<=0){
				if(a<b){
					aWP = 1;
				}else{
					bWP = 1;
				}
			}else if(a>b){
				bWP = 1-b/a;
			}else{
				aWP = 1-a/b;
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
		case 2:

			//USE A AND B AS TURN REGULATORS
			//We use the circular aproximation, since the turn must be sharper the closer to 0 the value is, and soften the turn the further away
			//from the center
			
			//------------Analize waypoint
			//Left and right turn in waypoint
			std::cout<<"WayPoint direction: "<<b<<","<<a<<"\n";
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

	std::cout<<"Probando modelo: "<<decision<<" con la aproximacion "<<aproximation<<std::endl;
	std::cout<<"wayPoint: "<<bWP<<" , "<<aWP<<std::endl;
	//std::cout<<"Colisiones: "<<a_pertenency<<" , "<<b_pertenency<<std::endl;
	return decision;

}

float FuzzyLogic::acelerar_frenar(std::vector<VObject*> array, glm::vec3 waypoint, float a, float b, float maxR)
{
	//final turn decision
	float decision = 0.0f;

	//Markers
	float INIT_BRAKE = 0;
	float END_BRAKE = 40;
	float INIT_NONE1 = 30;
	float END_NONE1 = 50;
	float INIT_NONE2 = 100;
	float END_NONE2 = 120;
	float INIT_ACCEL = 40;
	float END_ACCEL = 130;

	//Pertenencys
	float brake_pertenency=0;
	float accelerate_pertenency=0;
	float none_pertenency=0;



	//ACCELERATION PERTENENCY
	/*if(distance>END_ACCEL)
	{
		accelerate_pertenency=1;
	}
	else if(distance<INIT_ACCEL)
	{
		accelerate_pertenency=0;
	}
	else
	{
		accelerate_pertenency=distance/(END_ACCEL-INIT_ACCEL);
	}

	//BRAKE_PERTENENCY
	if(distance>END_BRAKE)
	{
		brake_pertenency=0;
	}
	else
	{
		brake_pertenency=distance/(END_BRAKE-INIT_BRAKE);
		brake_pertenency=1-brake_pertenency;
	}


	//defuzzyfier

	if(accelerate_pertenency>none_pertenency)
	{
		decision=1;
	}
	else if(none_pertenency>accelerate_pertenency && none_pertenency>brake_pertenency)
	{
		decision=2;
	}
	else if(brake_pertenency>none_pertenency)
	{
		decision=0;
	}*/

	return decision;
}

//Inferes the fuzzy value in a line with the type given
float FuzzyLogic::inferL(float value, float limit1, float limit2, int type){
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
}

//Inferes the fuzzy value in a triangular function given the parameters
float FuzzyLogic::inferT(float value, float limit1, float limit2, float limit3){
	if(value<limit1){
		return 0;
	}else if(value>limit3){
		return 0;
	}else if(limit1<=value<limit2){
		return value/(limit2-limit1);
	}else if(limit2<=value<=limit3){
		return 1-value/(limit3-limit2);
	}
}

//Inferes the fuzzy value in a trapeizodal function
float FuzzyLogic::inferT2(float value, float limit1, float limit2, float limit3, float limit4){
	if(value<limit1){
		return 0;
	}else if(value>limit4){
		return 0;
	}else if(limit1<=value<limit2){
		return value/(limit2-limit1);
	}else if(limit2<=value<limit3){
		return 1;
	}else if(limit3<=value<=limit4){
		return 1-value/(limit4-limit3);
	}
}

//Inferes the fuzzy value with a circular inference
float FuzzyLogic::inferC(float value, float limit1, float limit2, float limit3){
		if(value<limit1){
			return 0;
		}else if(value>limit3){
			return 0;
		}else if(limit1<=value<limit2){
			float cos=value/(limit2-limit1);
			return sqrt(1-cos*cos);
		}else if(limit2<=value<=limit3){
			float cos=1-value/(limit2-limit1);
			return sqrt(1-cos*cos);
		}
}

FuzzyLogic::FuzzyLogic(){

}

FuzzyLogic::~FuzzyLogic(){

}