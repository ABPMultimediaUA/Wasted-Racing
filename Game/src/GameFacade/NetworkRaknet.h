#pragma once

#include "INetworkFacade.h"

class NetworkRaknet : public INetworkFacade {

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    NetworkRaknet() : INetworkFacade() {}

    //Destructor
    virtual ~NetworkRaknet() {}

    //==============================================================
    // Engine Related functions
    //==============================================================

    //Creates a window depending on the engine
    virtual void openNetwork();

    //Updates window info in the engine
    virtual void updateNetwork();

    //Closes engine window
    virtual void closeNetwork();

    //==============================================================
    // Physics Related functions
    //==============================================================

    //Renders a single object
    virtual void updateObject(int id);

    //Renders all the scene
    virtual void updateObjects();

private: 

};