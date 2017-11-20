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
#include <glm/glm.hpp>
#include "driverChoice.h"
#include "FuzzyLogic.h"
#include "Sensor.h"
#include "WayPoint.h"

using namespace irr;

/*
To receive events like mouse and keyboard input, or GUI events like "the OK
button has been clicked", we need an object which is derived from the
irr::IEventReceiver object. There is only one method to override:
irr::IEventReceiver::OnEvent(). This method will be called by the engine once
when an event happens. What we really want to know is whether a key is being
held down, and so we will remember the current state of each key.
*/
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

/////////////////////Altered code
//_______TESTING

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

	/*
	Create the node which will be moved with the WSAD keys. We create a
	sphere node, which is a built-in geometry primitive. We place the node
	at (0,0,30) and assign a texture to it to let it look a little bit more
	interesting. Because we have no dynamic lights in this scene we disable
	lighting for each model (otherwise the models would be black).
	*/
	scene::ISceneNode * node = smgr->addSphereSceneNode();
	if (node)
	{
		node->setPosition(core::vector3df(0,0,30));
		node->setMaterialTexture(0, driver->getTexture("../../media/wall.bmp"));
		node->setMaterialFlag(video::EMF_LIGHTING, false);
	}
	
	scene::ISceneNode * bola1 = smgr->addSphereSceneNode();
	if (bola1)
	{
		bola1->setPosition(core::vector3df(0,0,30));
		bola1->setMaterialTexture(0, driver->getTexture("../../media/wall.bmp"));
		bola1->setMaterialFlag(video::EMF_LIGHTING, false);
		bola1->setScale(core::vector3df(0.2f,0.2f,0.2f));
	}

	scene::ISceneNode * bola2 = smgr->addSphereSceneNode();
	if (bola2)
	{
		bola2->setPosition(core::vector3df(0,0,30));
		bola2->setMaterialTexture(0, driver->getTexture("../../media/wall.bmp"));
		bola2->setMaterialFlag(video::EMF_LIGHTING, false);
		bola2->setScale(core::vector3df(0.2f,0.2f,0.2f));
	}


	/*
	Now we create another node, movable using a scene node animator. Scene
	node animators modify scene nodes and can be attached to any scene node
	like mesh scene nodes, billboards, lights and even camera scene nodes.
	Scene node animators are not only able to modify the position of a
	scene node, they can also animate the textures of an object for
	example. We create a cube scene node and attach a 'fly circle' scene
	node animator to it, letting this node fly around our sphere scene node.
	*/
	/*scene::ISceneNode* n = smgr->addCubeSceneNode();

	if (n)
	{
	node animator to it, letting this node fly around our sphere scene node.
	*/
	/*scene::ISceneNode* n = smgr->addCubeSceneNode();

	if (n)
	{
		n->setMaterialTexture(0, driver->getTexture("../../media/t351sml.jpg"));
		n->setMaterialFlag(video::EMF_LIGHTING, false);
		scene::ISceneNodeAnimator* anim =
			smgr->createFlyCircleAnimator(core::vector3df(0,0,30), 20.0f);
		if (anim)
		{
			n->addAnimator(anim);
			anim->drop();
		}
	}*/

	/*
	The last scene node we add to show possibilities of scene node animators is
	a b3d model, which uses a 'fly straight' animator to run between to points.
	*/
	/*scene::IAnimatedMeshSceneNode* anms =
		smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/ninja.b3d"));

	if (anms)
	{
		scene::ISceneNodeAnimator* anim =
			smgr->createFlyStraightAnimator(core::vector3df(100,0,60),
			core::vector3df(-100,0,60), 3500, true);
		if (anim)
		{
			anms->addAnimator(anim);
			anim->drop();
		}

		/*
		To make the model look right we disable lighting, set the
		frames between which the animation should loop, rotate the
		model around 180 degrees, and adjust the animation speed and
		the texture. To set the right animation (frames and speed), we
		would also be able to just call
		"anms->setMD2Animation(scene::EMAT_RUN)" for the 'run'
		animation instead of "setFrameLoop" and "setAnimationSpeed",
		but this only works with MD2 animations, and so you know how to
		start other animations. But a good advice is to not use
		hardcoded frame-numbers...
		*/
	/*	anms->setMaterialFlag(video::EMF_LIGHTING, false);

		anms->setFrameLoop(0, 13);
		anms->setAnimationSpeed(15);
//		anms->setMD2Animation(scene::EMAT_RUN);

		anms->setScale(core::vector3df(2.f,2.f,2.f));
		anms->setRotation(core::vector3df(0,-90,0));
//		anms->setMaterialTexture(0, driver->getTexture("../../media/sydney.bmp"));

	}*/


	/*
	To be able to look at and move around in this scene, we create a first
	person shooter style camera and make the mouse cursor invisible.
	*/
	smgr->addCameraSceneNodeFPS();
	device->getCursorControl()->setVisible(false);

	/*
	Add a colorful irrlicht logo
	*/
	device->getGUIEnvironment()->addImage(
		driver->getTexture("../../media/irrlichtlogoalpha2.tga"),
		core::position2d<s32>(10,20));

	gui::IGUIStaticText* diagnostics = device->getGUIEnvironment()->addStaticText(
		L"", core::rect<s32>(10, 10, 400, 20));
	diagnostics->setOverrideColor(video::SColor(255, 255, 255, 0));

	/*
	We have done everything, so lets draw it. We also write the current
	frames per second and the name of the driver to the caption of the
	window.
	*/
	int lastFPS = -1;

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();


	///Own Code
	//-------------------------------------------------------------------------------
	scene::ISceneNode* n = smgr->addCubeSceneNode();
	if (n)
	{
		n->setPosition(core::vector3df(200,0,30));
		n->setMaterialTexture(0, driver->getTexture("../../media/t351sml.jpg"));
		n->setMaterialFlag(video::EMF_LIGHTING, false);
	}
	//-------------------------------------------------------------------------------

	// This is the movemen speed in units per second.

	//OWN VARIABLES
	//----------------------------------------------------
	float MOVEMENT_SPEED = 20.f;
	const f32 CUBE_SPEED = 50.f;
	const f32 ROTATE_SPEED = 0.01f;


	int angle = 55; //angle in º
	double anglePlayer = 0;

	float maxRadius = 150.f;
	
	const f32 pi = 3.141592653f;
	
	const f32 ACCELERATION_SPEED = 0.1f;

	const f32 BRAKE_SPEED = -0.5f;


	//Previous calculus
	double angleRad = angle * pi / 180;

	//get position
	core::vector3df nodePosition = node->getPosition();
	core::vector3df nodePos2 = n->getPosition();

	//Initializing sensor
	Sensor s(nodePosition, angleRad, maxRadius,-pi/2);

	//----------------------------------------------------

	while(device->run())
	{
		// Work out a frame delta time.
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;
		

		/* Check if keys W, S, A or D are being held down, and move the
		sphere node around respectively. */

		//Own Code
		//----------------------------------------------------
		//Move the cube for testing
		if(receiver.IsKeyDown(irr::KEY_KEY_W))
			nodePos2.X -= CUBE_SPEED * frameDeltaTime;
		else if(receiver.IsKeyDown(irr::KEY_KEY_S))
			nodePos2.X += CUBE_SPEED * frameDeltaTime;
		if(receiver.IsKeyDown(irr::KEY_KEY_A))
			nodePos2.Z -= CUBE_SPEED * frameDeltaTime;
		else if(receiver.IsKeyDown(irr::KEY_KEY_D))
			nodePos2.Z += CUBE_SPEED * frameDeltaTime;
		
		//Object data
		core::vector3df velocity(MOVEMENT_SPEED*frameDeltaTime*cos(anglePlayer) ,0.f,MOVEMENT_SPEED*frameDeltaTime*sin(anglePlayer));

		//DETECTING IF POINT IS NOT IN FRONT
		bool inside1 = s.detectFieldVision(velocity,nodePos2);
		//bool inside2 = s.detectFieldVision(velocity,point2);
		// bool inside3 = s.detectFieldVision(velocity,point3);
		
		//DETECTING WHICH SIDE TO TURN AND HOW MUCH
		double giroPorcentaje = FuzzyLogic::girar(sqrt((nodePos2.X-nodePosition.X)*(nodePos2.X-nodePosition.X) 
									+ (nodePos2.Z-nodePosition.Z) * (nodePos2.Z-nodePosition.Z) ), s.a, s.b, maxRadius);


		//decide to move
		if(inside1){		
			//ROTATE
			anglePlayer += giroPorcentaje * ROTATE_SPEED;

			//std::cout<<"Angulo: "<<anglePlayer<<" con porcentaje "<<giroPorcentaje<<std::endl;

			s.updateAngle(giroPorcentaje*ROTATE_SPEED);

			//MOVE
			nodePosition += velocity;
		}



		//set positions
		node->setPosition(nodePosition);
		n->setPosition(nodePos2);
		s.updatePosition(nodePosition);

		//VISUAL DEBUG
		bola1->setPosition(nodePosition+maxRadius*core::vector3df(sin(-angleRad+pi/2-anglePlayer), 0.f, cos(-angleRad+pi/2-anglePlayer)));
      	bola2->setPosition(nodePosition+maxRadius*core::vector3df(sin(angleRad+pi/2-anglePlayer), 0.f, cos(angleRad+pi/2-anglePlayer)));
        


		//----------------------------------------------------


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
