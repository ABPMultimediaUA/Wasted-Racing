#pragma once

#include <iostream>
#include <cstdint>

#include "../GameManager/InputManager.h"
#include "../GameObject/ObjectRenderComponent.h"
#include "../GameObject/GameObject.h"

class GameObject;

//Window data structures
struct vec2d {
    int x;
    int y;
};

struct Window {
    vec2d size;
    bool fullscreen;
    bool vsync;
};

class IRenderFacade{

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    IRenderFacade() {}

    //Destructor
    virtual ~IRenderFacade() {}

    //Initializer
    void init(int x, int y, bool fScreen, bool vsync) {

        window.size.x = x;
        window.size.y = y;

        window.fullscreen = fScreen;
        window.vsync = vsync;

        inputManager = &InputManager::getInstance();

    }

    //==============================================================
    // Engine Related functions
    //==============================================================

    //Creates a window depending on the engine
    virtual void openWindow() = 0;

    //Updates window info in the engine
    virtual void updateWindow() = 0;

    //Closes engine window
    virtual void closeWindow() = 0;

    //==============================================================
    // Render Related functions
    //==============================================================

    //Renders all the scene
    virtual void renderDraw() = 0;

    //Add a camera to the game
    virtual void addCamera() = 0;

    //Update the current camera
    virtual void updateCamera() = 0;

    //Add an object to the game
    virtual void addObject(IComponent::Pointer ptr) = 0;

    //Delete an object of the game
    virtual void deleteObject(IComponent::Pointer ptr) = 0;

    //Add a light to the game
    virtual void addLight() = 0;

    //Change the position of an object in-game
    virtual void updateObjectTransform(uint16_t id, GameObject::TransformationData transform) = 0;

    //==============================================================
    // Window Related functions
    //==============================================================

    //Resizes the window
    void resizeWindow(int x, int y) {
        window.size.x = x;
        window.size.y = y;

        updateWindow();
    }

    //Changes fullscreen state and returns it
    bool fullscreenWindow() {
         
        if(window.fullscreen)
            window.fullscreen = false;
        else 
            window.fullscreen = true;

        updateWindow();

        return window.fullscreen;

    }

    //Changes sync state and return it
    bool syncWindow() {

        if(window.vsync)
            window.vsync = false;
        else 
            window.vsync = true;

        updateWindow();

        return window.vsync;
    }

    Window& getWindow(){
        return window;
    }

    //Sets the gameObject to be targeted by the camera
    void setCameraTarget(GameObject& newCameraTarget) {
        cameraTarget = &newCameraTarget;
    }
    //Returns the gameObject to be targeted by the camera
    GameObject& getCameraTarget() {
        return *cameraTarget;
    }


protected:

    //Window declaration
    Window window;
    //InputManager declaration
    InputManager* inputManager;
    //Camera target
    GameObject* cameraTarget;

};