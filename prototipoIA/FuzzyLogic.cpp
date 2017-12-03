#include "FuzzyLogic.h"
#include <iostream>

//Decides where to turn and in which grade (with a percentage of 0 to 1)
float FuzzyLogic::girar(std::vector<VObject*> array, glm::vec3 waypoint, float distance, float a, float b, float maxR)
{	
	//APROXIMATION:
	int aproximation = 3;

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
			//We use the circular aproximation, since the turn mulst be sharper the closer to 0 the value is, and soften the turn the further away
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

		//Real Fuzzy Logic - Cheap aproximation with angles, tangents and regret
		case 3:
			float steeringNone, steeringLeft, steeringRight;

			//calculate the arctg being a the right side, then b over a is the right choice. Returns in radians.
			float atan_w = glm::atan(a,b)/3.14159265358979323846264338327f;

			//fuzzifier and inference
			//---------------GENERALIZE--v-------v----v
			//waypoints
			float wp_left = inferT(atan_w,0.25f,0.375f,0.51f);
			float wp_center = inferT(atan_w,0.2f,0.25f,0.3f);
			float wp_right = inferT(atan_w,-0.01f,0.125f,0.25f);

			if(atan_w<0 || atan_w>0.51){
				if(a<b){
					wp_right = 1.f;
				}else{
					wp_left = 1.f;
				}
			}

			if(array.size()>0){
				float atan_obs = 0.0f;
				for(unsigned i = 0; i<array.size(); i++){
					atan_obs += (glm::atan(array.at(i)->getA(),array.at(i)->getB()) / 3.14159265358979323846264338327f )/array.size();
				}

				//collisions
				float obs_left = inferT(atan_obs,0.25f,0.375f,0.51f);
				float obs_center = inferT(atan_obs,0.2f,0.25f,0.3f);
				float obs_right = inferT(atan_obs,-0.01f,0.125f,0.25f);

				if(atan_obs<0 || atan_obs>0.51){
					if(a<b){
						obs_right = 1.f;
					}else{
						obs_left = 1.f;
					}
				}

				steeringLeft  = wp_center > obs_center ? wp_center : obs_center;
				steeringRight = wp_center > obs_left   ? wp_center : obs_left;
				steeringLeft  = wp_center > obs_right  ? wp_center : obs_right;
				steeringLeft  = wp_left   > obs_center ? wp_left   : obs_center;
				steeringNone  = wp_left   > obs_left   ? wp_left   : obs_left;
				steeringLeft  = wp_left   > obs_right  ? wp_left   : obs_right;
				steeringRight = wp_right  > obs_center ? wp_right  : obs_center;
				steeringRight = wp_right  > obs_left   ? wp_right  : obs_left;
				steeringNone  = wp_right  > obs_right  ? wp_right  : obs_right;

			}else{
				//ruleset
				//---------------GENERALIZE----conjunction and function result

				steeringLeft = wp_left;
				steeringNone = wp_center;
				steeringRight = wp_right;
			}

			//defuzzifier inference
			//Here we use the centroid point between the defuzzified inferences, to pinpoint the crisp steering value
			//---------------GENERALIZE---everything
			float op1_cx, op1_cy, op1_area, op2_cx, op2_cy, op2_area, op3_cx, op3_cy, op3_area;

			centroidT(&op1_cx, &op1_cy, &op1_area, steeringNone, -0.2f, 0.f, 0.2f);
			centroidT(&op2_cx, &op2_cy, &op2_area, steeringRight, -1.f, -0.95f, -0.05f);
			centroidT(&op3_cx, &op3_cy, &op3_area, steeringLeft, 0.05f, 0.95f, 1.0f);

			std::cout<<"Centro: "<<op1_cx<<", dech: "<<op2_cx<<", izq: "<<op3_cx<<std::endl;
			std::cout<<"Center area: "<<op1_area<<", right area: "<<op2_area<<", left area: "<<op3_area<<std::endl;

			//adding all the centroids and crisping end result
			float cx = (op1_cx * op1_area + op2_cx * op2_area + op3_cx * op3_area ) / (op1_area + op2_area + op3_area);
			float cy = (op1_cy * op1_area + op2_cy * op2_area + op3_cy * op3_area ) / (op1_area + op2_area + op3_area);

			std::cout<<"Donde vas payo: "<<cx<<std::endl;
			
			decision = cx;

			break;
	}

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
				return ((value-limit1)/(limit2-limit1));
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
				return 1-(value-limit1)/(limit2-limit1);
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
				
				float cos=(value-limit1)/(limit2-limit1);
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
				
				float cos=1-(value-limit1)/(limit2-limit1);
				return sqrt(1-cos*cos);
			}

			break;

	}
}

