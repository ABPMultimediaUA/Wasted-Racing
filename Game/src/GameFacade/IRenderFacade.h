#pragma once

#include <string>

#include "../GameManager/InputManager.h"

class GameObject;

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

    //Renders a single object
    virtual void renderObject(std::string id) = 0;

    //Renders all the scene
    virtual void renderDraw() = 0;

    //Gets camera object
    GameObject& getCamera() {
    }
    

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


protected:

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

    //Window declaration
    Window window;
    //InputManager declaration
    InputManager* inputManager;
};