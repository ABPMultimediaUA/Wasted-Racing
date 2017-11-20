#pragma once

#include "IPhysicsFacade.h"

class PhysicsIrrlicht : public IPhysicsFacade {

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    PhysicsIrrlicht() : IPhysicsFacade() {}

    //Destructor
    virtual ~PhysicsIrrlicht() {}

    //==============================================================
    // Engine Related functions
    //==============================================================

    //Creates a window depending on the engine
    virtual void openPhysics();

    //Updates window info in the engine
    virtual void updatePhysics();

    //Closes engine window
    virtual void closePhysics();

    //==============================================================
    // Physics Related functions
    //==============================================================

    //Renders a single object
    virtual void updateObject(int id);

    //Renders all the scene
    virtual void updateObjects();

private: 

};