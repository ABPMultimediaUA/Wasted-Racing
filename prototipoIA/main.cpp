/** Example 004 Movement

This Tutorial shows how to move and animate SceneNodes. The
basic concept of SceneNodeAnimators is shown as well as manual
movement of nodes using the keyboard.  We'll demonstrate framerate
independent movement, which means moving by an amount dependent
on the duration of the last run of the Irrlicht loop.

Example 19.MouseAndJoystick shows how to handle those kinds of input.

As always, I include the header files, use the irr namespace,
and tell the linker to link with the .lib file.
*/
#ifdef _MSC_VER
// We'll also define this to stop MSVC complaining about sprintf().
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(../Game/lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>
#include "driverChoice.h"
#include <irrMath.h>
#include <LAPALPhysics.h>
#include <glm/glm.hpp>

using namespace irr;

//OWN CODE
//_______TESTING

class MyFuzzyLogic
{
public:

	static int girar(int distance)
	{	
		int decision = 0;

		//Markers
		float INIT_BRAKE = 0;
		float END_BRAKE = 40;
		float INIT_NONE1 = 30;
		float END_NONE1 = 50;
		float INIT_NONE2 = 100;
		float END_NONE2 = 120;
		float INIT_ACCEL = 110;
		float END_ACCEL = 130;		

		//Pertenencys
		float noturn_pertenency=0;
		float softturn_pertenency=0;
		float hardturn_pertenency=0;

		//No turn pertenency
		if(distance>END_ACCEL)
		{
			noturn_pertenency=1;
		}
		else if(distance<INIT_ACCEL)
		{
			noturn_pertenency=0;
		}
		else
		{
			noturn_pertenency=distance/(END_ACCEL-INIT_ACCEL);
		}

		//Soft turn PERTENENCY
		if(distance>END_NONE2)
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
		}

		//hard turn pertenency
		if(distance>END_BRAKE)
		{
			hardturn_pertenency=0;
		}
		else
		{
			hardturn_pertenency=distance/(END_BRAKE-INIT_BRAKE);
			hardturn_pertenency=1-hardturn_pertenency;
		}


		//Defuzzyfier
		if(noturn_pertenency>softturn_pertenency)
		{
			decision=0;
		}
		else if(softturn_pertenency>noturn_pertenency && softturn_pertenency>hardturn_pertenency)
		{
			decision=1;
		}
		else if(hardturn_pertenency>softturn_pertenency)
		{
			decision=2;
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

class Sensor{
public:
    
    core::vector3df position;
    double angleVision = 0;
    float maxRadius = 0;
    
    Sensor( core::vector3df p, double ang, float maxR){
        position = p;
        angleVision = ang;
        maxRadius = maxR;
    }
    
    ~Sensor(){
    };
    
    //Detect if point is in the field of view
    bool detectFieldVision( core::vector3df vel,  core::vector3df p){
        //Calculate field of view left and right vectores, given the angle apart of the velocity vector and the position of the object
        core::vector3df sensorLeft = position + core::vector3df(vel.X * sin(angleVision), vel.Y, vel.Z * cos(angleVision));
        core::vector3df sensorRight = position + core::vector3df(vel.X * sin(-angleVision), vel.Y, vel.Z * cos(-angleVision));
        
        
        //calculate a and b coefficients, to compose the point
        double a = 0, b = 0;
        if(sensorRight.X*sensorLeft.X != sensorRight.Z) 
           b =  (p.X * sensorLeft.X - p.Z ) / (sensorRight.X*sensorLeft.X - sensorRight.Z);
        if(sensorLeft.Z != 0)
            a = (p.Z - b * sensorRight.Z / sensorLeft.Z );
        
        //Analyze conditions to not be in sensor field of view
        if(a < 0 || b < 0) return false;
        if( a+b > maxRadius) return false;
            
        return true;
    }
    
    void updatePosition( core::vector3df pos){
        position = pos;
    }
};

int main()
{
	// This is the movemen speed in units per second.
	f32 MOVEMENT_SPEED = 20.f;

    int angle = 30; //angle in º
    
    float maxRadius = 2.f;
    
    const f32 pi = 3.141592653f;
    
	const f32 ACCELERATION_SPEED = 0.1f;

	const f32 BRAKE_SPEED = -0.5f;

    //OUR OWN CODE
    //_______TESTING PHYSICS________-
    //Create points in space
    core::vector3df point(0.3f ,0.f ,0.5f);
     core::vector3df point2(0.f ,0.f ,1.f);
    core::vector3df point3(0.f ,0.f ,-1.f);
    
    //Previous calculus
    double angleRad = angle * pi / 180;
    
    //Object data
    core::vector3df position(0.f,0.f,0.f);
    core::vector3df velocity(MOVEMENT_SPEED*sin(angleRad) ,0.f ,MOVEMENT_SPEED*cos(angleRad));
        
    //Initializing sensor
    Sensor s(position, angleRad, maxRadius);
    
    //DETECTING IF POINT
    bool inside1 = s.detectFieldVision(velocity,point);
    //bool inside2 = s.detectFieldVision(velocity,point2);
   // bool inside3 = s.detectFieldVision(velocity,point3);
    
    //OUTPUTS
    if(inside1)
        std::cout << "punto 1 dentro del área" <<std::endl;
   /* if(inside2)
        std::cout << "punto 2 dentro del área" <<std::endl;
    if(inside3)
        std::cout << "punto 3 dentro del área" <<std::endl;*/
    
    /*core::vector3df nodePosition = node->getPosition();
    core::vector3df node2Position = n->getPosition();

    if(receiver.IsKeyDown(irr::KEY_KEY_A))
        node2Position.X -= 20.f * frameDeltaTime;
    else if(receiver.IsKeyDown(irr::KEY_KEY_D))
        node2Position.X += 20.f * frameDeltaTime;

    /*
    int decision = MyFuzzyLogic::acelerar_frenar(distance);

    switch(decision)
    {
    case 0: if(MOVEMENT_SPEED<=0)
            {
                MOVEMENT_SPEED=0;
            }
            else
            {
                MOVEMENT_SPEED+=BRAKE_SPEED;
            }
            break;
    case 1: MOVEMENT_SPEED+=ACCELERATION_SPEED;
            break;
    case 2: break;
    }
    

    int decision2 = MyFuzzyLogic::girar(distance);

    switch(decision2)
    {
        case 0: break;
        case 1: angle+=0.005f;
                break;
        case 2: angle+=0.01f;
                break;
    }
    */
	
	return 0;
}


/*
That's it. Compile and play around with the program.
**/
