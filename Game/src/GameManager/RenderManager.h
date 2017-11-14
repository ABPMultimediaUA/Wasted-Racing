#pragma once

#include "../GameFacade/IRenderFacade.h"

class RenderManager{

public: 

    //Constructor
    RenderManager() {}

    //Destructor
    ~RenderManager() {}

    //Initialization
    void init(int engine);

    //Update
    void update();

    //Shutdown
    void close();

    //Static class getter
    static RenderManager& getInstance();

private:

    IRenderFacade* renderFacade;

};