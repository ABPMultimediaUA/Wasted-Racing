#pragma once

#include <redpanda/RedPandaStudio.h>

#include <map>
#include <string>

#include "IRenderFacade.h"
#include "InputRedPanda.h"

#include "../GameManager/InputManager.h"

class RenderRedPanda : public IRenderFacade {

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    RenderRedPanda() : IRenderFacade() {}

    //Destructor
    virtual ~RenderRedPanda() {}

    //==============================================================
    // Engine Related functions
    //==============================================================

    //Creates a window depending on the engine
    virtual void openWindow();

    //Updates window info in the engine
    virtual void updateWindow();

    //Closes engine window
    virtual void closeWindow();

    //==============================================================
    // Render Related functions
    //==============================================================

    //Renders all the scene
    virtual void renderDraw();

    //Add a camera to the game
    virtual void addCamera();

    //Update the current camera
    virtual void interpolateCamera(float accTime, float maxTime);

    //Add an object to the game
    virtual void addObject(IComponent* ptr);

    //Add an object to the game (Cylinder or Cone)
    virtual void addObject(IComponent* ptr, float radius, float length, int tesselation, bool transparency);
    
    //Add a light to the game
    virtual void addLight(IComponent* ptr);

    //Delete an object of the game
    virtual void deleteObject(IComponent* ptr);

    //Change the position of an object in-game
    virtual void updateObjectTransform(uint16_t id, GameObject::TransformationData transform);


    ///////////////////////////////
    ///////      DEBUG      ///////    
    ///////////////////////////////
    
    virtual void createRectangle2D(glm::vec2 pos, std::string img);

    virtual void deleteRectangle2D();
    
    virtual void createRectangleCol2D(glm::vec2 pos, std::string img);

    virtual void deleteRectangleCol2D();

    virtual void createTitleText(glm::vec2 pos, std::string text);

    virtual void deleteTitleText();

    virtual void setTitleText(std::string text);

    virtual void createDescriptionText(glm::vec2 pos, std::string text);

    virtual void deleteDescriptionText();

    virtual void setDescriptionText(std::string text);

    virtual void createSubTitleText(glm::vec2 pos, std::string text);

    virtual void deleteSubTitleText();

    virtual void setSubTitleText(std::string text);

    virtual void createSubDescriptionText(glm::vec2 pos, std::string text);

    virtual void deleteSubDescriptionText();

    virtual void setSubDescriptionText(std::string text);

private: 
    //Update the logo video
    virtual void updateLogo();

    virtual void drawGUI();

    virtual void createItemIcon(glm::vec2 pos, std::string img);

    virtual void deleteItemIcon();

    virtual void updateItemIcon();

private:

    //RenderRedPanda own window
    rps::RedPandaStudio* device;

    //RedPanda node map
    std::map<uint16_t, TNode*> nodeMap;

    //RedPanda camera
    //rps::TNode* camera;

};
