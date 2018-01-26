#pragma once

#include <irrlicht.h>
#include <map>

#include "IRenderFacade.h"

#include "../GameManager/InputManager.h"

class RenderIrrlicht : public IRenderFacade {

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    RenderIrrlicht() : IRenderFacade() {}

    //Destructor
    virtual ~RenderIrrlicht() {}

    //==============================================================
    // Engine Related functions
    //==============================================================

    //Creates a window depending on the engine
    virtual void openWindow();

    //Updates window info in the engine
    virtual void updateWindow();

    //Closes engine window
    virtual void closeWindow();

    //==============================================================
    // Render Related functions
    //==============================================================

    //Renders all the scene
    virtual void renderDraw();

    //Add a camera to the game
    virtual void addCamera();

    //Update the current camera
    virtual void interpolateCamera(float accTime, float maxTime);

    //Add an object to the game
    virtual void addObject(IComponent* ptr);

    //Add an object to the game (Cylinder or Cone)
    virtual void addObject(IComponent* ptr, float radius, float length, int tesselation, bool transparency);
    
    //Add a light to the game
    virtual void addLight(IComponent* ptr);

    //Delete an object of the game
    virtual void deleteObject(IComponent* ptr);

    //Change the position of an object in-game
    virtual void updateObjectTransform(uint16_t id, GameObject::TransformationData transform);

    //Update the logo video
    virtual void updateLogo();

    virtual void createRectangle2D(glm::vec2 pos, std::string img);

    virtual void deleteRectangle2D();

    virtual void drawGUI();

    virtual void createItemIcon(glm::vec2 pos, std::string img);

    virtual void deleteItemIcon();

    virtual void updateItemIcon();


    //Get device
    irr::IrrlichtDevice* getDevice(){
        return device;
    };

    //Get videodriver
    irr::video::IVideoDriver* getVideoDriver(){
        return videoDriver;
    };

    //Get scenemanager
    irr::scene::ISceneManager* getSceneManager(){
        return sceneManager;
    };

private:

    //Irrlicht own window
    irr::IrrlichtDevice* device;

    //Irrlicht scene manager
    irr::scene::ISceneManager* sceneManager;

    //Irrlicht video manager
    irr::video::IVideoDriver* videoDriver;

    //Irrlicht geometry creator
    const irr::scene::IGeometryCreator* geometryCreator;

    //Irrlicht node map
    std::map<uint16_t, irr::scene::ISceneNode*> nodeMap;

    //Irrlicht camera
    irr::scene::ICameraSceneNode* camera;

    irr::gui::IGUIStaticText* pos;

    irr::gui::IGUIStaticText* lap;

    irr::gui::IGUIStaticText* item;

    irr::gui::IGUIFont* font;

    irr::gui::IGUIElement* rectangle;

    irr::gui::IGUIElement* itemIMG;



};
