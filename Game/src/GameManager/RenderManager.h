#pragma once

#include "../GameFacade/IRenderFacade.h"
#include "QuadTree.h"
#include <vector>

class RenderManager{

public: 

    //Constructor
    RenderManager() {}

    //Destructor
    ~RenderManager() {
        delete renderFacade;
    }

    //Initialization
    void init(int engine);

    //Update
    void update();

    //Draw
    void draw();

    //Shutdown
    void close();

    //Take the vector and split it in a quadTree
    void splitQuadTree();

    //Static class getter
    static RenderManager& getInstance();

    //Component list getter
    std::vector<IComponent::Pointer>& getComponentList() {
        return renderComponentList;
    }

    //QuadTree getter
    QuadTree& getComponentTree() {
        return renderComponentTree;
    }

private:

    IRenderFacade* renderFacade;

    //When we start adding components, we add them in a list,
    //Once we've added them all, we split them in a QuadTree structure
    std::vector<IComponent::Pointer>      renderComponentList;
    QuadTree                              renderComponentTree;

    //Data for the quadTree
    unsigned int maxObjPerNode;
    int          updateRange;
    int          x0, x1, y0, y1; //Map dimensions

};