#include "FuzzyLogic.h"
#include "VObject.h"
#include <iostream>

double FuzzyLogic::girar(int distance, double a, double b, float maxR)
{	
	double decision = 0;

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

	//No turn pertenency
	if(distance>END_GIRO)
	{
		turn_pertenency=0;
	}
	else if(distance<INIT_GIRO)
	{
		turn_pertenency=1;
	}
	else
	{
		turn_pertenency=distance/(END_GIRO-INIT_GIRO);
		turn_pertenency=1-turn_pertenency;
	}


	//hard turn pertenency
	if(distance>END_NONE)
	{
		no_turn_pertenency=1;
	}
	else if(distance<INIT_NONE){
		no_turn_pertenency = 0;
	}
	else
	{
		no_turn_pertenency=distance/(END_NONE-INIT_NONE);
	}

	//USE A AND B AS TURN REGULATORS
	//We use the circular aproximation, since the turn must be sharper the closer to 0 the value is, and soften the turn the further away
	//from the center

	//Left turn expectance
	if(distance>END_A)
	{
		a_pertenency=0;
	}
	else if(distance<=INIT_A)
	{
		a_pertenency=1;
	}
	else
	{
		
		float a_cos=distance/(END_A-INIT_A);
		a_pertenency=sqrt(1-a_cos*a_cos);
	}

	//Right turn expectancy
	if(distance>END_B)
	{
		b_pertenency=0;
	}
	else if(distance<=INIT_B)
	{
		b_pertenency=1;
	}
	else
	{
		float b_cos=distance/(END_B-INIT_B);
		b_pertenency=sqrt(1-b_cos*b_cos);
	}

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

int FuzzyLogic::acelerar_frenar(int distance)
{
	int decision = 2;

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

FuzzyLogic::FuzzyLogic(){

}

FuzzyLogic::~FuzzyLogic(){

}