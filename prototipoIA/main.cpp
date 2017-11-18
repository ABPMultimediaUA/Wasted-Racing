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
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>
#include "driverChoice.h"
#include <irrMath.h>

using namespace irr;

class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}
	
	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

//OWN CODE

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

int main()
{
	// ask user for driver
	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	if (driverType==video::EDT_COUNT)
		return 1;

	// create device
	MyEventReceiver receiver;

	IrrlichtDevice* device = createDevice(driverType,
			core::dimension2d<u32>(640, 480), 16, false, false, false, &receiver);

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();


	scene::ISceneNode * node = smgr->addSphereSceneNode();
	if (node)
	{
		node->setPosition(core::vector3df(0,0,30));
		node->setMaterialTexture(0, driver->getTexture("../../media/wall.bmp"));
		node->setMaterialFlag(video::EMF_LIGHTING, false);
	}

	scene::ISceneNode* n = smgr->addCubeSceneNode();

	if (n)
	{
		n->setPosition(core::vector3df(200, 0, 30));
		n->setMaterialTexture(0, driver->getTexture("../../media/t351sml.jpg"));		
	}
	
	smgr->addCameraSceneNodeFPS();
	device->getCursorControl()->setVisible(false);

	int lastFPS = -1;

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();

	// This is the movemen speed in units per second.
	f32 MOVEMENT_SPEED = 20.f;
	
	f32 angle = 0.0f;

	const f32 ACCELERATION_SPEED = 0.1f;

	const f32 BRAKE_SPEED = -0.5f;

	while(device->run())
	{
		// Work out a frame delta time.
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;

        //OUR OWN CODE	
		core::vector3df nodePosition = node->getPosition();
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

		nodePosition += core::vector3df(MOVEMENT_SPEED * frameDeltaTime * cos(double(angle)), 0, (MOVEMENT_SPEED * frameDeltaTime * sin(double(angle))));

		node->setPosition(nodePosition);
		n->setPosition(node2Position);

        //---------------------------------------------------------------------------
        
        
		driver->beginScene(true, true, video::SColor(255,113,113,133));

		smgr->drawAll(); // draw the 3d scene
		device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)

		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw tmp(L"Movement Example - Irrlicht Engine [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
	}

	/*
	In the end, delete the Irrlicht device.
	*/
	device->drop();
	
	return 0;
}


/*
That's it. Compile and play around with the program.
**/
