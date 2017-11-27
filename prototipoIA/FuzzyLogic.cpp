#include "FuzzyLogic.h"
#include <iostream>

//Decides where to turn and in which grade (with a percentage of 0 to 1)
float FuzzyLogic::girar(glm::vec3 waypoint, float distance, float a, float b, float maxR)
{	
	//final turn decision
	float decision = 0;

	//Markers
	float INIT_GIRO = 0;
	float END_GIRO = 100;
	float INIT_NONE = 80;
	float END_NONE = 120;

	float INIT_A = 0;
	float END_A = maxR;
	float INIT_B = 0;
	float END_B = maxR;

	//Pertenencys
	float no_turn_pertenency=0;
	float turn_pertenency=0;
	float a_pertenency = 0;
	float b_pertenency = 0;

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

	std::cout<<"Probando modelo: "<<decision<<" con esperanza de"<<a_pertenency<<","<<b_pertenency<<std::endl;

	return decision;

}

float FuzzyLogic::acelerar_frenar(int distance)
{
	float decision = 2;

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
	if(distance>END_ACCEL)
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
	}

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

}

//Inferes the fuzzy value in a trapeizodal function
float FuzzyLogic::inferT2(float value, float limit1, float limit2, float limit3, float limit4){

}

//Inferes the fuzzy value with a circular inference
float FuzzyLogic::inferC(float value, float limit1, float limit2, float limit3){

}

FuzzyLogic::FuzzyLogic(){

}

FuzzyLogic::~FuzzyLogic(){

}