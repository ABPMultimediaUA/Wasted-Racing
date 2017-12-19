#include "AIDrivingComponent.h"
#include "WaypointComponent.h"
#include "../GameManager/WaypointManager.h"
#include <memory>
#include <iostream>

//Initilizer
void AIDrivingComponent::init() {
}

//Update
void AIDrivingComponent::update(float dTime) {
}

//Closer
void AIDrivingComponent::close() {
}


void AIDrivingComponent::checkList()
{
	auto wpManager = &WaypointManager::getInstance();

    std::vector<GameObject::Pointer> listNodes = wpManager->getWaypoints(); //Check if i can to use without this

	for (size_t i = 0; i < listNodes.size(); i++)
	{
		std::cout<<"WAYPOINT POS: "<<listNodes[i].get()->getTransformData().position.x<<"\n";
	}
}

//Decides where to turn and in which grade (with a percentage of 0 to 1)

/*//APARTADO DE MEJORAS//////
>Decidir si girar según los enemigos cercanos
>Diferenciar entre tipos de objetos
>Tener en cuenta físicas del terreno
>Personalidad agresiva o precavida
>Añadir para hacer drifting

//---------------------------*/
float AIDrivingComponent::girar(std::vector<VObject::Pointer> array, glm::vec3 waypoint, float a, float b)
{	
	//final turn decision
	float decision = 0.0f;

	//Fuzzy decisions
	float steeringNone = 0.0f, steeringLeft = 0.0f, steeringRight = 0.0f;

	//calculate the arctg being a the right side, then b over a is the right choice. Returns in radians.
	float atan_w = glm::atan(a,b)/3.14159265358979323846264338327f;

	//-----------_TESTS_-----------
	/*
	std::cout<<"Waypoint: "<<waypoint.x<<","<<waypoint.z<<std::endl;
	std::cout<<"Left side: "<<a<<std::endl;
	std::cout<<"Right side: "<<b<<std::endl;
	std::cout<<"ATAN: "<<atan_w<<std::endl;
	*/
	//-----------_TESTS_-----------

	//fuzzifier and inference
	//---------------GENERALIZE--v-------v----v
	//waypoints
	if(atan_w <=-0.75){ //Since the value of the atan 0.25 corresponds to the center, from 1 to 0.25 is left, from 0.25 to -0.75 is right, and -0.75 to -1 is left.
		atan_w += 2.f; 	//that's why we add this
	}
	float wp_left 	= inferL(atan_w		,0.25f  ,1.25f 	,0   	);
	float wp_center = inferT(atan_w		,0.23f	,0.25f 	,0.27f	);
	float wp_right 	= inferL(atan_w		,-0.75f	,0.25f  ,1   	);

	//If we have collisions to collide with
	if(array.size()>0){
		float atan_obs = 0.0f;

		//Accumulate the mean atan value of them all to pickpoint a medium point of no collisions
		for(unsigned i = 0; i<array.size(); ++i){
			atan_obs += (glm::atan(array[i].get()->getA(),array[i].get()->getB()) / 3.14159265358979323846264338327f )/array.size();
		}

		//collisions
		if(atan_obs <=-0.75){ 	//Same process as the waypoint
			atan_obs += 2.f;
		}
		float obs_left	 	= inferL(atan_obs		,0.25f  ,1.25f 	,0   	);
		float obs_center 	= inferT(atan_obs		,0.23f	,0.25f 	,0.27f	);
		float obs_right 	= inferL(atan_obs		,-0.75f	,0.25f  ,1   	);

		//-----------_TESTS_-----------
		std::cout<<"Obstacle values: "<<obs_left<<" - "<<obs_center<< " - " << obs_right<<std::endl;
		//-----------_TESTS_-----------


		//Apply ruleset.
		/*
		ORIGINAL APPROACH
		
		steeringLeft  = wp_center > obs_center ? wp_center : obs_center;
		steeringRight = wp_center > obs_left   ? wp_center : obs_left;
		steeringLeft  = wp_center > obs_right  ? wp_center : obs_right;
		steeringLeft  = wp_left   > obs_center ? wp_left   : obs_center;
		steeringNone  = wp_left   > obs_left   ? wp_left   : obs_left;
		steeringLeft  = wp_left   > obs_right  ? wp_left   : obs_right;
		steeringRight = wp_right  > obs_center ? wp_right  : obs_center;
		steeringRight = wp_right  > obs_left   ? wp_right  : obs_left;
		steeringNone  = wp_right  > obs_right  ? wp_right  : obs_right;
		*/

		//New Iteration approach
		steeringLeft = glm::min( glm::max(wp_left, wp_center), glm::max(obs_center, obs_right) );
		steeringNone = glm::min( glm::max(1-wp_left, wp_center, 1-wp_right), glm::max(obs_left, obs_right) );
		steeringRight = glm::min( glm::max(wp_right, wp_center), glm::max(obs_center,obs_left) );

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

	centroidT(&op1_cx, &op1_cy, &op1_area, steeringNone, -0.15f, 0.f, 0.15f);
	centroidT(&op2_cx, &op2_cy, &op2_area, steeringRight, -1.f, -0.995f, -0.05f);
	centroidT(&op3_cx, &op3_cy, &op3_area, steeringLeft, 0.05f, 0.995f, 1.0f);

	//adding all the centroids and crisping end result
	float cx = (op1_cx * op1_area + op2_cx * op2_area + op3_cx * op3_area ) / (op1_area + op2_area + op3_area);
	//float cy = (op1_cy * op1_area + op2_cy * op2_area + op3_cy * op3_area ) / (op1_area + op2_area + op3_area);

	//-----------_TESTS_-----------
	/*
	std::cout<<"Donde vas payo: "<<cx<<std::endl;
	std::cout<<"Centro: "<<op1_cx<<", dech: "<<op2_cx<<", izq: "<<op3_cx<<std::endl;
	std::cout<<"Center area: "<<op1_area<<", right area: "<<op2_area<<", left area: "<<op3_area<<std::endl;
	*/
	//-----------_TESTS_-----------

	decision = cx;

	return decision;

}


//Decides wheter the NPC should brake, do nothing or accelerate, and in which proportion. Takes in account where objects are, distance to closest one, where the NPC
//is going, and where it is headed to.
/*//APARTADO DE MEJORAS//////
>Añadir que si tienes que girar demasiado a la derecha o izquierda para llegar a tu objetivo, que frenes
>Añadir para hacer drifting

//---------------------------*/
float AIDrivingComponent::acelerar_frenar(std::vector<VObject::Pointer> array, float direction, float speed, float a_W, float b_w)
{
	//final turn decision
	float decision = 0.0f;

	float accelerating, none, braking;

	//fuzzifier and inference
	//---------------GENERALIZE--v-------v----v
	//Where am I going
	float going_left 	= inferL(direction,	0.2f,	1.0f,	0		);
	float going_center 	= inferT(direction,-0.3f,	0.0f,	0.3f	);
	float going_right 	= inferL(direction,-1.0f,	-0.2f,	1		);

	//Correlation between how much more do I need to rotate to arrive and my current speed
	/*float more_turn_left = inferT();
	float no_more_turn = inferT();
	float more_turn_right = inferT();*/

	//If there are objects to collide with
	if(array.size()>0 && speed!=0.0f){

		//Previous calculus
		float atan_obs = 0.0f, min_value = FLT_MAX;
		for(unsigned i = 0; i<array.size(); i++){
			atan_obs += (glm::atan(array[i].get()->getA(),array[i].get()->getB()) / 3.14159265358979323846264338327f )/array.size();
			min_value = glm::min(min_value,array[i].get()->getA()+array[i].get()->getB());
		}

		//Dividing between speed to get a time of impact
		min_value = (min_value >0 && speed > 0) ? min_value / speed : 0.f;
		
		//collisions
		//-----------------------------------
		float obs_left = inferL(atan_obs,0.3f,0.51f, 0);
		float obs_center = inferT(atan_obs,0.1f,0.25f,0.4f);
		float obs_right = inferL(atan_obs,-0.01f,0.2f, 1);

		//-----------------------------------

		//distance
		//-----------------------------------
		float obs_closeRange = inferL(min_value,0.0f,1.f, 1);
		float obs_mediumRange = inferT(min_value,0.5f,1.f,2.f);
		float obs_farRange = inferL(min_value,1.8f,10.f,1);

		//Ruleset

		accelerating = glm::min(
							obs_farRange, 
							glm::max(
								glm::min(obs_left,1-going_left), glm::min(obs_right, 1-going_right)
							)
					   ); //Acelerar cuando no hay objetos cerca y no estamos en su rumbo de colisión

		none =  glm::min(
					glm::max(
						glm::max(glm::min(obs_left,going_left), glm::min(obs_right, going_right)), glm::min(obs_center,going_center)
					), 
					glm::max(obs_mediumRange,obs_farRange)
				); //No aumentar la velocidad cuando estamos en el rumbo de colisión pero están lejos o a media distancia

		braking = glm::min(
						glm::max(
							glm::max(glm::min(obs_left,going_left), glm::min(obs_right, going_right)), glm::min(obs_center,going_center)
						), 
						obs_closeRange
				); //Frenar cuando vamos en rumbo de colisión y están cerca los objetos
	
		if(accelerating == 0 && none == 0 && braking == 0){
			accelerating = 1.f;
			none = 0.f;
			braking = 0.f;
		}

	//if there are no objects to collide with
	}else{
		//Ruleset

		accelerating = 1.0f;
		none = 0.0f;
		braking = 0.0f;

	}

	//defuzzifier inference
	//Here we use the centroid point between the defuzzified inferences, to pinpoint the crisp steering value
	//---------------GENERALIZE---everything
	float op1_cx, op1_cy, op1_area, op2_cx, op2_cy, op2_area, op3_cx, op3_cy, op3_area;

	centroidT(&op1_cx, &op1_cy, &op1_area, none, -0.3f, 0.f, 0.3f);
	centroidT(&op2_cx, &op2_cy, &op2_area, braking, -1.f, -0.99f, -0.05f);
	centroidT(&op3_cx, &op3_cy, &op3_area, accelerating, 0.2f, 0.99f, 1.0f);

	//adding all the centroids and crisping end result
	float cx = (op1_cx * op1_area + op2_cx * op2_area + op3_cx * op3_area ) / (op1_area + op2_area + op3_area);
	//float cy = (op1_cy * op1_area + op2_cy * op2_area + op3_cy * op3_area ) / (op1_area + op2_area + op3_area);	
	decision = cx;

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
	return -1;
}

//Inferes the fuzzy value in a triangular function given the parameters
float AIDrivingComponent::inferT(float value, float limit1, float limit2, float limit3){
	if(value<limit1){
		return 0.0f;
	}else if(value>limit3){
		return 0.0f;
	}else if(limit1<=value && value<limit2){
		return (value-limit1)/(limit2-limit1);
	}else if(limit2<=value && value<=limit3){
		return 1-(value-limit2)/(limit3-limit2);
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
		return (value-limit1)/(limit2-limit1);
	}else if(limit2<=value && value<limit3){
		return 1;
	}else if(limit3<=value && value<=limit4){
		return 1-(value-limit3)/(limit4-limit3);
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
			float cos=(value-limit1)/(limit2-limit1);
			return sqrt(1-cos*cos);
		}else if(limit2<=value && value<=limit3){
			float cos=1-(value-limit1)/(limit2-limit1);
			return sqrt(1-cos*cos);
		}
		return -1;
}

//--------------------------------------------
//DEFUZZING FORMULAE OF HELL: TRAPEZOIDS REILM
//--------------------------------------------

//Returns in Cx and Cy the triangle's X,Y positions of its centroid, given the height h
void AIDrivingComponent::centroidT(float* cx, float* cy, float* area, float h, float limit1, float limit2, float limit3){
	//if the height is 1, then it shall be a triangle
	if(h == 0.0f){
		*cx = 0.0f;
		*cy = 0.0f;
		*area = 0.0f;
	}else{
		if(h == 1.f){
			
			//float y1 = 0.f, y3 = 0.f, y2 = 0.5f, y4= 0.5f;
			//float x1 = limit1, x3 = limit3, x4 = (limit1+limit2)*0.5f, x2=(limit3+limit2)*0.5f;
			
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
void AIDrivingComponent::centroidT2(float* cx, float* cy, float* area, float h, float limit1, float limit2, float limit3, float limit4){
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