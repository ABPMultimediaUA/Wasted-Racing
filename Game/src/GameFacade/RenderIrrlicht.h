#pragma once

#include <irrlicht.h>

#include "IRenderFacade.h"

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

    //Renders a single object
    virtual void renderObject(std::string id);

    //Renders all the scene
    virtual void renderDraw();

private: 

    //Irrlicht own window
    IrrlichtDevice* device;

    //Irrlicht scene manager
    scene::ISceneManager* sceneManager;

    //Irrlicht video manager
    video::IVideoDriver* videoDriver;


};
