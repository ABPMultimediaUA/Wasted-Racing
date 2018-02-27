#pragma once

//=========================================================================
//                               INCLUDES                                //
//=========================================================================
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "RedPanda.h"

//=========================================================================
//                        RED PANDA STUDIO CLASS                         //
//=========================================================================

namespace rps {

class RedPandaStudio{

public:

    RedPandaStudio() {}
    ~RedPandaStudio() {}

    //////////////////////////////
    //  DEVICE CONSTRUCTORS
    static RedPandaStudio& createDevice(int width, int height, int depth, int framerate, bool vsync, bool fullscreen);
    void updateDevice();
    void dropDevice();

    //////////////////////////////
    //  NODE CONSTRUCTORS
    TNode* createObjectNode(TNode* parent, glm::vec3 position, const char* mesh);
    TNode* createCamera(TNode* parent, glm::vec3 position);
    TNode* createLight(TNode* parent, glm::vec3 position);

    //////////////////////////////
    //  GETTERS
    sf::RenderWindow* getWindow()   {   return window;      }
    GLuint getProgramID()           {   return programID;   }
    TNode* getSceneRoot()           {   return scene;       }
    ResourceManager* getResourceManager() { return resourceManager;  }

    //////////////////////////////
    //  SETTERS
    void setWindow(sf::RenderWindow* rw )   {   window = rw;    }
    void setProgramID(GLuint pID)           {   programID = pID;}     

private: 

    //////////////////////////////
    //  FUNCTIONS
    void initSFMLWindow(int width, int height, int depth, int framerate, bool vsync, bool fullscreen);
    void initOpenGL();
    void initScene();

    //////////////////////////////
    //  VARIABLES
    sf::RenderWindow* window;
    TNode *scene;
    ResourceManager *resourceManager;

    //////////////////////////////
    //  OPENGL VARIABLES
    GLuint programID;
    GLuint model;
    GLuint view;
    GLuint projection;

};

//Transformation facade
void translateNode(TNode* node, glm::vec3 position);
void rotateNode(TNode* node, float rotation, int axis);
void scaleNode(TNode* node, glm::vec3 scale);


}