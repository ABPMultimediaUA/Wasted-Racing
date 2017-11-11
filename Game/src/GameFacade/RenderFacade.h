#pragma once

#include <string>
#include <irrlicht.h>

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
    void Init(int x, int y, bool fScreen, bool sync);

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
    void resizeWindow(int x, int y);

    //Changes fullscreen state and returns it
    bool fullscreenWindow();

    //Changes sync state and return it
    bool syncWindow();


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
};