//Inferes the fuzzy value in a triangular function given the parameters
float FuzzyLogic::inferT(float value, float limit1, float limit2, float limit3){
	if(value<limit1){
		return 0.0f;
	}else if(value>limit3){
		return 0.0f;
	}else if(limit1<=value && value<limit2){
		return (value-limit1)/(limit2-limit1);
	}else if(limit2<=value && value<=limit3){
		return 1-(value-limit2)/(limit3-limit2);
	}
}

//Inferes the fuzzy value in a trapeizodal function
float FuzzyLogic::inferT2(float value, float limit1, float limit2, float limit3, float limit4){
	if(value<limit1){
		return 0;
	}else if(value>limit4){
		return 0;
	}else if(limit1<=value && value<limit2){
		return (value-limit1)/(limit2-limit1);
	}else if(limit2<=value && value<limit3){
		return 1;
	}else if(limit3<=value && value<=limit4){
		return 1-(value-limit3)/(limit4-limit3);
	}
}

//Inferes the fuzzy value with a circular inference
float FuzzyLogic::inferC(float value, float limit1, float limit2, float limit3){
		if(value<limit1){
			return 0;
		}else if(value>limit3){
			return 0;
		}else if(limit1<=value && value<limit2){
			float cos=(value-limit1)/(limit2-limit1);
			return sqrt(1-cos*cos);
		}else if(limit2<=value && value<=limit3){
			float cos=1-(value-limit1)/(limit2-limit1);
			return sqrt(1-cos*cos);
		}
}

//--------------------------------------------
//DEFUZZING FORMULAE OF HELL: TRAPEZOIDS REILM
//--------------------------------------------

//Returns in Cx and Cy the triangle's X,Y positions of its centroid, given the height h
void FuzzyLogic::centroidT(float* cx, float* cy, float* area, float h, float limit1, float limit2, float limit3){
	//if the height is 1, then it shall be a triangle
	if(h == 0.0f){
		*cx = 0.0f;
		*cy = 0.0f;
		*area = 0.0f;
	}else{
		if(h == 1.f){
			
			float y1 = 0.f, y3 = 0.f, y2 = 0.5f, y4= 0.5f;
			float x1 = limit1, x3 = limit3, x4 = (limit1+limit2)*0.5f, x2=(limit3+limit2)*0.5f;
			
			float m = 0.5f / ((limit3+limit2)*0.5f-limit1);
			float m2 = 0.5f / ((limit1+limit2)*0.5f-limit3);
			float n = - m * limit1;
			float n2 = -m2 * limit3;
			
			//Triangle Cx
			*cx = (n - n2) /(m2 - m);;

			//Triangle Cy
			*cy = m * (*cx) + n;

			//triangle area
			*area = (1+limit3-limit1)*0.5f;



		//if not, it is a trapezoid, and it shall be treated as one
		}else{
			float c = limit2 - (limit2-limit1) * (1 - h);
			float a = (limit3-limit2) * (1 - h ) + limit2 - c;
			float b = limit3-limit1;

			//Friggin' trapezoid Cx
			*cx = limit1 + (2*a*c + a*a + c*b + a*b + b*b) / (3*(a+b));
			
			//Friggin' trapezoid Cy
			*cy = h * (2*a + b) / (3*(a + b));

			//Trapezoid Area
			*area = h * (a + b)*0.5f;
		}
	}
}

//Returns in Cx and Cy the trapezoid's X,Y positions of its centroid, given the height h
void FuzzyLogic::centroidT2(float* cx, float* cy, float* area, float h, float limit1, float limit2, float limit3, float limit4){
	if(h == 0.0f){
		*cx = 0.0f;
		*cy = 0.0f;
		*area = 0.0f;
	}else{
		float c = limit2-limit1;
		float a = limit3-limit2;
		float b = limit4-limit1;

		*cx = limit1 + (2*a*c + a*a + c*b + a*b + b*b) / (3*(a+b));
		*cy = h * (2*a + b) / (3*(a + b));
		*area = h * (a + b)*0.5f;
	}
}



//--------------------------------------------
//Let them exist, shall they fuzzily breath
//--------------------------------------------
FuzzyLogic::FuzzyLogic(){

}

FuzzyLogic::~FuzzyLogic(){

}