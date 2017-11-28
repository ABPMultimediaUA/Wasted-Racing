
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
#include "PathPlanning.h"
#include "Environment.h"

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
			core::dimension2d<u32>(1024,800), 16, false, false, false, &receiver);

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
    
    //---------------------------------------------
    //OWN CODE
    //---------------------------------------------
    Environment env;
    
	scene::ISceneNode * node = smgr->addSphereSceneNode();
	if (node)
	{
		node->setPosition(core::vector3df(0,0,30));
		node->setMaterialTexture(0, driver->getTexture("media/wall.bmp"));
		node->setMaterialFlag(video::EMF_LIGHTING, false);
	}
	
	scene::ISceneNode * bola1 = smgr->addSphereSceneNode();
	if (bola1)
	{
		bola1->setPosition(core::vector3df(0,0,30));
		bola1->setMaterialTexture(0, driver->getTexture("media/wall.bmp"));
		bola1->setMaterialFlag(video::EMF_LIGHTING, false);
		bola1->setScale(core::vector3df(0.4f,0.4f,0.4f));
	}

	scene::ISceneNode * bola2 = smgr->addSphereSceneNode();
	if (bola2)
	{
		bola2->setPosition(core::vector3df(0,0,30));
		bola2->setMaterialTexture(0, driver->getTexture("media/wall.bmp"));
		bola2->setMaterialFlag(video::EMF_LIGHTING, false);
		bola2->setScale(core::vector3df(0.4f,0.4f,0.4f));
	}

    env.addNode(node);
    env.addNode(bola1);
    env.addNode(bola2);
	
	
	//---------------------------------------------
	//---------------------------------------------

	/*
/*	To be able to look at and move around in this scene, we create a first
	person shooter style camera and make the mouse cursor invisible.
	*/
	smgr->addCameraSceneNodeFPS();
	device->getCursorControl()->setVisible(false);

	/*
	We have done everything, so lets draw it. We also write the current
	frames per second and the name of the driver to the caption of the
	window.
	*/
	int lastFPS = -1;

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();

    
    //-------------------------------------------------------------------------------
	///Own Code
	//-------------------------------------------------------------------------------
	scene::ISceneNode* n = smgr->addCubeSceneNode();
	if (n)
	{
		n->setPosition(core::vector3df(200,0,30));
		n->setMaterialTexture(0, driver->getTexture("media/t351sml.jpg"));
		n->setMaterialFlag(video::EMF_LIGHTING, false);
	}

	scene::ISceneNode* obs1 = smgr->addCubeSceneNode();
	if (obs1)
	{
		obs1->setPosition(core::vector3df(100,0,40));
		obs1->setMaterialTexture(0, driver->getTexture("media/t351sml.jpg"));
		obs1->setMaterialFlag(video::EMF_LIGHTING, false);
	}

	scene::ISceneNode* obs2 = smgr->addCubeSceneNode();
	if (obs2)
	{
		obs2->setPosition(core::vector3df(150,0,27));
		obs2->setMaterialTexture(0, driver->getTexture("media/t351sml.jpg"));
		obs2->setMaterialFlag(video::EMF_LIGHTING, false);
	}
	
	
    env.addNode(n);
    env.addNode(obs1);
    env.addNode(obs2);
	//-------------------------------------------------------------------------------
     //-------------------------------------------------------------------------------

	// This is the movemen speed in units per second.

	//OWN VARIABLES
	//----------------------------------------------------
	float MOVEMENT_SPEED = 50.0f;
	const f32 CUBE_SPEED = 50.f;
	const f32 ROTATE_SPEED = 0.10f;


	int angle = 30; //angle in º
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
	core::vector3df obs1Pos = obs1->getPosition();
	core::vector3df obs2Pos = obs2->getPosition();
    
	//Waypoints
	PathPlanning* p = new PathPlanning();
	WayPoint* w1 = new WayPoint(glm::vec3(600.f, 0.f, 10.f), 30.f);
	WayPoint* w2 = new WayPoint(glm::vec3(320.f, 0.f, 10.f), 30.f);
	WayPoint* w3 = new WayPoint(glm::vec3(320.f, 0.f, 500.f), 30.f);
	WayPoint* w4 = new WayPoint(glm::vec3(600.f, 0.f, 500.f), 30.f);
	p->addWayPoint(w2);
	p->addWayPoint(w1);
	p->addWayPoint(w3);
	p->addWayPoint(w4);
    scene::ISceneNode* way1 = smgr->addCubeSceneNode();
	if (way1)
	{
		way1->setPosition(core::vector3df(600,0,10));
		way1->setMaterialTexture(0, driver->getTexture("media/t351sml.jpg"));
		way1->setMaterialFlag(video::EMF_LIGHTING, false);
		way1->setScale(core::vector3df(0.4f,3.0f,0.4f));
	}
    scene::ISceneNode* way2 = smgr->addCubeSceneNode();
	if (way2)
	{
		way2->setPosition(core::vector3df(320,0,10));
		way2->setMaterialTexture(0, driver->getTexture("media/t351sml.jpg"));
		way2->setMaterialFlag(video::EMF_LIGHTING, false);
		way2->setScale(core::vector3df(0.4f,3.0f,0.4f));
	}
	    scene::ISceneNode* way3 = smgr->addCubeSceneNode();
	if (way3)
	{
		way3->setPosition(core::vector3df(320,0,500));
		way3->setMaterialTexture(0, driver->getTexture("media/t351sml.jpg"));
		way3->setMaterialFlag(video::EMF_LIGHTING, false);
		way3->setScale(core::vector3df(0.4f,3.0f,0.4f));
	}
	scene::ISceneNode* way4 = smgr->addCubeSceneNode();
	if (way4)
	{
		way4->setPosition(core::vector3df(600,0,500));
		way4->setMaterialTexture(0, driver->getTexture("media/t351sml.jpg"));
		way4->setMaterialFlag(video::EMF_LIGHTING, false);
		way4->setScale(core::vector3df(0.4f,3.0f,0.4f));
	}

	//Initializing sensor
	Sensor s(glm::vec3(nodePosition.X,nodePosition.Y,nodePosition.Z), angleRad, maxRadius,-pi/2);

	//Initalizing Environment
	int size = 3;
	irr::scene::ISceneNode** list = new irr::scene::ISceneNode*[size];
	list[0] = n;
	list[1] = obs1;
	list[2] = obs2;
	//----------------------------------------------------

	while(device->run())
	{
		// Work out a frame delta time.
		const u32 now = device->getTimer()->getTime();
		double frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;
		

		/* Check if keys W, S, A or D are being held down, and move the
		sphere node around respectively. */

		//Own Code
		//----------------------------------------------------
		//Move the cube for testing
		glm::vec3 velocity(MOVEMENT_SPEED*frameDeltaTime*cos(anglePlayer) ,0.f,MOVEMENT_SPEED*frameDeltaTime*sin(anglePlayer));


		if(receiver.IsKeyDown(irr::KEY_KEY_W))
			nodePos2.X -= CUBE_SPEED * frameDeltaTime;
		else if(receiver.IsKeyDown(irr::KEY_KEY_S))
			nodePos2.X += CUBE_SPEED * frameDeltaTime;
		if(receiver.IsKeyDown(irr::KEY_KEY_A))
			nodePos2.Z -= CUBE_SPEED * frameDeltaTime;
		else if(receiver.IsKeyDown(irr::KEY_KEY_D))
			nodePos2.Z += CUBE_SPEED * frameDeltaTime;
	
		//----------------------------------
		//ADVANCED DETECTING TECHNIQUE
		//----------------------------------

        
        //PathPlanning assignations
    	p->setMaxSpeed(100.f);
        p->setFrame(frameDeltaTime);
    
		//---VObject array
		std::vector<VObject*> array;

		array = s.getAllFieldVisionObjects(list, size);
		//array = std::vector<VObject*>();

		//---Waypoint info
		glm::vec3 aux = p->getNextPoint(glm::vec3(nodePosition.X, nodePosition.Y, nodePosition.Z), velocity, MOVEMENT_SPEED);
		std::cout<<"WayPoint pos X: "<<aux.x<<"\n";
		std::cout<<"WayPoint pos Z: "<<aux.z<<"\n";

		s.detectFieldVision(velocity, aux);

		float giroPorcentaje = FuzzyLogic::girar(array, aux, 100.0f, s.a, s.b, maxRadius);

		//float giroPorcentaje = 0.0f;
		//ROTATE AND MOVE CONDITIONALLY
		if(receiver.IsKeyDown(irr::KEY_KEY_F)){	
			//ROTATE
			anglePlayer += giroPorcentaje * ROTATE_SPEED;

			//std::cout<<"Angulo: "<<anglePlayer<<" con porcentaje "<<giroPorcentaje<<std::endl;

			s.updateAngle(giroPorcentaje*ROTATE_SPEED);

			//MOVE
			nodePosition += irr::core::vector3df(velocity.x,velocity.y,velocity.z);

			std::cout<<"Speed: "<<velocity.x<<","<<velocity.y<<","<<velocity.z<<std::endl;
		}


		//set positions
		node->setPosition(nodePosition);
		n->setPosition(nodePos2);
		s.updatePosition(glm::vec3(nodePosition.X, nodePosition.Y, nodePosition.Z));

		//VISUAL DEBUG
		bola1->setPosition(nodePosition+maxRadius*core::vector3df(sin(-angleRad+pi/2-anglePlayer), 0.f, cos(-angleRad+pi/2-anglePlayer)));
      	bola2->setPosition(nodePosition+maxRadius*core::vector3df(sin(angleRad+pi/2-anglePlayer), 0.f, cos(angleRad+pi/2-anglePlayer)));
     
		//----------------------------------------------------

		driver->beginScene(true, true, video::SColor(255,113,113,133));

		smgr->drawAll(); // draw the 3d scene
		driver->endScene();
	}
    
	device->drop();
	
	return 0;
}
