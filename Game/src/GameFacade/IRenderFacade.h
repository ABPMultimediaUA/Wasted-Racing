#pragma once

#include <iostream>
#include <cstdint>
#include <string>

#include "../GameManager/InputManager.h"
#include "../GameObject/RenderComponent/ObjectRenderComponent.h"
#include "../GameObject/RenderComponent/LightRenderComponent.h"
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
    virtual void interpolateCamera(float accTime, float maxTime) = 0;

    //Add an object to the game
    virtual void addObject(IComponent* ptr) = 0;

    //Add an object to the game
    virtual void addObject(IComponent* ptr, float radius, float length, int tesselation, bool transparency) = 0;

    //Add a light to the game
    virtual void addLight(IComponent* ptr) = 0;

    //Delete an object or light of the game
    virtual void deleteObject(IComponent* ptr) = 0;

    //Change the position of an object in-game
    virtual void updateObjectTransform(uint16_t id, GameObject::TransformationData transform) = 0;

    //==============================================================
    //  VISUAL INTERFACE
    //==============================================================

    //Adds an image on the specific point given with the image specified
    virtual int32_t addImage(glm::vec2 pos, std::string img) = 0;

    //Substitutes an image with another at the same position
    virtual void changeImage(int32_t id, std::string img) = 0;

    //Deletes specified rectangle by its index in the rectangle array
    virtual void deleteImage(int32_t id) = 0;

    //Clean images off of the screen
    virtual void cleanImages() = 0;

    //Add rectangle of the given color and alpha channel, at the specified position with the given size
    virtual int32_t addRectangleColor(glm::vec2 pos, glm::vec2 size, int r, int g, int b, int a) = 0;

    //Change color of the rectangle known by the id given
    virtual void changeRectangleColor(int32_t id, int r, int g, int b, int a) = 0;

    //Deletes the rectangle with the passed id
    virtual void deleteRectangleColor(int32_t id) = 0;

    //Clean all rectangles off of the screen
    virtual void cleanRectangles() = 0;

    //Adds specified text at the specified point
    virtual int32_t addText(glm::vec2 pos, std::string text) = 0;

    //Adds specified text at the specified point with specified size
    virtual int32_t addText(glm::vec2 pos, glm::vec2 size, std::string text) = 0;

    //Changes the specified text with the given message
    virtual void changeText(int32_t id, std::string text) = 0;

    //deletes text in the specified position of the text array
    virtual void deleteText(int32_t id) = 0;

    //Erase all text from the screen
    virtual void cleanTexts() = 0;

    //Erase all visual interface elements from the screen
    virtual void cleanInterface() = 0;

    ///////////////////////////////
    ///////      DEBUG      ///////    
    ///////////////////////////////
    //Update logo video
    virtual void updateLogo() = 0;

    virtual void createRectangle2D(glm::vec2 pos, std::string img) = 0;

    virtual void deleteRectangle2D()  = 0;
    
    virtual void createRectangleCol2D(glm::vec2 pos, std::string img) = 0;

    virtual void deleteRectangleCol2D() = 0;

    virtual void createTitleText(glm::vec2 pos, std::string text) = 0;

    virtual void deleteTitleText() = 0;

    virtual void setTitleText(std::string text) = 0;

    virtual void createDescriptionText(glm::vec2 pos, std::string text) = 0;

    virtual void deleteDescriptionText() = 0;

    virtual void setDescriptionText(std::string text) = 0;

    virtual void createSubTitleText(glm::vec2 pos, std::string text) = 0;

    virtual void deleteSubTitleText() = 0;

    virtual void setSubTitleText(std::string text) = 0;

    virtual void createSubDescriptionText(glm::vec2 pos, std::string text) = 0;

    virtual void deleteSubDescriptionText() = 0;

    virtual void setSubDescriptionText(std::string text) = 0;
    
    virtual void drawGUI() = 0;

    virtual void createItemIcon(glm::vec2 pos, std::string img)  = 0;

    virtual void deleteItemIcon()  = 0;

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