#pragma once

//=========================================================================
//                               INCLUDES                                //
//=========================================================================
#include <SFML/Graphics.hpp>

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
    void dropDevice();

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
    GLuint programID;
    TNode *scene;
    ResourceManager *resourceManager;

};

}