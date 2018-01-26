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
    void update(float dTime);

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
    //Create cylinder or cone
    IComponent::Pointer createObjectRenderComponent(GameObject& newGameObject, ObjectRenderComponent::Shape newShape, const char* newStr, float radius, float length, int tesselation, bool transparency);
    //Camera component getter and setter
    IComponent::Pointer getCameraComponent() { return cameraComponent; }
    void setCameraComponent( IComponent::Pointer cam ) { cameraComponent = cam; }

    //AI Debug
    void renderAIDebug();
    void updateAIDebug();
    void createRenderNPC(); 
    void deleteRenderNPC(); 
    void createLinesObjects();
    void deleteLinesObjects();

    //Camera Debug
    void renderCameraDebug();
    void updateCameraDebug();
    void createCameraRender();
    void deleteCameraRender();

    //Debug Behaviour
    void renderBattleDebug();
    void updateBattleDebug(float dTime);
    void createBattleRender();
    void deleteBattleRender();
    void setRootBattle(std::string r)    {          root += " -> "+r;          }
    void setEndRootBattle(bool b)        {          end = b;            }
    bool getEndRootBattle()              {          return end;         }
    void setWaitBattle(bool b)           {          wait = b;            }
    bool getWaitBattle()                 {          return wait;         }
    
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

    //Debug IA
    unsigned int AIDebug = 0;
    bool lap = false;
    std::vector<GameObject> AIDebugPoint;
    GameObject::Pointer marker;
    GameObject::Pointer collisionCylinder;
    GameObject::Pointer visibilityArea;
    GameObject::Pointer visionTriangle;
    std::vector<GameObject::Pointer> linesObjects;
    float moveSeen = 0;
    float maxMoveSeen = 20.0;
    float moveNextPos = 0;
    float maxMoveNextPos = 30.0;
    bool activeDebugAI = false;

    //Debug Camera
    unsigned int AIDebugC = 0;
    bool lapC = false;
    std::vector<GameObject> AIDebugPointC;
    GameObject::Pointer cameraVision;
    bool collisionC = false;
    int aux = 0;
    bool activeDebugCamera = false;

    //Debug Behaviour
    unsigned int AIDebugB = 0;
    bool lapB = false;
    std::vector<GameObject> AIDebugPointB;
    std::string title = "BEHAVIOUR";
    std::string root = "";
    bool end = false;
    bool activeDebugBehaviour = false;
    bool success = false;
    float timeP = 5.0f;
    float maxTimeP = 5.0f;
    bool wait = false;
    

};