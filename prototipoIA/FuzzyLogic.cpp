
class MyFuzzyLogic
{
public:

	static double girar(int distance, double a, double b)
	{	
		double decision = 0;

		//Markers
		float INIT_GIRO = 0;
		float END_GIRO = 100;
		float INIT_NONE = 80;
		float END_NONE = 120;	

		//Pertenencys
		float no_turn_pertenency=0;
		float turn_pertenency=0;
		//Markers
		float INIT_GIRO = 0;
		float END_GIRO = 100;
		float INIT_NONE = 80;
		float END_NONE = 120;	

		//Pertenencys
		float no_turn_pertenency=0;
		float turn_pertenency=0;

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

		//Soft turn PERTENENCY
		/*if(distance>END_NONE2)
		{
			softturn_pertenency=0;
		}else if(distance>INIT_NONE2)
		{
			softturn_pertenency = distance/(END_NONE2-INIT_NONE2);
			softturn_pertenency = 1-softturn_pertenency;
		}else if(distance>END_NONE1)
		{
			softturn_pertenency=1;
		}
		else if(distance>INIT_NONE1)
		{
			softturn_perte	if(distance>END_GIRO)
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

		//Soft turn PERTENENCY
		/*if(distance>END_NONE2)
		{
			softturn_pertenency=0;
		}else if(distance>INIT_NONE2)
		{
			softturn_pertenency = distance/(END_NONE2-INIT_NONE2);
			softturn_pertenency = 1-softturn_pertenency;
		}else if(distance>END_NONE1)
		{
			softturn_pertenency=1;
		}
		else if(distance>INIT_NONE1)
		{
			softturn_pertenency = distance/(END_NONE1-INIT_NONE1);
		}
		else
		{
			softturn_pertenency=0;
		}*/

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


		//Defuzzyfier
		if(no_turn_pertenency>turn_pertenency)
		{
			decision=0;
		}
		else
		{
			if(a>b){
				decision=-turn_pertenency;
			}else{
				decision=turn_pertenency;
			}
		}
	
		return decision;

	}

	static int acelerar_frenar(int distance)
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

		//Soft turn PERTENENCY
		/*if(distance>END_NONE2)
		{
			softturn_pertenency=0;
		}else if(distance>INIT_NONE2)
		{
			softturn_pertenency = distance/(END_NONE2-INIT_NONE2);
			softturn_pertenency = 1-softturn_pertenency;
		}else if(distance>END_NONE1)
		{
			softturn_pertenency=1;
		}
		else if(distance>INIT_NONE1)
		{
			softturn_perte	if(distance>END_GIRO)
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

		//Soft turn PERTENENCY
		/*if(distance>END_NONE2)
		{
			softturn_pertenency=0;
		}else if(distance>INIT_NONE2)
		{
			softturn_pertenency = distance/(END_NONE2-INIT_NONE2);
			softturn_pertenency = 1-softturn_pertenency;
		}else if(distance>END_NONE1)
		{
			softturn_pertenency=1;
		}
		else if(distance>INIT_NONE1)
		{
			softturn_pertenency = distance/(END_NONE1-INIT_NONE1);
		}
		else
		{
			softturn_pertenency=0;
		}*/

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


		//Defuzzyfier
		if(no_turn_pertenency>turn_pertenency)
		{
			decision=0;
		}
		else
		{
			if(a>b){
				decision=-turn_pertenency;
			}else{
				decision=turn_pertenency;
			}
		}
	
		return decision;

	}

	static int acelerar_frenar(int distance)
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

}; 
