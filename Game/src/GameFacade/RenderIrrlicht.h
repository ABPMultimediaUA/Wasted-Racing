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
    override void openWindow();

    //Updates window info in the engine
    override void updateWindow();

    //Closes engine window
    override void closeWindow();

private: 


};
