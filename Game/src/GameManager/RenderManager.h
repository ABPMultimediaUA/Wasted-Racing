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

    IRenderFacade* getRenderFacade() {
        return renderFacade;
    }

    IComponent::Pointer createObjectRenderComponent(GameObject& newGameObject, ObjectRenderComponent::Shape newShape, const char* newStr);
    IComponent::Pointer createLightRenderComponent(GameObject& newGameObject, LightRenderComponent::Type newType, float newRadius);
    IComponent::Pointer createCameraRenderComponent(GameObject& newGameObject);

    //Camera component getter and setter
    IComponent::Pointer getCameraComponent() { return cameraComponent; }
    void setCameraComponent( IComponent::Pointer cam ) { cameraComponent = cam; }

    //AI Debug
    void renderAIDebug();
    void updateAIDebug();
    void createRenderNPC(); //create camera and render next point of npc
    void deleteRenderNPC(); //delete camera and render next point of npc

private:

    IRenderFacade* renderFacade;

    //When we start adding components, we add them in a list,
    //Once we've added them all, we split them in a QuadTree structure
    std::vector<IComponent::Pointer>      renderComponentList;
    QuadTree                              renderComponentTree;

    //Data for the quadTree
    //unsigned int maxObjPerNode;
    //int          updateRange;
    //int          x0, x1, y0, y1; //Map dimensions

    //We store just one camera component, so we can't have more than 1
    IComponent::Pointer cameraComponent;

    unsigned int AIDebug = 0;
    bool lap = false;
    std::vector<GameObject> AIDebugPoint;
    GameObject::Pointer marker;
    GameObject::Pointer collisionCylinder;

};