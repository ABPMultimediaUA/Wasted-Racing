#pragma once

class IPhysicsFacade{

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    IPhysicsFacade() {}

    //Destructor
    virtual ~IPhysicsFacade() {}

    //Initializer
    void init() {

    }

    //Closer
    void close() {

    }

    //==============================================================
    // Engine Related functions
    //==============================================================

    //Creates a window depending on the engine
    virtual void openPhysicsEngine() = 0;

    //Updates window info in the engine
    virtual void updatePhysicsEngine() = 0;

    //Closes engine window
    virtual void closePhysicsEngine() = 0;

    //==============================================================
    // Physics Related functions
    //==============================================================

    //Renders a single object
    virtual void updateObject(int id) = 0;

    //Renders all the scene
    virtual void updateObjects() = 0;

protected:

};