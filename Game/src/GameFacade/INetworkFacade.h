#pragma once

class INetworkFacade{

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    INetworkFacade() {}

    //Destructor
    virtual ~INetworkFacade() {}

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
    virtual void openNetworkEngine() = 0;

    //Updates window info in the engine
    virtual void updateNetworkEngine() = 0;

    //Closes engine window
    virtual void closeNetworkEngine() = 0;

    //==============================================================
    // Network Related functions
    //==============================================================

    //Renders a single object
    virtual void updateObject(int id) = 0;

    //Renders all the scene
    virtual void updateObjects() = 0;

protected:

};